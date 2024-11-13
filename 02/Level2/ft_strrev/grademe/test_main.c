#include <stdio.h>
#include <string.h>

// Variable global para contar errores
int g_tests_failed = 0;

// Prototipo de la función
char *ft_strrev(char *str);

// Función auxiliar para ejecutar tests
void run_test(char *test_name, char *input, const char *expected)
{
    char *result;
    
    printf("Test %s: ", test_name);
    
    // Hacemos una copia del input para no modificar el original
    char test_str[100];
    strcpy(test_str, input);
    
    result = ft_strrev(test_str);
    
    if (strcmp(result, expected) == 0 && result == test_str)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input: \"%s\"\n", input);
        printf("  Expected: \"%s\"\n", expected);
        printf("  Got: \"%s\"\n", result);
        if (result != test_str)
            printf("  Error: La función no devolvió el mismo puntero\n");
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing ft_strrev ===\n\n");

    // Test casos básicos
    run_test("Palabra simple", "Hello", "olleH");
    run_test("Palíndromo", "radar", "radar");
    run_test("Un carácter", "a", "a");
    run_test("String vacía", "", "");

    // Test casos más complejos
    run_test("Frase con espacios", "Hello World", "dlroW olleH");
    run_test("Números", "12345", "54321");
    run_test("Caracteres especiales", "!@#$%", "%$#@!");
    run_test("Alfanumérico", "abc123", "321cba");

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);

    return g_tests_failed;
}