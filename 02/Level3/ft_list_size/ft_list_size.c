/* ft_list_size.c */

#include "list.h"
#include <stdlib.h>

/*
Asignación: ft_list_size

Esta función devuelve el número de elementos en la lista enlazada
que se le pasa como argumento.

La función debe declararse de la siguiente manera:
int ft_list_size(t_list *begin_list);

Utilizamos la estructura t_list definida en ft_list.h:
typedef struct    s_list
{
    struct s_list *next;
    void          *data;
}                 t_list;
*/

int ft_list_size(t_list *begin_list)
{
    int count = 0;
    t_list *current = begin_list;

    // Recorremos la lista hasta llegar al final
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}