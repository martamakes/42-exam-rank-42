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

#include <unistd.h>
#include <stdlib.h>

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
    i--; // Ajustar para apuntar al último carácter válido (no el nulo)

    // Procesar la cadena de derecha a izquierda
    while (i >= 0)
    {
        // Saltar espacios desde el final
        while (i >= 0 && (argv[1][i] == ' ' || argv[1][i] == '\t'))
            i--;

        end = i + 1; // El final de la palabra (posición después del último carácter)

        // Encontrar el inicio de la palabra
        while (i >= 0 && argv[1][i] != ' ' && argv[1][i] != '\t')
            i--;

        start = i + 1; // El inicio de la palabra

        // Imprimir la palabra
        if (end > start) // Asegurarse de que hay una palabra para imprimir
            write(1, argv[1] + start, end - start);

        // Imprimir espacio si no es la primera palabra (quedan más palabras por procesar)
        if (i >= 0)
            write(1, " ", 1);
    }

    write(1, "\n", 1); // Salto de línea al final
    return 0;
}