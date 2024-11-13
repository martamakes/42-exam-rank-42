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
        
        execv("./do_op", args);
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
        printf("  Arguments: ");
        for (int i = 1; args[i]; i++)
            printf("%s ", args[i]);
        printf("\n");
        printf("  Expected : \"%s\"\n", expected);
        printf("  Got      : \"%s\"\n", output);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing do_op ===\n\n");
    
    // Test 1: Ejemplo del subject - multiplicación
    {
        char *args[] = {"./do_op", "123", "*", "456", NULL};
        run_test("Subject example 1", args, "56088\n");
    }
    
    // Test 2: Ejemplo del subject - división
    {
        char *args[] = {"./do_op", "9828", "/", "234", NULL};
        run_test("Subject example 2", args, "42\n");
    }
    
    // Test 3: Ejemplo del subject - suma con negativo
    {
        char *args[] = {"./do_op", "1", "+", "-43", NULL};
        run_test("Subject example 3", args, "-42\n");
    }
    
    // Test 4: Sin argumentos
    {
        char *args[] = {"./do_op", NULL};
        run_test("No arguments", args, "\n");
    }
    
    // Test 5: Resta
    {
        char *args[] = {"./do_op", "44", "-", "2", NULL};
        run_test("Subtraction", args, "42\n");
    }
    
    // Test 6: Módulo
    {
        char *args[] = {"./do_op", "45", "%", "3", NULL};
        run_test("Modulo", args, "0\n");
    }
    
    // Test 7: Números grandes
    {
        char *args[] = {"./do_op", "2147483647", "+", "0", NULL};
        run_test("Big numbers", args, "2147483647\n");
    }
    
    // Test 8: Números negativos
    {
        char *args[] = {"./do_op", "-21", "*", "-2", NULL};
        run_test("Negative numbers", args, "42\n");
    }
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}