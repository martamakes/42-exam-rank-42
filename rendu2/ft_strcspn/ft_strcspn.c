/* ************************************************************************** */
/*                                   ENUNCIADO                                  */
/* Reproduce exactamente el comportamiento de la función strcspn.              */
/* Esta función devuelve la longitud del segmento inicial de s que NO         */
/* contiene ninguno de los caracteres en reject.                              */
/* ************************************************************************** */

#include <stddef.h> // Para size_t

/*
** CONCEPTOS CLAVE:
** 1. Manejo de strings en C
** 2. Búsqueda de caracteres en string
** 3. Cálculo de longitud de segmentos
** 4. Retorno de size_t (tipo sin signo)
*/

size_t  ft_strcspn(const char *s, const char *reject)
{
    size_t i;
    size_t j;

    // Recorremos el string s
    i = 0;
    while (s[i])
    {
        // Para cada carácter de s, buscamos si está en reject
        j = 0;
        while (reject[j])
        {
            if (s[i] == reject[j])
                return (i);  // Si encontramos coincidencia, devolvemos la posición
            j++;
        }
        i++;
    }
    return (i);  // Si no hay coincidencias, devolvemos la longitud total de s
}