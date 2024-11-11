#include <stdio.h>
#include <stdlib.h>

// Función para calcular el Máximo Común Divisor (MCD) usando el algoritmo de Euclides
int gcd(int a, int b)
{
    int temp;
    while (b != 0)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main(int argc, char *argv[])
{
    // Verificar si el número de argumentos es correcto
    if (argc != 3)
    {
        printf("\n");
        return 0;
    }

    // Convertir los argumentos a enteros
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);

    // Verificar que ambos números sean positivos
    if (num1 <= 0 || num2 <= 0)
    {
        printf("\n");
        return 0;
    }

    // Calcular y mostrar el MCD
    int result = gcd(num1, num2);
    printf("%d\n", result);

    return 0;
}