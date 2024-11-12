// test_main.c
#include <stdio.h>

int max(int* tab, unsigned int len);

void test_max(int *arr, unsigned int size, int expected, char *description)
{
    int result = max(arr, size);
    
    printf("Test %s: ", description);
    if (result == expected)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Expected: %d\n", expected);
        printf("  Got: %d\n", result);
    }
}

int main(void)
{
    printf("\n=== Testing max ===\n\n");

    // Test 1: Array básico de positivos
    {
        int arr[] = {1, 2, 3, 4, 5};
        test_max(arr, 5, 5, "Array básico de positivos");
    }

    // Test 2: Array con negativos
    {
        int arr[] = {-5, -4, -3, -2, -1};
        test_max(arr, 5, -1, "Array de negativos");
    }

    // Test 3: Array mezclado
    {
        int arr[] = {-2, 0, 3, -1, 2};
        test_max(arr, 5, 3, "Array mezclado");
    }

    // Test 4: Array de un elemento
    {
        int arr[] = {42};
        test_max(arr, 1, 42, "Array de un elemento");
    }

    // Test 5: Array vacío
    {
        int arr[] = {};
        test_max(arr, 0, 0, "Array vacío");
    }

    // Test 6: Números grandes
    {
        int arr[] = {2147483647, 2147483646, 2147483645};
        test_max(arr, 3, 2147483647, "Números grandes");
    }

    // Test 7: Números repetidos
    {
        int arr[] = {5, 5, 5, 5, 5};
        test_max(arr, 5, 5, "Números repetidos");
    }

    // Test 8: El máximo está al principio
    {
        int arr[] = {9, 8, 7, 6, 5};
        test_max(arr, 5, 9, "Máximo al principio");
    }

    // Test 9: El máximo está al final
    {
        int arr[] = {5, 6, 7, 8, 9};
        test_max(arr, 5, 9, "Máximo al final");
    }

    // Test 10: El máximo está en el medio
    {
        int arr[] = {1, 2, 9, 4, 5};
        test_max(arr, 5, 9, "Máximo en el medio");
    }

    // Test 11: Array con cero
    {
        int arr[] = {-3, -2, 0, -1, -4};
        test_max(arr, 5, 0, "Array con cero");
    }

    // Test 12: Números negativos grandes
    {
        int arr[] = {-2147483648, -2147483647, -2147483646};
        test_max(arr, 3, -2147483646, "Números negativos grandes");
    }

    // Test 13: NULL pointer
    test_max(NULL, 5, 0, "NULL pointer");

    return 0;
}