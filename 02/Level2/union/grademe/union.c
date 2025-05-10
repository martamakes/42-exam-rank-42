#include <unistd.h>

/* Function to print union of two strings without duplicates */
void	ft_union(char *s1, char *s2)
{
	int		i;
	char	seen[256] = {0}; // Array to track seen characters (ASCII)

	/* Process first string */
	i = 0;
	while (s1[i])
	{
		/* If character hasn't been seen before */
		if (!seen[(unsigned char)s1[i]])
		{
			write(1, &s1[i], 1); // Print the character
			seen[(unsigned char)s1[i]] = 1; // Mark as seen
		}
		i++;
	}

	/* Process second string */
	i = 0;
	while (s2[i])
	{
		/* If character hasn't been seen before */
		if (!seen[(unsigned char)s2[i]])
		{
			write(1, &s2[i], 1); // Print the character
			seen[(unsigned char)s2[i]] = 1; // Mark as seen
		}
		i++;
	}
}

/* Main function to handle program execution */
int	main(int argc, char **argv)
{
	/* Check if exactly 2 arguments are provided */
	if (argc == 3)
		ft_union(argv[1], argv[2]); // Call union function
	write(1, "\n", 1); // Print newline
	return (0);
}
