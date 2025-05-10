#include <unistd.h>

/* Prints a single character to stdout */
void putchar(char c)
{
    write(1, &c, 1);
}

/* Prints an integer to stdout recursively */
void putnbr(int n)
{
    if(n < 0)
    {
        putchar('-');
        n = -n;
    }
    if(n > 9)
    {
        putnbr(n / 10);
    }
    char digit = n % 10 + '0';
    putchar(digit);
}

/* Main function that counts and displays the number of arguments */
int main(int argc, char **argv)
{
    (void)argv;  // Unused parameter
    
    // If no arguments are passed, print "0"
    if(argc == 1)
        write(1, "0", 1);
    // Otherwise print the number of arguments (argc - 1 to exclude program name)
    else
        putnbr(argc - 1);
    
    // Print newline character
    putchar('\n');
    return 0;
}
