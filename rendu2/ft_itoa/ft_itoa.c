/* ************************************************************************** */
/*                                   ENUNCIADO                                  */
/* Escribe una función que tome un int y lo convierta a un string terminado    */
/* en null. La función devuelve el resultado en un array de chars que debes    */
/* alocar con malloc.                                                          */
/* ************************************************************************** */

#include <stdlib.h>

/*
** CONCEPTOS CLAVE:
** 1. Manejo de memoria dinámica (malloc)
** 2. Conversión de números a strings
** 3. Manejo de casos especiales:
**    - Números negativos
**    - Número más pequeño posible (INT_MIN)
**    - Zero
** 4. Cálculo del tamaño necesario para el string
*/

// Calcula el número de dígitos que tendrá el número
static int	get_len(int n)
{
    int	len;

    len = (n <= 0) ? 1 : 0; // Para el signo o para el 0
    while (n != 0)
    {
        n = n / 10;
        len++;
    }
    return (len);
}

char	*ft_itoa(int nbr)
{
    char	*str;
    int		len;
    long	n; // Usamos long para manejar INT_MIN
    
    n = nbr;
    len = get_len(n);
    
    // Reservar memoria para los dígitos + posible signo + null terminator
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
        
    str[len] = '\0'; // Añadir null terminator
    
    // Manejar caso especial del 0
    if (n == 0)
    {
        str[0] = '0';
        return (str);
    }
    
    // Si es negativo, convertir a positivo y poner el signo
    if (n < 0)
    {
        str[0] = '-';
        n = -n;
    }
    
    // Convertir dígitos de derecha a izquierda
    while (n > 0)
    {
        len--;
        str[len] = (n % 10) + '0';
        n = n / 10;
    }
    
    return (str);
}