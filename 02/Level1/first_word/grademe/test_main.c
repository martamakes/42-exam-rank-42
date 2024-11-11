// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
** Función auxiliar para capturar la salida del programa
** Usa un pipe para redireccionar la salida estándar
*/
void capture_output(const char *input, char *output, size_t output_size) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Ejecutar el programa con el input
        char *args[] = {"./first_word", (char *)input, NULL};
        execv("./first_word", args);
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        int bytes_read = read(pipefd[0], output, output_size - 1);
        if (bytes_read > 0)
            output[bytes_read] = '\0';
        close(pipefd[0]);
        wait(NULL);
    }
}

/*
** Función de test que verifica diferentes casos
*/
void run_test(const char *input, const char *expected, const char *test_name) {
    char output[1024] = {0};
    
    printf("Test %s: ", test_name);
    capture_output(input, output, sizeof(output));
    
    if (strcmp(output, expected) == 0) {
        printf("\033[0;32mPASS ✓\033[0m\n");
        printf("Input: \"%s\"\n", input);
        printf("Expected: \"%s\"\n", expected);
        printf("Got: \"%s\"\n\n", output);
    } else {
        printf("\033[0;31mFAIL ✗\033[0m\n");
        printf("Input: \"%s\"\n", input);
        printf("Expected: \"%s\"\n", expected);
        printf("Got: \"%s\"\n\n", output);
    }
}

/*
** Programa principal que ejecuta todos los tests
*/
int main(void) {
    printf("Running tests for first_word...\n\n");

    // Test casos básicos
    run_test("FOR PONY", "FOR\n", "Basic");
    run_test("this        ...    is sparta", "this\n", "Multiple spaces");
    run_test("   ", "\n", "Only spaces");
    run_test("lorem,ipsum", "lorem,ipsum\n", "Word with comma");
    
    // Test casos especiales
    run_test("\t\t\tHello", "Hello\n", "Tabs");
    run_test("OneWord", "OneWord\n", "Single word");
    run_test("  Hello\tWorld  ", "Hello\n", "Mixed spaces and tabs");
    
    return 0;
}