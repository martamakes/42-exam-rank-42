#include <unistd.h>

/*
* Conceptos clave ALPHA_MIRROR:
* 1. Espejo alfabético: a->z, b->y, c->x, etc.
* 2. Fórmula: letra_espejo = 'a' + ('z' - letra)
* 3. Preservación de caso
* 4. Caracteres no alfabéticos intactos
*/

int main(int argc, char **argv)
{
    int i = 0;

    /*
    * Paso 1: Verificar número de argumentos
    * argc == 2 significa programa + un argumento
    */
    if (argc == 2)
    {
        /*
        * Paso 2: Procesar cada carácter de la string
        */
        while (argv[1][i])
        {
            /*
            * Paso 3a: Procesar minúsculas
            * Para cada letra, calculamos su opuesta:
            * - 'z' - (letra - 'a') da la posición espejo
            * Ejemplo para 'd':
            * - 'd' - 'a' = 3 (posición en alfabeto, 0-based)
            * - 'z' - 3 = 'w' (posición espejo)
            */
            if (argv[1][i] >= 'a' && argv[1][i] <= 'z')
                argv[1][i] = 'z' - (argv[1][i] - 'a');
            /*
            * Paso 3b: Procesar mayúsculas
            * Mismo proceso pero con 'A' y 'Z'
            */
            else if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
                argv[1][i] = 'Z' - (argv[1][i] - 'A');
            /*
            * Paso 4: Escribir el carácter
            * - Si era letra, escribimos su espejo
            * - Si no era letra, escribimos el original
            */
            write(1, &argv[1][i], 1);
            i++;
        }
    }
    /*
    * Paso 5: Escribir newline
    * - Requerido en todos los casos según el subject
    */
    write(1, "\n", 1);
    return (0);
}

/*
* EXPLICACIÓN ADICIONAL:
*
* 1. Lógica del espejo:
*    - Primera letra (a/A) ↔ Última letra (z/Z)
*    - Segunda letra (b/B) ↔ Penúltima letra (y/Y)
*    - Tercera letra (c/C) ↔ Antepenúltima letra (x/X)
*
* 2. Fórmula matemática:
*    - Para 'a': 'z' - (('a' - 'a') = 0) = 'z'
*    - Para 'b': 'z' - (('b' - 'a') = 1) = 'y'
*    - Para 'c': 'z' - (('c' - 'a') = 2) = 'x'
*
* 3. Complejidad:
*    - Tiempo: O(n) donde n es la longitud de la string
*    - Espacio: O(1) modificamos la string in-place
*/