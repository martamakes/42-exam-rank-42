# 🎓 EXAMSHELL - 42 School Exam Practice System

> **Complete simulator of the real 42 School examshell environment**

A **modular** practice system that replicates exactly the experience of the official 42 examshell, with complete persistence, advanced statistics, and workflow identical to the real exam.

![42 Logo](https://42.fr/wp-content/uploads/2021/05/42-Final-sigle-seul.svg)

## 📋 Key Features

- ✅ **100% authentic experience** - Workflow identical to 42's examshell
- ✅ **Authentic directory system** - `subject/` and `rendu/` like in real exam
- ✅ **Complete persistence** - Continue exactly where you left off
- ✅ **Multiple ranks** - Exam-rank-03, 04, 05 (different difficulty levels)
- ✅ **Advanced statistics** - Time tracking, progress, completed exercises
- ✅ **No automation** - Student must create directories and files manually
- ✅ **Tester-ready** - Structure ready for automatic testing

## 🚀 Quick Installation

### Prerequisites

- **GCC compiler** (usually pre-installed on 42 machines)
- **Make** (build system)
- **Unix/Linux environment** (macOS, Linux, or WSL on Windows)

### Installation Steps

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd examshell
   ```

2. **Compile the system:**
   ```bash
   make
   ```

3. **Verify installation:**
   ```bash
   ./examshell --help
   ```

4. **Optional - Install globally:**
   ```bash
   make install  # Requires sudo
   # Now you can use 'examshell' from anywhere
   ```

## 🎮 Getting Started

### Basic Usage

```bash
# Start with a specific rank
./examshell 03  # Exam Rank 03 (Intermediate)
./examshell 04  # Exam Rank 04 (Advanced)
./examshell 05  # Exam Rank 05 (Expert)

# Interactive rank selection
./examshell
```

### First Time Setup

When you first run the system:

1. **Choose your exam rank** (03, 04, or 05)
2. **System creates base directories** automatically
3. **You get assigned a random exercise** from level 1
4. **Read instructions** in `subject/[exercise-name]/subject.txt`
5. **Create your solution** manually in `rendu/[exercise-name]/`

## 📖 Complete Workflow Example

### Step 1: Start the Exam
```bash
./examshell 03
```

### Step 2: System Setup (Automatic)
```
=====================================
        42 SCHOOL - EXAMSHELL        
         Practice Environment        
=====================================

EXAM MODE: 1 exercise per level (like real exam)
Status: Working on level 1 exercise

📚 Current Exercise:
   Name: filter
   Level: 1

💻 EXAMSHELL WORKFLOW (like real exam):
  Terminal 1: This examshell (control)
  Terminal 2: cat subject/filter/subject.txt (read instructions)
  Terminal 3: Code your solution

✅ Subject: subject/filter/subject.txt
✅ Submit to: rendu/filter/
    (Create this directory and put your .c and .h files here)
```

### Step 3: Read the Subject (New Terminal)
```bash
# Open a new terminal and read the exercise instructions
cat subject/filter/subject.txt
```

### Step 4: Code Your Solution (New Terminal)
```bash
# Create the rendu directory for your exercise
mkdir rendu/filter

# Code your solution
vim rendu/filter/filter.c

# Compile and test
gcc rendu/filter/filter.c -o filter
echo "test input" | ./filter "pattern"
```

### Step 5: Navigate the Exam (Original Terminal)
```
Available options:
  [t] Run tester (coming soon)
  [g] Grademe - Mark as passed
  [n] Next exercise (same level)
  [r] Show rendu files
  [s] Show statistics
  [q] Quit (saves progress)

Enter your choice: r
```

Example output when checking files:
```
📁 Files in rendu/filter/:
  - filter.c ✓
  - filter.h ✓
  Total: 2 files

Reminder: You must manually create rendu/filter/ and put your files there
```

### Step 6: Submit Your Solution
```
Enter your choice: g

✅ Level 1 completed! Well done!
Moving to level 2...
```

## 🎯 Available Exercises

### Exam Rank 03 (Intermediate Level)
- **Level 1:** `filter`, `scanf`, `broken_gnl`
- **Level 2:** `n_queens`, `permutations`, `powerset`, `rip`, `tsp`

### Exam Rank 04 (Advanced Level)
- **Level 1:** `ft_popen`, `picoshell`, `sandbox`
- **Level 2:** `argo`, `vbc`

### Exam Rank 05 (Expert Level)
- **Level 1:** `string_bigint`, `vect2`
- **Level 2:** `bsq`, `game_of_life`

## 📁 Directory Structure

After running the system, you'll have:

```
examshell/
├── rendu/                     ← YOUR SOLUTIONS (persists between sessions)
│   ├── filter/               ← Create manually
│   │   ├── filter.c         ← Your solution
│   │   └── filter.h         ← Your headers (if needed)
│   └── [other-exercises]/
├── subject/                   ← EXERCISE INSTRUCTIONS (cleaned on exit)
│   └── filter/
│       └── subject.txt       ← Read this for instructions
├── .examshell_progress       ← YOUR PROGRESS (automatic)
└── examshell                 ← The main program
```

## 🎮 Menu Options Explained

| Option | Description |
|--------|-------------|
| `[t]` | **Run tester** - Automatic testing (coming soon) |
| `[g]` | **Grademe** - Mark current exercise as passed, advance to next level |
| `[n]` | **Next exercise** - Get a different exercise from the same level |
| `[r]` | **Show rendu files** - List your current solution files |
| `[s]` | **Show statistics** - Display progress, time, completed exercises |
| `[q]` | **Quit** - Save progress and exit (can resume later) |

## 📊 Progress System

The system tracks:
- ✅ **Current rank and level**
- ✅ **Completed exercises**
- ✅ **Session time and total time**
- ✅ **Overall completion percentage**

Example statistics:
```
=== EXAM STATISTICS ===
Exam Rank: 03
Current Level: 2

Progress:
  Level 1: ✅ COMPLETED
  Level 2: ❌ Not completed

Exercises completed: 2
  filter, scanf

Time tracking:
  Session time: 00:23:45
  Total time: 01:47:23
  Overall progress: 50%
======================
```

## 🔧 Advanced Features

### Session Management

- **Continue previous session:** Pick up exactly where you left off
- **Start new session:** Reset progress and clean `rendu/` directory
- **Change ranks:** Automatically detects rank changes and cleans directories

### File Management

- **Automatic cleanup:** `subject/` directories are cleaned when you quit
- **Persistence:** `rendu/` directories persist between sessions of the same rank
- **Manual control:** You decide when to reset or change ranks

## 🛠️ Build System

### Available Make Targets

```bash
make              # Compile the project
make clean        # Remove object files
make fclean       # Remove all generated files
make re           # Recompile everything
make install      # Install globally (requires sudo)
make uninstall    # Remove global installation
make test         # Run basic system tests
make help         # Show detailed help
```

### Debug Build

```bash
make debug        # Compile with debug flags and AddressSanitizer
```

## 🔍 Troubleshooting

### Common Issues

**"No exam ranks found" error:**
```bash
# Check that you have the exercise directories
ls -la exam-rank-*
# Should show: exam-rank-03, exam-rank-04, exam-rank-05
```

**Compilation errors:**
```bash
# Clean and recompile
make fclean && make
```

**Permission errors:**
```bash
# Make sure files are executable
chmod +x examshell
```

**Progress not saving:**
```bash
# Check if you have write permissions in the directory
ls -la .examshell_progress
```

### Getting Help

1. **Built-in help:** `./examshell --help`
2. **Make help:** `make help`
3. **Check structure:** `make structure`
4. **Run tests:** `make test`

## 📚 Study Tips

1. **🎯 Practice realistically** - Use 3 terminals like in real exam
2. **⏰ Time yourself** - The system tracks your time
3. **📖 Read subjects carefully** - Use `cat subject/[exercise]/subject.txt`
4. **🧪 Test thoroughly** - Compile and test your solutions
5. **💾 Don't rely on libft** - Implement everything from scratch
6. **🔍 Handle edge cases** - Empty input, malloc failures, etc.

## 🎯 Real Exam Preparation

This system prepares you for:
- **Time pressure** - Complete exercises under time constraints
- **Manual setup** - No automation, you create everything manually
- **Real workflow** - Multiple terminals, directory navigation
- **Authentic structure** - Exact same `subject/` and `rendu/` system
- **Progress tracking** - Understand your completion rate and time usage

## 📈 Contributing

This is an educational tool for 42 students. Contributions welcome:

1. **Add more exercises** to existing ranks
2. **Implement tester system** for automatic validation
3. **Improve user experience** while maintaining authenticity
4. **Bug fixes** and optimizations

## 📄 License

This project is educational software for 42 School students. Use responsibly and in accordance with 42's academic integrity policies.

---

## 🚀 Ready to Practice?

```bash
# Clone, compile, and start practicing!
git clone <repository-url>
cd examshell
make
./examshell 03

# Good luck! 🍀
```

**Remember:** This is practice, but treat it like the real exam. The more realistically you practice, the better prepared you'll be! 🎓

---

*Made with ❤️ for the 42 community*
