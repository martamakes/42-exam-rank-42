/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camel_to_snake.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42school.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:15:32 by mvigara-          #+#    #+#             */
/*   Updated: 2024/11/12 18:15:33 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/*
** Principales conceptos del ejercicio:
** 
** 1. Iteración de strings:
**    - Recorremos el string caracter a caracter
**    - No necesitamos malloc porque podemos escribir directamente
**
** 2. Detección de mayúsculas:
**    - Usamos el rango ASCII: 'A'(65) a 'Z'(90)
**    - Para convertir a minúscula sumamos 32 ('a'-'A')
**    - TODAS las letras deben estar en minúsculas en snake_case
**
** 3. Write syscall:
**    - Escribimos caracter a caracter
**    - Añadimos '_' antes de cada mayúscula (excepto al inicio)
*/

int main(int argc, char **argv)
{
    int i;
    char c;

    // Si no hay argumentos, solo imprime un salto de línea
    if (argc != 2)
    {
        write(1, "\n", 1);
        return (0);
    }

    i = 0;
    // Mientras no lleguemos al final de la cadena
    while (argv[1][i])
    {
        if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
        {
            // Si no es el primer carácter, añadimos el guión bajo
            if (i > 0)
                write(1, "_", 1);
            // Convertimos la mayúscula a minúscula
            c = argv[1][i] + ('a' - 'A');
            write(1, &c, 1);
        }
        else
        {
            // Si no es mayúscula, escribimos el caracter tal cual
            write(1, &argv[1][i], 1);
        }
        i++;
    }
    write(1, "\n", 1);
    return (0);
}