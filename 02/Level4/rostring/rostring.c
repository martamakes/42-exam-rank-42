#include <unistd.h>

// Función para escribir una cadena
void ft_putstr(char *str)
{
    while (*str)
        write(1, str++, 1);
}

// Función para verificar si un carácter es un espacio o tabulación
int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

int main(int argc, char **argv)
{
    int i, start, end, first_word_end;

    // Si no hay argumentos, imprimir un salto de línea y salir
    if (argc < 2)
    {
        write(1, "\n", 1);
        return 0;
    }

    i = 0;
    // Saltar espacios iniciales
    while (argv[1][i] && is_space(argv[1][i]))
        i++;

    // Encontrar el final de la primera palabra
    start = i;
    while (argv[1][i] && !is_space(argv[1][i]))
        i++;
    first_word_end = i;

    // Imprimir el resto de las palabras
    while (argv[1][i])
    {
        // Saltar espacios entre palabras
        while (argv[1][i] && is_space(argv[1][i]))
            i++;
        start = i;
        // Encontrar el final de la palabra actual
        while (argv[1][i] && !is_space(argv[1][i]))
            i++;
        end = i;

        // Imprimir la palabra si no está vacía
        if (start < end)
        {
            write(1, argv[1] + start, end - start);
            write(1, " ", 1);
        }
    }

    // Imprimir la primera palabra
    write(1, argv[1], first_word_end);
    write(1, "\n", 1);

    return 0;
}