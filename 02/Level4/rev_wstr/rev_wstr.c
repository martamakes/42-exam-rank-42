#include <unistd.h>

int main(int argc, char **argv)
{
    int i = 0;
    int start;
    int end;

    // Verificar si el número de argumentos es correcto
    if (argc != 2)
    {
        write(1, "\n", 1);
        return 0;
    }

    // Obtener la longitud de la cadena
    while (argv[1][i] != '\0')
        i++;

    // Procesar la cadena de derecha a izquierda
    while (i >= 0)
    {
        // Saltar espacios y caracteres nulos desde el final
        while (i >= 0 && (argv[1][i] == ' ' || argv[1][i] == '\t'))
            i--;

        end = i + 1; // El final de la palabra

        // Encontrar el inicio de la palabra
        while (i >= 0 && argv[1][i] != ' ' && argv[1][i] != '\t')
            i--;

        start = i + 1; // El inicio de la palabra

        // Imprimir la palabra
        if (end > start) // Asegurarse de que hay una palabra para imprimir
            write(1, argv[1] + start, end - start);

        // Imprimir espacio si no es la primera palabra
        if (i > 0)
            write(1, " ", 1);
    }

    write(1, "\n", 1); // Salto de línea al final
    return 0;
}