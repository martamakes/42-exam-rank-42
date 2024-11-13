/*
** ft_strcmp.c
**
** CONCEPTOS CLAVE:
**
** 1. strcmp:
**    - Compara dos strings carácter por carácter
**    - Devuelve la diferencia entre los primeros caracteres diferentes
**    - Si los strings son iguales, devuelve 0
**
** 2. Valores de retorno:
**    - > 0 si s1 es mayor que s2
**    - < 0 si s1 es menor que s2
**    - = 0 si s1 es igual a s2
**
** 3. Comparación ASCII:
**    - Cada carácter tiene un valor numérico según la tabla ASCII
**    - 'a' = 97, 'b' = 98, etc.
**    - La comparación se hace restando estos valores
*/

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    // Mientras no lleguemos al final de ningún string
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        // Si encontramos caracteres diferentes, retornamos su diferencia
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    // Al salir del bucle, retornamos la diferencia de los caracteres finales
    // (que puede ser 0 si ambos terminan a la vez)
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}