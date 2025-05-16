// test_main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int g_tests_failed = 0;

void capture_program_output(char *args[], char *output, int max_len) {
    int pipefd[2];
    pipe(pipefd);
    
    int pid = fork();
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execv("./pgcd", args);
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
        printf("  Expected: '%s'\n", expected);
        printf("  Got:      '%s'\n", output);
    }
}

int main(void) {
    printf("\n=== Testing pgcd ===\n\n");

    // Test sin argumentos
    {
        char *args[] = {"./pgcd", NULL};
        run_test("No arguments", args, "\n");
    }

    // Test con un solo argumento
    {
        char *args[] = {"./pgcd", "42", NULL};
        run_test("One argument", args, "\n");
    }

    // Tests del subject
    {
        char *args[] = {"./pgcd", "42", "10", NULL};
        run_test("Example 1: 42 10", args, "2\n");
    }
    {
        char *args[] = {"./pgcd", "42", "12", NULL};
        run_test("Example 2: 42 12", args, "6\n");
    }
    {
        char *args[] = {"./pgcd", "14", "77", NULL};
        run_test("Example 3: 14 77", args, "7\n");
    }
    {
        char *args[] = {"./pgcd", "17", "3", NULL};
        run_test("Example 4: 17 3", args, "1\n");
    }

    // Tests adicionales
    {
        char *args[] = {"./pgcd", "100", "100", NULL};
        run_test("Same numbers", args, "100\n");
    }
    {
        char *args[] = {"./pgcd", "13", "7", NULL};
        run_test("Coprime numbers", args, "1\n");
    }
    {
        char *args[] = {"./pgcd", "48", "18", NULL};
        run_test("Complex case", args, "6\n");
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