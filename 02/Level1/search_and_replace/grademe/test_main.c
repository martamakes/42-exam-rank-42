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
    char **args;          // Array de argumentos
    int arg_count;        // Número de argumentos
    char *expected;       // Output esperado
    char *description;    // Descripción del test
} t_test_case;

// Función para capturar la salida del programa con argumentos variables
char *capture_program_output(char **args, int arg_count) {
    int pipefd[2];
    char *output = NULL;
    size_t output_size = 1024;  // Tamaño inicial razonable
    
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
        
        exec_args[0] = "./search_and_replace";
        for (int i = 0; i < arg_count; i++)
            exec_args[i + 1] = args[i];
        exec_args[arg_count + 1] = NULL;
        
        execv("./search_and_replace", exec_args);
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
    printf("\n=== Testing search_and_replace ===\n");

    // Arrays de argumentos para los tests
    char *args1[] = {"Papache est un sabre", "a", "o"};
    char *args2[] = {"zaz", "art", "zul"};
    char *args3[] = {"zaz", "r", "u"};
    char *args4[] = {"jacob", "a", "b", "c", "e"};  // Test con argumentos extra
    char *args5[] = {"ZoZ eT Dovid oiME le METol.", "o", "a"};
    char *args6[] = {"wNcOre Un ExEmPle Pas Facilw a Ecrirw ", "w", "e"};
    char *args7[] = {"aaa", "a", "b"};
    char *args8[] = {"test", "tt", "a"};
    char *args9[] = {"test", "t", "aa"};
    char *args10[] = {"Hello World!", " ", "_"};
    char *args11[] = {"12321", "2", "5"};
    char *args12[] = {"^^^", "^", "v"};

    t_test_case tests[] = {
        // Tests del subject
        {args1, 3, "Popoche est un sobre\n", "Test básico del subject"},
        {args2, 3, "\n", "Test con argumentos de múltiples caracteres"},
        {args3, 3, "zaz\n", "Test sin coincidencias"},
        {args4, 5, "\n", "Test con más argumentos de los necesarios"},
        {args5, 3, "ZaZ eT David aiME le METal.\n", "Test con mayúsculas y minúsculas"},
        {args6, 3, "eNcOre Un ExEmPle Pas Facile a Ecrire \n", 
            "Test con reemplazo al inicio y final"},
        
        // Tests adicionales
        {NULL, 0, "\n", "Test sin argumentos"},
        {args7, 3, "bbb\n", "Test con múltiples reemplazos seguidos"},
        {args8, 3, "\n", "Test con segundo argumento de múltiples caracteres"},
        {args9, 3, "\n", "Test con tercer argumento de múltiples caracteres"},
        {args10, 3, "Hello_World!\n", "Test reemplazando espacios"},
        {args11, 3, "15351\n", "Test con números"},
        {args12, 3, "vvv\n", "Test con caracteres especiales"}
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