#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* EXPLICACIÓN DEL TEST:
* - Usamos pipe() para capturar la salida del programa
* - fork() para crear un proceso hijo que ejecutará el programa
* - En el padre, comparamos la salida con el resultado esperado
*
* CASOS DE PRUEBA:
* 1. String simple con una palabra
* 2. String con múltiples palabras
* 3. String con espacios al inicio/fin
* 4. String vacía
* 5. Múltiples argumentos
* 6. String con tabs y espacios múltiples
*/

void test_program(char **args, char *expected)
{
    int pipefd[2];
    char buffer[1024] = {0};
    
    if (pipe(pipefd) == -1) {
        printf("Error al crear el pipe\n");
        return;
    }
    
    int pid = fork();
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execv("./first_word", args);
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);
        
        if (strcmp(buffer, expected) == 0)
            printf("Test passed ✓: %s\n", args[1] ? args[1] : "null");
        else {
            printf("Test failed ✗: %s\n", args[1] ? args[1] : "null");
            printf("Expected: '%s'\n", expected);
            printf("Got     : '%s'\n", buffer);
        }
    }
}

int main(void)
{
    // Test 1: Palabra simple
    char *args1[] = {"./first_word", "hello", NULL};
    test_program(args1, "hello\n");
    
    // Test 2: Múltiples palabras
    char *args2[] = {"./first_word", "FOR PONY", NULL};
    test_program(args2, "FOR\n");
    
    // Test 3: Espacios al inicio/fin
    char *args3[] = {"./first_word", "  lorem,ipsum  ", NULL};
    test_program(args3, "lorem,ipsum\n");
    
    // Test 4: Solo espacios
    char *args4[] = {"./first_word", "   ", NULL};
    test_program(args4, "\n");
    
    // Test 5: Múltiples argumentos
    char *args5[] = {"./first_word", "a", "b", NULL};
    test_program(args5, "\n");
    
    // Test 6: Tabs y espacios múltiples
    char *args6[] = {"./first_word", "this        ...    is sparta", NULL};
    test_program(args6, "this\n");
    
    return 0;
}