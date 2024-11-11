#include <stdio.h>
#include <stdlib.h>

// Prototipo de la función
int *ft_rrange(int start, int end);

// Función auxiliar para imprimir el array
void print_array(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    int *result;
    int start, end, size;

    // Caso de prueba 1: (1, 3)
    start = 1;
    end = 3;
    size = end - start + 1;
    result = ft_rrange(start, end);
    printf("Test (1, 3): ");
    print_array(result, size);
    free(result);

    // Caso de prueba 2: (-1, 2)
    start = -1;
    end = 2;
    size = end - start + 1;
    result = ft_rrange(start, end);
    printf("Test (-1, 2): ");
    print_array(result, size);
    free(result);

    // Caso de prueba 3: (0, 0)
    start = 0;
    end = 0;
    size = 1;
    result = ft_rrange(start, end);
    printf("Test (0, 0): ");
    print_array(result, size);
    free(result);

    // Caso de prueba 4: (0, -3)
    start = 0;
    end = -3;
    size = start - end + 1;
    result = ft_rrange(start, end);
    printf("Test (0, -3): ");
    print_array(result, size);
    free(result);

    // Caso de prueba adicional: (5, 10)
    start = 5;
    end = 10;
    size = end - start + 1;
    result = ft_rrange(start, end);
    printf("Test (5, 10): ");
    print_array(result, size);
    free(result);

    return 0;
}