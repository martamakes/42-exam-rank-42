#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Variable global para contar los tests fallidos
int g_tests_failed = 0;

// Prototipo de la función a testear
void ft_putstr(char *str);

// Función para capturar la salida de ft_putstr
void test_ft_putstr(char *input, char *expected, const char *test_name) {
    int pipefd[2];
    char buffer[4096] = {0};
    
    // Crear pipe para capturar la salida
    if (pipe(pipefd) == -1) {
        perror("pipe");
        g_tests_failed++;
        return;
    }
    
    // Guardar el stdout original
    int stdout_backup = dup(STDOUT_FILENO);
    if (stdout_backup == -1) {
        perror("dup");
        g_tests_failed++;
        return;
    }
    
    // Redirigir stdout al pipe
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        perror("dup2");
        g_tests_failed++;
        return;
    }
    
    // Llamar a la función
    ft_putstr(input);
    
    // Restaurar stdout
    fflush(stdout);
    if (dup2(stdout_backup, STDOUT_FILENO) == -1) {
        perror("dup2");
        g_tests_failed++;
        return;
    }
    close(pipefd[1]);
    close(stdout_backup);
    
    // Leer la salida capturada
    ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
        g_tests_failed++;
        close(pipefd[0]);
        return;
    }
    buffer[bytes_read] = '\0';
    close(pipefd[0]);
    
    // Comparar con el resultado esperado
    printf("Test %s: ", test_name);
    if (strcmp(buffer, expected) == 0) {
        printf("\033[0;32m[OK]\033[0m\n");
        printf("Input: \"%s\"\n", input);
    } else {
        g_tests_failed++;
        printf("\033[0;31m[KO]\033[0m\n");
        printf("Input   : \"%s\"\n", input);
        printf("Expected: \"%s\"\n", expected);
        printf("Got     : \"%s\"\n", buffer);
    }
    printf("\n");
}

int main(void) {
    // Inicializamos el contador de fallos
    g_tests_failed = 0;

    printf("=== Tests para ft_putstr ===\n\n");
    
    // Test 1: String normal
    test_ft_putstr("Hello World", "Hello World", "string normal");
    
    // Test 2: String vacía
    test_ft_putstr("", "", "string vacía");
    
    // Test 3: String con espacios
    test_ft_putstr("   Spaces   ", "   Spaces   ", "string con espacios");
    
    // Test 4: String con caracteres especiales
    test_ft_putstr("Hello\tWorld\n", "Hello\tWorld\n", "caracteres especiales");
    
    // Test 5: String con números y símbolos
    test_ft_putstr("123!@#$%", "123!@#$%", "números y símbolos");
    
    // Retornamos el número de tests fallidos
    return g_tests_failed;
}