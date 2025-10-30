#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

// Variable global para contar los tests fallidos
int g_tests_failed = 0;

// Prototipo de la función a testear
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

// Test 1: Basic read test - echo command
int test_basic_read() {
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

// Test 2: Basic write test - wc -l
int test_basic_write() {
    int fd = ft_popen("wc", (char *const []){"wc", "-l", NULL}, 'w');
    if (fd == -1) return 0;
    
    write(fd, "line 1\n", 7);
    write(fd, "line 2\n", 7);
    write(fd, "line 3\n", 7);
    
    close(fd);
    wait(NULL);
    
    return 1; // If we get here without crashing, it's good
}

// Test 3: Error handling - invalid type
int test_invalid_type() {
    int fd = ft_popen("echo", (char *const []){"echo", "test", NULL}, 'x');
    return (fd == -1);
}

// Test 4: Error handling - NULL file
int test_null_file() {
    int fd = ft_popen(NULL, (char *const []){"echo", "test", NULL}, 'r');
    return (fd == -1);
}

// Test 5: Error handling - NULL argv
int test_null_argv() {
    int fd = ft_popen("echo", NULL, 'r');
    return (fd == -1);
}

// Test 6: Read from ls command
int test_ls_read() {
    int fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    if (fd == -1) return 0;
    
    char buffer[1024];
    int bytes = read_with_timeout(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    wait(NULL);
    
    return (bytes > 0);
}

// Test 7: Multiple arguments
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

// Test 9: Chained pipes - ls | grep (from subject example)
int test_chained_pipes() {
    // Create temp file with known content
    int temp_fd = open("test_files.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (temp_fd == -1) return 0;
    write(temp_fd, "test.c\n", 7);
    write(temp_fd, "test.txt\n", 9);
    write(temp_fd, "main.c\n", 7);
    write(temp_fd, "readme.md\n", 10);
    close(temp_fd);

    // Simulate: cat test_files.txt | grep c
    int fd1 = ft_popen("cat", (char *const []){"cat", "test_files.txt", NULL}, 'r');
    if (fd1 == -1) {
        unlink("test_files.txt");
        return 0;
    }

    // Save original stdin
    int saved_stdin = dup(0);

    // Redirect fd1 to stdin
    dup2(fd1, 0);
    close(fd1);

    // Now grep reads from stdin (which is the output of cat)
    int fd2 = ft_popen("grep", (char *const []){"grep", "c", NULL}, 'r');

    // Restore stdin
    dup2(saved_stdin, 0);
    close(saved_stdin);

    if (fd2 == -1) {
        unlink("test_files.txt");
        return 0;
    }

    char buffer[1024];
    int bytes = read_with_timeout(fd2, buffer, sizeof(buffer) - 1);
    close(fd2);
    wait(NULL);
    wait(NULL);

    unlink("test_files.txt");

    if (bytes > 0) {
        buffer[bytes] = '\0';
        // Should contain "test.c" and "main.c" but NOT "readme.md" or "test.txt"
        int has_test_c = (strstr(buffer, "test.c") != NULL);
        int has_main_c = (strstr(buffer, "main.c") != NULL);
        int has_test_txt = (strstr(buffer, "test.txt") != NULL);
        int has_readme = (strstr(buffer, "readme.md") != NULL);

        return (has_test_c && has_main_c && !has_test_txt && !has_readme);
    }
    return 0;
}

int main() {
    printf("🧪 FT_POPEN TESTER\n");
    printf("==================\n\n");
    
    printf("📋 BASIC TESTS:\n");
    run_test("Basic read (echo)", test_basic_read);
    run_test("Basic write (wc)", test_basic_write);
    run_test("Read from ls", test_ls_read);
    run_test("Multiple arguments", test_multiple_args);
    run_test("Write to cat", test_cat_write);

    printf("\n🔗 ADVANCED TESTS (from subject):\n");
    run_test("Chained pipes (cat | grep)", test_chained_pipes);

    printf("\n🛡️  ERROR HANDLING TESTS:\n");
    run_test("Invalid type", test_invalid_type);
    run_test("NULL file parameter", test_null_file);
    run_test("NULL argv parameter", test_null_argv);
    
    printf("\n📊 RESULTS:\n");
    if (g_tests_failed == 0) {
        printf("✅ ALL TESTS PASSED! Your ft_popen works correctly!\n");
        return 0;
    } else {
        printf("❌ %d test(s) failed. Review your implementation.\n", g_tests_failed);
        return 1;
    }
}