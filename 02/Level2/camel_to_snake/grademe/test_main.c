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
            char *args[] = {"./camel_to_snake", input, NULL};
            execv("./camel_to_snake", args);
        } else {
            char *args[] = {"./camel_to_snake", NULL};
            execv("./camel_to_snake", args);
        }
        perror("execv");
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        
        size_t total_read = 0;
        ssize_t bytes_read;
        char buffer[1024];
        
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
        
        if (total_read < output_size)
            output[total_read] = '\0';
        
        close(pipefd[0]);
        
        int status;
        waitpid(pid, &status, 0);
    }
    
    return output;
}

// Función para ejecutar un test
void run_test(t_test_case test) {
    printf("\n");
    
    char *output = capture_program_output(test.input);
    
    if (strcmp(output, test.expected) == 0) {
        printf("\033[0;32m[OK] \033[0m%s\n", test.description);
    } else {
        printf("\033[0;31m[KO] \033[0m%s\n", test.description);
        printf("  Input    : [%s]\n", test.input ? test.input : "NULL");
        printf("  Expected : [%s]\n", test.expected);
        printf("  Got      : [%s]\n", output);
        g_tests_failed++;
    }
    
    free(output);
}


int main(void) 
{
    printf("\n=== Testing camel_to_snake ===\n");

    t_test_case tests[] = {
        // Tests del subject
        {"hereIsACamelCaseWord", "here_is_a_camel_case_word\n",
            "Test del subject 1"},
        {"helloWorld", "hello_world\n",
            "Test del subject 2"},
        {NULL, "\n", "Test sin argumentos"},
        
        // Tests adicionales
        {"", "\n", "Test string vacía"},
        
        // Tests de casos simples
        {"hello", "hello\n",
            "Test palabra simple sin cambios"},
        {"Hello", "hello\n",
            "Test palabra con inicial mayúscula"},
        
        // Tests de casos complejos
        {"thisIsAVeryLongCamelCaseString",
         "this_is_a_very_long_camel_case_string\n",
         "Test string largo"},
        
        // Tests con siglas
        {"parseJSON", "parse_j_s_o_n\n",
            "Test con siglas"},
        {"handleXMLFile", "handle_x_m_l_file\n",
            "Test con siglas en medio"},
        
        // Tests con números
        {"user123Name", "user123_name\n",
            "Test con números"},
        
        // Tests de casos borde
        {"a", "a\n", "Test una letra"},
        {"A", "a\n", "Test una mayúscula"},
    };

    size_t num_tests = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < num_tests; i++) {
        run_test(tests[i]);
    }

    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}