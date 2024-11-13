/*
** pgcd.c
**
** CONCEPTOS CLAVE:
**
** 1. Algoritmo de Euclides:
**    - El MCD de dos números se puede calcular usando divisiones sucesivas
**    - Si a = bq + r, entonces MCD(a,b) = MCD(b,r)
**    - Se repite hasta que el resto sea 0
**    Ejemplo: MCD(42,10)
**    42 = 10 * 4 + 2
**    10 = 2 * 5 + 0
**    Por lo tanto, MCD(42,10) = 2
**
** 2. Números positivos:
**    - Los números deben ser estrictamente positivos
**    - Deben caber en un int
*/

#include <stdio.h>
#include <stdlib.h>

// Función para calcular el MCD usando el algoritmo de Euclides
int calculate_pgcd(int a, int b)
{
    int temp;

    // Mientras el divisor no sea 0
    while (b != 0)
    {
        temp = b;
        b = a % b;  // Nuevo divisor es el resto
        a = temp;   // Nuevo dividendo es el antiguo divisor
    }
    return (a);
}

int main(int argc, char **argv)
{
    int num1;
    int num2;

    // Si no hay exactamente 2 argumentos
    if (argc != 3)
    {
        printf("\n");
        return (0);
    }

    // Convertir strings a números
    num1 = atoi(argv[1]);
    num2 = atoi(argv[2]);

    // Si algún número no es positivo
    if (num1 <= 0 || num2 <= 0)
    {
        printf("\n");
        return (0);
    }

    // Calcular y mostrar el MCD
    printf("%d\n", calculate_pgcd(num1, num2));
    return (0);
}