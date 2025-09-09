# PICOSHELL Tester

## How to Use

1. **Create your solution:**
   ```bash
   mkdir -p rendu/picoshell
   cp your_picoshell.c rendu/picoshell/picoshell.c
   ```

2. **Run the tester:**
   ```bash
   cd exam-rank-04/level-1/picoshell/grademe
   ./test.sh
   ```

## What the Tester Validates

### ✅ **Compilation Tests**
- Compiles with `-Wall -Wextra -Werror`
- Links correctly with test programs

### ✅ **Basic Functionality Tests**
- **Single command**: Execute single command without pipeline
- **Two command pipeline**: Basic pipe (cmd1 | cmd2)
- **Three command pipeline**: Extended pipe (cmd1 | cmd2 | cmd3)

### ✅ **Error Handling Tests**
- **NULL parameters**: Handles NULL cmds array
- **Empty command array**: Gracefully handles empty input
- **Nonexistent commands**: Returns error for invalid commands
- **Pipeline error propagation**: Propagates errors through pipeline

### ✅ **Advanced Functionality Tests**
- **Complex pipeline**: 4+ commands in sequence
- **Multiple arguments**: Commands with various parameters
- **Stress test**: Many commands chained together

### ✅ **Integration Tests**
- **Subject examples**: Tests examples from subject (echo | cat | sed)
- **Real command pipeline**: Tests with actual system commands (ls | grep)

### ✅ **Resource Management Tests**
- **Multiple runs**: Tests for memory/FD leaks
- **Process cleanup**: Ensures no zombie processes

## Expected Test Results

For a **perfect implementation**, you should see:
```
🧪 PICOSHELL COMPREHENSIVE TESTER
====================================

=== COMPILATION TESTS ===
✓ Basic compilation
✓ Test compilation

=== BASIC FUNCTIONALITY TESTS ===
✓ Single command execution
✓ Two command pipeline
✓ Three command pipeline

=== ERROR HANDLING TESTS ===
✓ NULL cmds handling
✓ Empty command array
✓ Nonexistent command error
✓ Pipeline error propagation

=== ADVANCED FUNCTIONALITY TESTS ===
✓ Complex pipeline (4 commands)
✓ Multiple command arguments
✓ Stress test (many commands)

=== INTEGRATION TESTS ===
✓ Subject example integration
✓ Real command pipeline (ls | grep)

=== RESOURCE MANAGEMENT TESTS ===
✓ Resource management (multiple runs)

=== TEST RESULTS SUMMARY ===
Total tests: 15
Passed: 15
Failed: 0

🎉 ALL TESTS PASSED!
```

## Common Issues Found by Tester

### 🐛 **NULL Parameter Validation**
**Issue:** Segmentation fault when `cmds` parameter is NULL
```c
int picoshell(char **cmds[])
{
    // ❌ Missing NULL check causes segfault
    while (cmds[i])  // Crashes if cmds is NULL
```
**Fix:** Add parameter validation
```c
int picoshell(char **cmds[])
{
    if (!cmds)  // ✅ Add NULL check
        return 1;
    // ... rest of implementation
}
```

### 🐛 **Pipeline Data Flow Issues**
**Issue:** Commands don't receive expected input/output
- Check `dup2()` calls for stdin/stdout redirection
- Ensure proper file descriptor closing
- Verify pipe read/write ends are used correctly

### 🐛 **Process Management**
**Issue:** Zombie processes or hanging
- Make sure all child processes are `wait()`ed
- Close all unused file descriptors
- Handle fork() errors properly

### 🐛 **Error Propagation**
**Issue:** Pipeline continues even when commands fail
- Check exit codes in `wait()` calls
- Return 1 if any command in pipeline fails
- Handle `execvp()` failures in child processes

## Pipeline Implementation Guide

### 🔧 **Core Algorithm**
```c
for each command in pipeline:
    1. Create pipe (if not last command)
    2. Fork child process
    3. In child:
       - Redirect stdin from previous pipe (if exists)
       - Redirect stdout to current pipe (if not last)
       - Execute command
    4. In parent:
       - Close unused pipe ends
       - Save current pipe for next iteration
       - Continue to next command
5. Wait for all children and collect exit codes
```

### 🔧 **File Descriptor Management**
```
Command 1: ls
Parent: prev_fd = -1, creates pipe1
Child1: stdin=terminal, stdout=pipe1[1]
Parent: close(pipe1[1]), prev_fd = pipe1[0]

Command 2: grep
Parent: prev_fd = pipe1[0], creates pipe2  
Child2: stdin=pipe1[0], stdout=pipe2[1]
Parent: close(pipe1[0]), close(pipe2[1]), prev_fd = pipe2[0]

Command 3: wc (last)
Parent: prev_fd = pipe2[0], no new pipe
Child3: stdin=pipe2[0], stdout=terminal
Parent: close(pipe2[0]), wait_all()
```

## Testing Your Implementation

### 🧪 **Manual Testing**
```bash
# Test basic pipeline
echo -e "apple\nbanana\ncherry" | grep a | wc -l  # Should output: 2

# Test your implementation
char *cmd1[] = {"echo", "-e", "apple\\nbanana\\ncherry", NULL};
char *cmd2[] = {"grep", "a", NULL};
char *cmd3[] = {"wc", "-l", NULL};
char **cmds[] = {cmd1, cmd2, cmd3, NULL};
int result = picoshell(cmds);  // Should output: 2
```

### 🧪 **Debugging Tips**
1. **Add debug prints** to trace execution flow
2. **Test with simple commands** first (echo, cat)
3. **Check file descriptors** with `lsof` if processes hang
4. **Use valgrind** to detect memory issues
5. **Test error cases** explicitly (NULL, nonexistent commands)

## Key Implementation Points

### ⚡ **Critical Success Factors**
- ✅ **Parameter validation** (NULL checks)
- ✅ **Proper pipe management** (create/close at right times)  
- ✅ **Correct redirection** (dup2 stdin/stdout)
- ✅ **Complete process cleanup** (wait for all children)
- ✅ **Error handling** (fork, pipe, execvp failures)
- ✅ **Resource management** (close unused FDs)

This tester will help you identify and fix these critical issues before the real exam! 🎓