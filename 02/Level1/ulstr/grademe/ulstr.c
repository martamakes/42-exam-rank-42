#include <unistd.h>

/*
* Conceptos clave ULSTR:
* 1. Manipulación de casos en caracteres ASCII
* 2. Identificación de caracteres alfabéticos
* 3. Diferencia entre mayúsculas y minúsculas en ASCII
* 4. Preservación de caracteres no alfabéticos
*/

int main(int argc, char **argv)
{
    int i = 0;

    /*
    * Paso 1: Verificar número de argumentos
    * argc == 2 significa programa + 1 argumento
    */
    if (argc == 2)
    {
        /*
        * Paso 2: Recorrer cada carácter de la string
        * hasta encontrar el terminador nulo
        */
        while (argv[1][i])
        {
            /*
            * Paso 3a: Si es minúscula, convertir a mayúscula
            * - En ASCII, la diferencia entre mayúsculas y minúsculas es 32
            * - También podemos usar: argv[1][i] -= 32;
            */
            if (argv[1][i] >= 'a' && argv[1][i] <= 'z')
                argv[1][i] = argv[1][i] - 32;
            /*
            * Paso 3b: Si es mayúscula, convertir a minúscula
            * - Sumamos 32 para convertir de mayúscula a minúscula
            * - También podemos usar: argv[1][i] += 32;
            */
            else if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
                argv[1][i] = argv[1][i] + 32;
            /*
            * Paso 4: Escribir el carácter
            * - Si era letra, escribimos su caso invertido
            * - Si no era letra, escribimos el carácter original
            */
            write(1, &argv[1][i], 1);
            i++;
        }
    }
    /*
    * Paso 5: Escribir salto de línea
    * - Requerido en todos los casos según el subject
    */
    write(1, "\n", 1);
    return (0);
}

/*
* EXPLICACIÓN ADICIONAL:
*
* 1. Tabla ASCII y casos:
*    - Mayúsculas: 65 ('A') a 90 ('Z')
*    - Minúsculas: 97 ('a') a 122 ('z')
*    - Diferencia: 32 posiciones
*
* 2. Por qué funciona:
*    - Al restar 32 a una minúscula, obtenemos su mayúscula
*    - Al sumar 32 a una mayúscula, obtenemos su minúscula
*    - Los caracteres no alfabéticos no se modifican
*
* 3. Complejidad:
*    - Tiempo: O(n) donde n es la longitud de la string
*    - Espacio: O(1) modificamos la string in-place
*
* 4. Alternativas:
*    - Podríamos usar XOR con 32:
*      argv[1][i] ^= 32;
*    - Esto funciona porque 32 es la diferencia entre casos
*/