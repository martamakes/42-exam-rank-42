// test_main.c para str_capitalizer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Contador global de errores
int g_tests_failed = 0;

// Estructura para los casos de prueba
typedef struct s_test_case {
    char **args;          // Array de argumentos
    int arg_count;        // Número de argumentos
    char *expected;       // Output esperado
    char *description;    // Descripción del test
} t_test_case;

// Función para capturar la salida del programa
char *capture_program_output(char **args, int arg_count) {
    int pipefd[2];
    char *output = NULL;
    size_t output_size = 1024;
    
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
        
        // Preparar argumentos para execv
        char **exec_args = malloc((arg_count + 2) * sizeof(char *));
        if (!exec_args) {
            perror("malloc");
            exit(1);
        }
        
        exec_args[0] = "./str_capitalizer";
        for (int i = 0; i < arg_count; i++)
            exec_args[i + 1] = args[i];
        exec_args[arg_count + 1] = NULL;
        
        execv("./str_capitalizer", exec_args);
        perror("execv");
        free(exec_args);
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
    
    char *output = capture_program_output(test.args, test.arg_count);
    
    if (strcmp(output, test.expected) == 0) {
        printf("\033[0;32m[OK] \033[0m%s\n", test.description);
    } else {
        printf("\033[0;31m[KO] \033[0m%s\n", test.description);
        printf("  Arguments (%d): ", test.arg_count);
        for (int i = 0; i < test.arg_count; i++)
            printf("[%s] ", test.args[i]);
        printf("\n");
        printf("  Expected : [%s]\n", test.expected);
        printf("  Got      : [%s]\n", output);
        g_tests_failed++;
    }
    
    free(output);
}

int main(void) {
    printf("\n=== Testing str_capitalizer ===\n");

    // Arrays de argumentos para los tests
    char *args1[] = {"a FiRSt LiTTlE TESt"};
    char *args2[] = {"__SecONd teST A LITtle BiT   Moar comPLEX"};
    char *args3[] = {"   But... This iS not THAT COMPLEX"};
    char *args4[] = {"     Okay, this is the last 1239809147801 but not    the least    t"};
    char *args5[] = {"a"};
    char *args6[] = {"hello"};
    char *args7[] = {"HELLO"};
    char *args8[] = {"hElLo WoRlD"};
    char *args9[] = {"   spaces   multiple   spaces   "};
    char *args10[] = {"tabs\t\tmultiple\ttabs\t\t"};
    char *args11[] = {"!@#$% special ^&*() chars"};

    t_test_case tests[] = {
        // Tests del subject
        {args1, 1, "A First Little Test\n", "Test básico del subject"},
        {args2, 1, "__second Test A Little Bit   Moar Complex\n", 
            "Test con underscores y espacios múltiples"},
        {args3, 1, "   But... This Is Not That Complex\n", 
            "Test con puntuación y espacios iniciales"},
        {args4, 1, "     Okay, This Is The Last 1239809147801 But Not    The Least    T\n", 
            "Test con números y espacios múltiples"},
        
        // Tests adicionales
        {NULL, 0, "\n", "Test sin argumentos"},
        {args5, 1, "A\n", "Test con una sola letra"},
        {args6, 1, "Hello\n", "Test con palabra en minúsculas"},
        {args7, 1, "Hello\n", "Test con palabra en mayúsculas"},
        {args8, 1, "Hello World\n", "Test con mezcla de mayúsculas y minúsculas"},
        {args9, 1, "   Spaces   Multiple   Spaces   \n", 
            "Test con múltiples espacios"},
        {args10, 1, "Tabs\t\tMultiple\tTabs\t\t\n", 
            "Test con tabulaciones"},
        {args11, 1, "!@#$% Special ^&*() Chars\n", 
            "Test con caracteres especiales"}
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