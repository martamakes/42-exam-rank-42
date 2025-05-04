// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// Variable global para rastrear fallos
int g_tests_failed = 0;

// Función para capturar la salida del programa
void capture_program_output(char **args, char *output, size_t output_size) {
    int pipefd[2];
    pipe(pipefd);
    pid_t pid = fork();
    
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execv(args[0], args);
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        
        // Leer en un bucle para capturar toda la salida
        size_t total_bytes = 0;
        ssize_t bytes;
        
        while ((bytes = read(pipefd[0], output + total_bytes, 
                             output_size - total_bytes - 1)) > 0) {
            total_bytes += bytes;
            if (total_bytes >= output_size - 1)
                break;
        }
        
        output[total_bytes] = '\0';
        close(pipefd[0]);
        wait(NULL);
    }
}


// Función para ejecutar un test individual
void run_test(const char *test_name, char **args, const char *expected) {
    printf("Test: %s\n", test_name);
    
    char output[4096] = {0};
    capture_program_output(args, output, sizeof(output));
    
    if (strcmp(output, expected) == 0) {
        printf("\033[0;32m[OK]\033[0m Test passed\n");
    } else {
        printf("\033[0;31m[KO]\033[0m Test failed\n");
        printf("Input: \"%s\"\n", args[1]);
        printf("Expected output: \"");
        const char *ptr = expected;
        while (*ptr) {
            if (*ptr == '\n')
                printf("$\n");
            else
                printf("%c", *ptr);
            ptr++;
        }
        printf("\"\nYour output: \"");
        ptr = output;
        while (*ptr) {
            if (*ptr == '\n')
                printf("$\n");
            else
                printf("%c", *ptr);
            ptr++;
        }
        printf("\"\n");
        g_tests_failed++;
    }
    printf("\n");
}

int main(void) {
    printf("\n=== Testing first_word ===\n\n");
    
    char *program_name = "./first_word";
    g_tests_failed = 0; // Inicializamos el contador
    
    // Test casos básicos
    {
        char *args[] = {program_name, "FOR PONY", NULL};
        run_test("Caso básico", args, "FOR\n");
    }
    
    // Test múltiples espacios
    {
        char *args[] = {program_name, "   hello   world   ", NULL};
        run_test("Múltiples espacios", args, "hello\n");
    }
    
    // Test solo espacios
    {
        char *args[] = {program_name, "   ", NULL};
        run_test("Solo espacios", args, "\n");
    }
    
    // Test palabra con coma
    {
        char *args[] = {program_name, "lorem,ipsum", NULL};
        run_test("Palabra con coma", args, "lorem,ipsum\n");
    }
    
    // Test con tabs
    {
        char *args[] = {program_name, "\t\t\tHello", NULL};
        run_test("Tabs", args, "Hello\n");
    }
    
    // Resumen final
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
        printf("Revisa los casos que fallaron arriba\n");
    }
    printf("\n");
    
    return g_tests_failed;
}