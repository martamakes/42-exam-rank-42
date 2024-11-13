// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

t_list  *sort_list(t_list *lst, int (*cmp)(int, int));

int g_tests_failed = 0;

// Funciones de comparación para testing
int ascending(int a, int b)
{
    return (a <= b);
}

int descending(int a, int b)
{
    return (a >= b);
}

// Función auxiliar para crear un nuevo nodo
t_list *create_node(int data)
{
    t_list *node = malloc(sizeof(t_list));
    if (!node)
        return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

// Función auxiliar para crear una lista desde un array
t_list *create_list(int *arr, int size)
{
    t_list *head = NULL;
    t_list *current = NULL;
    
    for (int i = 0; i < size; i++)
    {
        t_list *node = create_node(arr[i]);
        if (!head)
            head = node;
        else
            current->next = node;
        current = node;
    }
    return head;
}

// Función auxiliar para liberar la lista
void free_list(t_list *lst)
{
    t_list *current;
    while (lst)
    {
        current = lst;
        lst = lst->next;
        free(current);
    }
}

// Función auxiliar para verificar si una lista está ordenada
int is_sorted(t_list *lst, int (*cmp)(int, int))
{
    while (lst && lst->next)
    {
        if (!cmp(lst->data, lst->next->data))
            return 0;
        lst = lst->next;
    }
    return 1;
}

// Función para imprimir una lista
void print_list(t_list *lst)
{
    while (lst)
    {
        printf("%d", lst->data);
        if (lst->next)
            printf(" -> ");
        lst = lst->next;
    }
    printf("\n");
}

void run_test(char *test_name, int *arr, int size, int (*cmp)(int, int))
{
    printf("Test %s: ", test_name);
    
    // Crear lista
    t_list *lst = create_list(arr, size);
    printf("\nInput:  ");
    print_list(lst);
    
    // Ordenar lista
    lst = sort_list(lst, cmp);
    printf("Output: ");
    print_list(lst);
    
    // Verificar si está ordenada
    if (is_sorted(lst, cmp))
    {
        printf("\033[0;32m[OK]\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("List is not properly sorted\n\n");
        g_tests_failed++;
    }
    
    free_list(lst);
}

int main(void)
{
    printf("\n=== Testing sort_list ===\n\n");

    // Test lista básica ascendente
    {
        int arr[] = {4, 2, 7, 1, 3};
        run_test("Basic ascending", arr, 5, ascending);
    }

    // Test lista básica descendente
    {
        int arr[] = {4, 2, 7, 1, 3};
        run_test("Basic descending", arr, 5, descending);
    }

    // Test lista con duplicados
    {
        int arr[] = {3, 3, 1, 2, 2, 1};
        run_test("With duplicates", arr, 6, ascending);
    }

    // Test lista ya ordenada
    {
        int arr[] = {1, 2, 3, 4, 5};
        run_test("Already sorted", arr, 5, ascending);
    }

    // Test lista invertida
    {
        int arr[] = {5, 4, 3, 2, 1};
        run_test("Reversed", arr, 5, ascending);
    }

    // Test lista con un elemento
    {
        int arr[] = {42};
        run_test("Single element", arr, 1, ascending);
    }

    // Resumen
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);

    return g_tests_failed;
}