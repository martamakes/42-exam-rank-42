#include <unistd.h>

int ft_atoi(const char *str)
{
    int result = 0;
    int sign = 1;
    
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return (result * sign);
}

int is_prime(int n)
{
    if (n < 2)
        return 0;
    
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

void ft_putnbr(int n)
{
    char c;
    
    if (n >= 10)
        ft_putnbr(n / 10);
    c = n % 10 + '0';
    write(1, &c, 1);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        write(1, "0\n", 2);
        return 0;
    }
    
    // Check if argument contains only digits
    char *p = argv[1];
    while (*p)
    {
        if (*p < '0' || *p > '9')
        {
            write(1, "0\n", 2);
            return 0;
        }
        p++;
    }
    
    int num = ft_atoi(argv[1]);
    if (num <= 0)
    {
        write(1, "0\n", 2);
        return 0;
    }
    
    int sum = 0;
    for (int i = 2; i <= num; i++)
    {
        if (is_prime(i))
            sum += i;
    }
    ft_putnbr(sum);
    write(1, "\n", 1);
    return 0;
}

/*
// Test cases
// Compile with: gcc rendu/add_prime_sum/add_prime_sum.c -o add_prime_sum
// Run with: ./add_prime_sum <number>
// Example outputs:
// ./add_prime_sum 5 => 10
// ./add_prime_sum 7 | cat -e => 17$
// ./add_prime_sum | cat -e => 0$
*/
