#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_tests_failed = 0;

// Prototipo de la función
int ft_atoi_base(const char *str, int str_base);

void run_test(char *test_name, const char *input, int base, int expected)
{
    int result = ft_atoi_base(input, base);
    
    printf("Test %s: ", test_name);
    
    if (result == expected)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input: \"%s\" (base %d)\n", input, base);
        printf("  Expected: %d\n", expected);
        printf("  Got: %d\n", result);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing ft_atoi_base ===\n\n");

    // Test conversiones básicas mencionadas en el subject
    run_test("Base 4 test", "123", 4, 27);     // "0123" son los caracteres válidos
    run_test("Base 16 lowercase", "12fdb3", 16, 1244595);
    run_test("Base 16 uppercase", "12FDB3", 16, 1244595);

    // Test con signo negativo (mencionado en el subject)
    run_test("Negative number", "-12fdb3", 16, -1244595);

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);

    return g_tests_failed;
}