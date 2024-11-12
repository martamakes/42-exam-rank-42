// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
            char *args[] = {"./rev_wstr", input, NULL};
            execv("./rev_wstr", args);
        } else {
            char *args[] = {"./rev_wstr", NULL};
            execv("./rev_wstr", args);
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
    printf("\n=== Testing rev_wstr ===\n");

    t_test_case tests[] = {
        // Tests del subject
        {"You hate people! But I love gatherings. Isn't it ironic?",
         "ironic? it Isn't gatherings. love I But people! hate You\n",
         "Test complejo del subject"},
        
        {"abcdefghijklm",
         "abcdefghijklm\n",
         "Test palabra única"},
        
        {"Wingardium Leviosa",
         "Leviosa Wingardium\n",
         "Test dos palabras"},
        
        {"", "\n",
         "Test string vacía"},

        // Tests adicionales
        {"Hello World",
         "World Hello\n",
         "Test básico dos palabras"},
        
        {"The Quick Brown Fox",
         "Fox Brown Quick The\n",
         "Test múltiples palabras"},
        
        {"a",
         "a\n",
         "Test una letra"},
        
        {"a b c",
         "c b a\n",
         "Test letras separadas"},
        
        {"Hello World! OpenAI GPT",
         "GPT OpenAI World! Hello\n",
         "Test con puntuación"},
        
        {"42 School",
         "School 42\n",
         "Test con números"},
        
        {"One Two Three Four Five",
         "Five Four Three Two One\n",
         "Test cinco palabras"},
        
        {"A B C D E F G",
         "G F E D C B A\n",
         "Test muchas palabras cortas"},
        
        {"Testing... Multiple... Dots...",
         "Dots... Multiple... Testing...\n",
         "Test con puntos suspensivos"},
        
        {"Programming in C",
         "C in Programming\n",
         "Test con preposición"},
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