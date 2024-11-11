int	ft_atoi(const char *str)
{
    int res = 0;
    int sign = 1;

    while(*str == ' ' || *str == '\n' || *str == '\t')
        *str++;
    if(*str == '+' || *str == '-')
    {
        while(*str++ == '-')
            sign -= 1;
        if(*str == '+')
            *str++;
    }
    while(*str >= '0' && *str <= '9')
    {
        res =  res * 10 + (*str - '0');
        str++;
    }
    return res*sign;
}