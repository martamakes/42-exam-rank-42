#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    // Check if we have exactly 3 arguments (program name + 3 parameters)
    if (argc != 4)
    {
        printf("\n");  // Print newline if incorrect number of arguments
        return (0);
    }

    // Convert first and third arguments to integers using atoi
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[3]);
    
    // Get the operator from the second argument
    char operator = argv[2][0];  // We only need the first character

    // Perform the operation based on the operator
    int result = 0;
    if (operator == '+')
        result = num1 + num2;
    else if (operator == '-')
        result = num1 - num2;
    else if (operator == '*')
        result = num1 * num2;
    else if (operator == '/')
    {
        // Handle division by zero
        if (num2 == 0)
        {
            printf("\n");
            return (0);
        }
        result = num1 / num2;
    }
    else if (operator == '%')
    {
        // Handle modulo by zero
        if (num2 == 0)
        {
            printf("\n");
            return (0);
        }
        result = num1 % num2;
    }
    else
    {
        // If operator is invalid, just print newline
        printf("\n");
        return (0);
    }

    // Print the result followed by newline
    printf("%d\n", result);
    
    return (0);
}
