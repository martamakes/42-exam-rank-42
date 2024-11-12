// test_main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int g_tests_failed = 0;

// Función auxiliar para capturar la salida del programa
void capture_program_output(char *args[], char *output, int max_len) {
    int pipefd[2];
    pipe(pipefd);
    
    int pid = fork();
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execv("./paramsum", args);
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        int n = read(pipefd[0], output, max_len - 1);
        output[n] = '\0';
        close(pipefd[0]);
    }
}

void run_test(char *test_name, char *args[], char *expected) {
    char output[1024] = {0};
    
    capture_program_output(args, output, sizeof(output));
    
    printf("Test %s: ", test_name);
    if (strcmp(output, expected) == 0) {
        printf("\033[0;32m[OK]\033[0m\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n");
        g_tests_failed++;
        printf("  Expected: '%s'\n", expected);
        printf("  Got:      '%s'\n", output);
    }
}

int main(void) {
    printf("\n=== Testing paramsum ===\n\n");

    // Test sin argumentos
    char *test1[] = {"./paramsum", NULL};
    run_test("No arguments", test1, "0\n");

    // Test con tres argumentos (ejemplo del subject)
    char *test2[] = {"./paramsum", "6", "12", "24", NULL};
    run_test("Three arguments", test2, "3\n");

    // Test con seis argumentos (ejemplo del subject)
    char *test3[] = {"./paramsum", "1", "2", "3", "5", "7", "24", NULL};
    run_test("Six arguments", test3, "6\n");

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}