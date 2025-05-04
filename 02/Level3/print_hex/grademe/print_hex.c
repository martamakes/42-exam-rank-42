#include <unistd.h>
int ft_atoi(char *s)
{
    int r = 0;
    int sign = 1;
    while(*s == ' ' || (*s >= 9 && *s <= 13))
        s++;
    if(*s == '-' || *s == '+')
    {
        if(*s == '-')
        {
            sign = -1;
        }
        s++;
    }
    while(*s >= '0' && *s <= '9')
    {
        r = r * 10 + (*s - '0');
        s++;
    }
    return(r * sign);
}
void to_hex(unsigned int n)
{
    char *base = "0123456789abcdef";
    if(n >= 16)
        to_hex(n / 16);
    write(1, &base[n % 16], 1);
}

int main(int argc, char **argv)
{
    if(argc == 2)
    {
        to_hex(ft_atoi(argv[1]));
    }
    write(1, "\n", 1);
    return 0;
}