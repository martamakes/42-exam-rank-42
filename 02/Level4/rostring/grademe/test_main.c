#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 4096
int g_tests_failed = 0;

// Función para capturar la salida del programa
void capture_output(char *const args[], char *output)
{
    int pipefd[2];
    int status;
    
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        execv("./rostring", args);
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

// Función auxiliar para ejecutar un test individual
void run_test(char *test_name, char *const args[], const char *expected)
{
    char output[BUFFER_SIZE] = {0};
    
    printf("Test %s: ", test_name);
    capture_output(args, output);
    
    if (strcmp(output, expected) == 0)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("Expected: \"%s\"\n", expected);
        printf("Got     : \"%s\"\n", output);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing rostring ===\n\n");
    
    // Test 1: Caso básico
    {
        char *args[] = {"./rostring", "Que la lumiere soit et la lumiere fut", NULL};
        run_test("Basic case", args, "la lumiere soit et la lumiere fut Que\n");
    }
    
    // Test 2: Múltiples espacios y tabs
    {
        char *args[] = {"./rostring", "     AkjhZ zLKIJz , 23y", NULL};
        run_test("Multiple spaces", args, "zLKIJz , 23y AkjhZ\n");
    }
    
    // Test 3: Una sola palabra
    {
        char *args[] = {"./rostring", "first", NULL};
        run_test("Single word", args, "first\n");
    }
    
    // Test 4: Espacios al final
    {
        char *args[] = {"./rostring", "abc   ", NULL};
        run_test("Trailing spaces", args, "abc\n");
    }
    
    // Test 5: Sin argumentos
    {
        char *args[] = {"./rostring", NULL};
        run_test("No arguments", args, "\n");
    }
    
    // Test 6: String vacío
    {
        char *args[] = {"./rostring", "", NULL};
        run_test("Empty string", args, "\n");
    }
    
    // Test 7: Solo espacios
    {
        char *args[] = {"./rostring", "   ", NULL};
        run_test("Only spaces", args, "\n");
    }
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}