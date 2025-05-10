#include <unistd.h>

void rotone(char *str)
{
    while (*str)
    {
        if ((*str >= 'a' && *str < 'z') || (*str >= 'A' && *str < 'Z'))
            *str += 1;
        else if (*str == 'z')
            *str = 'a';
        else if (*str == 'Z')
            *str = 'A';
        write(1, str, 1);
        str++;
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
        rotone(argv[1]);
    write(1, "\n", 1);
    return (0);
}
