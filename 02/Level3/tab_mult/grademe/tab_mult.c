#include <unistd.h>

// Function to print a number using write
void	print_num(int num)
{
	char	c;

	if (num > 9)
		print_num(num / 10);
	c = num % 10 + '0';
	write(1, &c, 1);
}

// Function to print a string using write
void	print_str(char *str)
{
	while (*str)
		write(1, str++, 1);
}

int	main(int argc, char **argv)
{
	int	num;
	int	i;

	// If no arguments, just print newline
	if (argc != 2)
	{
		write(1, "\n", 1);
		return (0);
	}

	// Convert argument to integer
	num = 0;
	i = 0;
	while (argv[1][i])
	{
		num = num * 10 + (argv[1][i] - '0');
		i++;
	}

	// Print multiplication table from 1 to 9
	i = 1;
	while (i <= 9)
	{
		print_num(i);
		print_str(" x ");
		print_num(num);
		print_str(" = ");
		print_num(i * num);
		print_str("\n");
		i++;
	}
	return (0);
}
