/*
Asignación: expand_str

Este programa toma una cadena como argumento y la muestra con exactamente tres
espacios entre cada palabra, sin espacios ni tabulaciones al principio o al final,
seguido de un salto de línea.

Una "palabra" se define como una sección de la cadena delimitada por espacios/tabulaciones,
o por el inicio/fin de la cadena.

Si el número de parámetros no es 1, o si no hay palabras, simplemente se muestra
un salto de línea.

Ejemplos:
./expand_str "See? It's easy to print the same thing"
See?   It's   easy   to   print   the   same   thing

./expand_str " this        time it      will     be    more complex  "
this   time   it   will   be   more   complex

./expand_str "No S*** Sherlock..." "nAw S*** ShErLaWQ..."
(muestra solo un salto de línea)

./expand_str ""
(muestra solo un salto de línea)
*/

#include <unistd.h>

void expand_str(char *str)
{
    int i = 0;
    int word_printed = 0;

    // Saltamos los espacios iniciales
    while (str[i] == ' ' || str[i] == '\t')
        i++;

    while (str[i])
    {
        if (str[i] != ' ' && str[i] != '\t')
        {
            write(1, &str[i], 1);
            word_printed = 1;
        }
        else if ((str[i] == ' ' || str[i] == '\t') && word_printed)
        {
            // Buscamos la próxima palabra
            int j = i + 1;
            while (str[j] == ' ' || str[j] == '\t')
                j++;
            
            // Si hay una palabra siguiente, imprimimos tres espacios
            if (str[j])
            {
                write(1, "   ", 3);
                i = j - 1;
            }
            word_printed = 0;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
        expand_str(argv[1]);
    write(1, "\n", 1);
    return 0;
}