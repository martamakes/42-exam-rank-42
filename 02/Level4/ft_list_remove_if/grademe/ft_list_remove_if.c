#include "ft_list.h"
#include <stdlib.h>

void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
    if (!begin_list || !*begin_list)
        return;
    
    t_list *current;
    current = *begin_list;

    if((*cmp)(current->data, data_ref) == 0)
    {
        *begin_list = current->next;
        free(current);
        ft_list_remove_if(begin_list, data_ref, cmp);
    }
    else
    {
        ft_list_remove_if(&(current->next), data_ref, cmp);
    }
}
