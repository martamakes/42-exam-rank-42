/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42school.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:18:58 by mvigara-          #+#    #+#             */
/*   Updated: 2024/11/12 18:19:01 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/*
** Conceptos clave del ejercicio:
** 1. Manejo de strings:
**    - Recorrer la cadena desde el final hacia el principio
**    - Identificar delimitadores (espacios y tabs)
**
** 2. Identificación de la última palabra:
**    - Saltar espacios finales
**    - Encontrar el final de la última palabra
**    - Encontrar el inicio de la última palabra
**
** 3. Manejo de casos especiales:
**    - Cadenas solo con espacios
**    - Múltiples espacios entre palabras
**    - Cadena sin espacios (una sola palabra)
*/

int main(int argc, char **argv)
{
    int start;
    int end;
    
    if (argc != 2)
    {
        write(1, "\n", 1);
        return (0);
    }
    
    // Encontrar el último carácter no espacio
    end = 0;
    while (argv[1][end])
        end++;
    end--;
    while (end >= 0 && (argv[1][end] == ' ' || argv[1][end] == '\t'))
        end--;
        
    if (end < 0)
    {
        write(1, "\n", 1);
        return (0);
    }
    
    // Encontrar el inicio de la última palabra
    start = end;
    while (start >= 0 && argv[1][start] != ' ' && argv[1][start] != '\t')
        start--;
    start++;
    
    // Escribir la última palabra
    while (start <= end)
        write(1, &argv[1][start++], 1);
    write(1, "\n", 1);
    
    return (0);
}
