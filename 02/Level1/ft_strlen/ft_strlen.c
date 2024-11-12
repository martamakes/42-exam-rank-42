int ft_strlen(char *str)
{
    int i = 0;
    int len = 0;
    
    while (str[i])
    {
        if ((str[i] & 0xC0) != 0x80)  // Si no es un byte de continuación UTF-8
            len++;
        i++;
    }
    return (len);
}

//#include <stdio.h>
//int main()
//{
//    char *str = "hola";
//    int len = ft_strlen(str);
//    printf("la longitud de %s es %d", str, len);
//}