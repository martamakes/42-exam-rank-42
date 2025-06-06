// test_main.c para sort_list - VERSIÓN SIMPLIFICADA (solo enunciado)
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

t_list *sort_list(t_list *lst, int (*cmp)(int, int));

int g_tests_failed = 0;

// Funciones de comparación del enunciado
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
    if (size <= 0)
        return NULL;
        
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

// Función para contar elementos de la lista
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

// Función para imprimir una lista
void print_list(t_list *lst)
{
    printf("[");
    while (lst)
    {
        printf("%d", lst->data);
        if (lst->next)
            printf(", ");
        lst = lst->next;
    }
    printf("]");
}

void run_test(char *test_name, int *arr, int size, int (*cmp)(int, int))
{
    printf("Test %s:\n", test_name);
    
    // Crear lista
    t_list *lst = create_list(arr, size);
    printf("Input:  ");
    print_list(lst);
    printf("\n");
    
    // Contar elementos originales
    int original_count = count_elements(lst);
    
    // Ordenar lista
    lst = sort_list(lst, cmp);
    
    printf("Output: ");
    print_list(lst);
    printf("\n");
    
    // Verificar conteo (duplicados deben mantenerse)
    int final_count = count_elements(lst);
    if (original_count != final_count)
    {
        printf("\033[0;31m[KO] - Perdió elementos: %d -> %d (duplicados deben mantenerse)\033[0m\n\n", original_count, final_count);
        g_tests_failed++;
        free_list(lst);
        return;
    }
    
    // Verificar si está ordenada según la función cmp
    if (is_sorted(lst, cmp))
    {
        printf("\033[0;32m[OK]\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO] - Lista no está ordenada según función cmp\033[0m\n\n");
        g_tests_failed++;
    }
    
    free_list(lst);
}

int main(void)
{
    printf("\n=== Testing sort_list - VERSIÓN ENUNCIADO ===\n\n");

    // Test 1: Ejemplo básico con orden ascendente (del enunciado)
    {
        int arr[] = {4, 2, 7, 1, 3};
        run_test("Basic ascending", arr, 5, ascending);
    }

    // Test 2: Ejemplo básico con orden descendente
    {
        int arr[] = {4, 2, 7, 1, 3};
        run_test("Basic descending", arr, 5, descending);
    }

    // Test 3: Con duplicados (enunciado dice "Duplications must remain")
    {
        int arr[] = {3, 3, 1, 2, 2, 1};
        run_test("With duplicates", arr, 6, ascending);
    }

    // Test 4: Lista NULL (caso edge básico)
    {
        printf("Test Lista NULL:\n");
        printf("Input:  NULL\n");
        
        t_list *result = sort_list(NULL, ascending);
        
        printf("Output: ");
        if (result == NULL)
        {
            printf("NULL\n");
            printf("\033[0;32m[OK] - Retornó NULL correctamente\033[0m\n\n");
        }
        else
        {
            printf("NO-NULL\n");
            printf("\033[0;31m[KO] - Debería retornar NULL para lista vacía\033[0m\n\n");
            g_tests_failed++;
        }
    }

    // Test 5: Un solo elemento
    {
        int arr[] = {42};
        run_test("Un solo elemento", arr, 1, ascending);
    }

    // Test 6: Dos elementos
    {
        int arr[] = {5, 3};
        run_test("Dos elementos", arr, 2, ascending);
    }

    // Test 7: Lista ya ordenada
    {
        int arr[] = {1, 2, 3, 4, 5};
        run_test("Ya ordenada", arr, 5, ascending);
    }

    // Resumen
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32m✅ Todos los tests pasaron! Tu sort_list cumple con el enunciado.\033[0m\n");
    else
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);

    printf("\n\033[0;33m📝 REQUISITOS DEL ENUNCIADO VERIFICADOS:\033[0m\n");
    printf("✅ Ordenar usando función cmp\n");
    printf("✅ Retornar puntero al primer elemento\n");
    printf("✅ Mantener duplicados\n");
    printf("✅ Función cmp retorna != 0 si están en orden correcto\n");
    printf("✅ Manejar entradas consistentes\n");
    
    printf("\n\033[0;31m⚠️  PUNTOS CLAVE DEL ENUNCIADO:\033[0m\n");
    printf("• sort_list(t_list *lst, int (*cmp)(int, int))\n");
    printf("• Usar función cmp para seleccionar orden\n");
    printf("• Duplicados deben mantenerse\n");
    printf("• Entradas siempre serán consistentes\n");
    printf("• cmp retorna != 0 si a y b están en orden correcto\n");

    printf("\n\033[0;36m💡 EJEMPLO DEL ENUNCIADO:\033[0m\n");
    printf("int ascending(int a, int b) { return (a <= b); }\n");
    printf("- ascending(1, 2) = 1 (están en orden) → no intercambiar\n");
    printf("- ascending(3, 1) = 0 (mal orden) → intercambiar\n");

    return g_tests_failed;
}

/*
TESTS SEGÚN EL ENUNCIADO:

✅ REQUERIMIENTOS ESPECÍFICOS:
1. Función debe ordenar la lista
2. Usar función cmp para seleccionar orden
3. Retornar puntero al primer elemento
4. Mantener duplicados
5. Entradas consistentes (no testear casos raros)

✅ CASOS BÁSICOS NECESARIOS:
1. Lista normal con ascending/descending
2. Lista con duplicados
3. Lista NULL (caso edge mínimo)
4. Un elemento
5. Dos elementos
6. Lista ya ordenada

❌ CASOS ELIMINADOS (no en enunciado):
- Función cmp NULL (enunciado dice "entradas consistentes")
- Números negativos/extremos (no mencionados)
- Listas muy largas (no mencionadas)
- Funciones cmp especiales (no mencionadas)
- Verificación de bucles (no mencionada)

ENFOQUE:
Solo testear lo que específicamente pide el enunciado.
Mantener simplicidad y enfoque en los requisitos reales.
*/