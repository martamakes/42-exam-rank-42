#include <unistd.h>

/*
** Conceptos clave:
** 1. Manejo de espacios:
**    - Identificar espacios/tabs como delimitadores
**    - Ignorar espacios/tabs al inicio y final
**    - Colocar exactamente 3 espacios entre palabras
**
** 2. Identificación de palabras:
**    - Una palabra es una sección delimitada por espacios/tabs
**    - O por el inicio/fin del string
**
** 3. Edge cases:
**    - String vacío
**    - Múltiples espacios/tabs consecutivos
**    - Espacios/tabs al inicio o final
*/

int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

int is_word_char(char c)
{
    return (c != '\0' && !is_space(c));
}

void expand_str(char *str)
{
    int i;
    int need_spaces;

    i = 0;
    need_spaces = 0;

    // Saltar espacios iniciales
    while (str[i] && is_space(str[i]))
        i++;

    // Procesar cada carácter
    while (str[i])
    {
        // Si encontramos un carácter no espacio
        if (is_word_char(str[i]))
        {
            // Si necesitamos espacios antes de la palabra, los escribimos
            if (need_spaces)
                write(1, "   ", 3);
            write(1, &str[i], 1);
            need_spaces = 0;
        }
        // Si encontramos un espacio después de un carácter no espacio
        else if (!need_spaces && i > 0 && is_word_char(str[i - 1]))
        {
            // Marcamos que necesitaremos espacios antes de la próxima palabra
            need_spaces = 1;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    // Verificar número de argumentos
    if (argc == 2)
        expand_str(argv[1]);
    write(1, "\n", 1);
    return (0);
}