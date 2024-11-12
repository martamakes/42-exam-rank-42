#include <unistd.h>

/* 
* Conceptos clave ROT13:
* 1. Aritmética modular: Usamos %26 para "dar la vuelta" al alfabeto
* 2. Manejo de ASCII: Restamos 'a' o 'A' para trabajar en rango 0-25
* 3. Preservación de caso: Tratamos mayúsculas y minúsculas por separado
*/

int main(int argc, char **argv)
{
    int i = 0;  // Índice para recorrer la string

    /* 
    * Paso 1: Verificar número de argumentos
    * - argc == 2 significa: el nombre del programa (1) + un argumento (1)
    * - Si no hay exactamente 2, solo imprimimos newline
    */
    if (argc == 2)
    {
        /* 
        * Paso 2: Recorrer la string mientras no llegue al final ('\0') 
        * - argv[1] es el primer argumento (la string a procesar)
        * - argv[1][i] es cada carácter de esa string
        */
        while(argv[1][i])
        {
            /* 
            * Paso 3a: Procesar minúsculas
            * - Verificamos si es una letra minúscula (entre 'a' y 'z')
            * Para aplicar ROT13:
            * 1. argv[1][i] - 'a' : Convertimos letra a número (0-25)
            * 2. + 13 : Avanzamos 13 posiciones
            * 3. % 26 : Si nos pasamos de 25, volvemos al inicio (aritmética modular)
            * 4. + 'a' : Convertimos de nuevo a letra
            */
            if (argv[1][i] >= 'a' && argv[1][i] <= 'z')
                argv[1][i] = (argv[1][i] - 'a' + 13) % 26 + 'a';
            /* 
            * Paso 3b: Procesar mayúsculas
            * - Mismo proceso pero usando 'A' como base
            */
            else if (argv[1][i] >= 'A' && argv[1][i] <= 'Z')
                argv[1][i] = (argv[1][i] - 'A' + 13) % 26 + 'A';
            
            /* 
            * Paso 4: Escribir el carácter
            * - Si era letra, escribimos la versión rotada
            * - Si no era letra, escribimos el carácter original sin modificar
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
* 1. ¿Por qué ROT13 es especial?
*    - 13 es la mitad de 26 (letras del alfabeto)
*    - Aplicar ROT13 dos veces devuelve el texto original
*    - Es un cifrado simétrico: la misma operación sirve para cifrar y descifrar
*
* 2. Aritmética modular con %26:
*    Ejemplo con 'z':
*    - 'z' - 'a' = 25
*    - 25 + 13 = 38
*    - 38 % 26 = 12
*    - 12 + 'a' = 'm'
*
* 3. Complejidad:
*    - Tiempo: O(n) donde n es la longitud de la string
*    - Espacio: O(1) porque modificamos la string in-place
*
* 4. Alternativa:
*    Se podría usar un array de mapeo directo, pero esta solución
*    es más elegante y eficiente en espacio:
*    char rot13[26] = {'n','o','p',...};
*/