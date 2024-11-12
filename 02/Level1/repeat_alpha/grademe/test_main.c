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
        ssize_t bytes = read(pipefd[0], output, output_size - 1);
        if (bytes >= 0)
            output[bytes] = '\0';
        close(pipefd[0]);
        wait(NULL);
    }
}

// Función para generar la salida esperada
void generate_expected(const char *input, char *expected) {
    const char *ptr = input;
    char *out = expected;
    
    while (*ptr) {
        if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z')) {
            int repeat = (*ptr >= 'a') ? (*ptr - 'a' + 1) : (*ptr - 'A' + 1);
            for (int i = 0; i < repeat; i++)
                *out++ = *ptr;
        } else {
            *out++ = *ptr;
        }
        ptr++;
    }
    *out = '\0';
}

// Función para ejecutar un test individual
void run_test(const char *test_name, char **args, const char *input) {
    printf("Test: %s\n", test_name);
    
    char output[4096] = {0};
    char expected[4096] = {0};
    
    generate_expected(input, expected);
    capture_program_output(args, output, sizeof(output));
    
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
        g_tests_failed++; // Incrementamos el contador de fallos
    }
    printf("\n");
}

int main(void) {
    char *program_name = "./repeat_alpha";
    g_tests_failed = 0; // Inicializamos el contador
    
    // Test 1: Letra minúscula simple
    {
        char *args[] = {program_name, "a", NULL};
        run_test("Letra minúscula simple", args, "a");
    }
    
    // Test 2: Letra mayúscula simple
    {
        char *args[] = {program_name, "B", NULL};
        run_test("Letra mayúscula simple", args, "B");
    }
    
    // Test 3: Palabra con minúsculas
    {
        char *args[] = {program_name, "abc", NULL};
        run_test("Palabra con minúsculas", args, "abc");
    }
    
    // Test 4: Caracteres especiales
    {
        char *args[] = {program_name, "a!b", NULL};
        run_test("Caracteres especiales", args, "a!b");
    }
    
    // Test 5: String vacío
    {
        char *args[] = {program_name, "", NULL};
        run_test("String vacío", args, "");
    }
    
    return g_tests_failed; // Retornamos el número de fallos
}