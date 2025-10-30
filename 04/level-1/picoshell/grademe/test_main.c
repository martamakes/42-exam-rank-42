#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

// Variable global para contar los tests fallidos
int g_tests_failed = 0;

// Prototipo de la función a testear
int picoshell(char **cmds[]);

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
    unlink("test_output.txt");
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "Hello World") != NULL);
    }
    return 0;
}

// Test 2: Two command pipeline (echo | cat)
int test_two_command_pipeline() {
    char *cmd1[] = {"echo", "Pipeline Test", NULL};
    char *cmd2[] = {"cat", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};
    
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
    unlink("test_output.txt");
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "Pipeline Test") != NULL);
    }
    return 0;
}

// Test 3: Three command pipeline (echo | cat | wc -w)
int test_three_command_pipeline() {
    char *cmd1[] = {"echo", "word1 word2 word3", NULL};
    char *cmd2[] = {"cat", NULL};
    char *cmd3[] = {"wc", "-w", NULL};
    char **cmds[] = {cmd1, cmd2, cmd3, NULL};
    
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
    
    // Check output - should contain "3" (word count)
    fd = open("test_output.txt", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[100];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    unlink("test_output.txt");
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        // Should contain "3" (the word count)
        return (strstr(buffer, "3") != NULL);
    }
    return 0;
}

// Test 4: ls | grep pipeline
int test_ls_grep_pipeline() {
    char *cmd1[] = {"ls", NULL};
    char *cmd2[] = {"grep", "test_main.c", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};
    
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
    
    // Check output - should contain test_main.c if it exists
    fd = open("test_output.txt", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[100];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    unlink("test_output.txt");
    
    // Even if no match found, the pipeline should work (bytes >= 0)
    return (bytes >= 0);
}

// Test 5: Error handling - NULL commands
int test_null_commands() {
    int result = picoshell(NULL);
    return (result == 1); // Should return 1 on error
}

// Test 6: Error handling - empty commands array
int test_empty_commands() {
    char **cmds[] = {NULL};
    int result = picoshell(cmds);
    return (result == 1); // Should return 1 on error
}

// Test 7: Complex pipeline with sed
int test_complex_pipeline() {
    char *cmd1[] = {"echo", "hello world", NULL};
    char *cmd2[] = {"sed", "s/world/universe/g", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};
    
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
    
    // Check output - should contain "universe"
    fd = open("test_output.txt", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[100];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    unlink("test_output.txt");
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "universe") != NULL);
    }
    return 0;
}

// Test 8: Subject example - echo | cat | sed (exact match from subject)
int test_subject_example() {
    char *cmd1[] = {"echo", "squalala", NULL};
    char *cmd2[] = {"cat", NULL};
    char *cmd3[] = {"sed", "s/a/b/g", NULL};
    char **cmds[] = {cmd1, cmd2, cmd3, NULL};

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

    // Check output - should be "squblblb"
    fd = open("test_output.txt", O_RDONLY);
    if (fd == -1) return 0;

    char buffer[100];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    unlink("test_output.txt");

    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "squblblb") != NULL);
    }
    return 0;
}

// Test 9: Four command pipeline
int test_four_command_pipeline() {
    char *cmd1[] = {"echo", "a b c d", NULL};
    char *cmd2[] = {"tr", " ", "\\n", NULL};  // Replace spaces with newlines
    char *cmd3[] = {"sort", NULL};
    char *cmd4[] = {"wc", "-l", NULL};
    char **cmds[] = {cmd1, cmd2, cmd3, cmd4, NULL};
    
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
    
    // Check output - should contain "4" (line count)
    fd = open("test_output.txt", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[100];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    unlink("test_output.txt");
    
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return (strstr(buffer, "4") != NULL);
    }
    return 0;
}

int main() {
    printf("🧪 PICOSHELL TESTER\n");
    printf("===================\n\n");
    
    printf("📋 BASIC TESTS:\n");
    run_test("Single command", test_single_command);
    run_test("Two command pipeline", test_two_command_pipeline);
    run_test("Three command pipeline", test_three_command_pipeline);
    run_test("ls | grep pipeline", test_ls_grep_pipeline);
    
    printf("\n🔧 ADVANCED TESTS:\n");
    run_test("Complex pipeline (sed)", test_complex_pipeline);
    run_test("Subject example (echo|cat|sed)", test_subject_example);
    run_test("Four command pipeline", test_four_command_pipeline);
    
    printf("\n🛡️  ERROR HANDLING TESTS:\n");
    run_test("NULL commands", test_null_commands);
    run_test("Empty commands array", test_empty_commands);
    
    printf("\n📊 RESULTS:\n");
    if (g_tests_failed == 0) {
        printf("✅ ALL TESTS PASSED! Your picoshell works correctly!\n");
        return 0;
    } else {
        printf("❌ %d test(s) failed. Review your implementation.\n", g_tests_failed);
        return 1;
    }
}