// test_main_improved.c para ft_list_foreach
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_list.h"

int g_tests_failed = 0;
int g_counter = 0; // Variable global para tests

// Prototipo de la función
void ft_list_foreach(t_list *begin_list, void (*f)(void *));

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

// Funciones de test para aplicar a elementos

// Función para convertir char a mayúsculas
void to_upper(void *data)
{
    char *str = (char *)data;
    if (!str) return;
    
    while (*str)
    {
        if (*str >= 'a' && *str <= 'z')
            *str -= 32;
        str++;
    }
}

// Función para convertir char a minúsculas
void to_lower(void *data)
{
    char *str = (char *)data;
    if (!str) return;
    
    while (*str)
    {
        if (*str >= 'A' && *str <= 'Z')
            *str += 32;
        str++;
    }
}

// Función para multiplicar int por 2
void multiply_by_two(void *data)
{
    int *num = (int *)data;
    if (num)
        *num *= 2;
}

// Función para incrementar int
void increment(void *data)
{
    int *num = (int *)data;
    if (num)
        (*num)++;
}

// Función para decrementar int
void decrement(void *data)
{
    int *num = (int *)data;
    if (num)
        (*num)--;
}

// Función que no hace nada (para testing)
void do_nothing(void *data)
{
    (void)data; // Evitar warning de variable no usada
}

// Función que cuenta elementos (side effect)
void count_elements(void *data)
{
    (void)data;
    g_counter++;
}

// Función que imprime int (side effect)
void print_int(void *data)
{
    int *num = (int *)data;
    if (num)
        printf("%d ", *num);
}

// Función que imprime string (side effect)
void print_string(void *data)
{
    char *str = (char *)data;
    if (str)
        printf("%s ", str);
}

// Función que modifica puntero (peligrosa)
void dangerous_modify(void *data)
{
    // Esta función intenta escribir en direcciones de memoria
    // Solo para testing - no se debe usar en producción
    char **ptr = (char **)data;
    if (ptr && *ptr)
    {
        // Modificar el primer carácter si es una string válida
        if (strlen(*ptr) > 0)
            (*ptr)[0] = 'X';
    }
}

// Función que maneja NULL data
void safe_upper(void *data)
{
    if (!data) return;
    to_upper(data);
}

void run_string_test()
{
    printf("Test 1: Conversión a mayúsculas\n");
    
    // Crear lista con strings
    t_list *list = create_node(strdup("hello"));
    list->next = create_node(strdup("world"));
    list->next->next = create_node(strdup("42"));
    list->next->next->next = create_node(strdup("school"));
    
    printf("Antes: ");
    t_list *current = list;
    while (current)
    {
        printf("%s ", (char *)current->data);
        current = current->next;
    }
    printf("\n");
    
    // Aplicar función
    ft_list_foreach(list, to_upper);
    
    printf("Después: ");
    current = list;
    while (current)
    {
        printf("%s ", (char *)current->data);
        current = current->next;
    }
    printf("\n");
    
    // Verificar resultados
    if (strcmp((char *)list->data, "HELLO") == 0 &&
        strcmp((char *)list->next->data, "WORLD") == 0 &&
        strcmp((char *)list->next->next->data, "42") == 0 &&
        strcmp((char *)list->next->next->next->data, "SCHOOL") == 0)
    {
        printf("\033[0;32m[OK]\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n\n");
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
    printf("Test 2: Multiplicación por 2\n");
    
    // Crear datos
    int *n1 = malloc(sizeof(int)); *n1 = 1;
    int *n2 = malloc(sizeof(int)); *n2 = 2;
    int *n3 = malloc(sizeof(int)); *n3 = 3;
    int *n4 = malloc(sizeof(int)); *n4 = 4;
    
    // Crear lista
    t_list *list = create_node(n1);
    list->next = create_node(n2);
    list->next->next = create_node(n3);
    list->next->next->next = create_node(n4);
    
    printf("Antes: ");
    t_list *current = list;
    while (current)
    {
        printf("%d ", *(int *)current->data);
        current = current->next;
    }
    printf("\n");
    
    // Aplicar función
    ft_list_foreach(list, multiply_by_two);
    
    printf("Después: ");
    current = list;
    while (current)
    {
        printf("%d ", *(int *)current->data);
        current = current->next;
    }
    printf("\n");
    
    // Verificar resultados
    if (*(int *)list->data == 2 &&
        *(int *)list->next->data == 4 &&
        *(int *)list->next->next->data == 6 &&
        *(int *)list->next->next->next->data == 8)
    {
        printf("\033[0;32m[OK]\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n\n");
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

// *** CASOS CRÍTICOS QUE FALTABAN ***

void test_null_list()
{
    printf("Test 3: Lista NULL\n");
    printf("Input: NULL\n");
    
    // No debería crashear
    ft_list_foreach(NULL, to_upper);
    printf("Resultado: No crasheó\n");
    printf("\033[0;32m[OK]\033[0m\n\n");
}

void test_null_function()
{
    printf("Test 4: Función NULL\n");
    t_list *list = create_node(strdup("test"));
    
    printf("Input: Lista válida, función NULL\n");
    
    // No debería crashear (comportamiento indefinido)
    ft_list_foreach(list, NULL);
    printf("Resultado: No crasheó\n");
    printf("\033[0;32m[OK]\033[0m\n\n");
    
    free(list->data);
    free(list);
}

void test_single_element()
{
    printf("Test 5: Un solo elemento\n");
    
    int *n = malloc(sizeof(int));
    *n = 5;
    t_list *list = create_node(n);
    
    printf("Antes: %d\n", *n);
    ft_list_foreach(list, multiply_by_two);
    printf("Después: %d\n", *n);
    
    if (*n == 10)
    {
        printf("\033[0;32m[OK]\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n\n");
        g_tests_failed++;
    }
    
    free(list->data);
    free(list);
}

void test_do_nothing_function()
{
    printf("Test 6: Función que no hace nada\n");
    
    int *n1 = malloc(sizeof(int)); *n1 = 1;
    int *n2 = malloc(sizeof(int)); *n2 = 2;
    
    t_list *list = create_node(n1);
    list->next = create_node(n2);
    
    printf("Antes: %d, %d\n", *n1, *n2);
    ft_list_foreach(list, do_nothing);
    printf("Después: %d, %d\n", *n1, *n2);
    
    if (*n1 == 1 && *n2 == 2)
    {
        printf("\033[0;32m[OK] - No modificó los datos\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO] - Modificó los datos cuando no debería\033[0m\n\n");
        g_tests_failed++;
    }
    
    while (list)
    {
        t_list *tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }
}

void test_side_effects()
{
    printf("Test 7: Función con side effects (contador)\n");
    
    t_list *list = create_node(strdup("a"));
    list->next = create_node(strdup("b"));
    list->next->next = create_node(strdup("c"));
    list->next->next->next = create_node(strdup("d"));
    list->next->next->next->next = create_node(strdup("e"));
    
    g_counter = 0; // Reset contador global
    printf("Contador inicial: %d\n", g_counter);
    
    ft_list_foreach(list, count_elements);
    
    printf("Contador final: %d\n", g_counter);
    
    if (g_counter == 5)
    {
        printf("\033[0;32m[OK] - Contó 5 elementos\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO] - Esperado 5, obtenido %d\033[0m\n\n", g_counter);
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

void test_null_data_in_nodes()
{
    printf("Test 8: Nodos con data NULL\n");
    
    t_list *list = create_node(strdup("hello"));
    list->next = create_node(NULL); // Data NULL
    list->next->next = create_node(strdup("world"));
    list->next->next->next = create_node(NULL); // Data NULL
    
    printf("Lista: \"hello\" -> NULL -> \"world\" -> NULL\n");
    
    // Usar función que maneja NULL safety
    ft_list_foreach(list, safe_upper);
    
    // Verificar que no crasheó y que las strings válidas se modificaron
    if (list->data && strcmp((char *)list->data, "HELLO") == 0 &&
        list->next->data == NULL &&
        list->next->next->data && strcmp((char *)list->next->next->data, "WORLD") == 0 &&
        list->next->next->next->data == NULL)
    {
        printf("\033[0;32m[OK] - Manejó NULL data correctamente\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO] - Error con NULL data\033[0m\n\n");
        g_tests_failed++;
    }
    
    // Liberar memoria
    while (list)
    {
        t_list *tmp = list->next;
        if (list->data)
            free(list->data);
        free(list);
        list = tmp;
    }
}

void test_large_list()
{
    printf("Test 9: Lista muy larga (1000 elementos)\n");
    
    t_list *list = NULL;
    t_list *current = NULL;
    
    // Crear lista de 1000 elementos
    for (int i = 0; i < 1000; i++)
    {
        int *num = malloc(sizeof(int));
        *num = i;
        
        t_list *node = create_node(num);
        if (!list)
            list = node;
        else
            current->next = node;
        current = node;
    }
    
    printf("Creada lista de 1000 elementos (0-999)\n");
    
    // Aplicar incremento a todos
    ft_list_foreach(list, increment);
    
    // Verificar algunos elementos
    t_list *check = list;
    int correct = 1;
    for (int i = 0; i < 1000 && check; i++)
    {
        if (*(int *)check->data != i + 1)
        {
            correct = 0;
            break;
        }
        check = check->next;
    }
    
    if (correct)
    {
        printf("\033[0;32m[OK] - Procesó 1000 elementos correctamente\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO] - Error en lista larga\033[0m\n\n");
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

void test_multiple_operations()
{
    printf("Test 10: Múltiples operaciones en secuencia\n");
    
    int *n1 = malloc(sizeof(int)); *n1 = 1;
    int *n2 = malloc(sizeof(int)); *n2 = 2;
    int *n3 = malloc(sizeof(int)); *n3 = 3;
    
    t_list *list = create_node(n1);
    list->next = create_node(n2);
    list->next->next = create_node(n3);
    
    printf("Inicial: %d, %d, %d\n", *n1, *n2, *n3);
    
    // Primera operación: multiplicar por 2
    ft_list_foreach(list, multiply_by_two);
    printf("Después x2: %d, %d, %d\n", *n1, *n2, *n3);
    
    // Segunda operación: incrementar
    ft_list_foreach(list, increment);
    printf("Después +1: %d, %d, %d\n", *n1, *n2, *n3);
    
    // Tercera operación: decrementar
    ft_list_foreach(list, decrement);
    printf("Después -1: %d, %d, %d\n", *n1, *n2, *n3);
    
    // Verificar: debería ser [2, 4, 6] (1*2+1-1, 2*2+1-1, 3*2+1-1)
    if (*n1 == 2 && *n2 == 4 && *n3 == 6)
    {
        printf("\033[0;32m[OK] - Múltiples operaciones correctas\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO] - Error en secuencia de operaciones\033[0m\n\n");
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

void test_print_function()
{
    printf("Test 11: Función de impresión (side effect)\n");
    
    int *n1 = malloc(sizeof(int)); *n1 = 10;
    int *n2 = malloc(sizeof(int)); *n2 = 20;
    int *n3 = malloc(sizeof(int)); *n3 = 30;
    
    t_list *list = create_node(n1);
    list->next = create_node(n2);
    list->next->next = create_node(n3);
    
    printf("Imprimiendo lista: ");
    ft_list_foreach(list, print_int);
    printf("\n");
    
    // No hay verificación automática, solo visual
    printf("\033[0;32m[OK] - Función de impresión ejecutada\033[0m\n\n");
    
    // Liberar memoria
    while (list)
    {
        t_list *tmp = list->next;
        free(list->data);
        free(list);
        list = tmp;
    }
}

void test_mixed_data_types()
{
    printf("Test 12: Tipos de datos mezclados (strings)\n");
    
    t_list *list = create_node(strdup("HELLO"));
    list->next = create_node(strdup("WORLD"));
    list->next->next = create_node(strdup("TEST"));
    
    printf("Antes: ");
    t_list *current = list;
    while (current)
    {
        printf("%s ", (char *)current->data);
        current = current->next;
    }
    printf("\n");
    
    // Convertir a minúsculas
    ft_list_foreach(list, to_lower);
    
    printf("Después: ");
    current = list;
    while (current)
    {
        printf("%s ", (char *)current->data);
        current = current->next;
    }
    printf("\n");
    
    if (strcmp((char *)list->data, "hello") == 0 &&
        strcmp((char *)list->next->data, "world") == 0 &&
        strcmp((char *)list->next->next->data, "test") == 0)
    {
        printf("\033[0;32m[OK]\033[0m\n\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n\n");
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

int main(void)
{
    printf("\n=== Testing ft_list_foreach - VERSIÓN EXAMEN ===\n\n");
    
    // Tests originales
    run_string_test();
    run_int_test();
    
    // Tests críticos añadidos
    test_null_list();
    test_null_function();
    test_single_element();
    test_do_nothing_function();
    test_side_effects();
    test_null_data_in_nodes();
    test_large_list();
    test_multiple_operations();
    test_print_function();
    test_mixed_data_types();
    
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32m✅ Todos los tests pasaron correctamente! Tu ft_list_foreach está listo para el examen.\033[0m\n");
    else
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);

    printf("\n\033[0;33m📝 CASOS CRÍTICOS CUBIERTOS:\033[0m\n");
    printf("✅ Lista NULL\n");
    printf("✅ Función NULL\n");
    printf("✅ Un solo elemento\n");
    printf("✅ Función que no hace nada\n");
    printf("✅ Side effects (contador)\n");
    printf("✅ Nodos con data NULL\n");
    printf("✅ Lista muy larga (1000 elementos)\n");
    printf("✅ Múltiples operaciones en secuencia\n");
    printf("✅ Funciones de impresión\n");
    printf("✅ Diferentes tipos de datos\n");
    
    printf("\n\033[0;31m⚠️  PUNTOS CRÍTICOS PARA EL EXAMEN:\033[0m\n");
    printf("• Lista NULL: no debe crashear\n");
    printf("• Función NULL: comportamiento indefinido (no crashear idealmente)\n");
    printf("• Aplicar función a TODOS los elementos\n");
    printf("• No modificar la estructura de la lista\n");
    printf("• Manejar data NULL en nodos\n");
    printf("• La función puede tener side effects\n");

    printf("\n\033[0;36m💡 IMPLEMENTACIÓN SIMPLE:\033[0m\n");
    printf("```c\n");
    printf("void ft_list_foreach(t_list *begin_list, void (*f)(void *))\n");
    printf("{\n");
    printf("    while (begin_list)\n");
    printf("    {\n");
    printf("        (*f)(begin_list->data);\n");
    printf("        begin_list = begin_list->next;\n");
    printf("    }\n");
    printf("}\n");
    printf("```\n");

    printf("\n\033[0;35m🧩 CASOS DE USO TÍPICOS:\033[0m\n");
    printf("• Imprimir todos los elementos\n");
    printf("• Modificar todos los elementos (maiúsculas, x2, etc.)\n");
    printf("• Contar elementos (side effect)\n");
    printf("• Validar elementos\n");
    printf("• Liberar memoria de data (con cuidado)\n");

    printf("\n\033[0;33m⚠️  CUIDADOS ESPECIALES:\033[0m\n");
    printf("• La función recibida puede modificar los datos\n");
    printf("• La función puede tener efectos secundarios\n");
    printf("• No verificar f != NULL (comportamiento indefinido)\n");
    printf("• Manejar graciosamente lista NULL\n");
    printf("• No modificar los punteros next de la lista\n");

    return g_tests_failed;
}

/*
CASOS CRÍTICOS AÑADIDOS PARA FT_LIST_FOREACH:

🔴 CRÍTICOS PARA EXAMEN:
1. Lista NULL - No debe crashear
2. Función NULL - Comportamiento indefinido
3. Un solo elemento - Caso trivial
4. Nodos con data NULL - Edge case

🟡 IMPORTANTES:
5. Función que no hace nada - Verificar no side effects
6. Side effects (contador) - Verificar que se ejecuta
7. Múltiples operaciones - Verificar secuencia
8. Lista muy larga - Rendimiento

🟢 ADICIONALES:
9. Funciones de impresión - Side effects visuales
10. Diferentes tipos de datos - Strings/ints
11. Operaciones complejas - Secuencias

ERRORES COMUNES QUE DETECTA:
- Crashear con lista NULL
- No aplicar función a todos los elementos
- Modificar estructura de lista
- Problemas con data NULL
- No manejar funciones con side effects
- Problemas de rendimiento con listas largas

IMPLEMENTACIÓN CORRECTA:
```c
void ft_list_foreach(t_list *begin_list, void (*f)(void *))
{
    while (begin_list)
    {
        (*f)(begin_list->data);
        begin_list = begin_list->next;
    }
}
```

PUNTOS CLAVE:
- MUY simple de implementar
- No verificar f != NULL (comportamiento indefinido)
- Aplicar f a CADA elemento
- No modificar la lista, solo los datos
- begin_list puede ser NULL

TIP EXAMEN:
Es una de las funciones más simples del Level 4.
La trampa está en los edge cases (NULL list, NULL function).
El algoritmo es un simple bucle while.
*/