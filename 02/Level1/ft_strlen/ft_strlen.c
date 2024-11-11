int	ft_strlen(char *str)
{
    int i = 0;
    while(*str++)
        i++;
    return (i);
}

//#include <stdio.h>
//int main()
//{
//    char *str = "hola";
//    int len = ft_strlen(str);
//    printf("la longitud de %s es %d", str, len);
//}