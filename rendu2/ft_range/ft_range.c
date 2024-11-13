#include <stdlib.h>

/*
** KEY POINTS PARA ESTUDIAR:
**
** 1. Cálculo del tamaño del array:
**    - Necesitamos saber cuántos números hay entre start y end
**    - Fórmula: abs(end - start) + 1 (el +1 es porque incluimos ambos extremos)
**    - Ejemplo: entre 1 y 3 hay: |3-1| + 1 = 3 números (1,2,3)
**
** 2. Asignación dinámica de memoria:
**    - malloc(size * sizeof(int))
**    - Importante verificar si malloc fue exitoso
**
** 3. Manejo de rangos:
**    - Ascendente: start < end
**    - Descendente: start > end
**    - Caso especial: start = end
**
** 4. Protección contra overflow:
**    - Verificar que el rango no sea demasiado grande
**    - Verificar que malloc no falle
*/

int    *ft_range(int start, int end)
{
    int     *range;
    int     size;
    int     i;
    int     step;

    // Calculamos el tamaño necesario y la dirección del paso
    size = (end >= start) ? (end - start + 1) : (start - end + 1);
    step = (end >= start) ? 1 : -1;

    // Asignamos memoria para el array
    range = (int *)malloc(sizeof(int) * size);
    if (!range)
        return (NULL);

    // Llenamos el array
    i = 0;
    while (i < size)
    {
        range[i] = start + (i * step);
        i++;
    }

    return (range);
}