#include <unistd.h>
#include <stdlib.h>

char to_lower(char c)
{
    if(c >= 'A' && c <= 'Z')
    {
        return (c + 32);
    }
    return c;
}

int is_upper(char c)
{
    return(c >= 'A' && c <= 'Z');
}


int main (int argc, char **argv)
{
    if (argc != 2)
    {
        write(1, "\n", 1);
        return 1;
    }
    char *s = argv[1];
    int printed = 0;

    while(*s)
    {
        if(*s && is_upper(*s))
        {
            if(printed)
            {
                write(1, "_",1);
                char c = to_lower(*s);
                write(1, &c, 1);
            }
            else
            {
                char c = to_lower(*s);
                write(1, &c, 1);
            }
            
            s++;
        }
        else
        {
            while(*s && !is_upper(*s))
            {
                write(1, s, 1);
                printed = 1;
                s++;
            }
        }
        
    }
    write(1,"\n", 1);
    return 0;
}