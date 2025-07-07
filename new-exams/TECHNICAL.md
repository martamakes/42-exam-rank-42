# 🔧 TECHNICAL DOCUMENTATION

## 📁 Project Architecture

### Modular Design

The examshell system is built with a modular architecture for maintainability and scalability:

```
examshell/
├── examshell.h          # Main header with all prototypes and structures
├── main.c               # Entry point and argument parsing
├── ui.c                 # User interface and display functions
├── persistence.c        # Progress saving/loading system
├── exercises.c          # Exercise and rank management
├── exam_session.c       # Main exam execution logic
└── Makefile            # Modular compilation system
```

### Core Data Structures

```c
// Exercise representation
typedef struct s_exercise {
    char    *name;           // Exercise name (e.g., "filter")
    char    *path;           // Path to exercise directory
    int     completed;       // Completion status
    int     attempts;        // Number of attempts
    int     time_spent;      // Time spent on exercise
} t_exercise;

// Progress tracking
typedef struct s_progress_data {
    int     rank;                                    // Current exam rank
    int     level1_completed;                        // Level 1 completion status
    int     level2_completed;                        // Level 2 completion status
    int     current_level;                           // Current working level
    int     total_time;                              // Total time spent
    time_t  session_start;                           // Session start time
    char    completed_exercises[MAX_EXERCISES][MAX_NAME_LEN]; // Completed exercises list
    int     completed_count;                         // Number of completed exercises
} t_progress_data;

// Main session state
typedef struct s_exam_session {
    int             rank;                 // Exam rank (03, 04, 05)
    t_exercise      *current_exercise;    // Currently active exercise
    t_exercise      *level1_exercises;    // Level 1 exercise array
    t_exercise      *level2_exercises;    // Level 2 exercise array
    int             level1_count;         // Number of level 1 exercises
    int             level2_count;         // Number of level 2 exercises
    int             level1_completed;     // Level 1 completion flag
    int             level2_completed;     // Level 2 completion flag
    t_progress_data progress;             // Progress data
    time_t          session_start_time;   // Session start timestamp
} t_exam_session;
```

## 🔄 Program Flow

### 1. Initialization (main.c)
```
1. Parse command line arguments
2. Detect available exam ranks
3. Validate requested rank
4. Check for previous session or start new
5. Launch main exam session
```

### 2. Session Management (exam_session.c)
```
1. Load or initialize progress data
2. Load exercises from appropriate level directories
3. Select random exercise (avoiding completed ones)
4. Create necessary directories (subject/, rendu/ base)
5. Enter main menu loop
6. Handle user actions (grademe, next, quit, etc.)
7. Save progress on exit
```

### 3. Directory Management (ui.c)
```
subject/ directories:
- Created when exercise is selected
- Contains copy of subject.txt
- Cleaned up on program exit

rendu/ directories:
- Base directory created automatically
- Exercise subdirectories created manually by student
- Persists between sessions of same rank
- Cleaned only on rank change or manual reset
```

### 4. Persistence System (persistence.c)
```
.examshell_progress file format:
RANK=03
LEVEL1_COMPLETED=1
LEVEL2_COMPLETED=0
CURRENT_LEVEL=2
TOTAL_TIME=3456
COMPLETED_COUNT=2
EXERCISE=filter
EXERCISE=scanf
```

## 🛠️ Key Functions

### Exercise Loading (exercises.c)
```c
int load_exercises_from_level(const char *level_path, t_exercise **exercises);
```
- Scans directory for exercise subdirectories
- Allocates memory for exercise array
- Populates exercise structures with paths and names

### Progress Management (persistence.c)
```c
int load_progress(t_progress_data *progress, int rank);
int save_progress(t_exam_session *session);
```
- Handles loading/saving of session state
- Manages time tracking
- Maintains completed exercise list

### Directory Operations (ui.c)
```c
void create_subject_directory(const char *exercise_name, const char *exercise_path);
void cleanup_subject_directories(void);
void create_rendu_directory(void);
void cleanup_rendu_directory(void);
```
- Subject directories: temporary, cleaned on exit
- Rendu directories: persistent, cleaned only on rank change

## 🔍 Testing Strategy

### Unit Testing Areas
1. **Exercise loading** - Verify correct parsing of directory structure
2. **Progress persistence** - Test save/load cycles
3. **Directory management** - Verify creation/cleanup operations
4. **Session state** - Test state transitions and validation

### Integration Testing
1. **Full workflow** - Complete exercise session from start to finish
2. **Rank switching** - Verify proper cleanup and initialization
3. **Resume sessions** - Test continuation of interrupted sessions

### Manual Testing Checklist
```bash
# Basic functionality
./examshell 03           # Start rank 03
[g] (without rendu files) # Test grademe
[n]                      # Test next exercise
[r]                      # Test rendu listing
[s]                      # Test statistics
[q] and restart          # Test persistence

# Directory management
mkdir rendu/filter       # Create exercise directory
touch rendu/filter/filter.c  # Create solution file
[r]                      # Verify file listing
./examshell 04           # Test rank change (should clean rendu)

# Edge cases
./examshell 99           # Invalid rank
./examshell             # No arguments (interactive)
```

## 🚀 Future Enhancements

### Tester Integration
```c
// Future tester.c module
int run_tester(const char *exercise_name);
int compile_solution(const char *exercise_path);
int run_test_cases(const char *exercise_name);
```

### Network Features
```c
// Future network.c module
int submit_to_server(const char *exercise_name);
int download_new_exercises(int rank);
int check_leaderboard(void);
```

### Enhanced Statistics
```c
// Future stats.c module
void generate_detailed_report(t_progress_data *progress);
void export_progress_csv(t_progress_data *progress);
void show_time_analysis(t_progress_data *progress);
```

## 📝 Code Style Guidelines

### Naming Conventions
- **Functions:** `snake_case` (e.g., `load_exercises_from_level`)
- **Variables:** `snake_case` (e.g., `exercise_count`)
- **Structures:** `t_` prefix (e.g., `t_exercise`)
- **Constants:** `UPPER_CASE` (e.g., `MAX_EXERCISES`)

### Error Handling
- Use return codes (0 = success, 1 = error)
- Print user-friendly error messages
- Use `perror()` for system call errors
- Always free allocated memory on error paths

### Memory Management
- Use `malloc/free` explicitly (no libft)
- Always check malloc return values
- Free all allocated memory before exit
- Use valgrind for leak detection

## 🔧 Adding New Exercises

### Directory Structure
```
exam-rank-XX/
├── level-1/
│   └── new_exercise/
│       ├── subject.txt     # Exercise instructions
│       └── solution.c      # Reference solution (optional)
└── level-2/
    └── another_exercise/
        ├── subject.txt
        └── solution.c
```

### Subject.txt Format
```
Assignment name: exercise_name
Expected files: exercise_name.c
Allowed functions: read, write, malloc, free, printf, etc.
--------------------------------------------------------------------------------

[Exercise description and requirements]

Examples:
$> ./exercise_name arg1 arg2
expected_output$
```

### Integration Steps
1. Add exercise directory with `subject.txt`
2. Test with examshell system
3. Verify directory detection
4. Test exercise selection and display
5. Add to documentation

## 📊 Performance Considerations

### Memory Usage
- Exercise arrays allocated dynamically based on actual count
- Progress data kept minimal (no unnecessary caching)
- String operations use fixed buffers where possible

### File I/O
- Progress file written only when necessary
- Directory operations batched when possible
- Subject files read on-demand

### Startup Time
- Fast rank detection using filesystem scanning
- Lazy loading of exercise metadata
- Minimal initialization overhead

---

*This technical documentation is for developers contributing to or extending the examshell system.*
