// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Variable global para fallos
int g_tests_failed = 0;

// Prototipo de la función
char *rev_print(char *str);

// Función para capturar output
void capture_output(char *input, char *output, int size) {
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
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        char *ret = rev_print(input);
        if (ret != input) {
            printf("ERROR: La función no devolvió el puntero original\n");
            exit(1);
        }
        exit(0);
    } else {
        // Proceso padre
        close(pipefd[1]);
        
        // Limpiar el buffer de salida
        memset(output, 0, size);
        
        // Leemos la salida en un bucle hasta que no haya más datos
        char buffer[1024];
        int total_read = 0;
        ssize_t n;
        
        while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            if (total_read + n >= size) {
                n = size - total_read - 1;  // Dejar espacio para el null terminator
                if (n <= 0) break;
            }
            memcpy(output + total_read, buffer, n);
            total_read += n;
        }
        
        output[total_read] = '\0';
        close(pipefd[0]);
        
        waitpid(pid, &status, 0);
    }
}

void print_output(const char *str) {
    printf("\"");
    while (*str) {
        if (*str == '\n')
            printf("\\n");
        else
            printf("%c", *str);
        str++;
    }
    printf("\"");
}

void run_test(const char *test_name, char *input, char *expected) {
    char student_output[4096] = {0};
    char expected_output[4096] = {0};
    
    // Obtener salida del código del estudiante
    capture_output(input, student_output, sizeof(student_output));
    
    // Construir salida esperada con \n
    snprintf(expected_output, sizeof(expected_output), "%s\n", expected);
    
    // Imprimir el encabezado del test
    printf("Test %-20s: ", test_name);
    fflush(stdout);
    
    // Comparar resultados
    if (strcmp(student_output, expected_output) == 0) {
        printf("\033[0;32m[OK]\033[0m\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("    Input    : \"%s\"\n", input);
        printf("    Expected : ");
        print_output(expected_output);
        printf("\n");
        printf("    Got      : ");
        print_output(student_output);
        printf("\n\n");
        g_tests_failed++;
    }
    fflush(stdout);
}

int main(void) {
    printf("\n=== Testing rev_print ===\n\n");
    printf("Assignment: Write a function that takes a string and displays\n");
    printf("           the string in reverse followed by a newline.\n");
    printf("           Must return the string passed as parameter.\n\n");

    // Tests exactamente como aparecen en el subject
    run_test("Subject Example 1", "Hello world", "dlrow olleH");
    run_test("Subject Example 2", "tnirp esreveR", "Reverse print");
    run_test("Subject Example 3", "", "");

    // Resumen
    printf("\n=== Test Summary ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mAll tests passed! ✓\033[0m\n");
    } else {
        printf("\033[0;31m%d tests failed ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}