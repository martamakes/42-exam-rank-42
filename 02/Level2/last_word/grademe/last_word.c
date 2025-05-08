#include <unistd.h>

int is_space(char c)
{
    return(c == ' ' || c == '\t');
}

int is_printable(char c)
{
    return(c >= 33 && c <= 126);
}

void last_word(char *s)
{
    int j = 0;
    for(int i = 0; s[i]; i++)
    {
        if(is_space(s[i]) && is_printable(s[i + 1]))
            j = i + 1;
    }
    while(is_printable(s[j]))
    {
        write(1, &s[j], 1);
        j++;
    }
}


int main(int argc, char **argv)
{
    if(argc == 2)
        last_word(argv[1]);
    write(1, "\n", 1);
    return 0;
}