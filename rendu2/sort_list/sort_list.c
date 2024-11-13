/*
** DESCRIPCIÓN: Ordena una lista enlazada usando una función de comparación.
**
** PARÁMETROS:
** - lst: Puntero al primer elemento de la lista
** - cmp: Función que compara dos enteros y determina su orden
**
** RETORNO:
** - Puntero al primer elemento de la lista ordenada
**
** EJEMPLO:
** Con ascending(a,b) = (a <= b):
** Input:  4 -> 2 -> 7 -> 1 -> 3
** Output: 1 -> 2 -> 3 -> 4 -> 7
*/

/* 
** CONCEPTOS CLAVE:
** 1. Lista Enlazada:
**    - Estructura con un valor y un puntero al siguiente nodo
**    - Cada nodo está conectado al siguiente
**    - El último nodo apunta a NULL
**
** 2. Puntero a Función:
**    - int (*cmp)(int, int): Es un puntero a una función que:
**      * Toma dos enteros como parámetros
**      * Retorna un entero (0 si hay que intercambiar)
**
** 3. Algoritmo Bubble Sort:
**    - Compara elementos adyacentes
**    - Intercambia si no están en orden
**    - Repite hasta que no haya intercambios
*/

#include "list.h"

/* La estructura t_list está definida así en list.h:
typedef struct s_list
{
    int             data;
    struct s_list   *next;
} t_list;
*/

t_list  *sort_list(t_list *lst, int (*cmp)(int, int))
{
    t_list  *current;
    int     swapped;
    int     temp;

    // Si la lista está vacía o tiene un solo elemento
    if (!lst || !lst->next)
        return (lst);

    // Bubble sort
    swapped = 1;
    while (swapped)
    {
        swapped = 0;
        current = lst;
        
        // Recorremos la lista
        while (current->next)
        {
            // Si el orden no es correcto (cmp retorna 0)
            if (!cmp(current->data, current->next->data))
            {
                // Intercambiamos los valores
                temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = 1;
            }
            current = current->next;
        }
    }
    return (lst);
}