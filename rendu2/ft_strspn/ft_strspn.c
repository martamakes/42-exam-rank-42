/* 
** CONCEPTOS CLAVE:
**
** 1. Búsqueda de caracteres:
**    - Debemos verificar si cada carácter de s está en accept
**    - Continuar hasta encontrar un carácter que no está en accept
**    - Retornar la cantidad de caracteres encontrados
**    
** 2. Manejo de strings:
**    - Recorrer s carácter por carácter
**    - Para cada carácter, buscar en toda la cadena accept
**    - No modificar las cadenas originales (const)
**    
** 3. size_t:
**    - Tipo de dato sin signo usado para tamaños
**    - Garantiza que nunca será negativo
**    - Es el tipo de retorno estándar para funciones de longitud
*/

#include <stddef.h>  // Para size_t

// Función auxiliar para buscar un carácter en una cadena
static int is_in_accept(char c, const char *accept)
{
    while (*accept)
    {
        if (c == *accept)
            return (1);
        accept++;
    }
    return (0);
}

size_t  ft_strspn(const char *s, const char *accept)
{
    size_t  count;
    
    count = 0;
    // Mientras el carácter actual esté en accept
    while (s[count] && is_in_accept(s[count], accept))
        count++;
    
    return (count);
}