// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Prototipo de la función a testear
void ft_putstr(char *str);

// Función para capturar la salida de ft_putstr
void test_ft_putstr(char *input, char *expected) {
    int pipefd[2];
    char buffer[4096] = {0};
    
    // Crear pipe para capturar la salida
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    
    // Guardar el stdout original
    int stdout_backup = dup(STDOUT_FILENO);
    
    // Redirigir stdout al pipe
    dup2(pipefd[1], STDOUT_FILENO);
    
    // Llamar a la función
    ft_putstr(input);
    
    // Restaurar stdout
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    close(pipefd[1]);
    
    // Leer la salida capturada
    read(pipefd[0], buffer, sizeof(buffer));
    close(pipefd[0]);
    
    // Comparar con el resultado esperado
    if (strcmp(buffer, expected) == 0) {
        printf("\033[0;32m[OK]\033[0m Test con input: \"%s\"\n", input);
    } else {
        printf("\033[0;31m[KO]\033[0m Test con input: \"%s\"\n", input);
        printf("Esperado: \"%s\"\n", expected);
        printf("Obtenido: \"%s\"\n", buffer);
    }
}

int main(void) {
    printf("=== Tests para ft_putstr ===\n\n");
    
    // Test 1: String normal
    test_ft_putstr("Hello World", "Hello World");
    
    // Test 2: String vacía
    test_ft_putstr("", "");
    
    // Test 3: String con espacios
    test_ft_putstr("   Spaces   ", "   Spaces   ");
    
    // Test 4: String con caracteres especiales
    test_ft_putstr("Hello\tWorld\n", "Hello\tWorld\n");
    
    // Test 5: String con números y símbolos
    test_ft_putstr("123!@#$%", "123!@#$%");
    
    return 0;
}