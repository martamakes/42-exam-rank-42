#include <unistd.h>
#include <stdio.h>

int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

int main(int argc, char **argv)
{
     if (argc != 2)
    {
        write(1, "\n", 1);
        return 0;
    }
    int i = 0;
    while (argv[1][i] && is_space(argv[1][i])) i++;
    int first_word_start = i;
    while (argv[1][i] && !is_space(argv[1][i])) i++;
    int first_word_end = i;

    int word_printed = 0;

    while(argv[1][i])
    {
        if(!is_space(argv[1][i]))
        {
            if(word_printed)
                write(1, " ", 1);
            while(argv[1][i] && !is_space(argv[1][i]))
            {
                write(1, &argv[1][i], 1);
                i++;
            }
            word_printed = 1;
        }
        else
            i++;
    }

    if(first_word_end > first_word_start)
    {
        if(word_printed)
            write(1, " ", 1);
        write(1, &argv[1][first_word_start], first_word_end - first_word_start);
    }
    write(1, "\n", 1);
    return 0;
}