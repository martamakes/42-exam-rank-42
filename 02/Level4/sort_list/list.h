#ifndef LIST_H
# define LIST_H

typedef struct s_list
{
	int				data;
	struct s_list	*next;
}	t_list;

t_list	*sort_list(t_list *lst, int (*cmp)(int, int));

#endif