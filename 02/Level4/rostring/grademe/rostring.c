#include <unistd.h>
#include <stdlib.h>

int is_space(char c)
{
    return(c == ' '|| c == '\t');
}

void rostring(char *s)
{
    int i = 0, end = 0, start = 0, fend = 0, fstart = 0;

    while(s[i] && is_space(s[i]))
        i++;
    fstart = i;
    while(s[i] &&  !is_space(s[i]))
        i++;
    fend = i;

    while(s[i])
    {
        while(s[i] && is_space(s[i]))
            i++;
        start = i;
        while(s[i] && !is_space(s[i]))
            i++;
        end = i;
        if(start < end)
        {
            write(1, s + start, end - start);
            write(1, " ", 1);
        }
    }
   
    write(1, s + fstart, fend - fstart);

}

int main(int argc, char **argv)
{
    if(argc >= 2)
        rostring(argv[1]);
    write(1, "\n", 1);
    return 0;
}