#include <stdio.h>
#include <stdlib.h>

char **ft_split(char *str);

int main()
{
    char *str = "Hello  world\tThis is\na test";
    char **result = ft_split(str);

    if (result)
    {
        for (int i = 0; result[i] != NULL; i++)
            printf("%s\n", result[i]);

        // Liberar memoria
        for (int i = 0; result[i] != NULL; i++)
            free(result[i]);
        free(result);
    }

    return 0;
}