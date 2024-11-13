/* ************************************************************************** */
/*                                   ENUNCIADO                                  */
/* Escribe una función que elimine de la lista pasada cualquier elemento cuyo  */
/* dato sea "igual" al dato de referencia.                                     */
/* La función de comparación cmp toma dos void* y retorna 0 cuando ambos       */
/* parámetros son iguales.                                                     */
/* ************************************************************************** */

#include "ft_list.h"
#include <stdlib.h>

/*
** CONCEPTOS CLAVE:
** 1. Punteros a punteros (para modificar el head de la lista)
** 2. Manejo de memoria dinámica (free)
** 3. Punteros a funciones (callback de comparación)
** 4. Recorrido de lista enlazada
** 5. Casos especiales:
**    - Lista vacía
**    - Eliminar el primer elemento
**    - Eliminar elementos consecutivos
*/

/* ft_list_remove_if.c */

#include <stdlib.h>
#include "ft_list.h"

void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
    /* Si la lista está vacía o no existe, no hay nada que hacer */
    if (!begin_list || !*begin_list)
        return;

    t_list *current = *begin_list;

    /* Caso especial: si el primer nodo debe ser eliminado */
    if ((*cmp)(current->data, data_ref) == 0)
    {
        /* Actualizamos el inicio de la lista al siguiente elemento */
        *begin_list = current->next;
        /* Liberamos la memoria del nodo actual */
        free(current);
        /* Llamada recursiva para seguir comprobando desde el nuevo inicio */
        ft_list_remove_if(begin_list, data_ref, cmp);
        return;
    }
    /* Llamada recursiva para el resto de la lista */
    ft_list_remove_if(&current->next, data_ref, cmp);
}
