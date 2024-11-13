#include <unistd.h>

/*
** KEY POINTS PARA ESTUDIAR:
**
** 1. Técnica de dos punteros:
**    - Un puntero para la string que buscamos (s1)
**    - Otro puntero para la string donde buscamos (s2)
**    - Solo avanzamos en s1 cuando encontramos coincidencia
**
** 2. Orden de los caracteres:
**    - No solo deben estar todos los caracteres
**    - Deben estar en el mismo orden relativo
**
** 3. Manejo eficiente:
**    - No necesitamos almacenar nada
**    - Podemos verificar mientras recorremos
**
** 4. Casos especiales:
**    - Strings vacías
**    - Caracteres repetidos
**    - No match completo
*/

void ft_putstr(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

int wdmatch(char *s1, char *s2)
{
    int i;
    int j;

    i = 0;
    j = 0;
    // Mientras no hayamos llegado al final de ninguna string
    while (s1[i] && s2[j])
    {
        // Si encontramos una coincidencia, avanzamos en s1
        if (s1[i] == s2[j])
            i++;
        // Siempre avanzamos en s2
        j++;
    }
    // Si llegamos al final de s1, encontramos todos los caracteres
    return (s1[i] == '\0');
}

int main(int argc, char **argv)
{
    // Si no hay exactamente 2 argumentos
    if (argc != 3)
    {
        write(1, "\n", 1);
        return (0);
    }

    // Si encontramos match, imprimimos la primera string
    if (wdmatch(argv[1], argv[2]))
    {
        ft_putstr(argv[1]);
    }
    write(1, "\n", 1);
    return (0);
}