#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// Función de comparación para ordenar en orden ascendente
int ascending(int a, int b)
{
    return (a <= b);
}

// Función para crear un nuevo nodo
t_list *new_node(int data)
{
    t_list *node = (t_list *)malloc(sizeof(t_list));
    if (node)
    {
        node->data = data;
        node->next = NULL;
    }
    return node;
}

// Función para imprimir la lista
void print_list(t_list *lst)
{
    while (lst)
    {
        printf("%d ", lst->data);
        lst = lst->next;
    }
    printf("\n");
}

// Función para liberar la memoria de la lista
void free_list(t_list *lst)
{
    t_list *temp;
    while (lst)
    {
        temp = lst;
        lst = lst->next;
        free(temp);
    }
}

int main(void)
{
    // Crear una lista de prueba
    t_list *list = new_node(4);
    list->next = new_node(2);
    list->next->next = new_node(7);
    list->next->next->next = new_node(2);
    list->next->next->next->next = new_node(-6);

    printf("Lista original: ");
    print_list(list);

    // Ordenar la lista
    list = sort_list(list, ascending);

    printf("Lista ordenada: ");
    print_list(list);

    // Liberar la memoria
    free_list(list);

    return 0;
}