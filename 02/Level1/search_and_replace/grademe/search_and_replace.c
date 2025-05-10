#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 4 || argv[2][1] != '\0' || argv[3][1] != '\0')
    {
        write(1, "\n", 1);
        return (0);
    }

    char *str = argv[1];
    char find = argv[2][0];
    char replace = argv[3][0];
    int i = 0;

    while (str[i])
    {
        if (str[i] == find)
            write(1, &replace, 1);
        else
            write(1, &str[i], 1);
        i++;
    }
    write(1, "\n", 1);
    return (0);
}
