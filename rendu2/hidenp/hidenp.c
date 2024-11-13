/* 
** CONCEPTOS CLAVE:
**
** 1. Búsqueda secuencial:
**    - Debemos encontrar los caracteres en orden
**    - No importa si hay otros caracteres entre medio
**    - Una vez que usamos una posición de s2, no podemos volver atrás
**    
** 2. Casos especiales:
**    - String vacío siempre está escondido (retorna 1)
**    - Si no hay 2 argumentos, solo imprime newline
**    
** 3. Manejo de cadenas:
**    - No necesitamos modificar las cadenas
**    - Solo necesitamos una pasada por cada cadena
**    - Es importante mantener el orden de los caracteres
*/

#include <unistd.h>

// Función auxiliar para escribir un caracter
void    ft_putchar(char c)
{
    write(1, &c, 1);
}

int     main(int argc, char **argv)
{
    int i; // índice para s1
    int j; // índice para s2
    
    // Si no hay exactamente 2 argumentos, imprime newline y termina
    if (argc != 3)
    {
        write(1, "\n", 1);
        return (0);
    }
    
    i = 0;
    j = 0;
    
    // Mientras no lleguemos al final de ninguna cadena
    while (argv[1][i] && argv[2][j])
    {
        // Si encontramos un caracter que coincide, avanzamos en s1
        if (argv[1][i] == argv[2][j])
            i++;
        // Siempre avanzamos en s2
        j++;
    }
    
    // Si llegamos al final de s1, significa que encontramos todos los caracteres
    if (argv[1][i] == '\0')
        ft_putchar('1');
    else
        ft_putchar('0');
    
    ft_putchar('\n');
    return (0);
}