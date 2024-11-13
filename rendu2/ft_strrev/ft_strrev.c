/*
** KEY POINTS PARA ESTUDIAR:
**
** 1. Inversión in-place:
**    - No se debe usar memoria adicional para una nueva string
**    - Se modifica la string original directamente
**
** 2. Técnica de dos punteros:
**    - start: comienza al inicio de la string
**    - end: comienza al final de la string
**    - Se van intercambiando caracteres hasta que se crucen los punteros
**
** 3. Longitud de string:
**    - Se debe encontrar el final de la string ('\0')
**    - No podemos usar strlen, debemos implementarlo
**
** 4. Intercambio de valores:
**    - Se usa una variable temporal para el intercambio
**    - También se podría hacer con operaciones XOR para evitar la var temporal
*/
#include <unistd.h>
char    *ft_strrev(char *str)
{
    int     i;
    int     len;
    char    temp;

    // Si la string está vacía, retornamos
    if (!str)
        return (NULL);

    // Calculamos la longitud de la string
    len = 0;
    while (str[len])
        len++;

    // Invertimos la string usando dos índices
    i = 0;
    while (i < len / 2)
    {
        // Intercambiamos los caracteres
        temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
        i++;
    }

    return (str);
}