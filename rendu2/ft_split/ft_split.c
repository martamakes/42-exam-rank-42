/* 
** ft_split.c
**
** CONCEPTOS CLAVE:
**
** 1. Memoria dinámica en dos niveles:
**    - Matriz principal para almacenar punteros a strings
**    - Memoria individual para cada palabra
**    - Necesidad de NULL al final del array
**
** 2. Delimitadores:
**    - Espacios: ' '
**    - Tabuladores: '\t'
**    - Saltos de línea: '\n'
**    - Inicio y fin de string
**
** 3. Gestión de strings:
**    - Saltar múltiples delimitadores seguidos
**    - Copiar substrings (palabras)
**    - Medir longitudes exactas
**
** 4. Algoritmo en dos pasadas:
**    1ª pasada: Contar palabras
**    2ª pasada: Extraer y copiar palabras
**
** 5. Memory management:
**    - Liberar memoria en caso de error
**    - Asegurar que no hay memory leaks
*/

#include <stdlib.h>

// Función para verificar si un caracter es un separador
int is_separator(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

// Función para contar palabras
int count_words(char *str)
{
    int count = 0;
    int in_word = 0;

    while (*str)
    {
        if (!is_separator(*str) && !in_word)
        {
            in_word = 1;
            count++;
        }
        else if (is_separator(*str))
            in_word = 0;
        str++;
    }
    return (count);
}

// Función para obtener la longitud de una palabra
int word_len(char *str)
{
    int len = 0;
    
    while (str[len] && !is_separator(str[len]))
        len++;
    return (len);
}

// Función para liberar memoria en caso de error
void free_split(char **split, int words)
{
    while (words--)
        free(split[words]);
    free(split);
}

char    **ft_split(char *str)
{
    char    **split;
    int     words;
    int     i;
    int     len;

    // Si str es NULL, devolver array con solo NULL
    if (!str)
    {
        split = (char **)malloc(sizeof(char *));
        if (!split)
            return (NULL);
        split[0] = NULL;
        return (split);
    }

    // Contar palabras y asignar memoria para array de punteros
    words = count_words(str);
    split = (char **)malloc(sizeof(char *) * (words + 1));
    if (!split)
        return (NULL);

    // Inicializar índice y procesar cada palabra
    i = 0;
    while (i < words)
    {
        // Saltar separadores
        while (*str && is_separator(*str))
            str++;

        // Obtener longitud de la palabra actual
        len = word_len(str);

        // Asignar memoria y copiar la palabra
        split[i] = (char *)malloc(sizeof(char) * (len + 1));
        if (!split[i])
        {
            free_split(split, i);
            return (NULL);
        }

        // Copiar la palabra
        int j = 0;
        while (j < len)
        {
            split[i][j] = str[j];
            j++;
        }
        split[i][j] = '\0';

        // Avanzar a la siguiente palabra
        str += len;
        i++;
    }
    split[words] = NULL;  // Terminar array con NULL
    return (split);
}