/* 
** CONCEPTOS CLAVE:
**
** 1. Asignación dinámica de memoria:
**    - malloc: reserva memoria en el heap
**    - El tamaño debe incluir el '\0' final
**    - Hay que verificar si malloc fue exitoso
**    
** 2. Manejo de strings:
**    - Calcular longitud para saber cuánta memoria pedir
**    - Copiar caracter por caracter incluyendo el '\0'
**    - No usar funciones de string.h (strlen, strcpy)
**    
** 3. Manejo de punteros:
**    - Puntero original no debe modificarse
**    - Devolver puntero a la nueva memoria
**    - Manejar caso de error (NULL)
*/

#include <stdlib.h>

// Función auxiliar para calcular longitud de string
static int  ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

char    *ft_strdup(char *src)
{
    char    *dup;
    int     i;
    int     len;

    // Obtener longitud de la cadena original
    len = ft_strlen(src);
    
    // Reservar memoria para la nueva cadena (len + 1 para el '\0')
    dup = (char *)malloc(sizeof(char) * (len + 1));
    
    // Verificar si malloc fue exitoso
    if (!dup)
        return (NULL);
    
    // Copiar caracteres
    i = 0;
    while (src[i])
    {
        dup[i] = src[i];
        i++;
    }
    
    // Añadir el terminador nulo
    dup[i] = '\0';
    
    return (dup);
}