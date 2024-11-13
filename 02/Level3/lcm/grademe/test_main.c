// test_main.c
#include <stdio.h>

int g_tests_failed = 0;

// Prototipo de la función
unsigned int lcm(unsigned int a, unsigned int b);

void run_test(char *test_name, unsigned int a, unsigned int b, unsigned int expected)
{
    unsigned int result = lcm(a, b);
    
    printf("Test %s: ", test_name);
    if (result == expected)
    {
        printf("\033[0;32m[OK]\033[0m\n");
        printf("  Input: a=%u, b=%u\n", a, b);
        printf("  Expected: %u\n", expected);
        printf("  Got: %u\n\n", result);
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input: a=%u, b=%u\n", a, b);
        printf("  Expected: %u\n", expected);
        printf("  Got: %u\n\n", result);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing lcm ===\n\n");

    // Test casos básicos
    run_test("Basic case 1", 12, 18, 36);
    run_test("Basic case 2", 4, 6, 12);
    
    // Test números primos entre sí
    run_test("Coprime numbers", 7, 13, 91);
    
    // Test números iguales
    run_test("Same numbers", 5, 5, 5);
    
    // Test con 1
    run_test("One and number", 1, 5, 5);
    
    // Test con 0
    run_test("Zero case 1", 0, 5, 0);
    run_test("Zero case 2", 7, 0, 0);
    run_test("Both zero", 0, 0, 0);
    
    // Test números grandes
    run_test("Large numbers", 98, 102, 4998);

    // Resumen
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);

    return g_tests_failed;
}