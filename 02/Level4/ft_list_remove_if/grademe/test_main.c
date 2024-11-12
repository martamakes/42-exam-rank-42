#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"

// Contador global de errores
int g_tests_failed = 0;

// Función de comparación para strings
int cmp_str(void *data1, void *data2)
{
    return strcmp((char *)data1, (char *)data2);
}

// Función de comparación para enteros
int cmp_int(void *data1, void *data2)
{
    return (*(int *)data1 - *(int *)data2);
}

// Función auxiliar para crear nodo
t_list *create_node(void *data)
{
    t_list *node = malloc(sizeof(t_list));
    if (!node)
        return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

// Función auxiliar para contar elementos
int count_elements(t_list *lst)
{
    int count = 0;
    while (lst)
    {
        count++;
        lst = lst->next;
    }
    return count;
}

// Función auxiliar para imprimir lista de strings
void print_list_str(t_list *lst)
{
    while (lst)
    {
        printf("%s -> ", (char *)lst->data);
        lst = lst->next;
    }
    printf("NULL\n");
}

// Test 1: Lista vacía
void test_empty_list(void)
{
    printf("Test 1: Lista vacía\n");
    t_list *lst = NULL;
    char *ref = "any";
    
    ft_list_remove_if(&lst, ref, cmp_str);
    if (lst == NULL)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m Lista debería seguir siendo NULL\n");
        g_tests_failed++;
    }
}

// Test 2: Eliminar primer elemento
void test_remove_first(void)
{
    printf("\nTest 2: Eliminar primer elemento\n");
    t_list *lst = create_node(strdup("one"));
    lst->next = create_node(strdup("two"));
    lst->next->next = create_node(strdup("three"));
    
    printf("Lista original: ");
    print_list_str(lst);
    
    char *ref = "one";
    ft_list_remove_if(&lst, ref, cmp_str);
    
    printf("Lista después de eliminar 'one': ");
    print_list_str(lst);
    
    if (lst && strcmp((char *)lst->data, "two") == 0 && count_elements(lst) == 2)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m Error al eliminar primer elemento\n");
        g_tests_failed++;
    }
    
    // Limpieza
    while (lst)
    {
        t_list *temp = lst;
        lst = lst->next;
        free(temp->data);
        free(temp);
    }
}

// Test 3: Eliminar múltiples elementos
void test_remove_multiple(void)
{
    printf("\nTest 3: Eliminar múltiples elementos iguales\n");
    
    int target = 42;
    int other = 24;
    
    t_list *lst = create_node(&target);
    lst->next = create_node(&other);
    lst->next->next = create_node(&target);
    lst->next->next->next = create_node(&target);
    
    int initial_count = count_elements(lst);
    printf("Elementos iniciales: %d\n", initial_count);
    
    ft_list_remove_if(&lst, &target, cmp_int);
    
    int final_count = count_elements(lst);
    printf("Elementos finales: %d\n", final_count);
    
    if (final_count == 1 && *(int *)lst->data == 24)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m Error al eliminar múltiples elementos\n");
        g_tests_failed++;
    }
    
    // Limpieza
    while (lst)
    {
        t_list *temp = lst;
        lst = lst->next;
        free(temp);
    }
}

// Test 4: Eliminar el último elemento
void test_remove_last(void)
{
    printf("\nTest 4: Eliminar último elemento\n");
    t_list *lst = create_node(strdup("one"));
    lst->next = create_node(strdup("two"));
    lst->next->next = create_node(strdup("three"));
    
    printf("Lista original: ");
    print_list_str(lst);
    
    char *ref = "three";
    ft_list_remove_if(&lst, ref, cmp_str);
    
    printf("Lista después de eliminar 'three': ");
    print_list_str(lst);
    
    if (lst && count_elements(lst) == 2 && 
        strcmp((char *)lst->next->data, "two") == 0)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m Error al eliminar último elemento\n");
        g_tests_failed++;
    }
    
    // Limpieza
    while (lst)
    {
        t_list *temp = lst;
        lst = lst->next;
        free(temp->data);
        free(temp);
    }
}

int main(void)
{
    printf("=== Testing ft_list_remove_if ===\n\n");

    test_empty_list();
    test_remove_first();
    test_remove_multiple();
    test_remove_last();

    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);

    return g_tests_failed;
}