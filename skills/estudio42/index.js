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
  const p = prompts[lang];

  // Welcome message
  console.log(p.welcome);
  console.log(p.rules);
  console.log(p.commands);

  // Check if it's an exam repository
  if (!isExamRepository(cwd)) {
    console.log('\n' + p.noExamStructure);
    return generateNonExamPrompt(lang);
  }

  // Detect current exercise
  const detected = detectExercise(cwd);

  let exerciseInfo;
  if (detected) {
    // Confirm detection
    console.log('\n' + p.detected.replace('{exercise}',
      `${detected.exercise} (Rank ${detected.rank}, Level ${detected.level})`));
    exerciseInfo = detected;
  } else {
    // Ask user for exercise info
    console.log('\n' + p.notFound);
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
    console.log('\n' + p.generatingTips);
    tipsContent = await generateTips(exerciseInfo, cwd, lang);

    if (tipsContent) {
      // Save tips
      const tipsDir = path.dirname(tipsPath);
      if (!fs.existsSync(tipsDir)) {
        fs.mkdirSync(tipsDir, { recursive: true });
      }
      fs.writeFileSync(tipsPath, tipsContent, 'utf8');
      console.log(p.tipsGenerated);
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
    const p = prompts[lang];
    console.log('\n' + p.notFound);
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

Ejercicio actual: ${exerciseInfo.exercise}
Rank: ${exerciseInfo.rank}
Level: ${exerciseInfo.level}

${p.rules}

${p.commands}

---

TIPS Y GUÍA PARA ESTE EJERCICIO:

${tipsContent || 'No hay tips disponibles todavía.'}

---

CONTEXTO DE SESIÓN:
- Usuario: @${config.username}
- Nivel de hints usado: 0/7
- Estado: Iniciando ejercicio

INSTRUCCIONES PARA TI (CLAUDE):

Ahora actúas como tutor socrático de 42. Sigue ESTRICTAMENTE estas reglas:

1. ❌ NO escribas código automáticamente
2. ✅ Haz preguntas guía para que el usuario descubra la solución
3. ✅ Si el usuario dice "dame una pista", da la siguiente pista del nivel correspondiente (1-7)
4. ✅ Si el usuario dice "revisa mi código", lee su código y da feedback constructivo SIN dar la solución
5. ✅ Si el usuario dice "¿por qué este error?", analiza el error y explica qué significa, pero no des la solución directa
6. ✅ Si el usuario dice "añade mi tip: [texto]", añade el tip a su sección personal en el archivo de tips
7. ✅ Si el usuario dice "escríbelo tú", SOLO ENTONCES puedes escribir/editar código
8. ✅ Mantén tracking de cuántas pistas ha usado (muéstralo como "💡 Pista nivel X/7")
9. ✅ Cuando llegue a 7 pistas, pregunta si quiere que escribas el código

PREGUNTA INICIAL:
"¿Qué quieres hacer? ¿Entender el ejercicio, revisar tu código, o necesitas una pista?"
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

Estoy aquí para ayudarte en modo tutor socrático. ¿Qué necesitas?

Opciones:
- Entender parte del código
- Debuggear un problema
- Implementar una feature
- Revisar tu código

Dime en qué estás trabajando y cómo puedo guiarte.
`;
}

/**
 * Generates prompt when exercise couldn't be detected
 */
function generateManualPrompt(lang) {
  const p = prompts[lang];

  return `
${p.welcome}

No pude detectar automáticamente el ejercicio actual.

Por favor, indícame:
1. ¿En qué rank estás? (02/03/04/05/06)
2. ¿Qué nivel? (1/2/3/4)
3. ¿Qué ejercicio estás haciendo?

O si prefieres, comparte el subject del ejercicio y te ayudaré a generar los tips.
`;
}

// Export for Claude Code skill system
module.exports = { execute };
