#include <unistd.h>

int is_char(char c)
{
    return((c>= 'a' && c< 'z') || (c>= 'A' && c< 'Z'));
}

void rotone(char *s)
{
    char c;
    for(int i = 0; s[i]; i++)
    {
        if(is_char(s[i]))
            c = s[i] + 1;
        else if(s[i] == 'z')
            c = 'a';
        else if(s[i] == 'Z')
            c = 'A';
        else
            c = s[i];
        write(1, &c, 1);       
    }
}

int main(int argc, char **argv)
{
    if(argc == 2)
        rotone(argv[1]);
    write(1,"\n", 1);
    return 0;
}