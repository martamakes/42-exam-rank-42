// test_main_improved.c para sort_list
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

// Función de comparación que siempre retorna 0 (para testing)
int always_equal(int a, int b)
{
    (void)a;
    (void)b;
    return (0);
}

// Función de comparación para números pares primero
int even_first(int a, int b)
{
    if ((a % 2) == (b % 2))
        return (a <= b);
    return ((a % 2) == 0);
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

// Función auxiliar para crear lista con todos elementos iguales
t_list *create_all_same(int value, int count)
{
    t_list *head = NULL;
    t_list *current = NULL;
    
    for (int i = 0; i < count; i++)
    {
        t_list *node = create_node(value);
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

// Función para verificar integridad de la lista
int check_list_integrity(t_list *lst)
{
    // Verificar que no hay bucles (máximo 1000 elementos)
    int count = 0;
    t_list *slow = lst;
    t_list *fast = lst;
    
    while (fast && fast->next && count < 1000)
    {
        slow = slow->next;
        fast = fast->next->next;
        count++;
        
        if (slow == fast)
            return 0; // Bucle detectado
    }
    return 1;
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
    
    // Verificar integridad
    if (!check_list_integrity(lst))
    {
        printf("\033[0;31m[KO] - Lista corrupta (posible bucle)\033[0m\n\n");
        g_tests_failed++;
        return;
    }
    
    // Verificar conteo
    int final_count = count_elements(lst);
    if (original_count != final_count)
    {
        printf("\033[0;31m[KO] - Perdió elementos: %d -> %d\033[0m\n\n", original_count, final_count);
        g_tests_failed++;
        free_list(lst);
        return;
    }
    
    // Verificar si está ordenada
    if (is_sorted(lst, cmp))
    {
        printf("\033[0;32m[OK]\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO] - Lista no está ordenada correctamente\033[0m\n\n");
        g_tests_failed++;
    }
    
    free_list(lst);
}

int main(void)
{
    printf("\n=== Testing sort_list - VERSIÓN EXAMEN ===\n\n");

    // Test 1: Lista básica ascendente
    {
        int arr[] = {4, 2, 7, 1, 3};
        run_test("Basic ascending", arr, 5, ascending);
    }

    // Test 2: Lista básica descendente
    {
        int arr[] = {4, 2, 7, 1, 3};
        run_test("Basic descending", arr, 5, descending);
    }

    // Test 3: Lista con duplicados
    {
        int arr[] = {3, 3, 1, 2, 2, 1};
        run_test("With duplicates", arr, 6, ascending);
    }

    // *** CASOS CRÍTICOS QUE FALTABAN ***

    // Test 4: Lista NULL (CRÍTICO PARA EXAMEN)
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
            printf("\033[0;31m[KO] - Debería retornar NULL\033[0m\n\n");
            g_tests_failed++;
            free_list(result);
        }
    }

    // Test 5: Un solo elemento (CRÍTICO)
    {
        int arr[] = {42};
        printf("Test Un solo elemento:\n");
        
        t_list *lst = create_list(arr, 1);
        printf("Input:  ");
        print_list(lst);
        printf("\n");
        
        lst = sort_list(lst, ascending);
        
        printf("Output: ");
        print_list(lst);
        printf("\n");
        
        if (lst && lst->data == 42 && lst->next == NULL)
        {
            printf("\033[0;32m[OK]\033[0m\n\n");
        }
        else
        {
            printf("\033[0;31m[KO] - Un elemento mal manejado\033[0m\n\n");
            g_tests_failed++;
        }
        
        free_list(lst);
    }

    // Test 6: Dos elementos desordenados
    {
        int arr[] = {5, 3};
        run_test("Dos elementos desordenados", arr, 2, ascending);
    }

    // Test 7: Dos elementos ordenados
    {
        int arr[] = {3, 5};
        run_test("Dos elementos ordenados", arr, 2, ascending);
    }

    // Test 8: Dos elementos iguales
    {
        int arr[] = {5, 5};
        run_test("Dos elementos iguales", arr, 2, ascending);
    }

    // Test 9: Todos elementos iguales
    {
        printf("Test Todos elementos iguales:\n");
        t_list *all_same = create_all_same(42, 5);
        
        printf("Input:  ");
        print_list(all_same);
        printf("\n");
        
        all_same = sort_list(all_same, ascending);
        
        printf("Output: ");
        print_list(all_same);
        printf("\n");
        
        // Verificar que siguen siendo 5 elementos con valor 42
        if (count_elements(all_same) == 5 && is_sorted(all_same, ascending))
        {
            printf("\033[0;32m[OK]\033[0m\n\n");
        }
        else
        {
            printf("\033[0;31m[KO] - Todos iguales mal manejado\033[0m\n\n");
            g_tests_failed++;
        }
        
        free_list(all_same);
    }

    // Test 10: Lista ya ordenada
    {
        int arr[] = {1, 2, 3, 4, 5};
        run_test("Ya ordenada", arr, 5, ascending);
    }

    // Test 11: Lista invertida
    {
        int arr[] = {5, 4, 3, 2, 1};
        run_test("Invertida", arr, 5, ascending);
    }

    // Test 12: Función de comparación NULL (EDGE CASE)
    {
        printf("Test Función comparación NULL:\n");
        int arr[] = {3, 1, 2};
        t_list *lst = create_list(arr, 3);
        
        printf("Input:  ");
        print_list(lst);
        printf("\n");
        
        // Esto puede crashear o comportarse de manera indefinida
        printf("Función cmp = NULL (comportamiento indefinido)\n");
        printf("\033[0;33m[SKIP] - No se puede testear de manera segura\033[0m\n\n");
        
        free_list(lst);
    }

    // Test 13: Función que siempre retorna 0
    {
        int arr[] = {3, 1, 4, 2};
        printf("Test Función siempre retorna 0:\n");
        
        t_list *lst = create_list(arr, 4);
        printf("Input:  ");
        print_list(lst);
        printf("\n");
        
        lst = sort_list(lst, always_equal);
        
        printf("Output: ");
        print_list(lst);
        printf("\n");
        
        // Con cmp que siempre retorna 0, el algoritmo debería manejar esto
        if (count_elements(lst) == 4)
        {
            printf("\033[0;32m[OK] - Mantuvo todos los elementos\033[0m\n\n");
        }
        else
        {
            printf("\033[0;31m[KO] - Perdió elementos con cmp especial\033[0m\n\n");
            g_tests_failed++;
        }
        
        free_list(lst);
    }

    // Test 14: Función de comparación personalizada (pares primero)
    {
        int arr[] = {1, 2, 3, 4, 5, 6};
        printf("Test Comparación personalizada (pares primero):\n");
        
        t_list *lst = create_list(arr, 6);
        printf("Input:  ");
        print_list(lst);
        printf("\n");
        
        lst = sort_list(lst, even_first);
        
        printf("Output: ");
        print_list(lst);
        printf("\n");
        
        // Debería tener: [2, 4, 6, 1, 3, 5]
        if (is_sorted(lst, even_first) && count_elements(lst) == 6)
        {
            printf("\033[0;32m[OK] - Comparación personalizada funciona\033[0m\n\n");
        }
        else
        {
            printf("\033[0;31m[KO] - Error con comparación personalizada\033[0m\n\n");
            g_tests_failed++;
        }
        
        free_list(lst);
    }

    // Test 15: Lista muy larga (rendimiento)
    {
        printf("Test Lista muy larga (100 elementos):\n");
        
        int *big_arr = malloc(100 * sizeof(int));
        for (int i = 0; i < 100; i++)
            big_arr[i] = 100 - i; // Orden inverso
        
        t_list *big_list = create_list(big_arr, 100);
        printf("Input:  Lista de 100 elementos en orden inverso\n");
        
        big_list = sort_list(big_list, ascending);
        
        printf("Output: ");
        if (is_sorted(big_list, ascending) && count_elements(big_list) == 100)
        {
            printf("Lista ordenada correctamente\n");
            printf("\033[0;32m[OK] - Lista larga procesada\033[0m\n\n");
        }
        else
        {
            printf("Error en lista larga\n");
            printf("\033[0;31m[KO] - Error con lista larga\033[0m\n\n");
            g_tests_failed++;
        }
        
        free(big_arr);
        free_list(big_list);
    }

    // Test 16: Números negativos
    {
        int arr[] = {-3, 1, -1, 4, -2, 0};
        run_test("Números negativos", arr, 6, ascending);
    }

    // Test 17: Valores extremos
    {
        int arr[] = {2147483647, -2147483648, 0, 1, -1};
        run_test("Valores extremos", arr, 5, ascending);
    }

    // Resumen
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32m✅ Todos los tests pasaron correctamente! Tu sort_list está lista para el examen.\033[0m\n");
    else
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);

    printf("\n\033[0;33m📝 CASOS CRÍTICOS CUBIERTOS:\033[0m\n");
    printf("✅ Lista NULL\n");
    printf("✅ Un solo elemento\n");
    printf("✅ Dos elementos (todos los casos)\n");
    printf("✅ Todos elementos iguales\n");
    printf("✅ Lista ya ordenada\n");
    printf("✅ Lista invertida\n");
    printf("✅ Función cmp especial\n");
    printf("✅ Lista muy larga (rendimiento)\n");
    printf("✅ Números negativos y extremos\n");
    printf("✅ Integridad de lista (no bucles)\n");
    
    printf("\n\033[0;31m⚠️  PUNTOS CRÍTICOS PARA EL EXAMEN:\033[0m\n");
    printf("• Lista NULL debe retornar NULL\n");
    printf("• Un elemento debe retornar el mismo elemento\n");
    printf("• Preservar todos los elementos (no perder nodos)\n");
    printf("• cmp retorna != 0 si están en orden correcto\n");
    printf("• No crear bucles en la lista\n");
    printf("• Manejar duplicados correctamente\n");

    printf("\n\033[0;36m💡 ALGORITMO BUBBLE SORT PARA LISTAS:\033[0m\n");
    printf("```c\n");
    printf("t_list *sort_list(t_list *lst, int (*cmp)(int, int))\n");
    printf("{\n");
    printf("    if (!lst) return NULL;\n");
    printf("    \n");
    printf("    int swapped = 1;\n");
    printf("    while (swapped)\n");
    printf("    {\n");
    printf("        swapped = 0;\n");
    printf("        t_list *current = lst;\n");
    printf("        \n");
    printf("        while (current->next)\n");
    printf("        {\n");
    printf("            if (!cmp(current->data, current->next->data))\n");
    printf("            {\n");
    printf("                // Intercambiar datos\n");
    printf("                int temp = current->data;\n");
    printf("                current->data = current->next->data;\n");
    printf("                current->next->data = temp;\n");
    printf("                swapped = 1;\n");
    printf("            }\n");
    printf("            current = current->next;\n");
    printf("        }\n");
    printf("    }\n");
    printf("    return lst;\n");
    printf("}\n");
    printf("```\n");

    return g_tests_failed;
}

/*
CASOS CRÍTICOS AÑADIDOS:

🔴 CRÍTICOS PARA EXAMEN:
1. Lista NULL - Debe retornar NULL sin crashear
2. Un solo elemento - Caso trivial básico
3. Dos elementos - Todos los subcasos posibles
4. Todos elementos iguales - No debe hacer bucles infinitos

🟡 IMPORTANTES:
5. Lista ya ordenada - Eficiencia
6. Lista invertida - Peor caso
7. Función cmp especial - Robustez
8. Lista muy larga - Rendimiento

🟢 ADICIONALES:
9. Números negativos y extremos
10. Integridad de lista - Detectar buclos
11. Preservación de elementos

ERRORES COMUNES QUE DETECTA:
- Crashear con lista NULL
- Perder nodos durante ordenamiento
- Crear bucles en la lista
- No manejar un solo elemento
- Problemas con función cmp especial
- No preservar duplicados

TIP EXAMEN:
- Bubble sort intercambiando DATOS (no punteros)
- Más simple y menos propenso a errores
- Verificar siempre lst != NULL antes de usar
*/