#include <stdlib.h>

char    *ft_strrev(char *str)
{
    if (!str)
        return (NULL);
    int len = 0;
    while(str[len])
        len++;
    if(len == 0)
        return str;
    int i = 0;  
    while(i < len / 2)
    {
        char t = str[len - 1 - i];
        str[len - 1 - i ] = str[i];
        str[i] = t;
        i++;
    }
    return str;
}
