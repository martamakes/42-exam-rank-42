#include <unistd.h>

void epur_str(char *str)
{
    int i = 0;
    int space = 0; // Flag to track if we need to add a space
    
    // Skip leading spaces and tabs
    while (str[i] == ' ' || str[i] == '\t')
        i++;
        
    // Process the string
    while (str[i])
    {
        // If current character is a space or tab
        if (str[i] == ' ' || str[i] == '\t')
        {
            space = 1; // Mark that we need to add a space
        }
        else
        {
            // If we need to add a space before this word
            if (space)
            {
                write(1, " ", 1);
                space = 0;
            }
            write(1, &str[i], 1); // Write the current character
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    // Check if we have exactly one argument
    if (argc == 2)
    {
        epur_str(argv[1]);
    }
    write(1, "\n", 1);
    return 0;
}
