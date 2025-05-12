#include <unistd.h>
#include <stdlib.h>

char *hidenp(char *s1, char *s2)
{
    int j = 0;
    for(int i = 0; s2[i]; i++)
    {
        if(s1[j] == s2[i])
            j++;
    }
    if(s1[j] == '\0')
        return s1;
    else
        return NULL;
}


int main(int argc, char **argv)
{
    if(argc == 3)
    {
        if(argv[1][0] == '\0')
        {
            write(1, "1\n", 2);
            return 0;
        }
        char *s;
        s= hidenp(argv[1], argv[2]);
        if(!s)
            write(1, "0", 1);
        else
        {
            if(*s)
                write(1, "1", 1);
        }
    }
    write(1,"\n", 1);
    return 0;
}