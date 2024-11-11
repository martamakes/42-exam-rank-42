#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
* EXPLICACIÓN DEL TEST:
* - Generamos la salida esperada con generate_expected_output()
* - Capturamos la salida del programa del estudiante
* - Comparamos ambas salidas
*
* CASOS DE PRUEBA:
* - Números normales (1, 2, 4, etc.)
* - Múltiplos de 3 (3, 6, 9, etc.)
* - Múltiplos de 5 (5, 10, etc.)
* - Múltiplos de 3 y 5 (15, 30, 45, etc.)
* - Verificar el formato (saltos de línea)
*/

// Función para generar la salida esperada
char *generate_expected_output(void)
{
    static char expected[4096] = {0};
    char temp[32];
    int len = 0;
    
    for (int i = 1; i <= 100; i++)
    {
        if (i % 3 == 0 && i % 5 == 0)
            len += snprintf(expected + len, sizeof(expected) - len, "fizzbuzz\n");
        else if (i % 3 == 0)
            len += snprintf(expected + len, sizeof(expected) - len, "fizz\n");
        else if (i % 5 == 0)
            len += snprintf(expected + len, sizeof(expected) - len, "buzz\n");
        else
            len += snprintf(expected + len, sizeof(expected) - len, "%d\n", i);
    }