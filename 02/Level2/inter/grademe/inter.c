// inter.c
#include <unistd.h>

void    ft_putchar(char c)
{
    write(1, &c, 1);
}

// Verifica si un caracter ya fue usado antes en el primer string
int     char_was_used(char *str, int pos, char c)
{
    int i;

    i = 0;
    while (i < pos)
    {
        if (str[i] == c)
            return (1);
        i++;
    }
    return (0);
}

// Verifica si un caracter existe en el segundo string
int     char_exists_in_str2(char *str, char c)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (1);
        i++;
    }
    return (0);
}

int     main(int argc, char **argv)
{
    int i;

    // Verificar número de argumentos
    if (argc != 3)
    {
        ft_putchar('\n');
        return (0);
    }

    // Recorrer el primer string
    i = 0;
    while (argv[1][i])
    {
        // Si el caracter existe en str2 y no fue usado antes en str1
        if (!char_was_used(argv[1], i, argv[1][i]) && 
            char_exists_in_str2(argv[2], argv[1][i]))
            ft_putchar(argv[1][i]);
        i++;
    }
    ft_putchar('\n');
    return (0);
}