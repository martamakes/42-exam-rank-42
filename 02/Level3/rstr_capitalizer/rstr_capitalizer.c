#include <unistd.h>

// Función para verificar si un carácter es una letra
int	is_letter(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// Función para convertir un carácter a minúscula
char	to_lowercase(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

// Función para convertir un carácter a mayúscula
char	to_uppercase(char c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

// Función para capitalizar la última letra de cada palabra
void	rstr_capitalizer(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		// Convertir el carácter actual a minúscula
		str[i] = to_lowercase(str[i]);
		// Si es una letra y el siguiente carácter es un espacio, tabulación o el final de la cadena
		if (is_letter(str[i]) && (str[i + 1] == ' ' || str[i + 1] == '\t' || str[i + 1] == '\0'))
			str[i] = to_uppercase(str[i]);
		write(1, &str[i], 1);
		i++;
	}
}

int	main(int argc, char **argv)
{
	int	i;

	// Si no hay argumentos, imprimir un salto de línea y salir
	if (argc == 1)
	{
		write(1, "\n", 1);
		return (0);
	}
	// Procesar cada argumento
	i = 1;
	while (i < argc)
	{
		rstr_capitalizer(argv[i]);
		// Imprimir un salto de línea solo después del último argumento
		if (i == argc - 1)
			write(1, "\n", 1);
        else
            write(1, " ", 1);
		i++;
	}
	return (0);
}