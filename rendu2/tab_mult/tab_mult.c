/*
** tab_mult.c
**
** CONCEPTOS CLAVE:
**
** 1. Argumentos de línea de comandos:
**    - argc: número de argumentos
**    - argv: array de strings con los argumentos
**
** 2. write:
**    - Función de bajo nivel para escribir a stdout
**    - write(fd, buffer, count)
**
** 3. Manejo de números:
**    - Conversión de string a int (ft_atoi)
**    - Conversión de int a string para mostrar
*/

#include <unistd.h>

// Función para escribir un caracter
void    ft_putchar(char c)
{
    write(1, &c, 1);
}

// Función para escribir un número
void    ft_putnbr(int nb)
{
    if (nb >= 10)
        ft_putnbr(nb / 10);
    ft_putchar(nb % 10 + '0');
}

// Función para convertir string a número
int     ft_atoi(char *str)
{
    int result;
    int i;

    result = 0;
    i = 0;
    // Saltamos espacios iniciales si los hay
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    // Procesamos cada dígito
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result);
}

int main(int argc, char **argv)
{
    int num;
    int i;

    // Si no hay argumentos, solo imprimimos newline
    if (argc != 2)
    {
        write(1, "\n", 1);
        return (0);
    }

    // Convertimos el argumento a número
    num = ft_atoi(argv[1]);
    
    // Imprimimos la tabla de multiplicar
    i = 1;
    while (i <= 9)
    {
        ft_putnbr(i);
        write(1, " x ", 3);
        ft_putnbr(num);
        write(1, " = ", 3);
        ft_putnbr(i * num);
        write(1, "\n", 1);
        i++;
    }
    return (0);
}