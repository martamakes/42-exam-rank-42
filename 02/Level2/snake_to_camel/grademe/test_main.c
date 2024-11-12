// test_main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int g_tests_failed = 0;

void capture_program_output(char *args[], char *output, int max_len) {
    int pipefd[2];
    pipe(pipefd);
    
    int pid = fork();
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execv("./snake_to_camel", args);
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        
        int total = 0;
        int n;
        while ((n = read(pipefd[0], output + total, max_len - total - 1)) > 0) {
            total += n;
            if (total >= max_len - 1)
                break;
        }
        output[total] = '\0';
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
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
        printf("  Input:    '%s'\n", args[1] ? args[1] : "");
        printf("  Expected: '%s'\n", expected);
        printf("  Got:      '%s'\n", output);
    }
}

int main(void) {
    printf("\n=== Testing snake_to_camel ===\n\n");

    // Test sin argumentos
    {
        char *args[] = {"./snake_to_camel", NULL};
        run_test("No arguments", args, "\n");
    }

    // Test del ejemplo del subject
    {
        char *args[] = {"./snake_to_camel", "here_is_a_snake_case_word", NULL};
        run_test("Example 1", args, "hereIsASnakeCaseWord\n");
    }

    // Test del ejemplo 2 del subject
    {
        char *args[] = {"./snake_to_camel", "hello_world", NULL};
        run_test("Example 2", args, "helloWorld\n");
    }

    // Tests adicionales
    {
        char *args[] = {"./snake_to_camel", "single", NULL};
        run_test("Single word", args, "single\n");
    }

    {
        char *args[] = {"./snake_to_camel", "a_b_c", NULL};
        run_test("Short words", args, "aBC\n");
    }

    {
        char *args[] = {"./snake_to_camel", "already_camelCase", NULL};
        run_test("Mixed case", args, "alreadyCamelCase\n");
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