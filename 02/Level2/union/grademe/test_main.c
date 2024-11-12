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
        execv("./union", args);
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
    printf("\n=== Testing union ===\n\n");

    // Test sin argumentos
    {
        char *args[] = {"./union", NULL};
        run_test("No arguments", args, "\n");
    }

    // Test con un solo argumento
    {
        char *args[] = {"./union", "rien", NULL};
        run_test("One argument", args, "\n");
    }

    // Test del ejemplo 1 del subject
    {
        char *args[] = {"./union", "zpadinton", "paqefwtdjetyiytjneytjoeyjnejeyj", NULL};
        run_test("Example 1", args, "zpadintoqefwjy\n");
    }

    // Test del ejemplo 2 del subject
    {
        char *args[] = {"./union", "ddf6vewg64f", "gtwthgdwthdwfteewhrtag6h4ffdhsd", NULL};
        run_test("Example 2", args, "df6vewg4thras\n");
    }

    // Test del ejemplo 3 del subject
    {
        char *args[] = {"./union", "rien", "cette phrase ne cache rien", NULL};
        run_test("Example 3", args, "rienct phas\n");
    }

    // Test con strings vacíos
    {
        char *args[] = {"./union", "", "", NULL};
        run_test("Empty strings", args, "\n");
    }

    // Test con caracteres especiales
    {
        char *args[] = {"./union", "!@#$%", "^&*()", NULL};
        run_test("Special characters", args, "!@#$%^&*()\n");
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