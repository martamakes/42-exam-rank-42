/* test_main.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Contador global de errores
int g_tests_failed = 0;

// Tamaño del buffer para la salida - aumentado para manejar casos grandes
#define BUFFER_SIZE 8192

// Función mejorada para capturar la salida del programa
void capture_program_output(const char **args, char *output, int max_size)
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
        // Proceso hijo
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(1);
        }
        close(pipefd[1]);
        
        execv("./rstr_capitalizer", (char *const *)args);
        perror("execv");
        exit(1);
    }
    
    // Proceso padre
    close(pipefd[1]);
    
    // Leer la salida en chunks para evitar desbordamiento
    int total_read = 0;
    int bytes_read;
    while ((bytes_read = read(pipefd[0], output + total_read, 
           max_size - total_read - 1)) > 0) {
        total_read += bytes_read;
        if (total_read >= max_size - 1)
            break;
    }
    
    output[total_read] = '\0';
    close(pipefd[0]);
    
    // Esperar a que el hijo termine y verificar su estado
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        exit(1);
    }
    
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        printf("Error: El programa terminó con estado %d\n", WEXITSTATUS(status));
    }
}

void run_test(char *test_name, const char **args, const char *expected)
{
    char output[BUFFER_SIZE] = {0};
    
    printf("Test %s: ", test_name);
    capture_program_output(args, output, BUFFER_SIZE);
    
    if (strcmp(output, expected) == 0) {
        printf("\033[0;32m[OK]\033[0m\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n");
        g_tests_failed++;
        printf("Expected: \"%s\"\n", expected);
        printf("Got:      \"%s\"\n", output);
    }
}

int main(void)
{
    printf("\n=== Testing rstr_capitalizer ===\n\n");

    // Test 1: Sin argumentos
    {
        const char *args[] = {"./rstr_capitalizer", NULL};
        run_test("No arguments", args, "\n");
    }

    // Test 2: Caso básico
    {
        const char *args[] = {"./rstr_capitalizer", "hello world", NULL};
        run_test("Basic test", args, "hellO worlD\n");
    }

    // Test 3: Caso con múltiples espacios y puntuación
    {
        const char *args[] = {"./rstr_capitalizer", 
                            "hello,   worlD...  IT", NULL};
        run_test("Complex test", args, "hello,   world...  iT\n");
    }

     // Test 4: Caso con un único char
    {
        const char *args[] = {"./rstr_capitalizer", 
                            "a", NULL};
        run_test("Single Letter", args, "A\n");
    }

    // Test 4: Ejemplo 1 del subject
    {
        const char *args[] = {"./rstr_capitalizer", "a FiRSt LiTTlE TESt", NULL};
        run_test("Subject example 1", args, "A firsT littlE tesT\n");
    }

    // Test 5: Ejemplo 2 del subject (múltiples argumentos)
    {
        const char *args[] = {"./rstr_capitalizer", 
                            "SecONd teST A LITtle BiT   Moar comPLEX",
                            "   But... This iS not THAT COMPLEX",
                            "     Okay, this is the last 1239809147801 but not    the least    t", 
                            NULL};
        run_test("Subject example 2", args, 
                "seconD tesT A littlE biT   moaR compleX\n"
                "   but... thiS iS noT thaT compleX\n"
                "     okay, thiS iS thE lasT 1239809147801 buT noT    thE leasT    T\n");
    }

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}
