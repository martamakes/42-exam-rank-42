#include "ft_list_foreach.h"

// Función que aplica una función dada a cada elemento de una lista enlazada
// Parámetros:
//   - begin_list: Puntero al primer elemento de la lista
//   - f: Puntero a la función que se aplicará a cada elemento
void ft_list_foreach(t_list *begin_list, void (*f)(void *))
{
    // Itera a través de la lista mientras haya elementos
    while (begin_list)
    {
        // Aplica la función f al dato del elemento actual
        // Esto cumple con el requisito del enunciado:
        // "La función apuntada por f se usará así: (*f)(list_ptr->data);"
        (*f)(begin_list->data);

        // Avanza al siguiente elemento de la lista
        // Esto permite recorrer toda la lista, aplicando f a cada elemento
        begin_list = begin_list->next;
    }
}