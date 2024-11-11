#include <stdio.h>

// Función para convertir un carácter a minúscula
char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return (c + ('a' - 'A')); // Convierte mayúscula a minúscula
    return (c); // Devuelve el carácter original si no es mayúscula
}

// Función para obtener el valor numérico de un dígito en una base dada
int get_digit(char c, int digits_in_base)
{
    int max_digit;

    // Determina el dígito máximo para la base dada
    if (digits_in_base <= 10)
        max_digit = digits_in_base - 1 + '0'; // Para bases hasta 10
    else
        max_digit = digits_in_base - 10 - 1 + 'a'; // Para bases 11-16

    // Convierte el carácter a su valor numérico
    if (c >= '0' && c <= '9' && c <= max_digit)
        return (c - '0'); // Para dígitos numéricos
    else if (c >= 'a' && c <= 'f' && c <= max_digit)
        return (10 + c - 'a'); // Para letras (bases > 10)
    else
        return (-1); // Carácter inválido para la base dada
}

int ft_atoi_base(const char *str, int str_base)
{
    int result = 0;
    int sign = 1;
    int digit;

    // Verificación de entradas válidas
    if (str == NULL || str_base < 2 || str_base > 16)
        return 0;

    // Manejo del signo negativo
    if (*str == '-')
    {
        sign = -1;
        ++str;
    }

    // Procesamiento de cada carácter de la cadena
    while (*str)
    {
        // Obtiene el valor numérico del dígito
        digit = get_digit(to_lower(*str), str_base);
        if (digit == -1)
            break; // Sale del bucle si encuentra un carácter inválido

        // Actualiza el resultado
        result = result * str_base + digit;
        ++str;
    }

    // Aplica el signo y devuelve el resultado
    return (result * sign);
}