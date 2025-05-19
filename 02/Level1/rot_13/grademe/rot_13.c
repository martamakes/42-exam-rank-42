#include <unistd.h>

char rot13(char c)
{
    if((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c += 13;
    else if((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c -= 13;
    return c;
}

void rot(char *s)
{
    while(*s)
    {
        char c = rot13(*s);
        write(1, &c, 1);
        s++;
    }
}

int main(int argc, char **argv)
{
    if(argc == 2)
        rot(argv[1]);
    write(1, "\n", 1);
    return 0;
}