#include <unistd.h>
#include <stdlib.h>

int ft_strlen(char *s)
{
    int i = 0;
    while (s[i])
        i++;
    return i;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        write(1, "\n", 1);
        return 0;
    }
    char *s = argv[1];
    char *p = malloc(ft_strlen(s) + 1); // Buffer para la primera palabra
    if (!p)
        return 1;

    int i = 0, j = 0;

    // Saltar espacios iniciales
    while (s[i] == ' ' || s[i] == '\t')
        i++;

    // Guardar la primera palabra
    while (s[i] && s[i] != ' ' && s[i] != '\t')
        p[j++] = s[i++];
    p[j] = '\0';

    // Saltar espacios después de la primera palabra
    while (s[i] == ' ' || s[i] == '\t')
        i++;

    int first = 1;
    // Imprimir el resto de las palabras
    while (s[i])
    {
        // Saltar espacios antes de la palabra
        while (s[i] == ' ' || s[i] == '\t')
            i++;
        if (!s[i])
            break;
        if (!first)
            write(1, " ", 1);
        first = 0;
        int start = i;
        while (s[i] && s[i] != ' ' && s[i] != '\t')
            i++;
        write(1, &s[start], i - start);
    }

    // Imprimir la primera palabra al final
    if (j > 0)
    {
        if (first == 0) // Si ya imprimiste palabras antes
            write(1, " ", 1);
        write(1, p, j);
    }
    write(1, "\n", 1);
    free(p);
    return 0;
}
