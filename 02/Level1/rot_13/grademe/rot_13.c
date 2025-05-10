#include <unistd.h>

void rot13(char *str)
{
    int i = 0;
    char c;

    while (str[i])
    {
        c = str[i];
        // Handle lowercase letters
        if (c >= 'a' && c <= 'z')
            c = (c - 'a' + 13) % 26 + 'a';
        // Handle uppercase letters
        else if (c >= 'A' && c <= 'Z')
            c = (c - 'A' + 13) % 26 + 'A';
        write(1, &c, 1);
        i++;
    }
}

int main(int argc, char **argv)
{
    // Check if there is exactly one argument
    if (argc == 2)
        rot13(argv[1]);
    write(1, "\n", 1);
    return (0);
}
