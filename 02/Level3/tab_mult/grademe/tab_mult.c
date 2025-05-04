#include <unistd.h>
#include <stdio.h>

void put_number(int n)
{
    char c;
    if (n < 0)
    {
        write(1,"-", 1);
        n = -n;
    }
    if (n > 9)
    {
        put_number(n/10);
        c = n % 10 + '0';
        write(1, &c, 1);
        
    }
    else
    {
        c = n + '0';
        write(1, &c, 1);
    }

}

int ft_atoi(char *s)
{
   int r = 0;
   int sign = 1;

   
        if(*s == ' ' || *s == '\t'  || *s == '\n' || *s == '\v' || *s == '\f' || *s == '\r')
            s++;
        while(*s == '+' || *s == '-')
        {
            if(*s == '-')
                sign *= -1;
            s++;
        }
        while(*s >= '0' && *s <= '9')
        {
            r = r * 10 + (*s - '0');
            s++;
        }
        return (r * sign);
}

int main(int argc, char **argv)
{
    
    if(argc == 2)
    {
    
        int num = ft_atoi(argv[1]);
        //put_number(num);
        int i = 1;
        while( i < 10)
        {
            int a = num * i;
            put_number(i);
            write(1," x ", 3);
            put_number(num);
            write(1," = ", 3);
            put_number(a);
            write(1, "\n", 1);
            i++;
        }  
    }
    else
        write(1, "\n",1);
    return 0;

}