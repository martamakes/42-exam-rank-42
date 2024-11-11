#include <stdio.h>
#include <stdlib.h>

void fprime(int nb)
{
    int factor = 2;
    int first = 1;

    // Manejo especial para 1
    if (nb == 1)
    {
        printf("1");
        return;
    }

    // Iterar mientras nb sea mayor que 1
    while (nb > 1)
    {
        // Si factor es un divisor de nb
        if (nb % factor == 0)
        {
            // Imprimir el factor
            if (first)
                first = 0;
            else
                printf("*");
            printf("%d", factor);

            // Dividir nb por el factor
            nb /= factor;
        }
        else
        {
            // Si no es un factor, incrementar y continuar
            factor++;
        }
    }
}

int main(int argc, char *argv[])
{
    // Verificar si hay exactamente un argumento
    if (argc != 2)
    {
        printf("\n");
        return 0;
    }

    // Convertir el argumento a entero
    int nb = atoi(argv[1]);

    // Llamar a la función fprime
    fprime(nb);

    printf("\n");
    return 0;
}
