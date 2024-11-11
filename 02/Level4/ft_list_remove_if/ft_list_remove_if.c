#include <stdlib.h> // Para usar free()
#include "ft_list.h" // Incluye la definición de t_list

void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
    t_list *current;
    t_list *temp;
    t_list *prev;

    if (begin_list == NULL || *begin_list == NULL)
        return;
    // Verifica si el puntero a la lista o la lista misma es NULL
    // Si es así, termina la función ya que no hay nada que procesar
    
    // Manejo especial para los primeros nodos que coincidan
    while (*begin_list && cmp((*begin_list)->data, data_ref) == 0)
    {
        // Si el primer nodo coincide, lo eliminamos
        temp = *begin_list;
        *begin_list = (*begin_list)->next;
        free(temp);
        // Continuamos con el siguiente nodo, que ahora es el primero
    }

    // Si la lista está vacía después de eliminar los primeros nodos, terminamos
    if (*begin_list == NULL)
        return;

    // Inicializamos current y prev para el recorrido principal
    current = (*begin_list)->next;
    prev = *begin_list;

    // Recorremos el resto de la lista
    while (current)
    {
        if (cmp(current->data, data_ref) == 0)
        {
            // Si encontramos una coincidencia, la eliminamos
            temp = current;
            prev->next = current->next; // Enlazamos el nodo anterior con el siguiente
            current = current->next; // Avanzamos al siguiente nodo
            free(temp); // Liberamos la memoria del nodo eliminado
        }
        else
        {
            // Si no hay coincidencia, simplemente avanzamos
            prev = current;
            current = current->next;
        }
    }
}