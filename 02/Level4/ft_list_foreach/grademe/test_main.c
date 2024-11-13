// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"

int g_tests_failed = 0;

// Función auxiliar para crear un nuevo nodo
t_list *create_node(void *data)
{
    t_list *node = malloc(sizeof(t_list));
    if (!node)
        return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

// Función auxiliar para liberar la lista
void free_list(t_list *list)
{
    t_list *current;
    while (list)
    {
        current = list;
        list = list->next;
        free(current);
    }
}

// Función para convertir int a mayúsculas
void to_upper(void *data)
{
    char *str = (char *)data;
    while (*str)
    {
        if (*str >= 'a' && *str <= 'z')
            *str -= 32;
        str++;
    }
}

// Función para multiplicar por 2
void multiply_by_two(void *data)
{
    int *num = (int *)data;
    *num *= 2;
}

void run_string_test()
{
    printf("Test with strings: ");
    
    // Crear lista con strings
    t_list *list = create_node(strdup("hello"));
    list->next = create_node(strdup("world"));
    list->next->next = create_node(strdup("42"));
    
    // Aplicar función
    ft_list_foreach(list, to_upper);
    
    // Verificar resultados
    if (strcmp((char *)list->data, "HELLO") == 0 &&
        strcmp((char *)list->next->data, "WORLD") == 0 &&
        strcmp((char *)list->next->next->data, "42") == 0)
    {
        printf("\033[0;32m[OK]\033[0m\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        g_tests_failed++;
    }
    
    // Liberar memoria
    while (list)
    {
        t_list *tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }
}

void run_int_test()
{
    printf("Test with integers: ");
    
    // Crear datos
    int *n1 = malloc(sizeof(int));
    int *n2 = malloc(sizeof(int));
    int *n3 = malloc(sizeof(int));
    *n1 = 1;
    *n2 = 2;
    *n3 = 3;
    
    // Crear lista
    t_list *list = create_node(n1);
    list->next = create_node(n2);
    list->next->next = create_node(n3);
    
    // Aplicar función
    ft_list_foreach(list, multiply_by_two);
    
    // Verificar resultados
    if (*(int *)list->data == 2 &&
        *(int *)list->next->data == 4 &&
        *(int *)list->next->next->data == 6)
    {
        printf("\033[0;32m[OK]\033[0m\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        g_tests_failed++;
    }
    
    // Liberar memoria
    while (list)
    {
        t_list *tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }
}

void run_null_tests()
{
    printf("Test with NULL list: ");
    
    // No debería crashear
    ft_list_foreach(NULL, to_upper);
    printf("\033[0;32m[OK]\033[0m\n");
    
    printf("Test with NULL function: ");
    t_list *list = create_node(strdup("test"));
    
    // No debería crashear
    ft_list_foreach(list, NULL);
    printf("\033[0;32m[OK]\033[0m\n");
    
    // Liberar memoria
    free(list->data);
    free(list);
}

int main(void)
{
    printf("\n=== Testing ft_list_foreach ===\n\n");
    
    run_string_test();
    run_int_test();
    run_null_tests();
    
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}