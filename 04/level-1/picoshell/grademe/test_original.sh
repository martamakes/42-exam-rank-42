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
    rm -f test_picoshell picoshell.o test_output.txt expected_output.txt
    rm -f test_main.c integration_test.c integration_test
    rm -f pipeline_test.c pipeline_test resource_test.c resource_test
    rm -f temp_file.txt integration_output.txt pipeline_output.txt
}

# Set up
echo -e "${YELLOW}🧪 PICOSHELL COMPREHENSIVE TESTER${NC}"
echo -e "${YELLOW}====================================${NC}"

# Navigate to rendu directory  
STUDENT_DIR="../../../../rendu/picoshell"
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}❌ Error: No solution found at $STUDENT_DIR${NC}"
    echo -e "${YELLOW}💡 Create your solution: mkdir -p rendu/picoshell && cp your_picoshell.c rendu/picoshell/picoshell.c${NC}"
    exit 1
fi

if [ ! -f "$STUDENT_DIR/picoshell.c" ]; then
    echo -e "${RED}❌ Error: picoshell.c not found at $STUDENT_DIR/picoshell.c${NC}"
    exit 1
fi

echo -e "${BLUE}📁 Testing solution at: $STUDENT_DIR/picoshell.c${NC}"

# Copy student solution for testing
cp "$STUDENT_DIR/picoshell.c" .

print_section "COMPILATION TESTS"

# Test 1: Basic compilation
gcc -Wall -Wextra -Werror -c picoshell.c 2>/dev/null
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

// Student's function prototype
int picoshell(char **cmds[]);

// Test 1: Single command (no pipe)
int test_single_command() {
    char *cmd1[] = {"echo", "Hello World", NULL};
    char **cmds[] = {cmd1, NULL};
    
    // Redirect output to file for verification
    int fd = open("test_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) return 0;
    
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    
    int result = picoshell(cmds);
    
    dup2(saved_stdout, STDOUT_FILENO);
    close(fd);
    close(saved_stdout);
    
    if (result != 0) return 0;
    
    // Check output
    fd = open("test_output.txt", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[100];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "Hello World") != NULL);
    }
    return 0;
}

// Test 2: Two command pipeline
int test_two_command_pipeline() {
    char *cmd1[] = {"echo", "-e", "line1\\nline2\\nline3", NULL};
    char *cmd2[] = {"wc", "-l", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};
    
    // Redirect output to file
    int fd = open("test_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) return 0;
    
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    
    int result = picoshell(cmds);
    
    dup2(saved_stdout, STDOUT_FILENO);
    close(fd);
    close(saved_stdout);
    
    if (result != 0) return 0;
    
    // Check if wc counted lines correctly
    fd = open("test_output.txt", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[50];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        // Should contain "3" (3 lines)
        return (strstr(buffer, "3") != NULL);
    }
    return 0;
}

// Test 3: Three command pipeline
int test_three_command_pipeline() {
    char *cmd1[] = {"echo", "-e", "apple\\nbanana\\ncherry\\napricot", NULL};
    char *cmd2[] = {"grep", "a", NULL};
    char *cmd3[] = {"wc", "-l", NULL};
    char **cmds[] = {cmd1, cmd2, cmd3, NULL};
    
    int fd = open("test_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) return 0;
    
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    
    int result = picoshell(cmds);
    
    dup2(saved_stdout, STDOUT_FILENO);
    close(fd);
    close(saved_stdout);
    
    if (result != 0) return 0;
    
    fd = open("test_output.txt", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[50];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        // Should contain "3" (apple, banana, apricot have 'a')
        return (strstr(buffer, "3") != NULL);
    }
    return 0;
}

// Test 4: Error handling - NULL cmds
int test_null_cmds() {
    int result = picoshell(NULL);
    return (result == 1); // Should return error
}

// Test 5: Error handling - empty command array
int test_empty_cmds() {
    char **cmds[] = {NULL};
    int result = picoshell(cmds);
    return (result == 0); // Should handle gracefully
}

// Test 6: Nonexistent command
int test_nonexistent_command() {
    char *cmd1[] = {"this_command_does_not_exist_12345", NULL};
    char **cmds[] = {cmd1, NULL};
    
    int result = picoshell(cmds);
    return (result == 1); // Should return error
}

// Test 7: Pipeline with nonexistent command
int test_pipeline_with_error() {
    char *cmd1[] = {"echo", "test", NULL};
    char *cmd2[] = {"this_command_does_not_exist_12345", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};
    
    int result = picoshell(cmds);
    return (result == 1); // Should return error
}

// Test 8: Complex pipeline (4 commands)
int test_complex_pipeline() {
    char *cmd1[] = {"echo", "-e", "1\\n2\\n3\\n4\\n5", NULL};
    char *cmd2[] = {"cat", NULL};
    char *cmd3[] = {"grep", "[35]", NULL};
    char *cmd4[] = {"wc", "-l", NULL};
    char **cmds[] = {cmd1, cmd2, cmd3, cmd4, NULL};
    
    int fd = open("test_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) return 0;
    
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    
    int result = picoshell(cmds);
    
    dup2(saved_stdout, STDOUT_FILENO);
    close(fd);
    close(saved_stdout);
    
    if (result != 0) return 0;
    
    fd = open("test_output.txt", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[50];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        // Should contain "2" (lines 3 and 5 match [35])
        return (strstr(buffer, "2") != NULL);
    }
    return 0;
}

// Test 9: Commands with multiple arguments
int test_multiple_arguments() {
    char *cmd1[] = {"echo", "test data here", NULL};
    char *cmd2[] = {"cat", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};
    
    int fd = open("test_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) return 0;
    
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    
    int result = picoshell(cmds);
    
    dup2(saved_stdout, STDOUT_FILENO);
    close(fd);
    close(saved_stdout);
    
    return (result == 0); // Basic success test
}

// Test 10: Stress test - many commands
int test_many_commands() {
    char *cmd1[] = {"echo", "data", NULL};
    char *cmd2[] = {"cat", NULL};
    char *cmd3[] = {"cat", NULL};
    char *cmd4[] = {"cat", NULL};
    char *cmd5[] = {"cat", NULL};
    char *cmd6[] = {"wc", "-c", NULL};
    char **cmds[] = {cmd1, cmd2, cmd3, cmd4, cmd5, cmd6, NULL};
    
    int result = picoshell(cmds);
    return (result == 0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <test_number>\n", argv[0]);
        return 1;
    }
    
    int test_num = atoi(argv[1]);
    int result = 0;
    
    switch (test_num) {
        case 1: result = test_single_command(); break;
        case 2: result = test_two_command_pipeline(); break;
        case 3: result = test_three_command_pipeline(); break;
        case 4: result = test_null_cmds(); break;
        case 5: result = test_empty_cmds(); break;
        case 6: result = test_nonexistent_command(); break;
        case 7: result = test_pipeline_with_error(); break;
        case 8: result = test_complex_pipeline(); break;
        case 9: result = test_multiple_arguments(); break;
        case 10: result = test_many_commands(); break;
        default: 
            printf("Invalid test number\n");
            return 1;
    }
    
    return result ? 0 : 1;
}
EOF

# Compile test program
gcc -Wall -Wextra -Werror picoshell.o test_main.c -o test_picoshell 2>/dev/null
if [ $? -eq 0 ]; then
    print_result "Test compilation" "PASS"
else
    print_result "Test compilation" "FAIL" "Could not compile test program"
    cleanup
    exit 1
fi

print_section "BASIC FUNCTIONALITY TESTS"

# Test 1: Single command (no pipeline)
timeout 10 ./test_picoshell 1
if [ $? -eq 0 ]; then
    print_result "Single command execution" "PASS"
else
    print_result "Single command execution" "FAIL" "Failed to execute single command"
fi

# Test 2: Two command pipeline
timeout 10 ./test_picoshell 2
if [ $? -eq 0 ]; then
    print_result "Two command pipeline" "PASS"
else
    print_result "Two command pipeline" "FAIL" "Failed to execute two-command pipeline"
fi

# Test 3: Three command pipeline
timeout 10 ./test_picoshell 3
if [ $? -eq 0 ]; then
    print_result "Three command pipeline" "PASS"
else
    print_result "Three command pipeline" "FAIL" "Failed to execute three-command pipeline"
fi

print_section "ERROR HANDLING TESTS"

# Test 4: NULL cmds parameter
timeout 5 ./test_picoshell 4
if [ $? -eq 0 ]; then
    print_result "NULL cmds handling" "PASS"
else
    print_result "NULL cmds handling" "FAIL" "Should handle NULL cmds parameter"
fi

# Test 5: Empty command array
timeout 5 ./test_picoshell 5
if [ $? -eq 0 ]; then
    print_result "Empty command array" "PASS"
else
    print_result "Empty command array" "FAIL" "Should handle empty command array"
fi

# Test 6: Nonexistent command
timeout 10 ./test_picoshell 6
if [ $? -eq 0 ]; then
    print_result "Nonexistent command error" "PASS"
else
    print_result "Nonexistent command error" "FAIL" "Should return error for nonexistent command"
fi

# Test 7: Pipeline with error
timeout 10 ./test_picoshell 7
if [ $? -eq 0 ]; then
    print_result "Pipeline error propagation" "PASS"
else
    print_result "Pipeline error propagation" "FAIL" "Should propagate errors in pipeline"
fi

print_section "ADVANCED FUNCTIONALITY TESTS"

# Test 8: Complex pipeline (4 commands)
timeout 15 ./test_picoshell 8
if [ $? -eq 0 ]; then
    print_result "Complex pipeline (4 commands)" "PASS"
else
    print_result "Complex pipeline (4 commands)" "FAIL" "Failed complex pipeline execution"
fi

# Test 9: Commands with multiple arguments
timeout 10 ./test_picoshell 9
if [ $? -eq 0 ]; then
    print_result "Multiple command arguments" "PASS"
else
    print_result "Multiple command arguments" "FAIL" "Failed with multiple arguments"
fi

# Test 10: Stress test
timeout 15 ./test_picoshell 10
if [ $? -eq 0 ]; then
    print_result "Stress test (many commands)" "PASS"
else
    print_result "Stress test (many commands)" "FAIL" "Failed stress test"
fi

print_section "INTEGRATION TESTS"

# Integration test: Real commands like the subject examples
cat > integration_test.c << 'EOF'
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int picoshell(char **cmds[]);

int main() {
    // Test case from subject: echo 'something' | cat | sed 's/a/b/g'
    char *cmd1[] = {"echo", "squalala", NULL};
    char *cmd2[] = {"cat", NULL};
    char *cmd3[] = {"sed", "s/a/b/g", NULL};
    char **cmds[] = {cmd1, cmd2, cmd3, NULL};
    
    // Redirect output to file for verification
    int fd = open("integration_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) return 1;
    
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    
    int result = picoshell(cmds);
    
    dup2(saved_stdout, STDOUT_FILENO);
    close(fd);
    close(saved_stdout);
    
    if (result != 0) return 1;
    
    // Verify output contains "squblblb" (squalala with a->b)
    fd = open("integration_output.txt", O_RDONLY);
    if (fd == -1) return 1;
    
    char buffer[100];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    unlink("integration_output.txt");
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "squblblb") != NULL) ? 0 : 1;
    }
    
    return 1;
}
EOF

gcc picoshell.o integration_test.c -o integration_test 2>/dev/null
if [ $? -eq 0 ]; then
    timeout 10 ./integration_test
    if [ $? -eq 0 ]; then
        print_result "Subject example integration" "PASS"
    else
        print_result "Subject example integration" "FAIL" "Subject example doesn't work correctly"
    fi
else
    print_result "Integration test compilation" "FAIL" "Could not compile integration test"
fi

# Pipeline test: ls | grep (if files exist)
cat > pipeline_test.c << 'EOF'
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int picoshell(char **cmds[]);

int main() {
    // Create test files
    close(open("test_file.txt", O_CREAT, 0644));
    close(open("another_test.txt", O_CREAT, 0644));
    
    // Test: ls | grep test
    char *cmd1[] = {"ls", NULL};
    char *cmd2[] = {"grep", "test", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};
    
    int fd = open("pipeline_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) return 1;
    
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    
    int result = picoshell(cmds);
    
    dup2(saved_stdout, STDOUT_FILENO);
    close(fd);
    close(saved_stdout);
    
    // Clean up test files
    unlink("test_file.txt");
    unlink("another_test.txt");
    unlink("pipeline_output.txt");
    
    return (result == 0) ? 0 : 1;
}
EOF

gcc picoshell.o pipeline_test.c -o pipeline_test 2>/dev/null
if [ $? -eq 0 ]; then
    timeout 10 ./pipeline_test
    if [ $? -eq 0 ]; then
        print_result "Real command pipeline (ls | grep)" "PASS"
    else
        print_result "Real command pipeline (ls | grep)" "FAIL" "Real pipeline test failed"
    fi
else
    print_result "Pipeline test compilation" "FAIL" "Could not compile pipeline test"
fi

print_section "RESOURCE MANAGEMENT TESTS"

# Test for zombie processes and resource leaks
cat > resource_test.c << 'EOF'
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int picoshell(char **cmds[]);

int main() {
    // Run multiple pipelines to test resource management
    for (int i = 0; i < 5; i++) {
        char *cmd1[] = {"echo", "test", NULL};
        char *cmd2[] = {"cat", NULL};
        char *cmd3[] = {"wc", "-c", NULL};
        char **cmds[] = {cmd1, cmd2, cmd3, NULL};
        
        int result = picoshell(cmds);
        if (result != 0) return 1;
    }
    
    // If we get here without hanging or crashing, resources are managed well
    return 0;
}
EOF

gcc picoshell.o resource_test.c -o resource_test 2>/dev/null
if [ $? -eq 0 ]; then
    timeout 15 ./resource_test
    if [ $? -eq 0 ]; then
        print_result "Resource management (multiple runs)" "PASS"
    else
        print_result "Resource management (multiple runs)" "FAIL" "Resource management issues detected"
    fi
else
    print_result "Resource test compilation" "FAIL" "Could not compile resource test"
fi

# Final results
print_section "TEST RESULTS SUMMARY"
TOTAL_TESTS=$((TESTS_PASSED + TESTS_FAILED))
echo -e "Total tests: ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! Your picoshell implementation is working correctly!${NC}"
    echo -e "${GREEN}📝 Your solution handles:${NC}"
    echo -e "${GREEN}   ✓ Single command execution${NC}"
    echo -e "${GREEN}   ✓ Simple and complex pipelines${NC}"
    echo -e "${GREEN}   ✓ Error handling (NULL params, failed commands)${NC}"
    echo -e "${GREEN}   ✓ Multiple command arguments${NC}"
    echo -e "${GREEN}   ✓ Process synchronization and cleanup${NC}"
    echo -e "${GREEN}   ✓ Resource management${NC}"
    cleanup
    exit 0
else
    echo -e "\n${RED}❌ Some tests failed. Review your implementation.${NC}"
    echo -e "${YELLOW}💡 Common issues:${NC}"
    echo -e "${YELLOW}   • Check NULL parameter validation${NC}"
    echo -e "${YELLOW}   • Ensure proper pipe creation and management${NC}"
    echo -e "${YELLOW}   • Verify dup2 usage for stdin/stdout redirection${NC}"
    echo -e "${YELLOW}   • Make sure to close all unused file descriptors${NC}"
    echo -e "${YELLOW}   • Check fork() and execvp() error handling${NC}"
    echo -e "${YELLOW}   • Ensure all child processes are waited for${NC}"
    echo -e "${YELLOW}   • Verify error code propagation in pipelines${NC}"
    cleanup
    exit 1
fi
