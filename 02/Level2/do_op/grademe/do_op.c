#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void operate(int n1, int n2, char *op)
{
    if(*op == '+')
    {
        printf("%d\n",n1+n2);
        return ;
    }
    if(*op == '-')
    {
        printf("%d\n",n1-n2);
        return ;
    }
    if(*op == '/')
    {
        printf("%d\n",n1/n2);
        return ;
    }
    if(*op == '*')
    {
        printf("%d\n",n1*n2);
        return ;
    }
    if(*op == '%')
    {
        printf("%d\n",n1%n2);
        return ;
    }
    else
        return ;        

}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        write(1, "\n", 1);
        return 1;
    }
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[3]);
    operate(n1, n2, argv[2]);
    return 0;
}
