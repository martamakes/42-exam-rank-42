const fs = require('fs');
const path = require('path');

/**
 * Finds the subject file for an exercise
 * @param {Object} exerciseInfo - {rank, level, exercise, path}
 * @param {string} projectRoot - Project root directory
 * @returns {string|null} - Path to subject file or null
 */
function findSubjectFile(exerciseInfo, projectRoot) {
  const possiblePaths = [
    // Pattern 1: 02/Level4/exercise/README.md
    path.join(projectRoot, exerciseInfo.rank, `Level${exerciseInfo.level}`, exerciseInfo.exercise, 'README.md'),
    // Pattern 2: 02/Level4/exercise/subject.txt
    path.join(projectRoot, exerciseInfo.rank, `Level${exerciseInfo.level}`, exerciseInfo.exercise, 'subject.txt'),
    // Pattern 3: 03/level-1/exercise/README.md
    path.join(projectRoot, exerciseInfo.rank, `level-${exerciseInfo.level}`, exerciseInfo.exercise, 'README.md'),
    // Pattern 4: 03/level-1/exercise/subject.txt
    path.join(projectRoot, exerciseInfo.rank, `level-${exerciseInfo.level}`, exerciseInfo.exercise, 'subject.txt'),
    // Pattern 5: In exercise path directly
    path.join(exerciseInfo.path, 'README.md'),
    path.join(exerciseInfo.path, 'subject.txt'),
    // Pattern 6: One level up
    path.join(path.dirname(exerciseInfo.path), 'README.md'),
    path.join(path.dirname(exerciseInfo.path), 'subject.txt'),
  ];

  for (const filePath of possiblePaths) {
    if (fs.existsSync(filePath)) {
      return filePath;
    }
  }

  return null;
}

/**
 * Generates tip file content from subject
 * @param {Object} exerciseInfo - {rank, level, exercise, path}
 * @param {string} subjectContent - Content of the subject file
 * @param {string} language - 'es' or 'en'
 * @returns {string} - Generated markdown content
 */
function generateTipsFromSubject(exerciseInfo, subjectContent, language = 'es') {
  const templates = {
    es: {
      title: `# ${exerciseInfo.exercise}`,
      subjectSection: '\n## 📋 Subject (Auto-extraído)\n\n',
      tipsSection: '\n## 💡 Tips Generales (Auto-generados)\n\n',
      conceptsLabel: '**Conceptos clave detectados:**\n',
      resourcesLabel: '\n**Recursos útiles:**\n',
      complexityLabel: '\n**Complejidad:**',
      allowedLabel: '\n**Allowed functions:**',
      approachSection: '\n## 🎯 Enfoque Sugerido (7 Niveles de Pistas)\n\n',
      userSection: '\n## 👤 Tips de Usuarios\n\n',
      waitingTip: '[Esperando tu primer tip personal]\n\n',
      collaborative: '[Otros usuarios añadirán sus tips aquí sin borrar los tuyos]\n'
    },
    en: {
      title: `# ${exerciseInfo.exercise}`,
      subjectSection: '\n## 📋 Subject (Auto-extracted)\n\n',
      tipsSection: '\n## 💡 General Tips (Auto-generated)\n\n',
      conceptsLabel: '**Key concepts detected:**\n',
      resourcesLabel: '\n**Useful resources:**\n',
      complexityLabel: '\n**Complexity:**',
      allowedLabel: '\n**Allowed functions:**',
      approachSection: '\n## 🎯 Suggested Approach (7 Hint Levels)\n\n',
      userSection: '\n## 👤 User Tips\n\n',
      waitingTip: '[Waiting for your first personal tip]\n\n',
      collaborative: '[Other users will add their tips here without deleting yours]\n'
    }
  };

  const t = templates[language];
  let content = t.title;

  // Add subject section
  content += t.subjectSection;
  content += '```\n' + subjectContent.trim() + '\n```\n';

  // Extract information from subject
  const concepts = extractConcepts(subjectContent);
  const allowedFunctions = extractAllowedFunctions(subjectContent);
  const resources = generateResources(concepts, allowedFunctions);
  const complexity = estimateComplexity(exerciseInfo, subjectContent);

  // Add tips section
  content += t.tipsSection;
  content += t.conceptsLabel;
  concepts.forEach(concept => {
    content += `- ${concept}\n`;
  });

  content += t.resourcesLabel;
  resources.forEach(resource => {
    content += `- ${resource}\n`;
  });

  content += t.complexityLabel + ` ${complexity}\n`;

  if (allowedFunctions.length > 0) {
    content += t.allowedLabel + ` ${allowedFunctions.join(', ')}\n`;
  }

  // Add approach section with hint levels template
  content += t.approachSection;
  content += generateHintLevelsTemplate(exerciseInfo, subjectContent, language);

  // Add user tips section
  content += t.userSection;
  content += t.waitingTip;
  content += t.collaborative;

  return content;
}

/**
 * Extracts key concepts from subject
 */
function extractConcepts(subject) {
  const concepts = [];
  const conceptMap = {
    'pipe': 'Pipes (comunicación entre procesos)',
    'fork': 'Fork (crear proceso hijo)',
    'dup2': 'Redirección de file descriptors',
    'exec': 'Ejecución de comandos',
    'malloc': 'Asignación dinámica de memoria',
    'linked list': 'Listas enlazadas',
    'recursion': 'Recursión',
    'string': 'Manipulación de strings',
    'pointer': 'Punteros',
    'array': 'Arrays',
    'loop': 'Iteración',
    'write': 'Salida estándar (write)',
    'read': 'Lectura de archivos/entrada',
    'file descriptor': 'File descriptors'
  };

  const lowerSubject = subject.toLowerCase();
  for (const [keyword, concept] of Object.entries(conceptMap)) {
    if (lowerSubject.includes(keyword)) {
      concepts.push(concept);
    }
  }

  return concepts.length > 0 ? concepts : ['A determinar según el subject'];
}

/**
 * Extracts allowed functions from subject
 */
function extractAllowedFunctions(subject) {
  const match = subject.match(/Allowed functions?\s*:\s*([^\n]+)/i);
  if (match) {
    return match[1].split(',').map(f => f.trim()).filter(f => f && f !== 'None');
  }
  return [];
}

/**
 * Generates resource recommendations
 */
function generateResources(concepts, allowedFunctions) {
  const resources = [];

  allowedFunctions.forEach(func => {
    if (func !== 'write' && func !== 'malloc' && func !== 'free') {
      resources.push(`\`man ${func}\``);
    }
  });

  // Add special recommendations
  if (allowedFunctions.includes('pipe')) {
    resources.push('`man pipe` ⭐ (tiene función de ejemplo útil)');
  }

  if (resources.length === 0) {
    resources.push('man pages relevantes según funciones usadas');
  }

  return resources;
}

/**
 * Estimates complexity based on exercise info
 */
function estimateComplexity(exerciseInfo, subject) {
  const level = parseInt(exerciseInfo.level);

  if (level <= 1) return 'Baja';
  if (level === 2) return 'Media';
  if (level === 3) return 'Media-Alta';
  if (level >= 4) return 'Alta';

  // Check subject content
  if (subject.toLowerCase().includes('pipe') && subject.toLowerCase().includes('fork')) {
    return 'Alta';
  }

  return 'Media';
}

/**
 * Generates hint levels template
 */
function generateHintLevelsTemplate(exerciseInfo, subject, language = 'es') {
  const templates = {
    es: {
      level1: '### Nivel 1: Pregunta Diagnóstica\n- ¿Qué hace exactamente esta función?\n- ¿Qué parámetros recibe y qué debe devolver?\n- ¿Cuál es el objetivo principal del ejercicio?\n\n',
      level2: '### Nivel 2: Pregunta Más Específica\n- ¿Qué estructura de datos necesitas?\n- ¿Qué casos especiales debes manejar?\n- ¿Hay algún patrón conocido que puedas aplicar?\n\n',
      level3: '### Nivel 3: Concepto Clave\n```\n[Los conceptos clave se añadirán cuando se generen hints específicos]\n```\n\n',
      level4: '### Nivel 4: Estrategia\n```\n1. [Paso 1]\n2. [Paso 2]\n3. [Paso 3]\n...\n```\n\n',
      level5: '### Nivel 5: Pseudocódigo\n```\n[Pseudocódigo se añadirá cuando se generen hints específicos]\n```\n\n',
      level6: '### Nivel 6: Código Parcial\n```c\n// Estructura básica\n// Tu implementación aquí\n```\n\n',
      level7: '### Nivel 7: Análisis de Edge Cases\n```\n⚠️ Errores comunes:\n1. [Error común 1]\n2. [Error común 2]\n\n✅ Checklist:\n- [ ] [Verificación 1]\n- [ ] [Verificación 2]\n```\n\n'
    },
    en: {
      level1: '### Level 1: Diagnostic Question\n- What exactly does this function do?\n- What parameters does it receive and what should it return?\n- What is the main goal of the exercise?\n\n',
      level2: '### Level 2: More Specific Question\n- What data structure do you need?\n- What special cases must you handle?\n- Is there a known pattern you can apply?\n\n',
      level3: '### Level 3: Key Concept\n```\n[Key concepts will be added when specific hints are generated]\n```\n\n',
      level4: '### Level 4: Strategy\n```\n1. [Step 1]\n2. [Step 2]\n3. [Step 3]\n...\n```\n\n',
      level5: '### Level 5: Pseudocode\n```\n[Pseudocode will be added when specific hints are generated]\n```\n\n',
      level6: '### Level 6: Partial Code\n```c\n// Basic structure\n// Your implementation here\n```\n\n',
      level7: '### Level 7: Edge Cases Analysis\n```\n⚠️ Common errors:\n1. [Common error 1]\n2. [Common error 2]\n\n✅ Checklist:\n- [ ] [Check 1]\n- [ ] [Check 2]\n```\n\n'
    }
  };

  const t = templates[language];
  return t.level1 + t.level2 + t.level3 + t.level4 + t.level5 + t.level6 + t.level7;
}

/**
 * Adds user tip to existing tips file
 */
function addUserTip(tipsFilePath, username, tipText) {
  const content = fs.readFileSync(tipsFilePath, 'utf8');
  const date = new Date().toISOString().split('T')[0];

  // Check if user section exists
  const userSectionRegex = new RegExp(`### @${username} \\([^)]+\\)`, 'g');
  const hasUserSection = userSectionRegex.test(content);

  let newContent;
  if (hasUserSection) {
    // Add to existing user section
    newContent = content.replace(
      userSectionRegex,
      `### @${username} (${date})`
    );
    const lines = newContent.split('\n');
    const sectionIndex = lines.findIndex(line => line.includes(`### @${username}`));
    lines.splice(sectionIndex + 1, 0, `- ${tipText}`);
    newContent = lines.join('\n');
  } else {
    // Create new user section
    const userSectionMarker = '## 👤 Tips de Usuarios';
    const insertPosition = content.indexOf(userSectionMarker) + userSectionMarker.length;
    const beforeInsert = content.substring(0, insertPosition);
    const afterInsert = content.substring(insertPosition);

    newContent = beforeInsert + `\n\n### @${username} (${date})\n- ${tipText}\n` + afterInsert;
  }

  fs.writeFileSync(tipsFilePath, newContent, 'utf8');
}

module.exports = {
  findSubjectFile,
  generateTipsFromSubject,
  addUserTip
};
