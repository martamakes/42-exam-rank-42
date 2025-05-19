#include <unistd.h>

int ft_atoi(char *s)
{
    int result = 0;
    while(*s >= '0' && *s <= '9')
    {
        result = result * 10 + (*s -'0');
        s++;
    }
    return result;
}

void print_hex(int n)
{
    char base[] = "0123456789abcdef";

    if(n >= 16)
        print_hex(n / 16);
    write(1, &base[n % 16], 1);
}

int main(int argc, char **argv)
{
    if(argc == 2)
        print_hex(ft_atoi(argv[1]));
    write(1, "\n", 1);
    return 0;
}