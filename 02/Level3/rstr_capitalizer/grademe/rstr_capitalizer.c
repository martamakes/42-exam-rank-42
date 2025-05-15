#include <unistd.h>

char to_lower(char c)
{
    if(c >= 'A' && c <= 'Z')
        return c += 32;
    else
        return c;
    
}

char to_upper(char c)
{
    if(c >= 'a' && c <= 'z')
        return c -= 32;
    else
        return c;
    
}

int is_up(char c)
{
    return(c >= 'A' && c <= 'Z');
}

int is_low(char c)
{
    return(c >= 'a' && c <= 'z');
}

int is_space(char c)
{
    return(c == ' ' || c == '\t');
}


void processs(char *s)
{
    for(int i = 0; s[i]; i++)
    {
        if((s[i]) && (is_space(s[i +1]) || !s[i + 1]))
        {
            char c = to_upper(s[i]);
            write(1, &c, 1);
        }
        else
        {
            char c = to_lower(s[i]);
            write(1, &c, 1);
        }
            
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