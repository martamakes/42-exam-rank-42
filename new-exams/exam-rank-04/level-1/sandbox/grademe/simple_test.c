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

void nice_function_with_output(void) {
    printf("Hello World\n");
    fflush(stdout);
}

void bad_function_sleep_and_exit(void) {
    sleep(1);
    exit(5);
}

void bad_function_stack_overflow(void) {
    // Cause stack overflow - very bad function
    char huge_buffer[100000];
    // Use volatile to prevent optimization
    volatile int x = 1;
    if (x) bad_function_stack_overflow();
    (void)huge_buffer;
}

void null_function_ptr(void) {
    // This will be used to test NULL function pointer
    void (*null_func)(void) = NULL;
    null_func();  // Should cause segfault
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

typedef struct {
    const char *name;
    void (*test_func)(void);
    int expected_result;
    unsigned int timeout;
    bool test_verbose;
    const char *expected_output;
} test_case_t;

int main(void) {
    test_case_t tests[] = {
        {"Nice function detection", nice_function, 1, 5, false, NULL},
        {"Bad exit code detection (1)", bad_function_exit_code_1, 0, 5, false, NULL},
        {"Bad exit code detection (42)", bad_function_exit_code_42, 0, 5, false, NULL},
        {"Segfault detection", bad_function_segfault, 0, 5, false, NULL},
        {"Abort signal detection", bad_function_abort, 0, 5, false, NULL},
        {"Timeout detection (2s)", bad_function_infinite_loop, 0, 2, false, NULL},
        {"Nice function with stdout", nice_function_with_output, 1, 5, false, NULL},
        {"Fast timeout detection (1s)", bad_function_infinite_loop, 0, 1, false, NULL},
        {"Sleep then bad exit", bad_function_sleep_and_exit, 0, 5, false, NULL},
        {"Verbose nice function", nice_function, 1, 5, true, "Nice function!"},
        {"Verbose exit code", bad_function_exit_code_42, 0, 5, true, "exited with code 42"},
        {"Verbose timeout", bad_function_infinite_loop, 0, 2, true, "timed out after 2 seconds"},
        {"Verbose segfault", bad_function_segfault, 0, 5, true, "Bad function:"},
        {"Stack overflow (very bad)", bad_function_stack_overflow, 0, 5, false, NULL},
        {"NULL function pointer", null_function_ptr, 0, 5, false, NULL},
        {NULL, NULL, 0, 0, false, NULL}  // Terminator
    };
    
    int passed = 0;
    int failed = 0;
    int total = 0;
    
    printf("\033[1;33m🧪 SANDBOX COMPREHENSIVE TESTER\033[0m\n");
    printf("\033[1;33m=================================\033[0m\n\n");
    
    for (int i = 0; tests[i].name; i++) {
        total++;
        printf("Testing: %s... ", tests[i].name);
        fflush(stdout);
        
        int result;
        if (tests[i].test_verbose && tests[i].expected_output) {
            // Test verbose output
            char output[512];
            result = capture_sandbox_output(tests[i].test_func, tests[i].timeout, tests[i].test_verbose, output, sizeof(output));
            
            if (result == tests[i].expected_result && strstr(output, tests[i].expected_output) != NULL) {
                printf("\033[0;32m✓ PASS\033[0m\n");
                passed++;
            } else {
                printf("\033[0;31m✗ FAIL\033[0m (expected result: %d, got: %d, output: '%.100s')\n", 
                       tests[i].expected_result, result, output);
                failed++;
            }
        } else {
            // Test return value only
            result = sandbox(tests[i].test_func, tests[i].timeout, tests[i].test_verbose);
            if (result == tests[i].expected_result) {
                printf("\033[0;32m✓ PASS\033[0m\n");
                passed++;
            } else {
                printf("\033[0;31m✗ FAIL\033[0m (expected: %d, got: %d)\n", tests[i].expected_result, result);
                failed++;
            }
        }
    }
    
    // Note: NULL function parameter test removed as it's ambiguous in subject
    // The subject doesn't clearly specify if NULL function should return -1

    printf("\n\033[0;34m=== TEST RESULTS SUMMARY ===\033[0m\n");
    printf("Total tests: \033[0;34m%d\033[0m\n", total);
    printf("Passed: \033[0;32m%d\033[0m\n", passed);
    printf("Failed: \033[0;31m%d\033[0m\n", failed);
    
    if (failed == 0) {
        printf("\n\033[0;32m🎉 ALL TESTS PASSED! Your sandbox implementation is working correctly!\033[0m\n");
        printf("\033[0;32m📝 Your solution handles:\033[0m\n");
        printf("\033[0;32m   ✓ Nice function detection (return 1)\033[0m\n");
        printf("\033[0;32m   ✓ Bad exit code detection (return 0)\033[0m\n");
        printf("\033[0;32m   ✓ Signal termination detection (SIGSEGV, SIGABRT, SIGFPE)\033[0m\n");
        printf("\033[0;32m   ✓ Timeout detection and process termination\033[0m\n");
        printf("\033[0;32m   ✓ Verbose output formatting\033[0m\n");
        return 0;
    } else {
        printf("\n\033[0;31m❌ Some tests failed. Review your implementation.\033[0m\n");
        printf("\033[1;33m💡 Common issues:\033[0m\n");
        printf("\033[1;33m   • Check signal handling setup (sigaction vs signal)\033[0m\n");
        printf("\033[1;33m   • Ensure timeout detection works (alarm + EINTR)\033[0m\n");
        printf("\033[1;33m   • Verify process state analysis (WIFEXITED, WIFSIGNALED)\033[0m\n");
        printf("\033[1;33m   • Make sure to kill and wait for timed-out processes\033[0m\n");
        printf("\033[1;33m   • Check verbose output format matches exactly\033[0m\n");
        printf("\033[1;33m   • Verify return values: 1=nice, 0=bad, -1=error\033[0m\n");
        return 1;
    }
}