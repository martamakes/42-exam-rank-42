#include <stdio.h>
#include <stdlib.h>
#include "ft_list.h"

// Contador global de errores
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

// Función auxiliar para liberar una lista
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

// Función auxiliar para ejecutar un test
void run_test(t_list *list, int expected, const char *test_name)
{
    int result = ft_list_size(list);
    
    printf("Test %s: ", test_name);
    
    if (result == expected)
    {
        printf("\033[0;32m[OK]\033[0m\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Expected: %d\n", expected);
        printf("  Got: %d\n", result);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing ft_list_size ===\n\n");

    // Test 1: Lista vacía
    run_test(NULL, 0, "empty list");

    // Test 2: Lista con un elemento
    t_list *single = create_node("one");
    run_test(single, 1, "single element");
    free_list(single);

    // Test 3: Lista con múltiples elementos
    t_list *multi = create_node("one");
    multi->next = create_node("two");
    multi->next->next = create_node("three");
    multi->next->next->next = create_node("four");
    multi->next->next->next->next = create_node("five");
    run_test(multi, 5, "five elements");
    free_list(multi);

    // Test 4: Lista grande
    t_list *large = NULL;
    t_list *current = NULL;
    int i;
    int size = 100;

    for (i = 0; i < size; i++)
    {
        if (!large)
        {
            large = create_node(&i);
            current = large;
        }
        else
        {
            current->next = create_node(&i);
            current = current->next;
        }
    }
    run_test(large, 100, "large list (100 elements)");
    free_list(large);

    // Test 5: Lista con elementos NULL
    t_list *null_data = create_node(NULL);
    null_data->next = create_node(NULL);
    null_data->next->next = create_node(NULL);
    run_test(null_data, 3, "three elements with NULL data");
    free_list(null_data);

    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
    {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    }
    else
    {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}
