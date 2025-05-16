// test_main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int g_tests_failed = 0;

// Función auxiliar para capturar la salida del programa
// Modificación de la función capture_program_output
void capture_program_output(char *args[], char *output, int max_len) {
    int pipefd[2];
    pipe(pipefd);
    
    int pid = fork();
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execv("./tab_mult", args);
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        
        // Leer en bucle hasta que no haya más datos
        int total = 0;
        int n;
        while ((n = read(pipefd[0], output + total, max_len - total - 1)) > 0) {
            total += n;
            if (total >= max_len - 1)
                break;
        }
        output[total] = '\0';
        close(pipefd[0]);
        
        // Esperar a que termine el proceso hijo
        waitpid(pid, NULL, 0);
    }
}

// Función para generar la salida esperada
void generate_expected_output(int num, char *expected) {
    char temp[1024] = {0};
    expected[0] = '\0';
    
    for (int i = 1; i <= 9; i++) {
        sprintf(temp, "%d x %d = %d\n", i, num, i * num);
        strcat(expected, temp);
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
        printf("  Expected:\n%s\n", expected);
        printf("  Got:\n%s\n", output);
    }
}

int main(void) {
    printf("\n=== Testing tab_mult ===\n\n");

    char expected[1024];

    // Test sin argumentos
    {
        char *args[] = {"./tab_mult", NULL};
        run_test("No arguments", args, "\n");
    }

    // Test con número 9 (ejemplo del subject)
    {
        char *args[] = {"./tab_mult", "9", NULL};
        generate_expected_output(9, expected);
        run_test("Number 9", args, expected);
    }

    // Test con número 19 (ejemplo del subject)
    {
        char *args[] = {"./tab_mult", "19", NULL};
        generate_expected_output(19, expected);
        run_test("Number 19", args, expected);
    }

    // Test con número pequeño
    {
        char *args[] = {"./tab_mult", "3", NULL};
        generate_expected_output(3, expected);
        run_test("Small number", args, expected);
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