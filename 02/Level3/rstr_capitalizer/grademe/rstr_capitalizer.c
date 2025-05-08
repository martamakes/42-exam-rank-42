#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void rstr_capitalizer(char *s)
{
    int i = 0;
    
    while (s[i])
    {
        // Make all letters lowercase
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 32;
            
        // If current char is a letter and next is space/tab/end, capitalize it
        if ((s[i] >= 'a' && s[i] <= 'z') &&
            (s[i + 1] == ' ' || s[i + 1] == '\t' || s[i + 1] == '\0'))
            s[i] -= 32;
            
        write(1, &s[i], 1);
        i++;
    }
    write(1, "\n", 1);
}


int main(int argc, char **argv)
{
    if(argc >= 2)
    {
        for(int i = 1; argv[i]; i++)
            rstr_capitalizer(argv[i]);
        
    }
    else
        write(1, "\n", 1);
    return 0;
}
