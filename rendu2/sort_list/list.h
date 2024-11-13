#ifndef LIST_H
# define LIST_H

/*
** Estructura de la lista enlazada:
** - data: valor entero almacenado en el nodo
** - next: puntero al siguiente nodo de la lista
*/
typedef struct s_list
{
    int             data;   // Valor almacenado en el nodo
    struct s_list   *next;  // Puntero al siguiente nodo
}   t_list;

/*
** Prototipo de la función sort_list que debe implementar el estudiante:
** - lst: puntero al primer elemento de la lista
** - cmp: puntero a la función de comparación
**   * La función cmp toma dos enteros como parámetros
**   * Retorna != 0 si están en orden correcto
**   * Retorna 0 si no están en orden correcto
*/
t_list  *sort_list(t_list *lst, int (*cmp)(int, int));

#endif