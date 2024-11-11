/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// Función para crear un nuevo nodo
t_list *create_node(void *data)
{
    t_list *new_node = (t_list *)malloc(sizeof(t_list));
    if (new_node == NULL)
    {
        printf("Error: No se pudo asignar memoria\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// Función para agregar un nodo al final de la lista
void add_node(t_list **begin_list, void *data)
{
    t_list *new_node = create_node(data);
    if (*begin_list == NULL)
    {
        *begin_list = new_node;
    }
    else
    {
        t_list *current = *begin_list;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Función para liberar la memoria de la lista
void free_list(t_list *begin_list)
{
    t_list *current = begin_list;
    t_list *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

// Declaración de la función ft_list_size
int ft_list_size(t_list *begin_list);

int main()
{
    t_list *my_list = NULL;

    // Agregar algunos elementos a la lista
    add_node(&my_list, "Primer elemento");
    add_node(&my_list, "Segundo elemento");
    add_node(&my_list, "Tercer elemento");
    add_node(&my_list, "Cuarto elemento");

    // Llamar a ft_list_size y mostrar el resultado
    int size = ft_list_size(my_list);
    printf("El tamaño de la lista es: %d\n", size);

    // Probar con una lista vacía
    t_list *empty_list = NULL;
    size = ft_list_size(empty_list);
    printf("El tamaño de la lista vacía es: %d\n", size);

    // Liberar la memoria
    free_list(my_list);

    return 0;
}