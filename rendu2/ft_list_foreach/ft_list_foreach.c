// ft_list_foreach.c
#include "ft_list.h"

void    ft_list_foreach(t_list *begin_list, void (*f)(void *))
{
    t_list *current;

    // Si la lista está vacía o no hay función, retornar
    if (!begin_list || !f)
        return;

    // Recorrer la lista aplicando la función a cada elemento
    current = begin_list;
    while (current)
    {
        // Aplicar la función al dato actual
        (*f)(current->data);
        current = current->next;
    }
}