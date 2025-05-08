#include <unistd.h>

int is_word_start(int i, char *str)
{
    return (i == 0 || str[i - 1] == ' ' || str[i - 1] == '\t');
}

void str_capitalizer(char *str)
{
    int i = 0;
    
    while (str[i])
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32; // Convertir a minúscula
        
        if (is_word_start(i, str) && str[i] >= 'a' && str[i] <= 'z')
            str[i] -= 32; // Convertir a mayúscula si es inicio de palabra
        
        write(1, &str[i], 1);
        i++;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        write(1, "\n", 1);
    else
    {
        for (int i = 1; i < argc; i++)
        {
            str_capitalizer(argv[i]);
            write(1, "\n", 1);
        }
    }
    return (0);
}
