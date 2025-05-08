#include <unistd.h>

void sr(char *s, char c1, char c2)
{
    for(int i = 0; s[i]; i++)
    {
        if(s[i] == c1)
            write(1, &c2, 1);
        else
            write(1, &s[i], 1);
    }
}
 int f_len(char *s)
 {
    int l = 0;
    while(s[l])
        l++;
    return l;
 }

int main(int argc, char **argv)
{
    if(argc == 4 && (f_len(argv[2]) == 1) && (f_len(argv[3]) == 1))
    {
        sr(argv[1], argv[2][0], argv[3][0]);
    }
    write(1, "\n", 1);
    return 0;
}