#include <unistd.h>

int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

void first_word(char *str)
{
    int i = 0;

    // Saltar espacios iniciales
    while (str[i] && is_space(str[i]))
        i++;

    // Imprimir la primera palabra
    while (str[i] && !is_space(str[i]))
    {
        write(1, &str[i], 1);
        i++;
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
        first_word(argv[1]);
    write(1, "\n", 1);
    return (0);
}