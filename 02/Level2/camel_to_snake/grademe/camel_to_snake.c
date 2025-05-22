#include <stdlib.h>
#include <unistd.h>

int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

int is_up(char c)
{
    return(c >= 'A' && c <= 'Z');
}

char to_lower(char c)
{
    if(c >= 'A' && c <= 'Z')
        return c += 32;
    else
        return c;
}

void process(char *s)
{
    while(is_space(*s))
        s++;
    int i = 0;
    while(s[i])
    {
        if(s[i] && !is_up(s[i]))
            write(1, &s[i], 1);
        if(s[i] && is_up(s[i]))
        {
            if(i != 0)
                write(1, "_", 1);
            char c = to_lower(s[i]);
            write(1, &c, 1);
        }
        i++;
    }
}


int main(int argc, char **argv)
{
    if(argc == 2)
        process(argv[1]);
    write(1, "\n", 1);
}