/*
** Test para repeat_alpha
** Compilar con: gcc -Wall -Wextra -Werror test_main.c -o test_prog
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Variable global para rastrear fallos
int g_tests_failed = 0;

// Función para capturar la salida del programa
void capture_program_output(const char *program_path, char **args, char *output, size_t output_size) {
    int pipefd[2];
    pipe(pipefd);
    
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        execv(program_path, args);
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        ssize_t bytes = read(pipefd[0], output, output_size - 1);
        if (bytes >= 0)
            output[bytes] = '\0';
        close(pipefd[0]);
        wait(NULL);
    }
}

// Función para ejecutar un test individual
void run_test(const char *test_name, const char *input) {
    printf("Test: %s\n", test_name);
    
    char output[4096] = {0};
    char expected[4096] = {0};
    
    // Preparamos los argumentos para el programa del estudiante
    char *student_args[] = {"./repeat_alpha", (char *)input, NULL};
    
    // Preparamos los argumentos para el programa de referencia
    char *reference_args[] = {"./reference_repeat_alpha", (char *)input, NULL};
    
    // Obtenemos la salida del programa de referencia
    capture_program_output("./reference_repeat_alpha", reference_args, expected, sizeof(expected));
    
    // Obtenemos la salida del programa del estudiante
    capture_program_output("./repeat_alpha", student_args, output, sizeof(output));
    
    if (strcmp(output, expected) == 0) {
        printf("\033[0;32m[OK]\033[0m Test passed\n");
    } else {
        printf("\033[0;31m[KO]\033[0m Test failed\n");
        printf("Expected output: ");
        const char *ptr = expected;
        while (*ptr) {
            if (*ptr == '\n')
                printf("$\n");
            else
                printf("%c", *ptr);
            ptr++;
        }
        printf("\nYour output: ");
        ptr = output;
        while (*ptr) {
            if (*ptr == '\n')
                printf("$\n");
            else
                printf("%c", *ptr);
            ptr++;
        }
        printf("\n");
        g_tests_failed++;
    }
    printf("\n");
}

int main(void) {
    g_tests_failed = 0;
    
    // Compilamos el programa de referencia
    system("gcc -Wall -Wextra -Werror reference_repeat_alpha.c -o reference_repeat_alpha");
    
    // Test 1: Letra minúscula simple
    run_test("Letra minúscula simple", "a");
    
    // Test 2: Letra mayúscula simple
    run_test("Letra mayúscula simple", "B");
    
    // Test 3: Palabra con minúsculas
    run_test("Palabra con minúsculas", "abc");
    
    // Test 4: Caracteres especiales
    run_test("Caracteres especiales", "a!b");
    
    // Test 5: String vacío
    run_test("String vacío", "");
    
    // Limpiamos el programa de referencia
    system("rm -f reference_repeat_alpha");
    
    return g_tests_failed;
}