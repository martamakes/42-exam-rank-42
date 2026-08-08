const fs = require('fs');
const path = require('path');

/**
 * Walks up from startPath to the nearest ancestor containing .git.
 */
function findProjectRoot(startPath) {
  let currentPath = startPath;
  while (currentPath !== '/' && !fs.existsSync(path.join(currentPath, '.git'))) {
    currentPath = path.dirname(currentPath);
  }
  return currentPath;
}

/**
 * Cross-references an exercise name (as found under rendu/<name>) against
 * every known "given" directory (e.g. <rank>/Level4/<name> or
 * <rank>/level-2/<name>) to recover its rank and level — rendu/<name>
 * alone carries no rank/level info.
 * @returns {{rank: string, level: string|null, path: string}|null}
 */
function findGivenInfoByExerciseName(exerciseName, projectRoot) {
  const ranks = ['02', '03', '04', '05', '06'];

  for (const rank of ranks) {
    const rankPath = path.join(projectRoot, rank);
    if (!fs.existsSync(rankPath)) {
      continue;
    }

    let levelEntries;
    try {
      levelEntries = fs.readdirSync(rankPath, { withFileTypes: true });
    } catch (err) {
      continue;
    }

    for (const levelEntry of levelEntries) {
      if (!levelEntry.isDirectory() || !/[Ll]evel/.test(levelEntry.name)) {
        continue;
      }

      const exercisePath = path.join(rankPath, levelEntry.name, exerciseName);
      if (fs.existsSync(exercisePath) && fs.statSync(exercisePath).isDirectory()) {
        return {
          rank,
          level: extractLevel(levelEntry.name),
          path: exercisePath
        };
      }
    }
  }

  return null;
}

/**
 * Detects the exercise when cwd is inside the shared, root-level rendu/
 * live-practice workspace (rendu/<exercise>/...) — NOT nested under any
 * rank folder. See 05/CONTEXT.md for the rendu vs rendu<N> distinction.
 * This is the primary case: it's where the student actually writes code.
 */
function detectFromRenduWorkspace(cwd, projectRoot) {
  const renduRoot = path.join(projectRoot, 'rendu');
  const relative = path.relative(renduRoot, cwd);

  if (!relative || relative.startsWith('..') || path.isAbsolute(relative)) {
    return null;
  }

  const exerciseName = relative.split(path.sep)[0];
  if (!exerciseName) {
    return null;
  }

  const givenInfo = findGivenInfoByExerciseName(exerciseName, projectRoot);
  if (!givenInfo) {
    return null;
  }

  return {
    rank: givenInfo.rank,
    level: givenInfo.level,
    exercise: exerciseName,
    path: path.join(renduRoot, exerciseName)
  };
}

/**
 * Detects the current exercise based on repository structure
 * @param {string} cwd - Current working directory
 * @returns {Object|null} - {rank, level, exercise, path} or null
 */
function detectExercise(cwd) {
  const projectRoot = findProjectRoot(cwd);

  // Strategy 1: cwd is inside the root-level rendu/<exercise>/ live-practice
  // workspace — the primary, real-world case (see 05/CONTEXT.md).
  const fromRendu = detectFromRenduWorkspace(cwd, projectRoot);
  if (fromRendu) {
    return fromRendu;
  }

  // Strategy 2: Look for recently modified .c files inside <rank>/rendu*
  // archive directories, if any exist directly under a rank folder.
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

  // Strategy 3: Check if we're in a given exercise directory, e.g.
  // 02/Level4/flood_fill or 05/level-2/bsq.
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
  const projectRoot = findProjectRoot(startPath);

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
  const projectRoot = findProjectRoot(cwd);

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
