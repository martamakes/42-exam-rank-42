#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/wait.h> // Añadido para wait

// Variable global para contar los tests fallidos
int g_tests_failed = 0;

// Función para generar la salida esperada
char *generate_expected_output(void)
{
    static char expected[4096] = {0};
    int len = 0;
    
    for (int i = 1; i <= 100; i++)
    {
        if (i % 3 == 0 && i % 5 == 0)
            len += snprintf(expected + len, sizeof(expected) - len, "fizzbuzz\n");
        else if (i % 3 == 0)
            len += snprintf(expected + len, sizeof(expected) - len, "fizz\n");
        else if (i % 5 == 0)
            len += snprintf(expected + len, sizeof(expected) - len, "buzz\n");
        else
            len += snprintf(expected + len, sizeof(expected) - len, "%d\n", i);
    }
    return expected;
}

// Función para imprimir una parte de la cadena hasta encontrar una diferencia
void print_difference_context(const char *expected, const char *got)
{
    int i = 0;
    while (expected[i] && got[i] && i < 50) // Mostramos hasta 50 caracteres de contexto
    {
        if (expected[i] != got[i])
        {
            printf("Diferencia encontrada en el carácter %d:\n", i);
            printf("Esperado (siguiente 10 chars): ");
            for (int j = 0; j < 10 && expected[i + j]; j++)
            {
                if (expected[i + j] == '\n')
                    printf("\\n");
                else
                    printf("%c", expected[i + j]);
            }
            printf("\nObtenido (siguiente 10 chars): ");
            for (int j = 0; j < 10 && got[i + j]; j++)
            {
                if (got[i + j] == '\n')
                    printf("\\n");
                else
                    printf("%c", got[i + j]);
            }
            printf("\n");
            return;
        }
        i++;
    }
    
    if (!expected[i] && got[i])
        printf("La salida es más larga de lo esperado\n");
    else if (expected[i] && !got[i])
        printf("La salida es más corta de lo esperado\n");
}

int main(void)
{
    // Inicializamos el contador de fallos
    g_tests_failed = 0;

    int pipefd[2];
    char buffer[4096] = {0};
    
    if (pipe(pipefd) == -1)
    {
        printf("Error al crear el pipe\n");
        g_tests_failed++;
        return g_tests_failed;
    }
    
    int pid = fork();
    if (pid == 0)
    {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        char *args[] = {"./fizzbuzz", NULL};
        execv("./fizzbuzz", args);
        exit(1);
    }
    else
    {
        // Proceso padre
        close(pipefd[1]);
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        close(pipefd[0]);
        wait(NULL);  // Esperamos al hijo
        
        if (bytes_read > 0)
            buffer[bytes_read] = '\0';
        
        char *expected = generate_expected_output();
        
        if (strcmp(buffer, expected) == 0)
        {
            printf("\033[0;32mTest passed ✓\033[0m\n");
            printf("El programa imprime correctamente todos los números del 1 al 100\n");
            printf("con fizz, buzz y fizzbuzz donde corresponde\n");
        }
        else
        {
            g_tests_failed++; // Incrementamos el contador de fallos
            printf("\033[0;31mTest failed ✗\033[0m\n");
            printf("La salida no coincide con el formato esperado\n");
            print_difference_context(expected, buffer);
        }
    }
    
    // Retornamos el número de tests fallidos
    return g_tests_failed;
}