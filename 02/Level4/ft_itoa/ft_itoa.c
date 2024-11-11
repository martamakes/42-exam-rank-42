#include <stdlib.h>

// Función auxiliar para contar el número de dígitos en un número
static int count_digits(int n)
{
    int count;

    // Inicializar el contador. Si n es 0 o negativo, empezamos con 1 (para el dígito 0 o el signo -)
    count = (n <= 0) ? 1 : 0;

    // Contar dígitos dividiendo repetidamente por 10
    while (n != 0)
    {
        n /= 10;
        count++;
    }
    return (count);
}

char *ft_itoa(int nbr)
{
    char    *str;       // Puntero para la cadena resultante
    int     len;        // Longitud de la cadena
    long    n;          // Usamos long para manejar INT_MIN
    int     sign;       // Flag para el signo (1 si es negativo, 0 si es positivo)

    // Convertir nbr a long para manejar INT_MIN
    n = nbr;

    // Determinar si el número es negativo
    sign = (n < 0) ? 1 : 0;

    // Si es negativo, convertirlo a positivo para el proceso
    n = (n < 0) ? -n : n;

    // Calcular la longitud de la cadena necesaria
    len = count_digits(nbr);

    // Asignar memoria para la cadena (longitud + posible signo negativo + null terminator)
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);  // Retornar NULL si falla la asignación de memoria

    // Añadir null terminator al final de la cadena
    str[len] = '\0';

    // Convertir dígitos a caracteres, de derecha a izquierda
    while (len > sign)
    {
        str[--len] = (n % 10) + '0';  // Convertir dígito a carácter y colocarlo
        n /= 10;                      // Pasar al siguiente dígito
    }

    // Si el número era negativo, añadir el signo menos al principio
    if (sign)
        str[0] = '-';

    return (str);
}