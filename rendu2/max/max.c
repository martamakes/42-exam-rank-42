/*
** Conceptos clave:
** 1. Manejo de arrays:
**    - Recorrido de elementos
**    - Comparación de números
**
** 2. Edge cases:
**    - Array vacío (len = 0)
**    - Números negativos
**    - Números repetidos
*/

int max(int* tab, unsigned int len)
{
    // Si el array está vacío, retornar 0
    if (len == 0 || !tab)
        return (0);
        
    // Inicializar el máximo con el primer elemento
    int max_value = tab[0];
    unsigned int i = 1;
    
    // Recorrer el array comparando cada elemento
    while (i < len)
    {
        if (tab[i] > max_value)
            max_value = tab[i];
        i++;
    }
    
    return (max_value);
}