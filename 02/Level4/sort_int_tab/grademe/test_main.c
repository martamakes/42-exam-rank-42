// test_main.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int g_tests_failed = 0;

// Prototipo de la función
void sort_int_tab(int *tab, unsigned int size);

// Función auxiliar para imprimir arrays
void print_array(int *arr, unsigned int size) {
    printf("[");
    for (unsigned int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1)
            printf(", ");
    }
    printf("]");
}

// Función para verificar si un array está ordenado
int is_sorted(int *arr, unsigned int size) {
    for (unsigned int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1])
            return 0;
    }
    return 1;
}

void run_test(char *test_name, int *input, int *expected, unsigned int size) {
    // Creamos una copia del input para no modificar el original
    int *test_array = (int *)malloc(size * sizeof(int));
    memcpy(test_array, input, size * sizeof(int));
    
    // Ejecutamos la función
    sort_int_tab(test_array, size);
    
    // Verificamos que el resultado sea correcto
    int is_correct = 1;
    for (unsigned int i = 0; i < size; i++) {
        if (test_array[i] != expected[i]) {
            is_correct = 0;
            break;
        }
    }
    
    printf("Test %s: ", test_name);
    if (is_correct) {
        printf("\033[0;32m[OK]\033[0m\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n");
        g_tests_failed++;
        printf("  Input:    "); print_array(input, size); printf("\n");
        printf("  Expected: "); print_array(expected, size); printf("\n");
        printf("  Got:      "); print_array(test_array, size); printf("\n");
    }
    
    free(test_array);
}

int main(void) {
    printf("\n=== Testing sort_int_tab ===\n\n");

    // Test 1: Array normal
    {
        int input[] = {5, 2, 8, 1, 9, 3};
        int expected[] = {1, 2, 3, 5, 8, 9};
        run_test("Normal array", input, expected, 6);
    }

    // Test 2: Array con duplicados
    {
        int input[] = {3, 2, 3, 1, 2, 1};
        int expected[] = {1, 1, 2, 2, 3, 3};
        run_test("Array with duplicates", input, expected, 6);
    }

    // Test 3: Array ya ordenado
    {
        int input[] = {1, 2, 3, 4, 5};
        int expected[] = {1, 2, 3, 4, 5};
        run_test("Already sorted array", input, expected, 5);
    }

    // Test 4: Array con números negativos
    {
        int input[] = {-3, 4, -1, 0, 2, -5};
        int expected[] = {-5, -3, -1, 0, 2, 4};
        run_test("Array with negative numbers", input, expected, 6);
    }

    // Test 5: Array pequeño
    {
        int input[] = {2, 1};
        int expected[] = {1, 2};
        run_test("Small array", input, expected, 2);
    }

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}