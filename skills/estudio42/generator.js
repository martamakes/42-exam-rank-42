const fs = require('fs');
const path = require('path');

/**
 * Finds the subject file for an exercise
 * @param {Object} exerciseInfo - {rank, level, exercise, path}
 * @param {string} projectRoot - Project root directory
 * @returns {string|null} - Path to subject file or null
 */
function findSubjectFile(exerciseInfo, projectRoot) {
  // Candidate "given" directories for this exercise, covering both naming
  // styles seen in this repo (02/Level4/... vs 03+/level-1/...), plus the
  // exercise path itself and its parent (covers exerciseInfo.path values
  // that already point at the given dir, e.g. from Strategy 3 detection).
  const candidateDirs = [
    path.join(projectRoot, exerciseInfo.rank, `Level${exerciseInfo.level}`, exerciseInfo.exercise),
    path.join(projectRoot, exerciseInfo.rank, `level-${exerciseInfo.level}`, exerciseInfo.exercise),
    exerciseInfo.path,
    path.dirname(exerciseInfo.path),
  ];

  // Known filenames, checked in priority order, per directory.
  const candidateNames = ['README.md', 'subject.txt', 'subject.en.txt'];

  for (const dir of candidateDirs) {
    for (const name of candidateNames) {
      const filePath = path.join(dir, name);
      if (fs.existsSync(filePath)) {
        return filePath;
      }
    }

    // Fallback: any subject*.txt in the directory — covers subject.en.txt
    // variants not in candidateNames without hardcoding every language.
    if (fs.existsSync(dir)) {
      try {
        const match = fs.readdirSync(dir).find((f) => /^subject.*\.txt$/i.test(f));
        if (match) {
          return path.join(dir, match);
        }
      } catch (err) {
        // Ignore unreadable directories and keep trying other candidates.
      }
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
      testSection: '\n## 🧪 Casos de Prueba del Subject\n\n',
      testGuidance: `
**Cuando digas "ayúdame con el main", te guiaré con preguntas:**
- ¿Qué casos del subject necesitas probar?
- ¿Qué output esperas para cada caso?
- ¿Qué funciones auxiliares necesitas?

Solo prueba lo que el subject pide. No inventes edge cases adicionales.
`,
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
      testSection: '\n## 🧪 Subject Test Cases\n\n',
      testGuidance: `
**When you say "help me with the main", I'll guide you with questions:**
- What cases from the subject do you need to test?
- What output do you expect for each case?
- What helper functions do you need?

Only test what the subject asks. Don't invent additional edge cases.
`,
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
  const concepts = extractConcepts(subjectContent, language);
  const allowedFunctions = extractAllowedFunctions(subjectContent);
  const resources = generateResources(allowedFunctions, language);
  const complexity = estimateComplexity(exerciseInfo, subjectContent, language);

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

  // Add test cases section
  content += t.testSection;
  content += extractTestCases(subjectContent, language);
  content += t.testGuidance;

  // Add user tips section
  content += t.userSection;
  content += t.waitingTip;
  content += t.collaborative;

  return content;
}

/**
 * Extracts key concepts from subject
 */
function extractConcepts(subject, language = 'es') {
  const conceptMaps = {
    es: {
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
    },
    en: {
      'pipe': 'Pipes (inter-process communication)',
      'fork': 'Fork (create a child process)',
      'dup2': 'File descriptor redirection',
      'exec': 'Command execution',
      'malloc': 'Dynamic memory allocation',
      'linked list': 'Linked lists',
      'recursion': 'Recursion',
      'string': 'String manipulation',
      'pointer': 'Pointers',
      'array': 'Arrays',
      'loop': 'Iteration',
      'write': 'Standard output (write)',
      'read': 'Reading files/input',
      'file descriptor': 'File descriptors'
    }
  };
  const conceptMap = conceptMaps[language] || conceptMaps.es;
  const fallback = language === 'en'
    ? ['To be determined from the subject']
    : ['A determinar según el subject'];

  const concepts = [];
  const lowerSubject = subject.toLowerCase();
  for (const [keyword, concept] of Object.entries(conceptMap)) {
    if (lowerSubject.includes(keyword)) {
      concepts.push(concept);
    }
  }

  return concepts.length > 0 ? concepts : fallback;
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
function generateResources(allowedFunctions, language = 'es') {
  const resources = [];

  allowedFunctions.forEach(func => {
    if (func !== 'write' && func !== 'malloc' && func !== 'free') {
      resources.push(`\`man ${func}\``);
    }
  });

  // Add special recommendations
  if (allowedFunctions.includes('pipe')) {
    resources.push(language === 'en'
      ? '`man pipe` ⭐ (has a useful example function)'
      : '`man pipe` ⭐ (tiene función de ejemplo útil)');
  }

  if (resources.length === 0) {
    resources.push(language === 'en'
      ? 'relevant man pages depending on the functions used'
      : 'man pages relevantes según funciones usadas');
  }

  return resources;
}

/**
 * Estimates complexity based on exercise info
 */
function estimateComplexity(exerciseInfo, subject, language = 'es') {
  const labels = language === 'en'
    ? { low: 'Low', medium: 'Medium', mediumHigh: 'Medium-High', high: 'High' }
    : { low: 'Baja', medium: 'Media', mediumHigh: 'Media-Alta', high: 'Alta' };

  const level = parseInt(exerciseInfo.level, 10);

  if (level <= 1) return labels.low;
  if (level === 2) return labels.medium;
  if (level === 3) return labels.mediumHigh;
  if (level >= 4) return labels.high;

  // Check subject content
  if (subject.toLowerCase().includes('pipe') && subject.toLowerCase().includes('fork')) {
    return labels.high;
  }

  return labels.medium;
}

/**
 * Extracts test cases from subject
 */
function extractTestCases(subject, language = 'es') {
  const templates = {
    es: {
      found: 'El subject muestra estos ejemplos:\n\n',
      notFound: 'Busca la sección de ejemplos en el subject.\n\n'
    },
    en: {
      found: 'The subject shows these examples:\n\n',
      notFound: 'Look for the examples section in the subject.\n\n'
    }
  };

  const t = templates[language];

  // Look for examples section
  const examplesMatch = subject.match(/(?:Examples?|For example)[:\s]+([\s\S]*?)(?=\n\n|Hints?:|$)/i);

  if (examplesMatch) {
    const examples = examplesMatch[1].trim();
    return t.found + '```\n' + examples + '\n```\n\n';
  }

  // Look for code blocks that might be examples
  const codeBlockMatch = subject.match(/```[\s\S]*?```|(?:int\s+main|void\s+\w+)\s*\([^)]*\)\s*{[\s\S]*?}/g);

  if (codeBlockMatch && codeBlockMatch.length > 0) {
    return t.found + codeBlockMatch.join('\n\n') + '\n\n';
  }

  return t.notFound;
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
