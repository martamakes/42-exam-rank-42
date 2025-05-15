#include <stdlib.h>
#include <stdio.h>

void fprime(char *s)
{
    int n = atoi(s);

    int first = 1;
    int factor = 2;

    if(n == 1)
    {
        printf("1");
        return;
    }
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



int main(int argc, char **argv)
{
    if(argc == 2)
    {
        fprime(argv[1]);
    }
    printf("\n");
}
