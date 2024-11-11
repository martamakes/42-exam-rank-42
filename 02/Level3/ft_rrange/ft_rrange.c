#include <stdlib.h>

/*
Función: ft_rrange

Esta función asigna (con malloc()) un array de enteros, lo llena con valores
consecutivos que comienzan en 'end' y terminan en 'start' (¡incluyendo start y end!),
y luego devuelve un puntero al primer valor del array.

Parámetros:
    start: El valor final del rango
    end: El valor inicial del rango

Retorno:
    Un puntero al array de enteros, o NULL si la asignación de memoria falla
*/

int *ft_rrange(int start, int end)
{
    int *range;
    int size;
    int i;
    int step;

    // Calculamos el tamaño del array
    size = (start <= end) ? (end - start + 1) : (start - end + 1);

    // Asignamos memoria para el array
    range = (int *)malloc(sizeof(int) * size);
    if (range == NULL)
        return NULL; // Si malloc falla, devolvemos NULL

    // Determinamos la dirección del paso (-1 o 1)
    step = (start <= end) ? -1 : 1;

    // Llenamos el array con valores consecutivos en orden inverso
    i = 0;
    while (i < size)
    {
        range[i] = end;
        end += step;
        i++;
    }

    return range;
}