// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 4096
int g_tests_failed = 0;

void capture_output(char *const args[], char *output)
{
    int pipefd[2];
    int status;
    
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
        
        execv("./add_prime_sum", args);
        perror("execv");
        exit(1);
    }
    
    close(pipefd[1]);
    
    int total = 0;
    int n;
    while ((n = read(pipefd[0], output + total, BUFFER_SIZE - total - 1)) > 0)
        total += n;
    
    output[total] = '\0';
    close(pipefd[0]);
    
    waitpid(pid, &status, 0);
}

void run_test(char *test_name, char *const args[], const char *expected)
{
    char output[BUFFER_SIZE] = {0};
    
    printf("Test %s: ", test_name);
    capture_output(args, output);
    
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
    printf("\n=== Testing add_prime_sum ===\n\n");
    
    // Test 1: Sin argumentos
    {
        char *args[] = {"./add_prime_sum", NULL};
        run_test("No arguments", args, "0\n");
    }
    
    // Test 2: Múltiples argumentos
    {
        char *args[] = {"./add_prime_sum", "5", "7", NULL};
        run_test("Multiple arguments", args, "0\n");
    }
    
    // Test 3: Número negativo
    {
        char *args[] = {"./add_prime_sum", "-5", NULL};
        run_test("Negative number", args, "0\n");
    }
    
    // Test 4: Número no válido
    {
        char *args[] = {"./add_prime_sum", "12a", NULL};
        run_test("Invalid number", args, "0\n");
    }
    
    // Test 5: Caso del subject (5)
    {
        char *args[] = {"./add_prime_sum", "5", NULL};
        run_test("Subject example 1 (5)", args, "10\n");
    }
    
    // Test 6: Caso del subject (7)
    {
        char *args[] = {"./add_prime_sum", "7", NULL};
        run_test("Subject example 2 (7)", args, "17\n");
    }
    
    // Test 7: Número pequeño (1)
    {
        char *args[] = {"./add_prime_sum", "1", NULL};
        run_test("Small number (1)", args, "0\n");
    }
    
    // Test 8: Número primo
    {
        char *args[] = {"./add_prime_sum", "11", NULL};
        run_test("Prime number (11)", args, "28\n");
    }
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}