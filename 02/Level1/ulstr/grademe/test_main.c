// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

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
            char *args[] = {"./ulstr", input, NULL};
            execv("./ulstr", args);
        } else {
            char *args[] = {"./ulstr", NULL};
            execv("./ulstr", args);
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

int main(void) {
    printf("\n=== Testing ulstr ===\n");

    t_test_case tests[] = {
        // Tests del subject
        {"L'eSPrit nE peUt plUs pRogResSer s'Il staGne et sI peRsIsTent VAnIte et auto-justification.", 
         "l'EspRIT Ne PEuT PLuS PrOGrESsER S'iL STAgNE ET Si PErSiStENT vaNiTE ET AUTO-JUSTIFICATION.\n",
         "Test largo del subject 1"},
        
        {"S'enTOuRer dE sECreT eSt uN sIGnE De mAnQuE De coNNaiSSanCe.  ",
         "s'ENtoUrER De SecREt EsT Un SigNe dE MaNqUe dE COnnAIssANcE.  \n",
         "Test largo del subject 2"},
        
        {"3:21 Ba  tOut  moUn ki Ka di KE m'en Ka fe fot",
         "3:21 bA  ToUT  MOuN KI kA DI ke M'EN kA FE FOT\n",
         "Test con números y espacios múltiples"},
        
        {NULL, "\n", "Test sin argumentos"},
        
        // Tests adicionales
        {"", "\n", "Test string vacía"},
        
        {"aA", "Aa\n", "Test simple mayúsculas/minúsculas"},
        
        {"Hello World!", "hELLO wORLD!\n", "Test con palabra capitalizada"},
        
        {"42madrid", "42MADRID\n", "Test con números al inicio"},
        
        {"!@#$%^", "!@#$%^\n", "Test solo caracteres especiales"},
        
        {"AbC123!@#dEf", "aBc123!@#DeF\n", "Test mezclado"},
        
        {"   SpAcEs   ", "   sPaCeS   \n", "Test con espacios"},
        
        {"Z", "z\n", "Test una letra mayúscula"},
        
        {"a", "A\n", "Test una letra minúscula"},
        
        {"zZ aA bB yY", "Zz Aa Bb Yy\n", "Test pares de letras"},
        
        {"A1a2A3a4A5a", "a1A2a3A4a5A\n", "Test alternando con números"}
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