#include <unistd.h>

/*
Programa: hidenp

Este programa toma dos cadenas como argumentos y muestra 1 seguido de un salto de línea
si la primera cadena está oculta en la segunda, de lo contrario muestra 0 seguido de
un salto de línea.

Una cadena s1 está oculta en s2 si es posible encontrar cada carácter de s1 en s2,
en el mismo orden en que aparecen en s1. Además, la cadena vacía se considera oculta
en cualquier cadena.

Si el número de parámetros no es 2, el programa muestra solo un salto de línea.
*/

void ft_putchar(char c)
{
    write(1, &c, 1);
}

int is_hidden(char *s1, char *s2)
{
    while (*s1 && *s2)
    {
        if (*s1 == *s2)
            s1++;
        s2++;
    }
    return (*s1 == '\0');
}

int main(int argc, char **argv)
{
    if (argc == 3)
    {
        if (is_hidden(argv[1], argv[2]))
            ft_putchar('1');
        else
            ft_putchar('0');
    }
    ft_putchar('\n');
    return (0);
}