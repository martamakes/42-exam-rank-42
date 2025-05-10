#include <unistd.h>
#include <stdio.h>

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
    int i= 0, start=0, end=0, fstart=0, fend=0;

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
     fstart = i;
     

    // Encontrar el final de la primera palabra
    while (argv[1][i] && !is_space(argv[1][i]))
        i++;
    fend = i;
    print_var(fstart, fend);
    

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
    
    // Imprimir la primera palabra si no está vacía
    if (fstart < fend)
    {
        write(1, argv[fstart], fend - fstart);
        write(1, "\n", 1);
    }

    return 0;
}