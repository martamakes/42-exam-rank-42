#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

# Print test result
print_result() {
    local test_name="$1"
    local result="$2"
    local details="$3"
    
    if [ "$result" = "PASS" ]; then
        echo -e "${GREEN}✓ $test_name${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}✗ $test_name${NC}"
        if [ -n "$details" ]; then
            echo -e "${RED}  $details${NC}"
        fi
        ((TESTS_FAILED++))
    fi
}

# Print section header
print_section() {
    echo -e "\n${BLUE}=== $1 ===${NC}"
}

# Clean up function
cleanup() {
    rm -f test_sandbox sandbox.o test_output.txt expected_output.txt
    rm -f test_main.c stress_test.c stress_test zombie_test.c zombie_test
    rm -f verbose_test.c verbose_test integration_test.c integration_test
    rm -f temp_file.txt test_temp.txt sandbox_output.txt zombie_count.txt
}

# Set up
echo -e "${YELLOW}🧪 SANDBOX COMPREHENSIVE TESTER${NC}"
echo -e "${YELLOW}=================================${NC}"

# Navigate to rendu directory  
STUDENT_DIR="../../../../rendu/sandbox"
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}❌ Error: No solution found at $STUDENT_DIR${NC}"
    echo -e "${YELLOW}💡 Create your solution: mkdir -p rendu/sandbox && cp your_sandbox.c rendu/sandbox/sandbox.c${NC}"
    exit 1
fi

if [ ! -f "$STUDENT_DIR/sandbox.c" ]; then
    echo -e "${RED}❌ Error: sandbox.c not found at $STUDENT_DIR/sandbox.c${NC}"
    exit 1
fi

echo -e "${BLUE}📁 Testing solution at: $STUDENT_DIR/sandbox.c${NC}"

# Copy student solution for testing
cp "$STUDENT_DIR/sandbox.c" .

print_section "COMPILATION TESTS"

# Test 1: Basic compilation
gcc -Wall -Wextra -Werror -c sandbox.c 2>/dev/null
if [ $? -eq 0 ]; then
    print_result "Basic compilation" "PASS"
else
    print_result "Basic compilation" "FAIL" "Code does not compile with -Wall -Wextra -Werror"
    cleanup
    exit 1
fi

# Create comprehensive test main
cat > test_main.c << 'EOF'
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>

// Student's function prototype
int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

// Test functions for sandbox
void nice_function(void) {
    // This function does nothing bad
    return;
}

void bad_function_exit_code_1(void) {
    exit(1);
}

void bad_function_exit_code_42(void) {
    exit(42);
}

void bad_function_segfault(void) {
    int *ptr = NULL;
    *ptr = 42;  // Causes segfault
}

void bad_function_abort(void) {
    abort();
}

void bad_function_infinite_loop(void) {
    while (1) {
        // This will timeout
    }
}

void bad_function_divide_by_zero(void) {
    int x = 1;
    int y = 0;
    volatile int result = x / y;  // Should cause SIGFPE
    (void)result;
}

void bad_function_recursive_segfault(void) {
    char buffer[1000];
    // Cause stack overflow
    bad_function_recursive_segfault();
    (void)buffer;  // Avoid unused variable warning
}

void nice_function_with_output(void) {
    printf("Hello World\n");
    fflush(stdout);
}

void bad_function_sleep_and_exit(void) {
    sleep(1);
    exit(5);
}

// Helper function to capture output
int capture_sandbox_output(void (*f)(void), unsigned int timeout, bool verbose, char *output, size_t output_size) {
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    
    pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }
    
    if (pid == 0) {
        // Child: redirect stdout to pipe and call sandbox
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        int result = sandbox(f, timeout, verbose);
        exit(result == 1 ? 0 : (result == 0 ? 1 : 2));
    }
    
    // Parent: read from pipe
    close(pipefd[1]);
    ssize_t bytes = read(pipefd[0], output, output_size - 1);
    close(pipefd[0]);
    
    if (bytes > 0) {
        output[bytes] = '\0';
    } else {
        output[0] = '\0';
    }
    
    int status;
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status)) {
        int exit_code = WEXITSTATUS(status);
        return exit_code == 0 ? 1 : (exit_code == 1 ? 0 : -1);
    }
    
    return -1;
}

// Test 1: Nice function should return 1
int test_nice_function() {
    int result = sandbox(nice_function, 5, false);
    return (result == 1);
}

// Test 2: Bad function (exit code 1) should return 0
int test_bad_exit_code_1() {
    int result = sandbox(bad_function_exit_code_1, 5, false);
    return (result == 0);
}

// Test 3: Bad function (exit code 42) should return 0
int test_bad_exit_code_42() {
    int result = sandbox(bad_function_exit_code_42, 5, false);
    return (result == 0);
}

// Test 4: Bad function (segfault) should return 0
int test_segfault_function() {
    int result = sandbox(bad_function_segfault, 5, false);
    return (result == 0);
}

// Test 5: Bad function (abort) should return 0
int test_abort_function() {
    int result = sandbox(bad_function_abort, 5, false);
    return (result == 0);
}

// Test 6: Timeout function should return 0
int test_timeout_function() {
    int result = sandbox(bad_function_infinite_loop, 2, false);
    return (result == 0);
}

// Test 7: Divide by zero should return 0
int test_divide_by_zero() {
    int result = sandbox(bad_function_divide_by_zero, 5, false);
    return (result == 0);
}

// Test 8: Verbose output for nice function
int test_verbose_nice() {
    char output[256];
    int result = capture_sandbox_output(nice_function, 5, true, output, sizeof(output));
    return (result == 1 && strstr(output, "Nice function!") != NULL);
}

// Test 9: Verbose output for exit code
int test_verbose_exit_code() {
    char output[256];
    int result = capture_sandbox_output(bad_function_exit_code_42, 5, true, output, sizeof(output));
    return (result == 0 && strstr(output, "Bad function: exited with code 42") != NULL);
}

// Test 10: Verbose output for timeout
int test_verbose_timeout() {
    char output[256];
    int result = capture_sandbox_output(bad_function_infinite_loop, 2, true, output, sizeof(output));
    return (result == 0 && strstr(output, "timed out after 2 seconds") != NULL);
}

// Test 11: Verbose output for signal (segfault)
int test_verbose_segfault() {
    char output[256];
    int result = capture_sandbox_output(bad_function_segfault, 5, true, output, sizeof(output));
    return (result == 0 && (strstr(output, "Segmentation fault") != NULL || 
                           strstr(output, "SIGSEGV") != NULL ||
                           strstr(output, "Bad function:") != NULL));
}

// Test 12: Nice function with output (should still be nice)
int test_nice_with_output() {
    int result = sandbox(nice_function_with_output, 5, false);
    return (result == 1);
}

// Test 13: Fast timeout (1 second)
int test_fast_timeout() {
    int result = sandbox(bad_function_infinite_loop, 1, false);
    return (result == 0);
}

// Test 14: Function that sleeps but exits bad
int test_sleep_and_bad_exit() {
    int result = sandbox(bad_function_sleep_and_exit, 5, false);
    return (result == 0);  // Should be bad due to exit code 5
}

// Test 15: Multiple consecutive sandbox calls (resource management)
int test_multiple_calls() {
    for (int i = 0; i < 3; i++) {
        if (sandbox(nice_function, 5, false) != 1) return 0;
        if (sandbox(bad_function_exit_code_1, 5, false) != 0) return 0;
        if (sandbox(bad_function_segfault, 5, false) != 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <test_number>\n", argv[0]);
        return 1;
    }
    
    int test_num = atoi(argv[1]);
    int result = 0;
    
    switch (test_num) {
        case 1: result = test_nice_function(); break;
        case 2: result = test_bad_exit_code_1(); break;
        case 3: result = test_bad_exit_code_42(); break;
        case 4: result = test_segfault_function(); break;
        case 5: result = test_abort_function(); break;
        case 6: result = test_timeout_function(); break;
        case 7: result = test_divide_by_zero(); break;
        case 8: result = test_verbose_nice(); break;
        case 9: result = test_verbose_exit_code(); break;
        case 10: result = test_verbose_timeout(); break;
        case 11: result = test_verbose_segfault(); break;
        case 12: result = test_nice_with_output(); break;
        case 13: result = test_fast_timeout(); break;
        case 14: result = test_sleep_and_bad_exit(); break;
        case 15: result = test_multiple_calls(); break;
        default: 
            printf("Invalid test number\n");
            return 1;
    }
    
    return result ? 0 : 1;
}
EOF

# Compile test program
gcc -Wall -Wextra -Werror sandbox.o test_main.c -o test_sandbox 2>/dev/null
if [ $? -eq 0 ]; then
    print_result "Test compilation" "PASS"
else
    print_result "Test compilation" "FAIL" "Could not compile test program"
    cleanup
    exit 1
fi

print_section "BASIC FUNCTIONALITY TESTS"

# Test 1: Nice function should return 1
timeout 10 ./test_sandbox 1
if [ $? -eq 0 ]; then
    print_result "Nice function detection" "PASS"
else
    print_result "Nice function detection" "FAIL" "Nice function should return 1"
fi

# Test 2: Bad function (exit code 1)
timeout 10 ./test_sandbox 2
if [ $? -eq 0 ]; then
    print_result "Bad exit code detection (1)" "PASS"
else
    print_result "Bad exit code detection (1)" "FAIL" "Function with exit(1) should return 0"
fi

# Test 3: Bad function (exit code 42)
timeout 10 ./test_sandbox 3
if [ $? -eq 0 ]; then
    print_result "Bad exit code detection (42)" "PASS"
else
    print_result "Bad exit code detection (42)" "FAIL" "Function with exit(42) should return 0"
fi

print_section "SIGNAL HANDLING TESTS"

# Test 4: Segmentation fault detection
timeout 10 ./test_sandbox 4
if [ $? -eq 0 ]; then
    print_result "Segfault detection" "PASS"
else
    print_result "Segfault detection" "FAIL" "Function causing segfault should return 0"
fi

# Test 5: Abort signal detection
timeout 10 ./test_sandbox 5
if [ $? -eq 0 ]; then
    print_result "Abort signal detection" "PASS"
else
    print_result "Abort signal detection" "FAIL" "Function calling abort() should return 0"
fi

# Test 7: Floating point exception
timeout 10 ./test_sandbox 7
if [ $? -eq 0 ]; then
    print_result "Divide by zero detection" "PASS"
else
    print_result "Divide by zero detection" "FAIL" "Function with division by zero should return 0"
fi

print_section "TIMEOUT TESTS"

# Test 6: Timeout detection
timeout 15 ./test_sandbox 6
if [ $? -eq 0 ]; then
    print_result "Timeout detection (2s)" "PASS"
else
    print_result "Timeout detection (2s)" "FAIL" "Infinite loop should timeout and return 0"
fi

# Test 13: Fast timeout
timeout 10 ./test_sandbox 13
if [ $? -eq 0 ]; then
    print_result "Fast timeout detection (1s)" "PASS"
else
    print_result "Fast timeout detection (1s)" "FAIL" "Should timeout after 1 second"
fi

print_section "VERBOSE OUTPUT TESTS"

# Test 8: Verbose nice function
timeout 10 ./test_sandbox 8
if [ $? -eq 0 ]; then
    print_result "Verbose nice function output" "PASS"
else
    print_result "Verbose nice function output" "FAIL" "Should output 'Nice function!'"
fi

# Test 9: Verbose exit code
timeout 10 ./test_sandbox 9
if [ $? -eq 0 ]; then
    print_result "Verbose exit code output" "PASS"
else
    print_result "Verbose exit code output" "FAIL" "Should output exit code in message"
fi

# Test 10: Verbose timeout
timeout 15 ./test_sandbox 10
if [ $? -eq 0 ]; then
    print_result "Verbose timeout output" "PASS"
else
    print_result "Verbose timeout output" "FAIL" "Should output timeout message with seconds"
fi

# Test 11: Verbose signal
timeout 10 ./test_sandbox 11
if [ $? -eq 0 ]; then
    print_result "Verbose signal output" "PASS"
else
    print_result "Verbose signal output" "FAIL" "Should output signal description"
fi

print_section "ADVANCED FUNCTIONALITY TESTS"

# Test 12: Nice function with output
timeout 10 ./test_sandbox 12
if [ $? -eq 0 ]; then
    print_result "Nice function with stdout" "PASS"
else
    print_result "Nice function with stdout" "FAIL" "Functions that write to stdout can still be nice"
fi

# Test 14: Sleep then bad exit
timeout 10 ./test_sandbox 14
if [ $? -eq 0 ]; then
    print_result "Sleep then bad exit" "PASS"
else
    print_result "Sleep then bad exit" "FAIL" "Function that sleeps but exits bad should return 0"
fi

print_section "RESOURCE MANAGEMENT TESTS"

# Test 15: Multiple consecutive calls
timeout 20 ./test_sandbox 15
if [ $? -eq 0 ]; then
    print_result "Multiple sandbox calls" "PASS"
else
    print_result "Multiple sandbox calls" "FAIL" "Should handle multiple consecutive calls"
fi

# Zombie process check
cat > zombie_test.c << 'EOF'
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

void quick_function(void) {
    return;
}

void quick_bad_function(void) {
    exit(1);
}

int main() {
    // Run many quick sandboxes to test for zombie accumulation
    for (int i = 0; i < 20; i++) {
        sandbox(quick_function, 5, false);
        sandbox(quick_bad_function, 5, false);
    }
    
    // Check if any zombies remain
    system("ps aux | grep -c '<defunct>' > zombie_count.txt 2>/dev/null || echo '0' > zombie_count.txt");
    
    FILE *f = fopen("zombie_count.txt", "r");
    if (!f) return 1;
    
    int zombie_count;
    fscanf(f, "%d", &zombie_count);
    fclose(f);
    unlink("zombie_count.txt");
    
    // If we have zombies, it's a problem (though some systems might have background zombies)
    return (zombie_count <= 1) ? 0 : 1;  // Allow 1 zombie for grep itself or other processes
}
EOF

gcc sandbox.o zombie_test.c -o zombie_test 2>/dev/null
if [ $? -eq 0 ]; then
    timeout 30 ./zombie_test
    if [ $? -eq 0 ]; then
        print_result "Zombie process prevention" "PASS"
    else
        print_result "Zombie process prevention" "FAIL" "Zombie processes detected after multiple calls"
    fi
else
    print_result "Zombie test compilation" "FAIL" "Could not compile zombie test"
fi

print_section "INTEGRATION TESTS"

# Stress test with various bad functions
cat > stress_test.c << 'EOF'
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

void nice_func(void) { return; }
void bad_exit(void) { exit(1); }
void bad_segfault(void) { int *p = NULL; *p = 42; }
void bad_abort(void) { abort(); }

int main() {
    // Mix of nice and bad functions with different timeouts
    typedef struct {
        void (*func)(void);
        int expected;
        unsigned int timeout;
    } test_case_t;
    
    test_case_t tests[] = {
        {nice_func, 1, 2},
        {bad_exit, 0, 2},
        {bad_segfault, 0, 2},
        {bad_abort, 0, 2},
        {nice_func, 1, 1},
        {bad_exit, 0, 3},
        {NULL, 0, 0}
    };
    
    for (int i = 0; tests[i].func; i++) {
        int result = sandbox(tests[i].func, tests[i].timeout, false);
        if (result != tests[i].expected) {
            return 1;
        }
    }
    
    return 0;
}
EOF

gcc sandbox.o stress_test.c -o stress_test 2>/dev/null
if [ $? -eq 0 ]; then
    timeout 20 ./stress_test
    if [ $? -eq 0 ]; then
        print_result "Stress test (mixed functions)" "PASS"
    else
        print_result "Stress test (mixed functions)" "FAIL" "Failed to handle mixed function types correctly"
    fi
else
    print_result "Stress test compilation" "FAIL" "Could not compile stress test"
fi

# Final results
print_section "TEST RESULTS SUMMARY"
TOTAL_TESTS=$((TESTS_PASSED + TESTS_FAILED))
echo -e "Total tests: ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! Your sandbox implementation is working correctly!${NC}"
    echo -e "${GREEN}📝 Your solution handles:${NC}"
    echo -e "${GREEN}   ✓ Nice function detection (return 1)${NC}"
    echo -e "${GREEN}   ✓ Bad exit code detection (return 0)${NC}"
    echo -e "${GREEN}   ✓ Signal termination detection (SIGSEGV, SIGABRT, SIGFPE)${NC}"
    echo -e "${GREEN}   ✓ Timeout detection and process termination${NC}"
    echo -e "${GREEN}   ✓ Verbose output formatting${NC}"
    echo -e "${GREEN}   ✓ Process cleanup (no zombies)${NC}"
    echo -e "${GREEN}   ✓ Resource management${NC}"
    cleanup
    exit 0
else
    echo -e "\n${RED}❌ Some tests failed. Review your implementation.${NC}"
    echo -e "${YELLOW}💡 Common issues:${NC}"
    echo -e "${YELLOW}   • Check signal handling setup (sigaction vs signal)${NC}"
    echo -e "${YELLOW}   • Ensure timeout detection works (alarm + EINTR)${NC}"
    echo -e "${YELLOW}   • Verify process state analysis (WIFEXITED, WIFSIGNALED)${NC}"
    echo -e "${YELLOW}   • Make sure to kill and wait for timed-out processes${NC}"
    echo -e "${YELLOW}   • Check verbose output format matches exactly${NC}"
    echo -e "${YELLOW}   • Ensure all child processes are reaped (no zombies)${NC}"
    echo -e "${YELLOW}   • Verify return values: 1=nice, 0=bad, -1=error${NC}"
    cleanup
    exit 1
fi
