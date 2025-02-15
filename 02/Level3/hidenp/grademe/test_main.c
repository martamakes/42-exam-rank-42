 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int g_tests_failed = 0;

// Función para capturar la salida del programa
void capture_output(char *const args[], char *output)
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
        
        execv("./hidenp", args);
        perror("execv");
        exit(1);
    }
    
    close(pipefd[1]);
    
    int n = read(pipefd[0], output, 2);
    output[n] = '\0';
    close(pipefd[0]);
    
    int status;
    waitpid(pid, &status, 0);
}

void run_test(char *test_name, char *const args[], const char *expected)
{
    char output[3] = {0}; // Suficiente para "1\n" o "0\n"
    
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
    printf("\n=== Testing hidenp ===\n\n");
    
    // Test 1: Ejemplo del subject
    {
        char *args[] = {"./hidenp", "fgex.;", 
            "tyf34gdgf;'ektufjhgdgex.;.;rtjynur6", NULL};
        run_test("Subject example 1", args, "1\n");
    }
    
    // Test 2: Otro ejemplo del subject
    {
        char *args[] = {"./hidenp", "abc", "2altrb53c.sse", NULL};
        run_test("Subject example 2", args, "1\n");
    }
    
    // Test 3: Ejemplo fallido del subject
    {
        char *args[] = {"./hidenp", "abc", "btarc", NULL};
        run_test("Subject example 3", args, "0\n");
    }
    
    // Test 4: String vacío
    {
        char *args[] = {"./hidenp", "", "cualquier cosa", NULL};
        run_test("Empty string", args, "1\n");
    }
    
    // Test 5: Sin argumentos
    {
        char *args[] = {"./hidenp", NULL};
        run_test("No arguments", args, "\n");
    }
    
    // Test 6: Strings iguales
    {
        char *args[] = {"./hidenp", "abc", "abc", NULL};
        run_test("Equal strings", args, "1\n");
    }
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}