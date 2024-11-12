#include <stdio.h>
#include <unistd.h>

// Contador global de errores
int g_tests_failed = 0;

// Prototipo de la función a testear
void ft_swap(int *a, int *b);

/* Función auxiliar para imprimir el resultado de cada test */
void print_test_result(char *test_name, int success)
{
    if (success)
        printf("\033[0;32m[OK] %s\033[0m\n", test_name);
    else
    {
        printf("\033[0;31m[KO] %s\033[0m\n", test_name);
        g_tests_failed++; // Incrementar contador cuando falla el test
    }
}

/* Test 1: Intercambio de números positivos */
void test_positive_numbers(void)
{
    int a = 42;
    int b = 24;
    int original_a = a;
    int original_b = b;
    
    ft_swap(&a, &b);
    
    int success = (a == original_b && b == original_a);
    print_test_result("Test con números positivos", success);
    printf("    Input:  a = %d, b = %d\n", original_a, original_b);
    printf("    Output: a = %d, b = %d\n", a, b);
    if (!success)
        printf("    Expected: a = %d, b = %d\n", original_b, original_a);
}

/* Test 2: Intercambio con un número negativo */
void test_negative_numbers(void)
{
    int a = -42;
    int b = 24;
    int original_a = a;
    int original_b = b;
    
    ft_swap(&a, &b);
    
    int success = (a == original_b && b == original_a);
    print_test_result("Test con un número negativo", success);
    printf("    Input:  a = %d, b = %d\n", original_a, original_b);
    printf("    Output: a = %d, b = %d\n", a, b);
    if (!success)
        printf("    Expected: a = %d, b = %d\n", original_b, original_a);
}

/* Test 3: Intercambio con el mismo número */
void test_same_number(void)
{
    int a = 42;
    int b = 42;
    int original_a = a;
    int original_b = b;
    
    ft_swap(&a, &b);
    
    int success = (a == original_b && b == original_a);
    print_test_result("Test con números iguales", success);
    printf("    Input:  a = %d, b = %d\n", original_a, original_b);
    printf("    Output: a = %d, b = %d\n", a, b);
    if (!success)
        printf("    Expected: a = %d, b = %d\n", original_b, original_a);
}

int main(void)
{
    // Inicializar contador de errores
    g_tests_failed = 0;

    printf("\033[1;34m=== Tests for ft_swap ===\033[0m\n\n");
    
    test_positive_numbers();
    test_negative_numbers();
    test_same_number();
    
    // Resumen final
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }
    
    return g_tests_failed; // Retornar número de tests fallidos
}