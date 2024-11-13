#include <stdio.h>

// Contador global de errores
int g_tests_failed = 0;

int is_power_of_2(unsigned int n);

void run_test(char *test_name, unsigned int input, int expected)
{
    printf("Test %s: ", test_name);
    
    int result = is_power_of_2(input);
    
    if (result == expected)
        printf("\033[0;32m[OK]\033[0m\n");
    else {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input    : %u\n", input);
        printf("  Expected : %d\n", expected);
        printf("  Got      : %d\n", result);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing is_power_of_2 ===\n\n");

    // Tests básicos - potencias de 2 conocidas
    run_test("Power 2^0", 1, 1);      // 2^0
    run_test("Power 2^1", 2, 1);      // 2^1
    run_test("Power 2^2", 4, 1);      // 2^2
    run_test("Power 2^3", 8, 1);      // 2^3
    run_test("Power 2^4", 16, 1);     // 2^4
    run_test("Power 2^5", 32, 1);     // 2^5
    
    // Test caso especial
    run_test("Zero", 0, 0);
    
    // Tests números que no son potencia de 2
    run_test("Not power 1", 3, 0);
    run_test("Not power 2", 6, 0);
    run_test("Not power 3", 7, 0);
    run_test("Not power 4", 12, 0);
    run_test("Not power 5", 15, 0);
    
    // Test número grande potencia de 2
    run_test("Big power", 1024, 1);   // 2^10
    
    // Test número grande no potencia de 2
    run_test("Big not power", 1023, 0);
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}