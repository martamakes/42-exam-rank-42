/*
** KEY POINTS PARA ESTUDIAR:
**
** 1. Conversión de bases:
**    - Cada dígito se multiplica por una potencia de la base
**    - Ejemplo base 16: "2A" = (2 * 16^1) + (10 * 16^0) = 42
**
** 2. Caracteres hexadecimales:
**    - Dígitos: '0'-'9' = 0-9
**    - Letras: 'a'-'f' o 'A'-'F' = 10-15
**
** 3. Validación de base:
**    - Base debe ser entre 2 y 16
**    - Caracteres deben ser válidos para la base
**
** 4. Manejo de signo:
**    - Solo considerar '-' al inicio
**    - Multiplicar resultado por -1 si es negativo
**
** 5. Corrección importante:
**    - Al encontrar un carácter inválido, debemos devolver el número
**      formado hasta ese punto
*/

// Función auxiliar para convertir un carácter a su valor numérico

int char_to_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');
    if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    return (-1);
}

int ft_atoi_base(const char *str, int str_base)
{
    int result;
    int sign;
    int i;

    if (str_base < 2 || str_base > 16)
        return (0);

    result = 0;
    sign = 1;
    i = 0;

    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }

    while (str[i])
    {
        int digit = char_to_digit(str[i]);
        if (digit >= 0 && digit < str_base)
            result = (result * str_base) + digit;
        i++;
    }

    return (result * sign);
}