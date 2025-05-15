#include <unistd.h>

int is_low(char c)
{
    return(c >= 'a' && c <= 'z');
}
int is_up(char c)
{
    return(c >= 'A' && c <= 'Z');
}

char to_upper(char c)
{
    if(is_low(c))
        return c -= 32;
    return c; 
}

char to_lower(char c)
{
    if(is_up(c))
        return c += 32;
    return c; 
}

int is_space(char c)
{
    return( c == ' ' || c == '\t');
}

void processs(char *s)
{
    char c;
    for(int i = 0; s[i]; i++)
    {
        if((s[i]) && (is_space(s[i - 1]) || i == 0))
        {
            c = to_upper(s[i]);   
        }
        else
        {
            c = to_lower(s[i]);
        }
        write(1, &c, 1);
    }
}


int main(int argc, char **argv)
{
    if(argc >= 2)
    {
        for(int i = 1; argv[i]; i++)
        {
            processs(argv[i]);
            write(1, "\n", 1);
        }
        
    }
    else
       write(1, "\n", 1);
    return 0;
}