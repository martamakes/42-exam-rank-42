#include <unistd.h>

void to_upper(char c)
{
   char uc = c - 32;
    write(1, &uc, 1);
}

void to_lower(char c)
{
    char lc = c + 32;
    write(1, &lc, 1);
}
int is_char(char c)
{
    return ((c >= 'a' && c<= 'z') || (c >= 'A' && c<= 'Z'));
}
int main(int argc, char **argv)
{
    int i = 0;
    if(argc == 2)
    {
        while(argv[1][i])
        {
            if(!is_char(argv[1][i])) write(1, &argv[1][i], 1);            
            if(argv[1][i] >= 'a' && argv[1][i] <= 'z') to_upper(argv[1][i]);
            else if (argv[1][i] >= 'A' && argv[1][i] <= 'Z') to_lower(argv[1][i]);
            i++;
        }

    }

    write(1, "\n", 1);
    return 0;
}
