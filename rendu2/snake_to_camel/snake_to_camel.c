/*
** snake_to_camel.c
**
** CONCEPTOS CLAVE:
**
** 1. snake_case:
**    - Palabras en minúscula
**    - Separadas por guiones bajos (_)
**    Ejemplo: "hello_world"
**
** 2. lowerCamelCase:
**    - Primera palabra en minúscula
**    - Siguientes palabras empiezan con mayúscula
**    - Sin separadores
**    Ejemplo: "helloWorld"
**
** 3. Manipulación de caracteres:
**    - Convertir minúscula a mayúscula: c - 32
**    - Encontrar y procesar guiones bajos
*/

#include <unistd.h>

void    snake_to_camel(char *str)
{
    int i;
    int capitalize;

    i = 0;
    capitalize = 0;
    while (str[i])
    {
        if (str[i] == '_')
        {
            capitalize = 1;
        }
        else
        {
            // Si debemos capitalizar, convertimos a mayúscula
            if (capitalize)
                str[i] = str[i] - 32;
            write(1, &str[i], 1);
            capitalize = 0;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    // Si no hay argumentos, solo mostrar newline
    if (argc != 2)
    {
        write(1, "\n", 1);
        return (0);
    }

    snake_to_camel(argv[1]);
    write(1, "\n", 1);
    return (0);
}