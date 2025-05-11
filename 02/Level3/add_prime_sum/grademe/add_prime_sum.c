#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int is_prime(int n)
{
    int i = 2;

    if(n <= 1)
        return 0;
    while(i * i <= n)
    {
        if(n % i == 0)
            return 0;
        i++;
    }
    return 1;    
}

void prime_sum(int n)
{
    int sum = 0;
    int i = 2;
    while(i <= n)
    {
        if(is_prime(i))
            sum += i;
        i++;
    }
   printf("%d\n", sum);
}

int is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

int ft_atoi(char *s)
{
    int sign = 1;
    int r = 0;
    int i = 0;
    while(s[i] == '-' || s[i] == '+')
    {
        if(s[i] == '-')
            sign = -1;
        i++;
    }
    while(s[i] && is_digit(s[i]))
    {
        r = r * 10 + (s[i] - '0');
        i++;
    }
    if(s[i] != '\0')
        return 0;
    return r * sign;

}

int main (int argc, char **argv)
{
    if(argc == 2)
    {
        int n = ft_atoi(argv[1]);
        prime_sum(n);
    }
    else
        write(1, "0\n", 2);
    return 0;

}