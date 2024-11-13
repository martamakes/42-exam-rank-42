/* ************************************************************************** */
/*                                             test_main.c                     */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

// Variable global para contar errores
int g_tests_failed = 0;

// Prototipo de la función a testear
int ft_atoi(const char *str);

// Función auxiliar para ejecutar tests
void run_test(char *test_name, const char *input, int expected)
{
    int result = ft_atoi(input);
    printf("Test %s: ", test_name);
    
    if (result == expected)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input: \"%s\"\n", input);
        printf("  Expected: %d\n", expected);
        printf("  Got: %d\n", result);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing ft_atoi ===\n\n");

    // Test casos básicos
    run_test("Número positivo simple", "42", 42);
    run_test("Número negativo simple", "-42", -42);
    run_test("Cero", "0", 0);

    // Test espacios en blanco
    run_test("Espacios antes", "   42", 42);
    run_test("Tabs y espacios", "\t\n\v\f\r 42", 42);

    // Test signos
    run_test("Signo positivo", "+42", 42);
    run_test("Múltiples signos", "+-42", 0);
    run_test("Signo después de número", "42-", 42);

    // Test casos especiales
    run_test("Solo espacios", "   ", 0);
    run_test("String vacío", "", 0);
    run_test("Letras después de número", "42abc", 42);

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);

    return g_tests_failed;
}