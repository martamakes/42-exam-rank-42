#include <unistd.h>

int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

void rev_wstr(char *s)
{
    int i = 0;
    while(s[i])
        i++;
    int end, start;
    i--;
    while(i >= 0)
    {
        while(i >= 0 && is_space(s[i]))
            i--;
        end = i + 1;
        while(i >= 0 && !is_space(s[i]))
            i--;
        start = i + 1;
        if(end > start)
            write(1, &s[start], end - start);
        if ( i > 0)
            write(1, " ", 1);
    }
}

int main(int argc, char **argv)
{
    if(argc == 2)
        rev_wstr(argv[1]);
    write(1, "\n", 1);
    return 0;
}
