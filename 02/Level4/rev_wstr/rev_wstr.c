#include <stdlib.h>
#include <unistd.h>

int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

void rev(char *s)
{
    int i  = 0;
    while(s[i]) i++;
    i--;
    int end = 0, start = 0;
    int first_word = 1;
    while(i >= 0 && is_space(s[i])) i--;
    while( i >= 0)
    {
        while(i >= 0 && is_space(s[i])) i--;
        if(i < 0) break;
        end = i + 1;
        while(i >= 0 && !is_space(s[i])) i--;
        start = i + 1;
        if(start < end)
        {
            if(!first_word)
                write(1, " ", 1);
            write(1, &s[start], end - start);
            first_word = 0;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
        rev(argv[1]);
    write(1, "\n", 1);
    return 0;
}