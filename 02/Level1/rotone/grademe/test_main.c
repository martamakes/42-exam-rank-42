// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Contador global de errores
int g_tests_failed = 0;

// Estructura para los casos de prueba
typedef struct s_test_case {
    char *input;          // Input para el programa
    char *expected;       // Output esperado
    char *description;    // Descripción del test
} t_test_case;

// Función para capturar la salida del programa
char *capture_program_output(char *input) {
    int pipefd[2];
    char *output = NULL;
    size_t output_size = 0;
    
    // Calcular tamaño inicial del buffer
    output_size = input ? strlen(input) + 2 : 2;
    output = malloc(output_size);
    if (!output) {
        perror("malloc");
        exit(1);
    }
    memset(output, 0, output_size);
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        free(output);
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        free(output);
        exit(1);
    }
    
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        if (input) {
            char *args[] = {"./rotone", input, NULL};
            execv("./rotone", args);
        } else {
            char *args[] = {"./rotone", NULL};
            execv("./rotone", args);
        }
        perror("execv");
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        
        size_t total_read = 0;
        ssize_t bytes_read;
        char buffer[1024];
        
        // Leer toda la salida, redimensionando el buffer si es necesario
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            if (total_read + bytes_read >= output_size) {
                output_size = total_read + bytes_read + 1;
                char *new_output = realloc(output, output_size);
                if (!new_output) {
                    perror("realloc");
                    free(output);
                    exit(1);
                }
                output = new_output;
            }
            memcpy(output + total_read, buffer, bytes_read);
            total_read += bytes_read;
        }
        
        // Asegurar terminación con \0
        if (total_read < output_size)
            output[total_read] = '\0';
        
        close(pipefd[0]);
        
        // Esperar a que termine el hijo
        int status;
        waitpid(pid, &status, 0);
    }
    
    return output;
}

// Función para ejecutar un test
void run_test(t_test_case test) {
    printf("\n");
    
    char *output = capture_program_output(test.input);
    
    // Comparar resultado
    if (strcmp(output, test.expected) == 0) {
        printf("\033[0;32m[OK] \033[0m%s\n", test.description);
    } else {
        printf("\033[0;31m[KO] \033[0m%s\n", test.description);
        printf("  Input: %s\n", test.input ? test.input : "sin argumentos");
        printf("  Expected: [%s]\n", test.expected);
        printf("  Got     : [%s]\n", output);
        printf("  Expected length: %zu\n", strlen(test.expected));
        printf("  Got length     : %zu\n", strlen(output));
        g_tests_failed++;
    }
    
    free(output);
}

int main(void) {
    printf("\n=== Testing rotone ===\n");

    // Array de casos de prueba
    t_test_case tests[] = {
        // Tests del subject
        {"abc", "bcd\n", "Test básico minúsculas"},
        {"Les stagiaires du staff ne sentent pas toujours tres bon.", "Mft tubhjbjsft ev tubgg of tfoufou qbt upvkpvst usft cpo.\n", 
            "Test frase larga con espacios y puntos"},
        {"AkjhZ zLKIJz , 23y ", "BlkiA aMLJKa , 23z \n", 
            "Test con mayúsculas, minúsculas y caracteres especiales"},
        {NULL, "\n", "Test sin argumentos"},
        {"", "\n", "Test string vacía"},
        
        // Tests adicionales para casos específicos
        {"z", "a\n", "Test wrap-around minúscula 'z' a 'a'"},
        {"Z", "A\n", "Test wrap-around mayúscula 'Z' a 'A'"},
        {"abcdefghijklmnopqrstuvwxyz", "bcdefghijklmnopqrstuvwxyza\n", 
            "Test alfabeto completo minúsculas"},
        {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", "BCDEFGHIJKLMNOPQRSTUVWXYZA\n", 
            "Test alfabeto completo mayúsculas"},
        {"123!@#$%", "123!@#$%\n", "Test solo caracteres especiales y números"},
        {"zZ aA", "aA bB\n", "Test casos borde mixtos"},
        {"The Quick Brown Fox!", "Uif Rvjdl Cspxo Gpy!\n", 
            "Test frase con mayúsculas inicial"},
        {"42Madrid", "42Nbesje\n", "Test con números y letras"}
    };

    // Ejecutar todos los tests
    size_t num_tests = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < num_tests; i++) {
        run_test(tests[i]);
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