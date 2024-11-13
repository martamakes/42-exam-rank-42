/* ************************************************************************** */
/*                                   ENUNCIADO                                  */
/* Escribe una función que devuelva el número de elementos en la lista         */
/* enlazada que se le pasa como parámetro.                                    */
/* ************************************************************************** */

#include "ft_list.h"

/*
** CONCEPTOS CLAVE:
** 1. Listas enlazadas en C
** 2. Recorrido de listas
** 3. Punteros y estructuras
** 4. Conteo de elementos
*/

int ft_list_size(t_list *begin_list)
{
    int count;
    t_list *current;

    count = 0;
    current = begin_list;
    
    // Recorremos la lista hasta llegar al final (NULL)
    while (current)
    {
        count++;
        current = current->next;
    }
    return (count);
}