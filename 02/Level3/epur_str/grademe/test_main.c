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
        
        execv("./epur_str", args);
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
    printf("\n=== Testing epur_str ===\n\n");
    
    // Test 1: Frase normal
    {
        char *args[] = {"./epur_str", "See? It's easy to print the same thing", NULL};
        run_test("Normal phrase", args, "See? It's easy to print the same thing\n");
    }
    
    // Test 2: Múltiples espacios y tabs
    {
        char *args[] = {"./epur_str", "  this\t  time it      will     be    more complex  . ", NULL};
        run_test("Multiple spaces", args, "this time it will be more complex .\n");
    }
    
    // Test 3: Sin argumentos
    {
        char *args[] = {"./epur_str", NULL};
        run_test("No arguments", args, "\n");
    }
    
    // Test 4: String vacío
    {
        char *args[] = {"./epur_str", "", NULL};
        run_test("Empty string", args, "\n");
    }
    
    // Test 5: Solo espacios y tabs
    {
        char *args[] = {"./epur_str", "  \t  \t  ", NULL};
        run_test("Only spaces and tabs", args, "\n");
    }
    
    // Test 6: Múltiples argumentos
    {
        char *args[] = {"./epur_str", "hello", "world", NULL};
        run_test("Multiple arguments", args, "\n");
    }
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}