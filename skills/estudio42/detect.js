const fs = require('fs');
const path = require('path');

/**
 * Detects the current exercise based on repository structure
 * @param {string} cwd - Current working directory
 * @returns {Object|null} - {rank, level, exercise, path} or null
 */
function detectExercise(cwd) {
  // Strategy 1: Check if we're inside a rendu/ directory
  if (cwd.includes('/rendu')) {
    const match = cwd.match(/(\d+)\/(rendu\d*|Level\d+)\//);
    if (match) {
      const rank = match[1].padStart(2, '0');
      const exerciseName = path.basename(cwd);
      return {
        rank,
        level: extractLevel(cwd),
        exercise: exerciseName,
        path: cwd
      };
    }
  }

  // Strategy 2: Look for recently modified .c files in rendu directories
  const renduDirs = findRenduDirectories(cwd);
  if (renduDirs.length > 0) {
    const mostRecent = getMostRecentlyModified(renduDirs);
    if (mostRecent) {
      return {
        rank: mostRecent.rank,
        level: mostRecent.level,
        exercise: mostRecent.exercise,
        path: mostRecent.path
      };
    }
  }

  // Strategy 3: Check if we're in an exercise directory
  const exerciseInfo = detectExerciseDirectory(cwd);
  if (exerciseInfo) {
    return exerciseInfo;
  }

  return null;
}

/**
 * Extracts level number from path
 */
function extractLevel(pathStr) {
  const levelMatch = pathStr.match(/[Ll]evel[_-]?(\d+)|level[_-]?(\d+)/);
  if (levelMatch) {
    return levelMatch[1] || levelMatch[2];
  }
  return null;
}

/**
 * Finds all rendu directories in the project
 */
function findRenduDirectories(startPath) {
  const results = [];
  const ranks = ['02', '03', '04', '05', '06'];

  // Go up to project root
  let projectRoot = startPath;
  while (projectRoot !== '/' && !fs.existsSync(path.join(projectRoot, '.git'))) {
    projectRoot = path.dirname(projectRoot);
  }

  ranks.forEach(rank => {
    const rankPath = path.join(projectRoot, rank);
    if (fs.existsSync(rankPath)) {
      findRenduInRank(rankPath, rank, results);
    }
  });

  return results;
}

/**
 * Recursively finds rendu directories in a rank
 */
function findRenduInRank(rankPath, rank, results) {
  try {
    const entries = fs.readdirSync(rankPath, { withFileTypes: true });

    entries.forEach(entry => {
      if (entry.isDirectory()) {
        const fullPath = path.join(rankPath, entry.name);

        if (entry.name.startsWith('rendu')) {
          // Found a rendu directory
          const level = extractLevel(path.dirname(fullPath));
          results.push({
            rank,
            level,
            exercise: entry.name,
            path: fullPath
          });
        } else if (entry.name.match(/[Ll]evel/)) {
          // Recurse into Level directories
          findRenduInRank(fullPath, rank, results);
        }
      }
    });
  } catch (err) {
    // Ignore permission errors
  }
}

/**
 * Gets the most recently modified directory based on .c files
 */
function getMostRecentlyModified(directories) {
  let mostRecent = null;
  let mostRecentTime = 0;

  directories.forEach(dir => {
    try {
      const files = fs.readdirSync(dir.path);
      const cFiles = files.filter(f => f.endsWith('.c') || f.endsWith('.h'));

      cFiles.forEach(file => {
        const filePath = path.join(dir.path, file);
        const stats = fs.statSync(filePath);
        if (stats.mtimeMs > mostRecentTime) {
          mostRecentTime = stats.mtimeMs;
          mostRecent = dir;
        }
      });
    } catch (err) {
      // Ignore errors
    }
  });

  return mostRecent;
}

/**
 * Detects if current directory is an exercise directory
 */
function detectExerciseDirectory(cwd) {
  // Check if we're in a structure like: 02/Level4/exercise_name
  const match = cwd.match(/\/(\d+)\/(Level\d+|level-\d+)\/([^\/]+)$/);
  if (match) {
    return {
      rank: match[1].padStart(2, '0'),
      level: match[2].replace(/[^\d]/g, ''),
      exercise: match[3],
      path: cwd
    };
  }
  return null;
}

/**
 * Checks if this is a 42 exam repository
 */
function isExamRepository(cwd) {
  // Go up to project root
  let projectRoot = cwd;
  while (projectRoot !== '/' && !fs.existsSync(path.join(projectRoot, '.git'))) {
    projectRoot = path.dirname(projectRoot);
  }

  // Check for typical exam structure
  const ranks = ['02', '03', '04', '05', '06'];
  const hasExamStructure = ranks.some(rank =>
    fs.existsSync(path.join(projectRoot, rank))
  );

  return hasExamStructure;
}

module.exports = {
  detectExercise,
  isExamRepository,
  extractLevel
};
