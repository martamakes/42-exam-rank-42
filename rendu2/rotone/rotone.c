#include <unistd.h>

/*
* Conceptos clave ROTONE:
* 1. Cifrado César con desplazamiento de 1
* 2. Uso de aritmética modular para el "wrap-around" (z->a)
* 3. Manejo de casos (mayúsculas/minúsculas)
* 4. Preservación de caracteres no alfabéticos
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
        * Paso 2: Mientras no llegue al final de la string ('\0')
        * Procesamos cada carácter
        */
        while (argv[1][i])
        {
            /*
            * Paso 3a: Procesar minúsculas
            * Si es minúscula (entre 'a' y 'z'):
            * 1. Si es 'z', convertir a 'a'
            * 2. Si no, avanzar una letra
            */
            if (argv[1][i] >= 'a' && argv[1][i] <= 'z')
            {
                if (argv[1][i] == 'z')
                    argv[1][i] = 'a';
                else
                    argv[1][i] += 1;
            }
            /*
            * Paso 3b: Procesar mayúsculas
            * Si es mayúscula (entre 'A' y 'Z'):
            * 1. Si es 'Z', convertir a 'A'
            * 2. Si no, avanzar una letra
            */
            else if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
            {
                if (argv[1][i] == 'Z')
                    argv[1][i] = 'A';
                else
                    argv[1][i] += 1;
            }
            /*
            * Paso 4: Escribir el carácter
            * - Si era letra, escribimos la siguiente letra
            * - Si no era letra, escribimos el carácter sin modificar
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
* 1. Diferencia con ROT13:
*    - ROT13 avanza 13 posiciones
*    - ROTONE avanza 1 posición
*    - ROT13 es simétrico (aplicarlo dos veces restaura el original)
*    - ROTONE necesita 26 aplicaciones para restaurar el original
*
* 2. Alternativas de implementación:
*    - Podríamos usar aritmética modular como en ROT13:
*      argv[1][i] = ((argv[1][i] - 'a' + 1) % 26) + 'a';
*    - La solución actual es más simple y legible para rotación de 1
*
* 3. Complejidad:
*    - Tiempo: O(n) donde n es la longitud de la string
*    - Espacio: O(1) modificamos la string in-place
*
* 4. Casos especiales manejados:
*    - String vacía -> solo newline
*    - Sin argumentos -> solo newline
*    - Caracteres especiales -> se mantienen igual
*    - Mayúsculas y minúsculas -> mantienen su caso
*/