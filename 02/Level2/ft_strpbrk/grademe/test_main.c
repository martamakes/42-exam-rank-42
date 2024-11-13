// test_main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int g_tests_failed = 0;

// Prototipo de la función a testear
char    *ft_strpbrk(const char *s1, const char *s2);

void run_test(const char *test_name, const char *s1, const char *s2)
{
    char *result = ft_strpbrk(s1, s2);
    char *expected = strpbrk(s1, s2);
    
    printf("Test %s: ", test_name);
    
    // Si ambos son NULL o apuntan al mismo carácter
    if ((result == NULL && expected == NULL) || 
        (result != NULL && expected != NULL && *result == *expected))
    {
        printf("\033[0;32m[OK]\033[0m\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("String 1: \"%s\"\n", s1);
        printf("String 2: \"%s\"\n", s2);
        printf("Expected: %s\n", expected ? expected : "NULL");
        printf("Got     : %s\n", result ? result : "NULL");
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing ft_strpbrk ===\n\n");

    // Test 1: Caso básico
    run_test("Basic match", "Hello World", "World");

    // Test 2: Sin coincidencias
    run_test("No match", "Hello", "xyz");

    // Test 3: String vacía en s2
    run_test("Empty s2", "Hello", "");

    // Test 4: String vacía en s1
    run_test("Empty s1", "", "Hello");

    // Test 5: Ambas strings vacías
    run_test("Both empty", "", "");

    // Test 6: Coincidencia al inicio
    run_test("Match at start", "Hello", "H");

    // Test 7: Coincidencia al final
    run_test("Match at end", "Hello", "o");

    // Test 8: Múltiples caracteres en s2
    run_test("Multiple chars in s2", "Hello World", "dW");

    // Test 9: Caracteres especiales
    run_test("Special chars", "Hello\tWorld", "\t ");

    // Test 10: Caso sensible a mayúsculas
    run_test("Case sensitive", "Hello", "hE");

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}