#include <stdlib.h>

// Función auxiliar para contar el número de palabras en la cadena
static int count_words(char *str)
{
    int count = 0;
    int in_word = 0;

    while (*str)
    {
        if (*str == ' ' || *str == '\t' || *str == '\n')
            in_word = 0;  // No estamos en una palabra
        else if (in_word == 0)
        {
            in_word = 1;  // Empezamos una nueva palabra
            count++;      // Incrementamos el contador de palabras
        }
        str++;  // Avanzamos al siguiente carácter
    }
    return count;
}

// Función auxiliar para obtener la longitud de una palabra
static int word_len(char *str)
{
    int len = 0;
    while (str[len] && str[len] != ' ' && str[len] != '\t' && str[len] != '\n')
        len++;
    return len;
}

// Función principal ft_split
char **ft_split(char *str)
{
    int words = count_words(str);  // Contamos el número de palabras
    char **result = malloc(sizeof(char *) * (words + 1));  // Asignamos memoria para el array de strings
    int i = 0;

    if (!result)
        return NULL;  // Si falla la asignación de memoria, retornamos NULL

    while (*str)
    {
        // Saltamos los espacios en blanco
        while (*str == ' ' || *str == '\t' || *str == '\n')
            str++;

        // Si llegamos al final de la cadena, salimos del bucle
        if (*str == '\0')
            break;

        // Obtenemos la longitud de la palabra actual
        int len = word_len(str);
        
        // Asignamos memoria para la palabra
        result[i] = malloc(sizeof(char) * (len + 1));
        if (!result[i])
        {
            // Si falla la asignación, liberamos toda la memoria asignada previamente
            while (i > 0)
                free(result[--i]);
            free(result);
            return NULL;
        }

        // Copiamos la palabra
        int j;
        for (j = 0; j < len; j++)
            result[i][j] = str[j];
        result[i][j] = '\0';  // Añadimos el carácter nulo al final de la palabra

        // Avanzamos al siguiente índice y a la siguiente palabra
        i++;
        str += len;
    }

    // Añadimos NULL al final del array para indicar el fin
    result[i] = NULL;

    return result;
}