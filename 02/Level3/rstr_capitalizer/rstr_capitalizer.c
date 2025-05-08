#include <unistd.h>

/* 
** CONCEPTOS CLAVE:
**
** 1. Manipulación de caracteres ASCII:
**    - Mayúsculas: 'A' (65) a 'Z' (90)
**    - Minúsculas: 'a' (97) a 'z' (122)
**    - Diferencia entre mayúsculas y minúsculas: 32
**    - Para convertir: mayúscula + 32 = minúscula, minúscula - 32 = mayúscula
**
** 2. Identificación de palabras:
**    - Una palabra se define como una sección de string delimitada por espacios/tabs
**      o el inicio/fin del string
**    - Caracteres no alfabéticos como puntuación son parte de las palabras
**
** 3. La clave de este ejercicio (rstr_capitalizer):
**    - Capitalizar (poner en MAYÚSCULA) la ÚLTIMA LETRA de cada palabra
**    - Poner el resto de LETRAS en minúscula
**    - Conservar los caracteres no alfabéticos tal como están
**    - Mantener todos los espacios exactamente como en la entrada
*/

// Función para verificar si un carácter es una letra
int     is_letter(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// Función para verificar si un carácter es un espacio o tab
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
void    rstr_capitalizer(char *str)
{
    int i = 0;
    
    while (str[i])
    {
        // Si el carácter es una letra, lo ponemos en minúscula por defecto
        if (is_letter(str[i]))
        {
            // Si es la última letra de una palabra (seguida por espacio o fin de cadena)
            if (!str[i + 1] || is_space(str[i + 1]))
                write(1, &to_upper(str[i]), 1);  // La ponemos en mayúscula
            else
                write(1, &to_lower(str[i]), 1);  // La ponemos en minúscula
        }
        else  // Si no es una letra, lo escribimos tal cual
        {
            write(1, &str[i], 1);
        }
        i++;
    }
}

int     main(int argc, char **argv)
{
    int i;

    // Si no hay argumentos, solo imprimimos newline
    if (argc == 1)
    {
        write(1, "\n", 1);
        return (0);
    }

    // Procesamos cada argumento
    i = 1;
    while (i < argc)
    {
        rstr_capitalizer(argv[i]);
        write(1, "\n", 1);
        i++;
    }
    return (0);
}
