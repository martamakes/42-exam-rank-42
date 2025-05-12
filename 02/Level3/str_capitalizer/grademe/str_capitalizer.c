#include <unistd.h>
char to_lower(char c)
{
    if(c >= 'A' && c <= 'Z')
        return(c += 32);
    return c;
}
int is_upper(char c)
{
    return(c >= 'A' && c <= 'Z');
}

char to_upper(char c)
{
    if(c >= 'a' && c <= 'z')
        return(c -= 32);
    return c;
}

int is_space(char c)
{
    return(c == ' ' || c == '\t');
}

void capi(char *s)
{
    int i;
    for(i = 0; s[i]; i++)
    {
        if(is_upper(s[i]))
            s[i] = to_lower(s[i]);
    }
    if(s[0] && !is_space(s[0]))
    {
        s[0] = to_upper(s[0]);
    }
    for(i = 0; s[i]; i++)
    {
        
        if(!is_space(s[i]) && is_space(s[i - 1]))
        {
            s[i] = to_upper(s[i]);
        }
        write(1, &s[i], 1);
    }

}
int main(int argc, char **argv)
{
    if(argc >= 2)
    {
        for(int i = 1; argv[i]; i++)
        {
            capi(argv[i]);
            write(1, "\n", 1);
        }
        
    }
    else
        write(1, "\n", 1);
    return 0;
}