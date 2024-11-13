#include <unistd.h>

/*
** KEY POINTS PARA ESTUDIAR:
**
** 1. Estados de procesamiento:
**    - Usamos un flag 'space_printed' para controlar si ya imprimimos un espacio
**    - Esto evita espacios duplicados entre palabras
**
** 2. Identificación de espacios:
**    - Espacios: ' ' (32 en ASCII)
**    - Tabulaciones: '\t' (9 en ASCII)
**
** 3. Técnica de dos punteros:
**    - start: marca el inicio de la cadena tras los espacios iniciales
**    - end: marca el final de la cadena antes de los espacios finales
**
** 4. Write vs Putchar:
**    - write(fd, &c, 1) es más básico que putchar
**    - write requiere <unistd.h>
**    - Primer argumento (1) es stdout
*/

int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

void epur_str(char *str)
{
    int i;
    int space_printed;

    // Inicializamos variables
    i = 0;
    space_printed = 0;

    // Saltamos espacios iniciales
    while (str[i] && is_space(str[i]))
        i++;

    // Procesamos la cadena
    while (str[i])
    {
        // Si encontramos un espacio
        if (is_space(str[i]))
        {
            // Si hay una palabra antes y no hemos impreso espacio
            if (!is_space(str[i - 1]) && str[i + 1] && !space_printed)
            {
                write(1, " ", 1);
                space_printed = 1;
            }
        }
        // Si encontramos un carácter no espacio
        else
        {
            write(1, &str[i], 1);
            space_printed = 0;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    // Verificamos que haya exactamente un argumento
    if (argc != 2)
    {
        write(1, "\n", 1);
        return (0);
    }

    epur_str(argv[1]);
    write(1, "\n", 1);
    return (0);
}