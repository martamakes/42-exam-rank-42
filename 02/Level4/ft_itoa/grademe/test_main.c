// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Contador global de errores
int g_tests_failed = 0;

// Prototipo de la función a testear
char *ft_itoa(int nbr);

// Función auxiliar para ejecutar un test
void run_test(int input, const char *expected, const char *test_name)
{
    char *result = ft_itoa(input);
    
    printf("Test %s: ", test_name);
    
    if (result == NULL)
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input: %d\n", input);
        printf("  Expected: %s\n", expected);
        printf("  Got: NULL (malloc error)\n");
        g_tests_failed++;
        return;
    }
    
    if (strcmp(result, expected) == 0)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input: %d\n", input);
        printf("  Expected: %s\n", expected);
        printf("  Got: %s\n", result);
        g_tests_failed++;
    }
    
    free(result);
}

int main(void)
{
    printf("\n=== Testing ft_itoa ===\n\n");

    // Tests básicos
    run_test(0, "0", "zero");
    run_test(1, "1", "single digit positive");
    run_test(-1, "-1", "single digit negative");
    run_test(42, "42", "two digits positive");
    run_test(-42, "-42", "two digits negative");
    
    // Tests con números más grandes
    run_test(1234567, "1234567", "big number");
    run_test(-1234567, "-1234567", "big negative number");
    
    // Tests de casos límite
    run_test(INT_MAX, "2147483647", "INT_MAX");
    run_test(INT_MIN, "-2147483648", "INT_MIN");
    run_test(1000000000, "1000000000", "billion");
    run_test(-1000000000, "-1000000000", "negative billion");
    
    // Tests adicionales
    run_test(2147483647, "2147483647", "max int");
    run_test(-2147483648, "-2147483648", "min int");
    run_test(10, "10", "simple two digits");
    run_test(-10, "-10", "simple negative two digits");

    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
    {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    }
    else
    {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}