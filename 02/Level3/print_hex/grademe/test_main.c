#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int g_tests_failed = 0;

void capture_output(char *const args[], char *output, size_t size)
{
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        execv("./print_hex", args);
        perror("execv");
        exit(1);
    }
    
    close(pipefd[1]);
    
    int n = read(pipefd[0], output, size - 1);
    output[n] = '\0';
    close(pipefd[0]);
    
    int status;
    waitpid(pid, &status, 0);
}

void run_test(char *test_name, char *const args[], const char *expected)
{
    char output[256] = {0};
    
    printf("Test %s: ", test_name);
    capture_output(args, output, sizeof(output));
    
    if (strcmp(output, expected) == 0)
        printf("\033[0;32m[OK]\033[0m\n");
    else {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("Expected: \"%s\"\n", expected);
        printf("Got     : \"%s\"\n", output);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing print_hex ===\n\n");
    
    // Test 1: Ejemplo del subject
    {
        char *args[] = {"./print_hex", "10", NULL};
        run_test("Subject example 1", args, "a\n");
    }
    
    // Test 2: Otro ejemplo del subject
    {
        char *args[] = {"./print_hex", "255", NULL};
        run_test("Subject example 2", args, "ff\n");
    }
    
    // Test 3: Ejemplo grande del subject
    {
        char *args[] = {"./print_hex", "5156454", NULL};
        run_test("Subject example 3", args, "4eae66\n");
    }
    
    // Test 4: Sin argumentos
    {
        char *args[] = {"./print_hex", NULL};
        run_test("No arguments", args, "\n");
    }
    
    // Test 5: Cero
    {
        char *args[] = {"./print_hex", "0", NULL};
        run_test("Zero", args, "0\n");
    }
    
    // Test 6: Número pequeño
    {
        char *args[] = {"./print_hex", "16", NULL};
        run_test("Small number", args, "10\n");
    }
    
    // Test 7: Número grande
    {
        char *args[] = {"./print_hex", "1000000", NULL};
        run_test("Big number", args, "f4240\n");
    }
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}