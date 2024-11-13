#include <unistd.h>

int is_alpha(char c)
{
    return((c >= 'A' && c<= 'Z') ||(c >= 'a' && c<= 'z'));
}
char rotone(char c)
{
    if (c == 'z')
        return ('a');
    else if (c == 'Z')
        return ('A');
    else if (is_alpha(c))
        return (c + 1);
    return (c);
}
int main(int argc, char **argv)
{
    int i  = 0;
    if(argc == 2)
    {
        while(argv[1][i])
        {
            char rotated = rotone(argv[1][i]);
            write(1, &rotated, 1);
            i++;
        }
    }
    write(1, "\n", 1);
}