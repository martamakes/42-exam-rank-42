// fprime.c
#include <stdio.h>
#include <stdlib.h>

void print_factors(int nb)
{
    int first = 1;  // Flag para saber si es el primer factor
    int factor = 2;
    
    // Caso especial: número 1
    if (nb == 1)
    {
        printf("1");
        return;
    }
    
    // Mientras el número sea mayor que 1
    while (nb > 1)
    {
        // Si encontramos un factor
        if (nb % factor == 0)
        {
            // Imprimir el factor con el formato correcto
            if (first)
                printf("%d", factor);
            else
                printf("*%d", factor);
            first = 0;
            
            // Dividir el número por el factor
            nb /= factor;
        }
        // Si no es factor, probar con el siguiente número
        else
        {
            // Optimización: después del 2, solo probar impares
            if (factor == 2)
                factor = 3;
            else
                factor += 2;
        }
    }
}

int main(int argc, char **argv)
{
    // Verificar número de argumentos
    if (argc != 2)
    {
        printf("\n");
        return (0);
    }
    
    // Convertir argumento a número
    int num = atoi(argv[1]);
    
    // Imprimir factores
    print_factors(num);
    printf("\n");
    
    return (0);
}