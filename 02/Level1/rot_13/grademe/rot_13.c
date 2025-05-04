#include <unistd.h>
#include <stdio.h>

int main (int argc, char **argv)
{
    if(argc == 2)
    {
        int i = 0;
        while(argv[1][i])
        {
            if((argv[1][i] >= 'a' && argv[1][i] <= 'z') || (argv[1][i] >= 'A' && argv[1][i] <= 'Z'))
            {
                
                char c = argv[1][i];
                if((argv[1][i] >= 'a' && argv[1][i] <= 'm') || (argv[1][i] >= 'A' && argv[1][i] <= 'M'))
                     c += 13;
                if((argv[1][i] >= 'n' && argv[1][i] <= 'z') || (argv[1][i] >= 'N' && argv[1][i] <= 'Z'))
                     c -= 13;
                write(1, &c, 1);
            }
            else
            {
                write(1, &argv[1][i], 1);
            }
            i++;        
        }

    }
    write(1, "\n", 1);
    return 0;
}