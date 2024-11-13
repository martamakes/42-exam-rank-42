// test_main.c
#include <stdio.h>
#include <stdlib.h>

int g_tests_failed = 0;

// Prototipo de la función
int     *ft_rrange(int start, int end);

// Función auxiliar para comparar arrays
int compare_arrays(int *arr1, int *arr2, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr1[i] != arr2[i])
            return 0;
    }
    return 1;
}

void run_test(char *test_name, int start, int end, int *expected, int size)
{
    printf("Test %s: ", test_name);
    
    int *result = ft_rrange(start, end);
    if (!result)
    {
        printf("\033[0;31m[KO] - malloc failed\033[0m\n");
        g_tests_failed++;
        return;
    }

    if (compare_arrays(result, expected, size))
    {
        printf("\033[0;32m[OK]\033[0m\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("Expected: ");
        for (int i = 0; i < size; i++)
            printf("%d ", expected[i]);
        printf("\nGot     : ");
        for (int i = 0; i < size; i++)
            printf("%d ", result[i]);
        printf("\n");
        g_tests_failed++;
    }

    free(result);
}

int main(void)
{
    printf("\n=== Testing ft_rrange ===\n\n");

    // Test 1: Caso del subject (1, 3)
    {
        int expected[] = {3, 2, 1};
        run_test("Subject example 1", 1, 3, expected, 3);
    }

    // Test 2: Caso del subject (-1, 2)
    {
        int expected[] = {2, 1, 0, -1};
        run_test("Subject example 2", -1, 2, expected, 4);
    }

    // Test 3: Caso del subject (0, 0)
    {
        int expected[] = {0};
        run_test("Subject example 3", 0, 0, expected, 1);
    }

    // Test 4: Caso del subject (0, -3)
    {
        int expected[] = {-3, -2, -1, 0};
        run_test("Subject example 4", 0, -3, expected, 4);
    }

    // Test 5: Rango grande positivo
    {
        int expected[] = {5, 4, 3, 2, 1, 0};
        run_test("Large positive range", 0, 5, expected, 6);
    }

    // Test 6: Rango grande negativo
    {
        int expected[] = {-1, -2, -3, -4, -5};
        run_test("Large negative range", -5, -1, expected, 5);
    }

    // Test 7: Cruzando el cero
    {
        int expected[] = {1, 0, -1, -2};
        run_test("Crossing zero", -2, 1, expected, 4);
    }

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}