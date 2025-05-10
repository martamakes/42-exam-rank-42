#include <stdio.h>
#include <stdlib.h>

void fprime(int n)
{
    int first = 1;
    int factor = 2;

    // Handle special case for 1
    if (n == 1)
    {
        printf("1");
        return;
    }

    // Factorize the number
    while (factor <= n)
    {
        if (n % factor == 0)
        {
            if (!first)
                printf("*");
            printf("%d", factor);
            first = 0;
            n /= factor;
        }
        else
        {
            factor++;
        }
    }
}

int main(int argc, char **argv)
{
    // Check for exactly one argument
    if (argc == 2)
    {
        int n = atoi(argv[1]);
        if (n > 0)
        {
            fprime(n);
        }
    }
    printf("\n");
    return 0;
}

/*
// Test cases
// Compile with: gcc rendu/fprime/fprime.c -o fprime
// Run with: ./fprime <number>
// Example outputs:
// ./fprime 225225 => 3*3*5*5*7*11*13
// ./fprime 8333325 => 3*3*5*5*7*11*13*37
// ./fprime 9539 => 9539
// ./fprime 804577 => 804577
// ./fprime 42 => 2*3*7
// ./fprime 1 => 1
// ./fprime => (empty line)
// ./fprime 42 21 => (empty line)
*/
