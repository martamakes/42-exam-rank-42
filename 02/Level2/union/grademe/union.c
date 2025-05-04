#include <unistd.h>

//mark as seen
void mark_seen(char *seen, char c)
{
    seen[(unsigned char)c] = 1;
}
//has it been seen?
int has_been_seen(char *seen, char c)
{
    return(seen[(unsigned char)c] == 1);
}
//process string
void process_string(char *seen, char *str)
{
    int i = 0;
    
    while (str[i])
    {
        if (!has_been_seen(seen, str[i]))
        {
            write(1, &str[i], 1);
            mark_seen(seen, str[i]);
        }
        i++;
    }
}

int main (int argc, char **argv)
{
    if(argc == 3)
    {
        //array para llevar el cómputo de chars vistos
        char seen[256] = {0};

        //process_string 1
        process_string(seen, argv[1]);
        //process_string 2
        process_string(seen, argv[2]);

    }
    write(1, "\n", 1);
    return 0;
}
