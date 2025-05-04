#include <unistd.h>

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        write(1, "\n", 1);
        return 1;
    }
    if(argv[1][0] == '\0' && argv[2])
    {
        write(1, "1\n", 2);
        return 0;
    }
    int i = 0;
    int j = 0;
    
    while(argv[1][i] && argv[2][j])
    {
        if(argv[1][i] == argv[2][j])
            i++;
        j++;
    }
    if(argv[1][i] == '\0')
        write(1, "1\n", 2);
    else
        write(1, "0\n", 2);
    return 0;
}