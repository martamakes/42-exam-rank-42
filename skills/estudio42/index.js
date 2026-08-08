const fs = require('fs');
const path = require('path');
const prompts = require('./prompts');
const { detectExercise, isExamRepository } = require('./detect');
const { findSubjectFile, generateTipsFromSubject } = require('./generator');

/**
 * Main skill entry point
 */
async function execute(args, context) {
  const cwd = process.cwd();

  // Initialize configuration
  const config = await initializeConfig(cwd);
  const lang = config.language;

  // Check if it's an exam repository. Every branch below returns ONE
  // self-contained prompt block (welcome + rules + commands + situation-
  // specific content) — nothing is printed piecemeal, since the caller
  // (cli.js) prints exactly what this function returns, once.
  if (!isExamRepository(cwd)) {
    return generateNonExamPrompt(lang);
  }

  // Detect current exercise
  const detected = detectExercise(cwd);

  let exerciseInfo;
  if (detected) {
    exerciseInfo = detected;
  } else {
    // Ask user for exercise info
    return generateManualPrompt(lang);
  }

  // Find or generate tips
  const tipsPath = getTipsPath(cwd, exerciseInfo);
  let tipsContent;

  if (fs.existsSync(tipsPath)) {
    // Load existing tips
    tipsContent = fs.readFileSync(tipsPath, 'utf8');
  } else {
    // Generate tips from subject
    tipsContent = await generateTips(exerciseInfo, cwd, lang);

    if (tipsContent) {
      // Save tips
      const tipsDir = path.dirname(tipsPath);
      if (!fs.existsSync(tipsDir)) {
        fs.mkdirSync(tipsDir, { recursive: true });
      }
      fs.writeFileSync(tipsPath, tipsContent, 'utf8');
    }
  }

  // Generate the tutor prompt
  return generateTutorPrompt(exerciseInfo, tipsContent, lang, config);
}

/**
 * Initializes .estudio42 configuration
 */
function initializeConfig(cwd) {
  const projectRoot = findProjectRoot(cwd);
  const configDir = path.join(projectRoot, '.estudio42');
  const configPath = path.join(configDir, 'config.json');

  if (!fs.existsSync(configDir)) {
    fs.mkdirSync(configDir, { recursive: true });
  }

  let config;
  if (fs.existsSync(configPath)) {
    config = JSON.parse(fs.readFileSync(configPath, 'utf8'));
  } else {
    // Create default config
    config = {
      language: 'es',
      username: process.env.USER || 'student',
      created: new Date().toISOString()
    };
    fs.writeFileSync(configPath, JSON.stringify(config, null, 2), 'utf8');

    // Create progress.json
    const progressPath = path.join(configDir, 'progress.json');
    const progress = {
      sessions: [],
      total_hints: 0,
      exercises_completed: 0
    };
    fs.writeFileSync(progressPath, JSON.stringify(progress, null, 2), 'utf8');

    // Create tips directory
    const tipsDir = path.join(configDir, 'tips');
    if (!fs.existsSync(tipsDir)) {
      fs.mkdirSync(tipsDir, { recursive: true });
    }
  }

  return config;
}

/**
 * Finds project root (directory with .git)
 */
function findProjectRoot(startPath) {
  let currentPath = startPath;
  while (currentPath !== '/' && !fs.existsSync(path.join(currentPath, '.git'))) {
    currentPath = path.dirname(currentPath);
  }
  return currentPath;
}

/**
 * Gets the path where tips should be stored
 */
function getTipsPath(cwd, exerciseInfo) {
  const projectRoot = findProjectRoot(cwd);
  return path.join(
    projectRoot,
    '.estudio42',
    'tips',
    `rank-${exerciseInfo.rank}`,
    `level-${exerciseInfo.level}`,
    `${exerciseInfo.exercise}.md`
  );
}

/**
 * Generates tips from subject file
 */
async function generateTips(exerciseInfo, cwd, lang) {
  const projectRoot = findProjectRoot(cwd);
  const subjectPath = findSubjectFile(exerciseInfo, projectRoot);

  if (!subjectPath) {
    return null;
  }

  const subjectContent = fs.readFileSync(subjectPath, 'utf8');
  return generateTipsFromSubject(exerciseInfo, subjectContent, lang);
}

/**
 * Generates the main tutor prompt
 */
function generateTutorPrompt(exerciseInfo, tipsContent, lang, config) {
  const p = prompts[lang];

  let prompt = `
${p.welcome}

${p.currentExercise} ${exerciseInfo.exercise}
Rank: ${exerciseInfo.rank}
Level: ${exerciseInfo.level}

${p.rules}

${p.commands}

---

${p.tipsHeader}

${tipsContent || p.noTipsAvailable}

---

${p.sessionContext}
- ${p.userLabel} @${config.username}
- ${p.hintsUsedLabel} 0/7
- ${p.statusLabel} ${p.statusStarting}

${p.instructionsHeader}

${p.instructionsIntro}
${p.instructionsRules}

${p.initialQuestionLabel}
"${p.initialQuestion}"
`;

  return prompt;
}

/**
 * Generates prompt for non-exam repositories
 */
function generateNonExamPrompt(lang) {
  const p = prompts[lang];

  return `
${p.welcome}

${p.noExamStructure}

${p.rules}

${p.commands}

${p.nonExamIntro}

${p.nonExamOptionsHeader}
${p.nonExamOptions}

${p.nonExamPrompt}
`;
}

/**
 * Generates prompt when exercise couldn't be detected
 */
function generateManualPrompt(lang) {
  const p = prompts[lang];

  return `
${p.welcome}

${p.rules}

${p.commands}

${p.manualDetectionIntro}
${p.manualDetectionAsk}
`;
}

// Export for Claude Code skill system
module.exports = { execute };
