#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

int match_space(FILE *f)
{
		// You may insert code here
	int c;
	while ((c = fgetc(f)) != EOF && isspace (c)) {} ;
	if (c != EOF)
		ungetc (c, f);
	else
		return -1;
	return (0);
}

int match_char(FILE *f, char c)
{
		// You may insert code here
	int n = fgetc (f);
	if (c == n)
		return 1;
	 
	ungetc(n, f);
	return (0);
}

int scan_char(FILE *f, va_list ap)
{
		// You may insert code here
	int c = fgetc (f);
	char *ptr = va_arg(ap, char*);

	if (c == EOF)
		return 0;
	*ptr = c;
	return 1;
}

int scan_int(FILE *f, va_list ap)
{
		// You may insert code here

	int sign = 1;
	int c = fgetc(f);

	int *ptr = va_arg(ap, int *);
	int result = 0;

	if (c == '-' || c == '+')
	{
		sign = (c == '-') ? -1 : 1;
		c = fgetc(f); 
	}
	if (!isdigit(c))
	{
		ungetc(c, f);
		return 0;
	}
	
	while (isdigit(c))
	{
		result = result*10 + (c - '0');
		c =fgetc(f);
	}
	ungetc(c, f);
	
	*ptr = result * sign;
	return 1;
}

int scan_string(FILE *f, va_list ap)
{
		// You may insert code here
	int c = fgetc(f);

	char *ptr = va_arg(ap, char*);
	int i = 0;

	if (c == EOF || isspace(c))  // Añade esto
    {
        ungetc(c, f);
        return 0;
    }

	while (c != EOF && !isspace(c))
	{
		ptr[i] = c;
		i++;
		c = fgetc(f);
	}
	ptr[i] = '\0';
	ungetc(c,f);
	return 1;
}


int	match_conv(FILE *f, const char **format, va_list ap)
{
	switch (**format)
	{
		case 'c':
			return scan_char(f, ap);
		case 'd':
			match_space(f);
			return scan_int(f, ap);
		case 's':
			match_space(f);
			return scan_string(f, ap);
		case EOF:
			return -1;
		default:
			return -1;
	}
}

int ft_vfscanf(FILE *f, const char *format, va_list ap)
{
	int nconv = 0;

	int c = fgetc(f);
	if (c == EOF)
		return EOF;
	ungetc(c, f);

	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (match_conv(f, &format, ap) != 1)
				break;
			else
				nconv++;
		}
		else if (isspace(*format))
		{
			if (match_space(f) == -1)
				break;
		}
		else if (match_char(f, *format) != 1)
			break;
		format++;
	}
	
	if (ferror(f))
		return EOF;
	return nconv;
}


int ft_scanf(const char *format, ...)
{
	// ...
	va_list ap;
	va_start (ap, format);

	int ret = ft_vfscanf(stdin, format, ap);
	// ...
	va_end (ap);
	return ret;
}
/*
int main ()
{
	int num;
	char letra;
	char palabra [100];

	printf ("Prueba:");
	ft_scanf("%d %c %s", &num, &letra, palabra);
	printf("Resultado: %d %c %s", num, letra, palabra);
	return 0;
}
	*/