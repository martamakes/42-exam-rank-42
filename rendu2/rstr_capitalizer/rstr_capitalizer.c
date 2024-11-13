/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rstr_capitalizer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42school.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 00:52:46 by mvigara-          #+#    #+#             */
/*   Updated: 2024/11/13 00:53:03 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

// Función para escribir un caracter
void    ft_putchar(char c)
{
    write(1, &c, 1);
}

// Función para verificar si un caracter es una letra
int     is_letter(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// Función para verificar si un caracter es un espacio o tab
int     is_space(char c)
{
    return (c == ' ' || c == '\t');
}

// Función para convertir a minúscula
char    to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return (c + 32);
    return (c);
}

// Función para convertir a mayúscula
char    to_upper(char c)
{
    if (c >= 'a' && c <= 'z')
        return (c - 32);
    return (c);
}

// Función que procesa cada string
void    process_string(char *str)
{
    int     i;
    int     is_last_letter;

    i = 0;
    while (str[i])
    {
        // Para cada caracter, necesitamos saber si es la última letra
        // de una palabra mirando el siguiente caracter
        is_last_letter = is_letter(str[i]) && 
            (str[i + 1] == '\0' || is_space(str[i + 1]) || !is_letter(str[i + 1]));
        
        // Si es la última letra de una palabra, la ponemos en mayúscula
        if (is_last_letter)
            ft_putchar(to_upper(str[i]));
        // Si no, la ponemos en minúscula
        else
            ft_putchar(to_lower(str[i]));
        i++;
    }
}

int     main(int argc, char **argv)
{
    int i;

    // Si no hay argumentos, solo imprimimos newline
    if (argc == 1)
    {
        ft_putchar('\n');
        return (0);
    }

    // Procesamos cada argumento
    i = 1;
    while (i < argc)
    {
        process_string(argv[i]);
        ft_putchar('\n');
        i++;
    }
    return (0);
}