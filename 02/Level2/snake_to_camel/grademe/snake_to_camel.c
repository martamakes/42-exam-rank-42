#include <unistd.h>
#include <stdlib.h>

char to_upper(char c)
{
    if(c >= 'a' && c<= 'z')
        return( c - 32);
    return c;
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        write(1,"\n", 1);
        return 0;
    }
    int i = 0;
    while (argv[1][i])
    {
        
        if(argv[1][i] != '_')
        {
            write(1, &argv[1][i], 1);
        }
        else if (argv[1][i] == '_')
        {
            i++;
            char c = argv[1][i];
            c = to_upper(c);
            write(1, &c, 1);

        }
        i++;   
        
    }
    
    write(1,"\n", 1);
    return 0;
}