#include <unistd.h>

void	print_hex(unsigned int num)
{
	char	hex_digits[] = "0123456789abcdef";
	char	hex_char;
	
	if (num >= 16)
		print_hex(num / 16);
	hex_char = hex_digits[num % 16];
	write(1, &hex_char, 1);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		write(1, "\n", 1);
		return (0);
	}
	
	unsigned int num = 0;
	int i = 0;
	
	// Convert string to number
	while (argv[1][i])
	{
		num = num * 10 + (argv[1][i] - '0');
		i++;
	}
	
	// Print hexadecimal number
	print_hex(num);
	write(1, "\n", 1);
	return (0);
}
