/*
Asignación: epur_str

Este programa toma una cadena como argumento y la muestra con exactamente un
espacio entre palabras, sin espacios ni tabulaciones al principio o al final,
seguido de un salto de línea (\n).

Una "palabra" se define como una parte de la cadena delimitada por espacios/tabulaciones,
o por el inicio/fin de la cadena.

Si el número de argumentos no es 1, o si no hay palabras para mostrar, el programa
muestra solo un salto de línea (\n).

Ejemplos:
./epur_str "See? It's easy to print the same thing"
See? It's easy to print the same thing

./epur_str " this        time it      will     be    more complex  . "
this time it will be more complex .

./epur_str "No S*** Sherlock..." "nAw S*** ShErLaWQ..."
(muestra solo un salto de línea)

./epur_str ""
(muestra solo un salto de línea)
*/

#include <unistd.h>

void epur_str(char *str)
{
    int i = 0;
    int word_started = 0;

    // Saltamos los espacios iniciales
    while (str[i] == ' ' || str[i] == '\t')
        i++;

    while (str[i])
    {
        if (str[i] != ' ' && str[i] != '\t')
        {
            write(1, &str[i], 1);
            word_started = 1;
        }
        else if ((str[i] == ' ' || str[i] == '\t') && word_started)
        {
            // Si hay una palabra siguiente, imprimimos un espacio
            int j = i + 1;
            while (str[j] == ' ' || str[j] == '\t')
                j++;
            if (str[j])
                write(1, " ", 1);
            i = j - 1;
            word_started = 0;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
        epur_str(argv[1]);
    write(1, "\n", 1);
    return 0;
}