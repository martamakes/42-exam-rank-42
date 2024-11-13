#include <unistd.h>
#include <stdlib.h>

/*
** KEY POINTS PARA ESTUDIAR:
**
** 1. Gestión de memoria dinámica:
**    - Usamos malloc para almacenar temporalmente palabras
**    - Importante liberar memoria con free
**
** 2. Técnica de procesamiento de strings:
**    - Identificar inicio y fin de cada palabra
**    - Manejar espacios múltiples y tabs
**    - Control de límites de la cadena
**
** 3. Rotación de elementos:
**    - La primera palabra se guarda aparte
**    - Se imprimen el resto de palabras
**    - Finalmente se imprime la primera palabra
**
** 4. Control de espacios:
**    - Un solo espacio entre palabras
**    - Sin espacios al inicio o final
*/

int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

// Función para contar palabras en la cadena
int count_words(char *str)
{
    int count = 0;
    int i = 0;
    int in_word = 0;

    while (str[i])
    {
        if (!is_space(str[i]) && !in_word)
        {
            in_word = 1;
            count++;
        }
        else if (is_space(str[i]))
            in_word = 0;
        i++;
    }
    return (count);
}

// Función para obtener la longitud de una palabra
int word_len(char *str)
{
    int len = 0;
    while (str[len] && !is_space(str[len]))
        len++;
    return (len);
}

void rostring(char *str)
{
    int i = 0;
    int first_word_len;
    char *first_word;
    int is_first_printed = 0;
    int words = count_words(str);

    if (words == 0)
        return;

    // Saltamos espacios iniciales
    while (is_space(str[i]))
        i++;

    // Guardamos la primera palabra
    first_word_len = word_len(str + i);
    first_word = malloc(first_word_len + 1);
    if (!first_word)
        return;
    for (int j = 0; j < first_word_len; j++)
        first_word[j] = str[i + j];
    first_word[first_word_len] = '\0';

    // Avanzamos a la siguiente palabra
    i += first_word_len;

    // Imprimimos el resto de palabras
    while (str[i])
    {
        if (!is_space(str[i]))
        {
            if (is_first_printed)
                write(1, " ", 1);
            while (str[i] && !is_space(str[i]))
            {
                write(1, &str[i], 1);
                i++;
            }
            is_first_printed = 1;
        }
        else
            i++;
    }

    // Imprimimos la primera palabra al final
    if (is_first_printed)
        write(1, " ", 1);
    write(1, first_word, first_word_len);
    free(first_word);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        write(1, "\n", 1);
        return (0);
    }

    rostring(argv[1]);
    write(1, "\n", 1);
    return (0);
}