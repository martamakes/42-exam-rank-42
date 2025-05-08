#include <unistd.h>

void process_char(char c)
{
    if(c == 'z')
        write(1, "a", 1);
    else if (c == 'Z')
        write(1, "A", 1);
    else if(c >= 'a' && c <= 'y')
    {
        c += 1;
        write(1, &c, 1);
    }
    else if(c >= 'A' && c <= 'Y')
    {
        c += 1;
        write(1, &c, 1);
    }
}

int is_alpha(char c)
{
    return( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') );
}

void rotone(char *c)
{
    int i = 0;
    while(c[i])
    {
        if(is_alpha(c[i]))
            process_char(c[i]);
        if(!is_alpha(c[i]))
            write(1, &c[i], 1);
        i++;
    }

}

int main(int argc, char **argv)
{
    if(argc == 2)
    {
        rotone(argv[1]);
    }
    write(1, "\n", 1);
    return 0;
}