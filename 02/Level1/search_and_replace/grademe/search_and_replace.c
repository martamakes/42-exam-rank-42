#include <unistd.h>

/*
* Conceptos clave SEARCH_AND_REPLACE:
* 1. Manejo de argumentos de línea de comandos
* 2. Reemplazo de caracteres en string
* 3. Verificación de condiciones antes de procesar
* 4. Preservación de la string original si no hay coincidencias
*/

int main(int argc, char **argv)
{
    int i = 0;

    /*
    * Paso 1: Verificaciones iniciales
    * - Debe haber exactamente 3 argumentos
    * - El segundo y tercer argumento deben ser caracteres únicos
    */
    if (argc == 4 && !argv[2][1] && !argv[3][1])
    {
        /*
        * Paso 2: Procesar la string
        * Recorremos la string de entrada (argv[1])
        * y reemplazamos cada ocurrencia de argv[2][0]
        * por argv[3][0]
        */
        while (argv[1][i])
        {
            /*
            * Si el carácter actual coincide con el buscado
            * escribimos el carácter de reemplazo
            * si no, escribimos el carácter original
            */
            if (argv[1][i] == argv[2][0])
                write(1, &argv[3][0], 1);
            else
                write(1, &argv[1][i], 1);
            i++;
        }
    }
    write(1, "\n", 1);
    return (0);
}

/*
* EXPLICACIÓN ADICIONAL:
*
* 1. Verificación de argumentos:
*    - argc == 4: programa + string + char_buscar + char_reemplazar
*    - !argv[2][1]: asegura que el segundo argumento es un solo char
*    - !argv[3][1]: asegura que el tercer argumento es un solo char
*
* 2. No necesitamos verificar si el carácter está en la string:
*    - Si no está, simplemente nunca se cumplirá la condición
*      del if y se imprimirá la string original
*
* 3. Complejidad:
*    - Tiempo: O(n) donde n es la longitud de la string
*    - Espacio: O(1) ya que solo usamos variables simples
*
* 4. La solución es eficiente porque:
*    - Solo recorre la string una vez
*    - No usa memoria adicional
*    - Escribe directamente cada carácter
*/