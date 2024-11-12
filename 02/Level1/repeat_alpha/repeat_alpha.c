/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repeat_alpha.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: 42 Madrid <marinjim@student.42madrid.com>  +#+  +:+       +#+      */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 10:00:00 by marinjim          #+#    #+#            */
/*   Updated: 2024/02/13 10:00:00 by marinjim         ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/*
* Función que determina cuántas veces debe repetirse un carácter
* @param c: el carácter a analizar
* @return: número de veces que debe repetirse (1 para no alfabéticos)
*/
static int	get_repeat_count(char c)
{
	// Si es minúscula, retornamos su posición en el alfabeto (a=1, b=2, etc)
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 1);
	// Si es mayúscula, hacemos lo mismo pero con el alfabeto mayúsculo
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 1);
	// Para cualquier otro carácter, retornamos 1
	return (1);
}

/*
* Función que escribe un carácter n veces
* @param c: carácter a escribir
* @param n: número de veces a escribir
*/
static void	write_char_n_times(char c, int n)
{
	while (n > 0)
	{
		write(1, &c, 1);
		n--;
	}
}

int	main(int argc, char **argv)
{
	int	i;
	int	repeat_count;

	// Si no hay exactamente un argumento, solo imprimimos newline
	if (argc != 2)
	{
		write(1, "\n", 1);
		return (0);
	}

	i = 0;
	// Mientras no lleguemos al final de la string
	while (argv[1][i])
	{
		// Obtenemos cuántas veces hay que repetir el carácter actual
		repeat_count = get_repeat_count(argv[1][i]);
		// Escribimos el carácter las veces necesarias
		write_char_n_times(argv[1][i], repeat_count);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}