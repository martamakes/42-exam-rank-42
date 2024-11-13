#include <unistd.h>

int ft_strlen(char *s)
{
    char *t;

    t = s;
    while(*s)
        s++;
    return(s - t);
}
void    ft_putstr(char *str)
{
    int len = ft_strlen(str);
    write(1, str, len);
    
}
