/*
** union.c
**
** CONCEPTOS CLAVE:
**
** 1. Orden de aparición:
**    - Los caracteres deben mostrarse en el orden que aparecen
**    - Primero los del primer string, luego los del segundo
**
** 2. Sin duplicados:
**    - Un carácter solo debe mostrarse una vez
**    - Usar array para trackear caracteres ya vistos
**
** 3. ASCII:
**    - Los caracteres son valores ASCII (0-127/255)
**    - Podemos usar array de booleanos para marcar vistos
*/

#include <unistd.h>

// Función para comprobar si un carácter ya ha sido visto
int has_been_seen(char c, char *seen)
{
    return (seen[(unsigned char)c]);
}

// Función para marcar un carácter como visto
void mark_as_seen(char c, char *seen)
{
    seen[(unsigned char)c] = 1;
}

// Función para procesar un string
void process_string(char *str, char *seen)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!has_been_seen(str[i], seen))
        {
            write(1, &str[i], 1);
            mark_as_seen(str[i], seen);
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    char seen[256] = {0};  // Array para trackear caracteres vistos

    // Si no hay exactamente 2 argumentos, mostrar newline y salir
    if (argc != 3)
    {
        write(1, "\n", 1);
        return (0);
    }

    // Procesar primer string
    process_string(argv[1], seen);
    
    // Procesar segundo string
    process_string(argv[2], seen);

    // Escribir newline final
    write(1, "\n", 1);
    return (0);
}