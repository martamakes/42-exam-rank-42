// ft_strpbrk.c
char    *ft_strpbrk(const char *s1, const char *s2)
{
    int i;

    // Si alguna string es NULL, retornar NULL
    if (!s1 || !s2)
        return (0);

    // Recorrer s1
    while (*s1)
    {
        // Para cada carácter de s1, buscar en s2
        i = 0;
        while (s2[i])
        {
            if (*s1 == s2[i])
                return ((char *)s1);
            i++;
        }
        s1++;
    }
    return (0);
}