#include <stdlib.h>
#include <unistd.h>

/*
** Conceptos clave:
** 1. Manejo de strings:
**    - Identificar palabras (delimitadas por espacios)
**    - No hay espacios adicionales (solo uno entre palabras)
**
** 2. Estrategia de inversión:
**    - Primero contar palabras
**    - Almacenar inicio y longitud de cada palabra
**    - Imprimir desde la última palabra a la primera
**
** 3. Gestión de memoria:
**    - Usar malloc/free para almacenar posiciones
**    - Liberar memoria correctamente
*/

typedef struct s_word
{
    int start;  // Posición inicial de la palabra
    int len;    // Longitud de la palabra
} t_word;

// Cuenta el número de palabras en el string
int count_words(char *str)
{
    int i = 0;
    int count = 0;

    while (str[i])
    {
        // Si encontramos un carácter no espacio
        if (str[i] != ' ' && (i == 0 || str[i - 1] == ' '))
            count++;
        i++;
    }
    return count;
}

void print_word(char *str, int start, int len)
{
    write(1, str + start, len);
}

void rev_wstr(char *str)
{
    int i = 0;
    int word_count = count_words(str);
    t_word *words;

    // Si no hay palabras, retornamos
    if (word_count == 0)
        return;

    // Reservar memoria para almacenar información de palabras
    words = malloc(sizeof(t_word) * word_count);
    if (!words)
        return;

    // Almacenar información de cada palabra
    int current_word = 0;
    while (str[i])
    {
        // Si encontramos el inicio de una palabra
        if (str[i] != ' ' && (i == 0 || str[i - 1] == ' '))
        {
            words[current_word].start = i;
            words[current_word].len = 0;
            // Contar longitud de la palabra
            while (str[i] && str[i] != ' ')
            {
                words[current_word].len++;
                i++;
            }
            current_word++;
        }
        else
            i++;
    }

    // Inicializamos i al último índice
    i = word_count - 1;
    
    while (i >= 0)
    {
        print_word(str, words[i].start, words[i].len);
        if (i > 0)  // Si no es la última palabra a imprimir
            write(1, " ", 1);
        i--;
    }

    free(words);
}

int main(int argc, char **argv)
{
    if (argc == 2)
        rev_wstr(argv[1]);
    write(1, "\n", 1);
    return (0);
}