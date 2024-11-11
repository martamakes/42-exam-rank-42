/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_username <your_email@student.42.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by your_username    #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by your_username   ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include <stdlib.h>

/*
** Enunciado:
** Implementar la función sort_list que ordena una lista enlazada.
** La función debe usar un puntero a función 'cmp' para determinar el orden.
** Debe preservar duplicados y manejar entradas consistentes.
** La función cmp retorna un valor distinto de 0 si a y b están en orden correcto,
** 0 en caso contrario.
*/

/*
** Esta función intercambia los datos de dos nodos de la lista.
** Se usa para reordenar los elementos sin modificar los punteros de la lista.
*/
static void	swap_data(t_list *a, t_list *b)
{
	int	temp;

	temp = a->data;
	a->data = b->data;
	b->data = temp;
}

/*
** sort_list ordena la lista usando el algoritmo de burbuja.
** Recorre la lista repetidamente, comparando elementos adyacentes
** y swapeándolos si están en el orden incorrecto según la función cmp.
*/
t_list	*sort_list(t_list *lst, int (*cmp)(int, int))
{
	int		swapped;
	t_list	*ptr;
	t_list	*lptr;

	if (!lst)
		return (NULL);
	lptr = NULL;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr = lst;
		while (ptr->next != lptr)
		{
			if ((*cmp)(ptr->data, ptr->next->data) == 0)
			{
				swap_data(ptr, ptr->next);
				swapped = 1;
			}
			ptr = ptr->next;
		}
		lptr = ptr;
	}
	return (lst);
}

/*
** Explicación del código:
** 1. swap_data: Función auxiliar que intercambia los datos de dos nodos.
** 2. sort_list: Implementa el algoritmo de burbuja para listas enlazadas:
**    - Usa un bucle externo que continúa mientras se realicen intercambios.
**    - El bucle interno recorre la lista, comparando pares de elementos.
**    - Si dos elementos están en orden incorrecto (cmp retorna 0), se intercambian.
**    - lptr se usa para optimizar, reduciendo el área de búsqueda en cada pasada.
**    - El proceso se repite hasta que no se realicen más intercambios.
** 3. La función preserva la estructura de la lista, solo modifica los datos.
** 4. Maneja correctamente listas vacías retornando NULL.
** 5. Retorna un puntero al primer elemento de la lista ordenada.
*/