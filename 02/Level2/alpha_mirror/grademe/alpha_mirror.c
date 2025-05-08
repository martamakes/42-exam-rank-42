#include <unistd.h>

void process_char(char c)
{
    if(c >= 'a' && c <= 'm')
        c = 'z' - (c - 'a');
    else if(c >= 'A' && c <= 'M')
        c = 'Z' - (c - 'A');
    else if(c >= 'N' && c <= 'Z')
        c = 'A' - (c - 'Z');
    else if(c >= 'n' && c <= 'z')
        c = 'a' - (c - 'z');
    write(1, &c, 1);

}

int main(int argc, char **argv)
{
    if(argc == 2)
    {
        int i = 0;
        while(argv[1][i])
        {
            process_char(argv[1][i]);
            i++;
        }

    }
    write(1, "\n", 1);
    return 0;
}