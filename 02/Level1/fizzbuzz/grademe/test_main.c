#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Función para ejecutar el programa y comparar la salida
void test_program(char **args, char *expected) {
    // Crear pipe para capturar la salida
    int pipefd[2];
    pipe(pipefd);
    
    int pid = fork();
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        execv("./fizzbuzz", args);
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        char buffer[1024] = {0};
        read(pipefd[0], buffer, sizeof(buffer));
        
        if (strcmp(buffer, expected) == 0)
            printf("Test passed ✓\n");
        else {
            printf("Test failed ✗\n");
            printf("Expected: %s\n", expected);
            printf("Got     : %s\n", buffer);
        }
    }
}

int main(void) {
    char *args1[] = {"./fizzbuzz", "arg1", "arg2", NULL};
    test_program(args1, "expected_output\n");
    return 0;
}
