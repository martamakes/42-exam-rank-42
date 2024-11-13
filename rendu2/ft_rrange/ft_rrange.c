/* 
** ft_rrange.c
**
** CONCEPTOS CLAVE:
**
** 1. Manejo de memoria dinámica:
**    - malloc: asigna memoria del tamaño especificado
**    - Cálculo del tamaño necesario
**    - Responsabilidad de verificar el retorno de malloc
**
** 2. Rangos de números:
**    - Los rangos incluyen ambos extremos
**    - Pueden ser positivos o negativos
**    - Pueden ser ascendentes o descendentes
**    - Cálculo del tamaño del rango
**
** 3. Orden inverso:
**    - A diferencia de range normal, este va de end a start
**    - Debemos llenar el array en orden descendente
**
** 4. Casos especiales:
**    - Cuando start = end (rango de un solo número)
**    - Cuando end > start (rango descendente)
**    - Cuando start > end (rango ascendente)
*/

#include <stdlib.h>

int     *ft_rrange(int start, int end)
{
    int     *range;
    int     size;
    int     i;
    int     step;

    // Calcular el tamaño necesario (abs(end - start) + 1)
    // El +1 es porque incluimos ambos extremos
    size = (end >= start) ? (end - start + 1) : (start - end + 1);

    // Asignar memoria para el array
    range = (int *)malloc(sizeof(int) * size);
    if (!range)
        return (NULL);

    // Determinar si vamos incrementando o decrementando
    step = (end >= start) ? -1 : 1;

    // Llenar el array empezando desde end
    i = 0;
    while (i < size)
    {
        range[i] = end;
        end += step;
        i++;
    }

    return (range);
}