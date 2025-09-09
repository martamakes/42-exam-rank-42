#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>

// Variable global para contar los tests fallidos
int g_tests_failed = 0;

// Prototipo de la función a testear
int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

// Test helper function
void run_test(const char *test_name, int (*test_func)()) {
    printf("Testing %s... ", test_name);
    if (test_func()) {
        printf("✓ PASS\n");
    } else {
        printf("✗ FAIL\n");
        g_tests_failed++;
    }
}

// Test functions for sandbox

// 1. Nice function - should return successfully
void nice_function(void) {
    // This function does nothing bad
    return;
}

// 2. Bad function - exits with code 1
void bad_function_exit_1(void) {
    exit(1);
}

// 3. Bad function - exits with code 42
void bad_function_exit_42(void) {
    exit(42);
}

// 4. Bad function - causes segfault
void bad_function_segfault(void) {
    int *p = NULL;
    *p = 42; // This will cause a segfault
}

// 5. Bad function - infinite loop (for timeout test)
void bad_function_infinite_loop(void) {
    while (1) {
        // Do nothing forever
        usleep(1000);
    }
}

// 6. Bad function - abort
void bad_function_abort(void) {
    abort();
}

// 7. Function that does some work but exits cleanly
void nice_function_with_work(void) {
    for (int i = 0; i < 1000; i++) {
        // Do some work
    }
    // Exit cleanly
}

// Test cases

// Test 1: Nice function should return 1
int test_nice_function() {
    int result = sandbox(nice_function, 5, false);
    return (result == 1);
}

// Test 2: Bad function (exit code 1) should return 0
int test_bad_exit_code_1() {
    int result = sandbox(bad_function_exit_1, 5, false);
    return (result == 0);
}

// Test 3: Bad function (exit code 42) should return 0
int test_bad_exit_code_42() {
    int result = sandbox(bad_function_exit_42, 5, false);
    return (result == 0);
}

// Test 4: Bad function (segfault) should return 0
int test_bad_segfault() {
    int result = sandbox(bad_function_segfault, 5, false);
    return (result == 0);
}

// Test 5: Timeout function should return 0
int test_timeout() {
    int result = sandbox(bad_function_infinite_loop, 1, false); // 1 second timeout
    return (result == 0);
}

// Test 6: Abort function should return 0
int test_abort() {
    int result = sandbox(bad_function_abort, 5, false);
    return (result == 0);
}

// Test 7: Nice function with work should return 1
int test_nice_function_with_work() {
    int result = sandbox(nice_function_with_work, 5, false);
    return (result == 1);
}

// Test 8: Test verbose mode (nice function)
int test_verbose_nice() {
    int result = sandbox(nice_function, 5, true);
    // Just check that it returns the right value
    // The verbose output will be visible but not captured
    return (result == 1);
}

// Test 9: Test verbose mode (bad function with exit code)
int test_verbose_bad_exit() {
    int result = sandbox(bad_function_exit_42, 5, true);
    // Just check that it returns the right value
    // The verbose output will be visible but not captured
    return (result == 0);
}

// Test 10: Error handling - NULL function pointer
int test_null_function() {
    int result = sandbox(NULL, 5, false);
    return (result == -1); // Should return -1 for error
}

int main() {
    printf("🧪 SANDBOX TESTER\n");
    printf("=================\n\n");
    
    printf("📋 BASIC TESTS:\n");
    run_test("Nice function", test_nice_function);
    run_test("Bad exit code 1", test_bad_exit_code_1);
    run_test("Bad exit code 42", test_bad_exit_code_42);
    run_test("Nice function with work", test_nice_function_with_work);
    
    printf("\n🚨 SIGNAL TESTS:\n");
    run_test("Segfault detection", test_bad_segfault);
    run_test("Abort detection", test_abort);
    
    printf("\n⏰ TIMEOUT TESTS:\n");
    run_test("Timeout detection", test_timeout);
    
    printf("\n📢 VERBOSE TESTS:\n");
    run_test("Verbose nice function", test_verbose_nice);
    run_test("Verbose bad exit", test_verbose_bad_exit);
    
    printf("\n🛡️  ERROR HANDLING TESTS:\n");
    run_test("NULL function pointer", test_null_function);
    
    printf("\n📊 RESULTS:\n");
    if (g_tests_failed == 0) {
        printf("✅ ALL TESTS PASSED! Your sandbox works correctly!\n");
        return 0;
    } else {
        printf("❌ %d test(s) failed. Review your implementation.\n", g_tests_failed);
        return 1;
    }
}