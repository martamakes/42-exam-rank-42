# 🚀 QUICK START GUIDE

> **Get up and running with examshell in 2 minutes**

## ⚡ Super Quick Setup

```bash
# 1. Get the code
git clone <repository-url>
cd examshell

# 2. Run setup (does everything automatically)
chmod +x setup.sh && ./setup.sh

# 3. Start practicing!
./examshell 03
```

## 🎯 First Session Walkthrough

### Step 1: Launch examshell
```bash
./examshell 03
```

You'll see:
```
=====================================
        42 SCHOOL - EXAMSHELL        
         Practice Environment        
=====================================

📚 Current Exercise:
   Name: filter
   Level: 1

✅ Subject: subject/filter/subject.txt
✅ Submit to: rendu/filter/
    (Create this directory and put your .c and .h files here)
```

### Step 2: Read the exercise (new terminal)
```bash
cat subject/filter/subject.txt
```

### Step 3: Code your solution (new terminal)
```bash
# Create your submission directory
mkdir rendu/filter

# Write your solution
vim rendu/filter/filter.c

# Test it
gcc rendu/filter/filter.c -o filter
echo "test input" | ./filter "pattern"
```

### Step 4: Check your files (back to examshell)
```
Enter your choice: r

📁 Files in rendu/filter/:
  - filter.c ✓
  Total: 1 files
```

### Step 5: Submit when ready
```
Enter your choice: g

✅ Level 1 completed! Well done!
Moving to level 2...
```

## 🎮 Menu Quick Reference

| Key | Action | Description |
|-----|--------|-------------|
| `r` | **Show files** | List your current solution files |
| `g` | **Submit** | Mark exercise as complete, advance level |
| `n` | **Next exercise** | Get different exercise (same level) |
| `s` | **Stats** | Show progress and time |
| `q` | **Quit** | Save and exit (resume later) |

## 📁 Directory Structure

```
examshell/
├── subject/filter/subject.txt    ← Read this for instructions
└── rendu/filter/                 ← CREATE THIS and put your files here
    ├── filter.c                  ← Your solution
    └── filter.h                  ← Your headers (if needed)
```

## 🔄 Between Sessions

```bash
# Continue where you left off
./examshell 03

# Start fresh (cleans rendu/ directory)
./examshell 03
# Choose 'n' for new session when prompted

# Switch ranks (auto-cleans rendu/)
./examshell 04  # Will ask to clean previous rank's files
```

## 🧪 Verify Everything Works

```bash
# Run the test suite
chmod +x test_system.sh && ./test_system.sh

# Should show: "🎉 ALL TESTS PASSED!"
```

## 🆘 Troubleshooting

**Program won't start:**
```bash
make fclean && make
./examshell 03
```

**No exercises found:**
```bash
ls exam-rank-*  # Should show exam-rank-03, exam-rank-04, exam-rank-05
```

**Permission denied:**
```bash
chmod +x examshell setup.sh test_system.sh
```

## 🎓 Study Tips

1. **🕐 Practice time management** - Track your session time
2. **🔄 Use all 3 terminals** - Like in real exam
3. **📖 Read subjects carefully** - Details matter
4. **🧪 Test edge cases** - Empty input, errors, etc.
5. **💾 No libft allowed** - Implement everything from scratch

## 📊 Available Content

- **Rank 03:** filter, scanf, broken_gnl, n_queens, permutations, powerset, rip, tsp
- **Rank 04:** ft_popen, picoshell, sandbox, argo, vbc  
- **Rank 05:** string_bigint, vect2, bsq, game_of_life

---

**That's it! You're ready to practice like it's the real exam! 🎯**

Need more details? Check `README.md` for full documentation.
