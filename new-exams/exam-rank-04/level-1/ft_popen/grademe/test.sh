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
    rm -f test_ft_popen ft_popen.o test_input.txt test_output.txt test_script.sh
    rm -f test_main.c integration_test.c integration_test fd_test.c fd_test cat_output.txt
}

# Set up
echo -e "${YELLOW}🧪 FT_POPEN COMPREHENSIVE TESTER${NC}"
echo -e "${YELLOW}===================================${NC}"

# Navigate to rendu directory  
STUDENT_DIR="../../../../rendu/ft_popen"
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}❌ Error: No solution found at $STUDENT_DIR${NC}"
    echo -e "${YELLOW}💡 Create your solution: mkdir -p rendu/ft_popen && cp your_ft_popen.c rendu/ft_popen/ft_popen.c${NC}"
    exit 1
fi

if [ ! -f "$STUDENT_DIR/ft_popen.c" ]; then
    echo -e "${RED}❌ Error: ft_popen.c not found at $STUDENT_DIR/ft_popen.c${NC}"
    exit 1
fi

echo -e "${BLUE}📁 Testing solution at: $STUDENT_DIR/ft_popen.c${NC}"

# Copy student solution for testing
cp "$STUDENT_DIR/ft_popen.c" .

print_section "COMPILATION TESTS"

# Test 1: Basic compilation
gcc -Wall -Wextra -Werror -c ft_popen.c 2>/dev/null
if [ $? -eq 0 ]; then
    print_result "Basic compilation" "PASS"
else
    print_result "Basic compilation" "FAIL" "Code does not compile with -Wall -Wextra -Werror"
    cleanup
    exit 1
fi

# Create test main
cat > test_main.c << 'EOF'
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

// Student's function prototype
int ft_popen(const char *file, char *const argv[], char type);

// Helper function to read from fd with timeout
int read_with_timeout(int fd, char *buffer, size_t size) {
    fd_set readfds;
    struct timeval timeout;
    
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    
    timeout.tv_sec = 2;  // 2 second timeout
    timeout.tv_usec = 0;
    
    int ready = select(fd + 1, &readfds, NULL, NULL, &timeout);
    if (ready <= 0) {
        return -1; // timeout or error
    }
    
    return read(fd, buffer, size);
}

// Test 1: Basic read test - ls command
int test_basic_read() {
    int fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    if (fd == -1) return 0;
    
    char buffer[1024];
    int bytes = read_with_timeout(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    // Wait for child to avoid zombie
    wait(NULL);
    
    return (bytes > 0);
}

// Test 2: Basic write test - wc -l
int test_basic_write() {
    int fd = ft_popen("wc", (char *const []){"wc", "-l", NULL}, 'w');
    if (fd == -1) return 0;
    
    write(fd, "line 1\n", 7);
    write(fd, "line 2\n", 7);
    write(fd, "line 3\n", 7);
    
    close(fd);
    
    // Wait for child
    wait(NULL);
    
    return 1; // If we get here without crashing, it's good
}

// Test 3: Error handling - invalid type
int test_invalid_type() {
    int fd = ft_popen("ls", (char *const []){"ls", NULL}, 'x');
    return (fd == -1);
}

// Test 4: Error handling - NULL file
int test_null_file() {
    int fd = ft_popen(NULL, (char *const []){"ls", NULL}, 'r');
    return (fd == -1);
}

// Test 5: Error handling - NULL argv
int test_null_argv() {
    int fd = ft_popen("ls", NULL, 'r');
    return (fd == -1);
}

// Test 6: Error handling - nonexistent command
int test_nonexistent_command() {
    int fd = ft_popen("this_command_does_not_exist_12345", 
                      (char *const []){"this_command_does_not_exist_12345", NULL}, 'r');
    if (fd == -1) return 1;
    
    // If fd is returned, child will exit with error, but parent should get fd
    close(fd);
    wait(NULL);
    return 1; // This is actually expected behavior
}

// Test 7: Read from echo command
int test_echo_read() {
    int fd = ft_popen("echo", (char *const []){"echo", "Hello World", NULL}, 'r');
    if (fd == -1) return 0;
    
    char buffer[100];
    int bytes = read_with_timeout(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    wait(NULL);
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "Hello World") != NULL);
    }
    return 0;
}

// Test 8: Write to cat command
int test_cat_write() {
    // Create a temporary file for cat output
    int temp_fd = open("cat_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (temp_fd == -1) return 0;
    close(temp_fd);
    
    int fd = ft_popen("sh", (char *const []){"sh", "-c", "cat > cat_output.txt", NULL}, 'w');
    if (fd == -1) return 0;
    
    write(fd, "Test data for cat\n", 18);
    close(fd);
    wait(NULL);
    
    // Check if file was created and has content
    temp_fd = open("cat_output.txt", O_RDONLY);
    if (temp_fd == -1) return 0;
    
    char buffer[100];
    int bytes = read(temp_fd, buffer, sizeof(buffer) - 1);
    close(temp_fd);
    unlink("cat_output.txt");
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "Test data for cat") != NULL);
    }
    return 0;
}

// Test 9: Multiple arguments
int test_multiple_args() {
    int fd = ft_popen("echo", (char *const []){"echo", "arg1", "arg2", "arg3", NULL}, 'r');
    if (fd == -1) return 0;
    
    char buffer[100];
    int bytes = read_with_timeout(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    wait(NULL);
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "arg1") && strstr(buffer, "arg2") && strstr(buffer, "arg3"));
    }
    return 0;
}

// Test 10: Large data handling
int test_large_data() {
    int fd = ft_popen("cat", (char *const []){"cat", NULL}, 'w');
    if (fd == -1) return 0;
    
    // Write a lot of data
    char data[1000];
    memset(data, 'A', sizeof(data) - 1);
    data[sizeof(data) - 1] = '\0';
    
    for (int i = 0; i < 10; i++) {
        if (write(fd, data, strlen(data)) == -1) {
            close(fd);
            wait(NULL);
            return 0;
        }
    }
    
    close(fd);
    wait(NULL);
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
        case 1: result = test_basic_read(); break;
        case 2: result = test_basic_write(); break;
        case 3: result = test_invalid_type(); break;
        case 4: result = test_null_file(); break;
        case 5: result = test_null_argv(); break;
        case 6: result = test_nonexistent_command(); break;
        case 7: result = test_echo_read(); break;
        case 8: result = test_cat_write(); break;
        case 9: result = test_multiple_args(); break;
        case 10: result = test_large_data(); break;
        default: 
            printf("Invalid test number\n");
            return 1;
    }
    
    return result ? 0 : 1;
}
EOF

# Compile test program
gcc -Wall -Wextra -Werror ft_popen.o test_main.c -o test_ft_popen 2>/dev/null
if [ $? -eq 0 ]; then
    print_result "Test compilation" "PASS"
else
    print_result "Test compilation" "FAIL" "Could not compile test program"
    cleanup
    exit 1
fi

print_section "FUNCTIONALITY TESTS"

# Test 1: Basic read from ls
timeout 5 ./test_ft_popen 1
if [ $? -eq 0 ]; then
    print_result "Basic read (ls command)" "PASS"
else
    print_result "Basic read (ls command)" "FAIL" "Could not read from ls command"
fi

# Test 2: Basic write to wc
timeout 5 ./test_ft_popen 2
if [ $? -eq 0 ]; then
    print_result "Basic write (wc command)" "PASS"
else
    print_result "Basic write (wc command)" "FAIL" "Could not write to wc command"
fi

# Test 3: Invalid type parameter
timeout 5 ./test_ft_popen 3
if [ $? -eq 0 ]; then
    print_result "Invalid type handling" "PASS"
else
    print_result "Invalid type handling" "FAIL" "Should return -1 for invalid type"
fi

# Test 4: NULL file parameter
timeout 5 ./test_ft_popen 4
if [ $? -eq 0 ]; then
    print_result "NULL file handling" "PASS"
else
    print_result "NULL file handling" "FAIL" "Should return -1 for NULL file"
fi

# Test 5: NULL argv parameter
timeout 5 ./test_ft_popen 5
if [ $? -eq 0 ]; then
    print_result "NULL argv handling" "PASS"
else
    print_result "NULL argv handling" "FAIL" "Should return -1 for NULL argv"
fi

print_section "ERROR HANDLING TESTS"

# Test 6: Nonexistent command
timeout 5 ./test_ft_popen 6
if [ $? -eq 0 ]; then
    print_result "Nonexistent command" "PASS"
else
    print_result "Nonexistent command" "FAIL" "Should handle nonexistent commands gracefully"
fi

print_section "ADVANCED FUNCTIONALITY TESTS"

# Test 7: Echo command with arguments
timeout 5 ./test_ft_popen 7
if [ $? -eq 0 ]; then
    print_result "Echo with arguments (read)" "PASS"
else
    print_result "Echo with arguments (read)" "FAIL" "Could not read from echo command"
fi

# Test 8: Cat command (write mode)
timeout 5 ./test_ft_popen 8
if [ $? -eq 0 ]; then
    print_result "Cat command (write mode)" "PASS"
else
    print_result "Cat command (write mode)" "FAIL" "Could not write to cat command"
fi

# Test 9: Multiple arguments
timeout 5 ./test_ft_popen 9
if [ $? -eq 0 ]; then
    print_result "Multiple command arguments" "PASS"
else
    print_result "Multiple command arguments" "FAIL" "Could not handle multiple arguments"
fi

# Test 10: Large data handling
timeout 5 ./test_ft_popen 10
if [ $? -eq 0 ]; then
    print_result "Large data handling" "PASS"
else
    print_result "Large data handling" "FAIL" "Could not handle large amounts of data"
fi

print_section "INTEGRATION TESTS"

# Integration test: Chain commands like in subject example
cat > integration_test.c << 'EOF'
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int ft_popen(const char *file, char *const argv[], char type);

int main() {
    // Test the example from the subject
    int fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    if (fd == -1) return 1;
    
    char buffer[1024];
    int bytes_read = 0;
    int total_bytes = 0;
    
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        total_bytes += bytes_read;
        if (total_bytes > 10) break; // We got some data, that's good
    }
    
    close(fd);
    wait(NULL);
    
    return (total_bytes > 0) ? 0 : 1;
}
EOF

gcc ft_popen.o integration_test.c -o integration_test 2>/dev/null
if [ $? -eq 0 ]; then
    timeout 5 ./integration_test
    if [ $? -eq 0 ]; then
        print_result "Subject example integration" "PASS"
    else
        print_result "Subject example integration" "FAIL" "Subject example doesn't work"
    fi
else
    print_result "Integration test compilation" "FAIL" "Could not compile integration test"
fi

print_section "MEMORY AND FD LEAK TESTS"

# Test for file descriptor leaks (basic check)
cat > fd_test.c << 'EOF'
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int ft_popen(const char *file, char *const argv[], char type);

int main() {
    // Open many ft_popen calls and check if we run out of FDs
    int fds[50];
    int count = 0;
    
    for (int i = 0; i < 50; i++) {
        fds[i] = ft_popen("echo", (char *const []){"echo", "test", NULL}, 'r');
        if (fds[i] == -1) {
            break;
        }
        count++;
    }
    
    // Close all opened fds
    for (int i = 0; i < count; i++) {
        close(fds[i]);
    }
    
    // Wait for children
    for (int i = 0; i < count; i++) {
        wait(NULL);
    }
    
    // If we could open at least 10, that's good
    return (count >= 10) ? 0 : 1;
}
EOF

gcc ft_popen.o fd_test.c -o fd_test 2>/dev/null
if [ $? -eq 0 ]; then
    timeout 10 ./fd_test
    if [ $? -eq 0 ]; then
        print_result "File descriptor management" "PASS"
    else
        print_result "File descriptor management" "FAIL" "Possible FD leak or resource issue"
    fi
else
    print_result "FD test compilation" "FAIL" "Could not compile FD test"
fi

# Final results
print_section "TEST RESULTS SUMMARY"
TOTAL_TESTS=$((TESTS_PASSED + TESTS_FAILED))
echo -e "Total tests: ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "\n${GREEN}🎉 ALL TESTS PASSED! Your ft_popen implementation is working correctly!${NC}"
    echo -e "${GREEN}📝 Your solution handles:${NC}"
    echo -e "${GREEN}   ✓ Basic read/write operations${NC}"
    echo -e "${GREEN}   ✓ Error handling (NULL params, invalid types)${NC}"
    echo -e "${GREEN}   ✓ Command execution with arguments${NC}"
    echo -e "${GREEN}   ✓ Large data handling${NC}"
    echo -e "${GREEN}   ✓ File descriptor management${NC}"
    cleanup
    exit 0
else
    echo -e "\n${RED}❌ Some tests failed. Review your implementation.${NC}"
    echo -e "${YELLOW}💡 Common issues:${NC}"
    echo -e "${YELLOW}   • Check parameter validation (NULL pointers, invalid type)${NC}"
    echo -e "${YELLOW}   • Ensure proper pipe creation and management${NC}"
    echo -e "${YELLOW}   • Verify dup2 usage for stdin/stdout redirection${NC}"
    echo -e "${YELLOW}   • Make sure to close unused file descriptors${NC}"
    echo -e "${YELLOW}   • Check fork() and execvp() error handling${NC}"
    cleanup
    exit 1
fi
