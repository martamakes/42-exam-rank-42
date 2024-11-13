/*
** sort_int_tab.c
**
** CONCEPTOS CLAVE:
**
** 1. Selection Sort:
**    - Algoritmo de ordenación simple y eficiente para arrays pequeños
**    - Funciona encontrando el elemento más pequeño y colocándolo al principio
**    - Complejidad: O(n²)
**    - Ventaja: Número mínimo de intercambios (máximo n-1)
**
** 2. In-place sorting:
**    - El ordenamiento se hace directamente en el array original
**    - No se necesita memoria adicional para arrays auxiliares
**
** 3. Preservación de duplicados:
**    - El algoritmo mantiene el orden relativo de elementos iguales
**    - Esto lo hace un algoritmo "estable"
*/

void    sort_int_tab(int *tab, unsigned int size)
{
    unsigned int    i;
    unsigned int    j;
    unsigned int    min_idx;
    int             temp;

    // Iteramos sobre todo el array excepto el último elemento
    i = 0;
    while (i < size - 1)
    {
        // Asumimos que el elemento actual es el mínimo
        min_idx = i;
        
        // Buscamos el elemento más pequeño en el resto del array
        j = i + 1;
        while (j < size)
        {
            if (tab[j] < tab[min_idx])
                min_idx = j;
            j++;
        }

        // Si encontramos un elemento más pequeño, lo intercambiamos
        if (min_idx != i)
        {
            temp = tab[i];
            tab[i] = tab[min_idx];
            tab[min_idx] = temp;
        }
        i++;
    }
}