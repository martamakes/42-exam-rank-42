// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int g_tests_failed = 0;

typedef struct s_test_case {
    char *input;
    char *expected;
    char *description;
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
            char *args[] = {"./last_word", input, NULL};
            execv("./last_word", args);
        } else {
            char *args[] = {"./last_word", NULL};
            execv("./last_word", args);
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
    printf("\n=== Testing last_word ===\n");

    t_test_case tests[] = {
        // Tests del subject
        {"FOR PONY", "PONY\n", 
            "Test básico del subject"},
        {"this        ...       is sparta, then again, maybe    not", "not\n",
            "Test con múltiples espacios y puntuación"},
        {"   ", "\n",
            "Test solo espacios"},
        {"  lorem,ipsum  ", "lorem,ipsum\n",
            "Test con espacios al inicio y final"},
            
        // Tests adicionales
        {"OneWord", "OneWord\n",
            "Test una sola palabra"},
        {"\t\t\tword\t\t", "word\n",
            "Test con tabulaciones"},
        {"First\tSecond  Third", "Third\n",
            "Test mezcla espacios y tabs"},
        {"Hello....World", "Hello....World\n",
            "Test palabra con puntuación"},
        {"   spaces   between   words   ", "words\n",
            "Test múltiples espacios"},
        {"     LastWord", "LastWord\n",
            "Test espacios solo al inicio"},
        {"FirstWord     ", "FirstWord\n",
            "Test espacios solo al final"},
        {"", "\n",
            "Test string vacía"},
        {"a", "a\n",
            "Test un solo carácter"},
        {" a ", "a\n",
            "Test un carácter con espacios"},
        {"   ...   ", "...\n",
            "Test solo puntuación con espacios"},
        {NULL, "\n",
            "Test input NULL"}
    };

    size_t num_tests = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < num_tests; i++)
        run_test(tests[i]);

    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);

    return g_tests_failed;
}