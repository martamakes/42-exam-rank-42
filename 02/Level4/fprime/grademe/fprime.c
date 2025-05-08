#include <stdio.h>
#include <stdlib.h>


void fprime(int n)
{
    int factor = 2;
    int first = 1;

    while(n > 1)
    {
        if(n % factor == 0)
        {
            if(first)
            {
                printf("%d", factor);
                first = 0;
            }
            else
            {
                printf("*%d", factor);
            }
            n /= factor;
        }
        else
            factor++;
    }

}



/*
void fprime(int n)
{
    
    if(n == 1)
    {
        printf("1");
        return ;
    }
    int factor = 2; // primer factor por el que dividir
    int first = 1; // si es la última no imprimimos asterisco
    while(n > 1)
    {
        //printf("n is over 1, it's %d\n", n);
        if(n % factor == 0) //si no hay resto es que es divisible
        {
            //printf("factor is %d, and the modulus %d from the factor if %d", factor, n, (n % factor));
            if(first)
            {
                printf("%d", factor); //imprimimos el factor
                first = 0;
            }
            else
               printf(" * %d", factor); 
            
            n /= factor; //dividimos el número por el factor para continuar
            //factor++;  //no actualizamos factor porque si no imprimiría los primos, si no los número consecutivos por los que se dividiría
        }
        else
         factor++;//si no es divisible, probamos conel siguiente factor)

    }

}*/

int main(int argc, char **argv)
{
    if(argc == 2 && (atoi(argv[1]) > 0))
    {
        int n = atoi(argv[1]);
        if(n == 1) printf("1");
        else
            fprime(n);
    }
    printf("\n");
    return 0;
}
