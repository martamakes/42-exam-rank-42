#include <stdio.h>

// Declaración de la función sort_int_tab
void sort_int_tab(int *tab, unsigned int size);

// Función para imprimir el array
void print_array(int *arr, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


int main(void)
{
    // Array de prueba
    int arr[] = {5, 2, 9, 1, 5, 6, 3, 8, 4, 7};
    unsigned int size = sizeof(arr) / sizeof(arr[0]);

    printf("Array original: ");
    print_array(arr, size);

    // Llamada a la función de ordenación
    sort_int_tab(arr, size);

    printf("Array ordenado: ");
    print_array(arr, size);

    return 0;
}