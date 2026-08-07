# 🎓 42 Exam Practice - UNIFIED SYSTEM 🚀

Complete and integrated system to practice **ALL** 42 Exam Ranks! A comprehensive exam preparation tool with exercises for ranks 02 through 06.

## ⚡ **Quick Start**

```bash
# 🚀 UNIFIED ACCESS TO ALL RANKS
./exam_master.sh
```

## 🎯 **Main Features**

**🔧 Exam System (02, 03, 04, 05, 06):**
- Complete exam simulation environment
- Step-by-step guided menus
- Progressive levels for each rank
- Automatic tests with validation
- Real exam-like workflow

**🎯 Key Features:**
- Unified access to ALL exam ranks (02-06)
- Automatic detection of available systems
- Direct access by exam rank
- Progress tracking per level

## 📋 Requirements

- Unix/Linux or macOS operating system
- GCC compiler
- Make (optional, for additional features)

## 🛠 **Quick Installation**

```bash
# Clone the repository
git clone https://github.com/martamakes/42-exam-rank-42.git

# Enter the directory
cd 42-exam-rank-42

# 🚀 START EXAM SYSTEM
./exam_master.sh
```

## 🎮 **Usage**

### **Recommended Method:**
```bash
./exam_master.sh      # Access to ALL exam ranks (02-06)
```

### **Direct Access to Specific Rank:**
```bash
cd 02 && ./exam.sh    # Rank 02
cd 03 && ./exam.sh    # Rank 03
cd 04 && ./exam.sh    # Rank 04
cd 05 && ./exam.sh    # Rank 05
cd 06 && ./exam.sh    # Rank 06
```

### **Alternative Method:**
```bash
make                  # Equivalent, via Makefile
make run_exam_02      # Run only Rank 02
```

> Note: `new-exams/` (an EXAMSHELL prototype, only `exam-rank-03/` actually exists there) and `legacy/` (older abandoned attempts `03-old`/`04-old`/`05-old`) are not part of the active workflow — the real system is `exam_master.sh` → `<rank>/exam.sh` as described above.

## 📦 **Project Structure**

```
42-exam-rank-42/
├── exam_master.sh        # 🚀 Unified launcher for all ranks
├── Makefile              # Alternative build/run system
│
├── 02/                   # Exam Rank 02
│   ├── exam.sh           # Rank 02 launcher
│   ├── Level1/ Level2/ Level3/ Level4/
│   ├── exam_progress/    # Progress tracking
│   └── rendu/            # Your solutions
│
├── 03/                   # Exam Rank 03
│   ├── exam.sh
│   ├── level-1/ level-2/    # subject + given code + grademe/test.sh per exercise
│   └── rendu/                # per-rank folder — not actually read by exam.sh, see note below
│
├── 04/                   # Exam Rank 04
│   ├── exam.sh
│   ├── level-1/ level-2/
│   └── rendu/
│
├── 05/                   # Exam Rank 05
│   ├── exam.sh
│   ├── level-1/ level-2/
│   ├── exam_progress/
│   ├── CONTEXT.md         # vocabulary + decisions for this rank's given/rendu/rendu5 model
│   └── rendu/
│
├── 06/                   # Exam Rank 06
│   ├── exam.sh
│   └── ...
│
├── rendu/                # The REAL practice workspace, shared by every rank.
│                          # Each rank's exam.sh reads/writes PROJECT_ROOT/rendu/<exercise> here.
│                          # Stays empty between sessions — write fresh here while practicing.
│
└── rendu2/ rendu4/ rendu5/…  # Personal archive of already-solved solutions, one per rank,
                               # for reference/study only — never read by exam.sh.
```

> The `rendu/` folders shown *inside* each rank directory above (`02/rendu`, `03/rendu`, ...) exist on disk but aren't the ones `exam.sh` actually uses — every rank's `exam.sh` resolves `RENDU_DIR` to the shared root `rendu/`. Treat the per-rank ones as stale/unused.

## 🎯 **How to Use the Exam System**

**Standard workflow:**
1. Launch: `./exam_master.sh`
2. Select your exam rank (02, 03, 04, 05, or 06)
3. The system will guide you through the exercises
4. Read the subject for each exercise
5. Write your solution in the appropriate `rendu/` directory
6. Validate your solution using the system's validation tools

**Tips:**
- ✅ Follow the menu prompts for guided experience
- ✅ Your progress is automatically tracked
- ✅ Each rank has exercises organized by difficulty level

## 📚 Exam Contents

### Exam Rank 02
Consists of 4 levels, each with exercises of incremental difficulty:

#### Level 1 (12 exercises)
- first_word, fizzbuzz, ft_putstr, ft_strcpy, ft_strlen, ft_swap, repeat_alpha, rev_print, rot_13, rotone, search_and_replace, ulstr

#### Level 2 (20 exercises)
- alpha_mirror, camel_to_snake, do_op, ft_atoi, ft_strcmp, ft_strcspn, ft_strdup, ft_strpbrk, ft_strspn, ft_strrev, inter, is_power_of_2, last_word, max, print_bits, reverse_bits, snake_to_camel, swap_bits, union, wdmatch

#### Level 3 (15 exercises)
- add_prime_sum, epur_str, expand_str, ft_atoi_base, ft_list_size, ft_range, ft_rrange, hidenp, lcm, paramsum, pgcd, print_hex, rstr_capitalizer, str_capitalizer, tab_mult

#### Level 4 (10 exercises)
- flood_fill, fprime, ft_itoa, ft_list_foreach, ft_list_remove_if, ft_split, rev_wstr, rostring, sort_int_tab, sort_list

### Exam Rank 03
Consists of two main exercises:
- **ft_printf**: Simplified implementation of the printf function
- **get_next_line**: Function to read lines from a file

### Exam Rank 04
Available in the `04/` directory with exercises organized by levels.

### Exam Rank 05
Consists of 2 levels with advanced C and C++ exercises:

#### Level 1 (3 exercises - C++)
- **bigint**: Implementation of an arbitrary precision integer class. Supports addition, comparison, and digit shifting (e.g., `42 << 3 == 42000`). The class stores numbers larger than SIZE_MAX without precision loss using string-based storage. Must implement operators: `+`, `+=`, `<<`, `>>`, `<<=`, `>>=`, `++`, `--`, `<`, `<=`, `>`, `>=`, `==`, `!=`, and output operator `<<`.

- **vect2**: Class representing a 2-dimensional mathematical vector with integer components. Implements addition, subtraction, scalar multiplication, and complete operator overloading including `[]` for component access, `<<` for output, comparison operators (`==`, `!=`), compound assignment operators (`+=`, `-=`, `*=`), and increment/decrement operators (`++`, `--`). Must support expressions like `v2 = v3 + v3 * 2` and `v2 = 3 * v2`.

- **polyset**: Implementation of Set and Bag data structures using arrays and binary search trees. Create `searchable_array_bag` and `searchable_tree_bag` classes that inherit from provided `array_bag` and `tree_bag` classes, implementing the searchable_bag abstract interface. Then create a `set` class that wraps a searchable_bag to enforce set semantics (no duplicates). All classes must follow Orthodox Canonical Form with proper const-correctness. See [`05/CONTEXT.md`](./05/CONTEXT.md) for the given/rendu/rendu5 folder model used in this rank.

#### Level 2 (2 exercises - C)
- **bsq**: Finds the largest square on a map while avoiding obstacles. The program reads a map from a file or stdin where the first line specifies: number of lines, empty character, obstacle character, and full character (space-separated). The program must identify and mark the biggest square possible using the "full" character. If multiple solutions exist, choose the topmost, then leftmost square. Must handle map validation and output "map error" to stderr for invalid maps.

- **game_of_life**: Simulation of Conway's Game of Life. Takes three arguments: `./life width height iterations`. Reads commands from stdin to draw the initial board configuration using a "pen" metaphor:
  - `w a s d`: move pen up, left, down, right
  - `x`: toggle pen (start/stop drawing)
  - Pen starts at top-left corner
  - Invalid commands are ignored
  - After reading all commands, simulate N iterations of Game of Life rules and output the result (alive cells as '0', dead cells as spaces).

### Exam Rank 06
Available in the `06/` directory. The system includes this rank for practice.

## 📝 Tips for the Exams

1. **Constant practice**: Try to solve each exercise multiple times until you can do it without consulting the solution.

2. **Time management**: Real exams have a time limit, so practice solving them under time pressure.

3. **Norminette**: Norminette is not enforced in the actual exam, so it's not included here.

4. **Debugging**: Learn to debug your code without a debugger (using strategic prints). At 42 campuses, you typically have Valgrind and gdb available.

5. **Memory**: Always check for memory leaks in functions that use malloc. Use valgrind to verify: `valgrind --leak-check=full ./your_program`

6. **Frequent exercises**:
   - **Rank 02**: Practice level 2 and 3 exercises extensively as they appear most frequently
   - **Rank 03**: ft_printf and get_next_line are the core exercises
   - **Rank 05**: Focus on operator overloading and proper class design

7. **C++ exercises (Rank 05)**:
   - Master operator overloading syntax
   - Understand Orthodox Canonical Form (default constructor, copy constructor, assignment operator, destructor)
   - Practice const-correctness
   - Know when to return by reference vs by value
   - Understand the difference between prefix and postfix increment/decrement

8. **Read the subject carefully**: Many students fail because they miss small details in the requirements. Read every line of the subject.

## 🤝 Contributing

Contributions are welcome! If you'd like to improve this repository:

1. Fork the project
2. Create a branch for your feature (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## ⚠️ Disclaimer

This project is not officially affiliated with 42 School. It is a practice tool created by and for students to help prepare for official exams.

## 📜 License

This project is under the MIT license. See `LICENSE` for more information.
All exercise subjects belong to 42 School.

## 🙏 Acknowledgments

- To the 42 community worldwide
- To all students who have contributed exercises, solutions, and improvements
- To the creators of the original exam practice systems
- Special thanks to all contributors who have helped document and expand this resource

---
Made with ❤️ by mvigara - 42 School Madrid student
