#include <unistd.h>
#include <stdio.h>

/*
** KEY POINTS PARA ESTUDIAR:
**
** 1. Gestión de palabras en strings:
**    - Identificar inicio y fin de cada palabra
**    - Manejar múltiples espacios/tabulaciones como un solo separador
**    - Ignorar espacios al inicio y final
**
** 2. Técnica de rotación:
**    - Identificar la primera palabra
**    - Imprimir el resto del string primero
**    - Añadir la primera palabra al final
**
** 3. Edge cases importantes:
**    - String vacía
**    - String con una sola palabra
**    - String que solo tiene espacios
**    - Espacios múltiples entre palabras
**    - Espacios al inicio o final
*/

// Función para verificar si un carácter es un espacio o tabulación
int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

int main(int argc, char **argv)
{
    int i = 0;
    int first_word_start = 0;
    int first_word_end = 0;
    int word_printed = 0;  // Flag para controlar si ya imprimimos alguna palabra

    // Si no hay argumentos, imprimir un salto de línea y salir
    if (argc != 2)
    {
        write(1, "\n", 1);
        return 0;
    }

    // Saltar espacios iniciales
    while (argv[1][i] && is_space(argv[1][i]))
        i++;
    
    // Guardar inicio de la primera palabra
    first_word_start = i;
    
    // Encontrar el final de la primera palabra
    while (argv[1][i] && !is_space(argv[1][i]))
        i++;
    
    // Guardar fin de la primera palabra
    first_word_end = i;
    
    // Saltar espacios entre palabras
    while (argv[1][i] && is_space(argv[1][i]))
        i++;
    
    // Imprimir el resto de palabras (desde la segunda hasta el final)
    while (argv[1][i])
    {
        // Si encontramos un carácter no-espacio
        if (!is_space(argv[1][i]))
        {
            // Si ya imprimimos una palabra, añadir espacio separador
            if (word_printed)
                write(1, " ", 1);
            
            // Imprimir todos los caracteres de esta palabra
            while (argv[1][i] && !is_space(argv[1][i]))
            {
                write(1, &argv[1][i], 1);
                i++;
            }
            word_printed = 1;  // Marcar que ya imprimimos una palabra
        }
        else
            i++;  // Saltar espacios
    }
    
    // Imprimir la primera palabra al final
    if (first_word_end > first_word_start)  // Si la primera palabra existe
    {
        // Agregar espacio solo si ya imprimimos alguna otra palabra
        if (word_printed)
            write(1, " ", 1);
        
        // Imprimir la primera palabra
        write(1, &argv[1][first_word_start], first_word_end - first_word_start);
    }
    
    // Terminar con salto de línea
    write(1, "\n", 1);

    return 0;
}