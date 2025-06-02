#include <stdio.h>
#include <stdlib.h>

void fprime(int n)
{
    int factor = 2;
    int first = 1;

    if(n == 1){ printf("1"); return;}

    while(n > 1)
    {
        if(n % factor == 0)
        {
            if(first)
                first = 0;
            else
                printf("*");
            printf("%d", factor);
            n /= factor;
        }
        else
            factor++;
    }
}


int main(int argc, char *argv[])
{
     if (argc != 2)
    {
        printf("\n");
        return 0;
    }

    fprime(atoi(argv[1]));
    printf("\n");
    return 0;
}