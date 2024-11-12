/*
** Conceptos clave:
** 1. Reglas de capitalización:
**    - Primera letra de cada palabra en mayúscula
**    - Resto de letras en minúscula
**    - Solo letras se capitalizan (no números/símbolos)
**
** 2. Definición de palabra:
**    - Delimitada por espacios/tabs
**    - O por inicio/fin del string
**    - Palabras de una letra también se capitalizan
**
** 3. Manejo de múltiples argumentos:
**    - Procesar cada argumento por separado
**    - Newline después de cada argumento
*/

/* ************************************************************************** */
/*                                                                            */
/*                                   ENUNCIADO                                */
/* Escribe un programa que tome uno o más strings como argumentos y que, por  */
/* cada argumento, capitalice la primera letra de cada palabra (si es letra), */
/* ponga el resto en minúsculas, y muestre el resultado por la salida        */
/* estándar seguido de un \n.                                                */
/*                                                                            */
/* Una "palabra" se define como una parte del string delimitada por          */
/* espacios/tabs, o por el inicio/fin del string. Si una palabra solo tiene  */
/* una letra, debe ser capitalizada.                                         */
/*                                                                            */
/* Si no hay argumentos, el programa debe mostrar \n.                        */
/* ************************************************************************** */

#include <unistd.h>

/*
** FUNCIONES AUXILIARES
** Estas funciones nos ayudan a hacer el código más legible y mantenible
*/

// Comprueba si un carácter es espacio o tabulación
int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

// Comprueba si un carácter es una letra (a-z o A-Z)
int is_letter(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// Convierte una letra a minúscula si es mayúscula
char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return (c + ('a' - 'A')); // Sumamos 32 (diferencia entre 'a' y 'A' en ASCII)
    return (c);
}

// Convierte una letra a mayúscula si es minúscula
char to_upper(char c)
{
    if (c >= 'a' && c <= 'z')
        return (c - ('a' - 'A')); // Restamos 32
    return (c);
}

/*
** FUNCIÓN PRINCIPAL DE PROCESAMIENTO
** Esta función procesa un string y aplica las reglas de capitalización
*/

void str_capitalizer(char *str)
{
    int i = 0;
    // Este flag indica si el carácter anterior era un espacio
    // Lo inicializamos a 1 para tratar el inicio como si fuera después de espacio
    int prev_was_space = 1;

    while (str[i])
    {
        if (is_letter(str[i]))
        {
            /* Si encontramos una letra, hay dos casos:
            ** 1. Si venimos después de un espacio (prev_was_space = 1),
            **    capitalizamos la letra
            ** 2. Si no venimos después de un espacio, convertimos a minúscula
            */
            if (prev_was_space)
                str[i] = to_upper(str[i]);
            else
                str[i] = to_lower(str[i]);
            prev_was_space = 0; // Ya no estamos después de un espacio
        }
        else if (is_space(str[i]))
            prev_was_space = 1;  // La siguiente letra deberá ser capitalizada
        else
            prev_was_space = 0;  // Caracteres que no son espacios ni letras
                                // marcan que no estamos después de un espacio
        write(1, &str[i], 1);   // Escribimos el carácter procesado
        i++;
    }
    write(1, "\n", 1);  // Añadimos el salto de línea al final
}

/*
** FUNCIÓN MAIN
** Maneja los argumentos y llama a str_capitalizer para cada uno
*/

int main(int argc, char **argv)
{
    // Si no hay argumentos, solo imprimimos un salto de línea
    if (argc == 1)
        write(1, "\n", 1);
    else
    {
        // Procesamos cada argumento, empezando por argv[1]
        int i = 1;
        while (i < argc)
            str_capitalizer(argv[i++]);
    }
    return (0);
}