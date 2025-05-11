#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print_hex(int n)
{
    char base[] = "0123456789abcdef";
    if(n >= 16)
        print_hex(n/16);
    write(1, &base[n % 16], 1);  
    
}

int ft_atoi(char *s)
{
    int i = 0;
    int sign = 1;
    int result = 0;
    if(s[i] == '-' || s[i]== '+')
    {
        if(s[i] == '-')
            sign = -1;
        i++;
    }
    while(s[i]>= '0' && s[i] <= '9' )
    {
        result = result * 10 + (s[i] - '0');
        i++;
    }
   

    return result * sign;
}
int main(int argc, char **argv)
{
    if(argc == 2)
    {
        int n = ft_atoi(argv[1]);
        //printf("n is %d\n", n);
        if(n == 0)
            write(1, "0", 1);
        else
        {
            print_hex(n);
        } 
    }
    write(1, "\n", 1);
    return 0;
}