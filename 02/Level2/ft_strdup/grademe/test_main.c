#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_tests_failed = 0;

char *ft_strdup(char *src);

void run_test(char *test_name, char *input)
{
    printf("Test %s: ", test_name);
    
    // Crear copia con ft_strdup
    char *result = ft_strdup(input);
    
    // Verificar que la copia no es NULL
    if (!result)
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input    : \"%s\"\n", input);
        printf("  Expected : Valid string\n");
        printf("  Got      : NULL\n");
        g_tests_failed++;
        return;
    }
    
    // Verificar que el contenido es igual
    if (strcmp(result, input) == 0)
    {
        // Verificar que es una copia real y no el mismo puntero
        if (result != input)
            printf("\033[0;32m[OK]\033[0m\n");
        else
        {
            printf("\033[0;31m[KO]\033[0m\n");
            printf("  Same pointer returned! Must be a new allocation\n");
            g_tests_failed++;
        }
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input    : \"%s\"\n", input);
        printf("  Expected : \"%s\"\n", input);
        printf("  Got      : \"%s\"\n", result);
        g_tests_failed++;
    }
    
    // Liberar memoria
    free(result);
}

int main(void)
{
    printf("\n=== Testing ft_strdup ===\n\n");
    
    // Test string vacío
    run_test("Empty string", "");
    
    // Test string corto
    run_test("Short string", "Hello");
    
    // Test string largo
    run_test("Long string", "This is a much longer string that will require more memory to duplicate properly");
    
    // Test con caracteres especiales
    run_test("Special chars", "Hello\tWorld\nNew\rLine!");
    
    // Test con espacios
    run_test("Spaces", "   Spaces   in   between   ");
    
    // Test con números
    run_test("Numbers", "12345 67890");
    
    // Test con símbolos
    run_test("Symbols", "!@#$%^&*()_+-=[]{}|;:,.<>?");
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}