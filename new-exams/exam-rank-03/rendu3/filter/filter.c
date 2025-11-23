#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 42
int main (int ac, char **av)
{
    if (ac !=2 || av[1][0] == '\0')
        return 1;

    char tmp [BUFFER_SIZE];
    int total_read = 0;

    char *result = NULL;
    ssize_t bytes;
    char *buffer;

    while ((bytes = read(0, tmp, BUFFER_SIZE)) > 0)
    {
        buffer = realloc (result, total_read + bytes);
        if (!buffer)
        {
            free(result);
            perror("Error: ");
            return 1;
        }
        result = buffer;
        memmove (result + total_read, tmp, bytes);
        total_read += bytes;

    }
    if (bytes < 0)
    {
            free(result);
            perror("Error: ");
            return 1;
    }

    if (!result)
		return 0;
	
	
    int len = strlen (av[1]);
    int i = 0;
    while (i < total_read)
    {
        char *filter = memmem (result + i, total_read - i, av[1], len);
        if (filter == NULL)
        {
            write (1, result + i, total_read - i);
            break;                                  // ✅ Sal del while (no del programa todavía)
        }
        write (1, result + i, filter - (result + i));

        for (int j = 0; j < len; j++)
                write (1, "*", 1);

            i = (filter - result) + len;
    }
    free (result);
    return 0;    

}



/*
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 42
int main (int ac, char **av)
{
    if (ac !=2 || av[1][0] == '\0')
        return 1;

    char tmp [BUFFER_SIZE];
    int total_read = 0;

    char *result = NULL;
    ssize_t bytes;
    char *buffer;

    while ((bytes = read(0, tmp, BUFFER_SIZE)) > 0)
    {
        buffer = realloc (result, total_read + bytes + 1);
        if (!buffer)
        {
            free(result);
            perror("Error: ");
            return 1;
        }
        result = buffer;
        memmove (result + total_read, tmp, bytes);
        total_read += bytes;
        result[total_read] = '\0';

    }
    if (bytes < 0)
    {
            free(result);
            perror("Error: ");
            return 1;
    }

    if (!result)
    {
            return 0;
    }

    int len = strlen (av[1]);
    int i = 0;
    while (i < total_read)
    {
        char *filter = memmem (result + i, total_read - i, av[1], len);
        if (filter == NULL)
        {
            write (1, result + i, total_read - i);
            break;
        }
        write (1, result + i, filter - (result + i));

        for (int j = 0; j < len; j++)
                write (1, "*", 1);

            i = (filter - result) + len;
    }
    free (result);
    return 0;    

}*/