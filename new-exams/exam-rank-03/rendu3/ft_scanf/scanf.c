#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

int match_space(FILE *f)
{
    int c;
    while ( (c = fgetc (f)) != EOF && isspace(c)) { }
    if (c != EOF)
        ungetc(c, f);   //Devuelvo el caracter no-espacio.
    return (0);
}

int match_char(FILE *f, char c)
{
	int n = fgetc (f);
    if (n == c)
		return 1;		// cuando coinciden → devuelve 1 (éxito)
	ungetc (n, f);
	return 0;			// cuando NO coinciden → devuelve 0 (fallo)
}

int scan_char(FILE *f, va_list ap)
{
	char *ptr = va_arg(ap, char*);  // Obtener dirección donde guardar
    int c = fgetc(f);               // Leer carácter (validos de 0-255 y -1 que es EOF-error/fin)
									// c puede ser 'A' (65) o EOF (-1)
    if (c == EOF)                   // Si c == -1
		return 0;                   // .... termina (no hay más carácteres)
	*ptr = c;                       // c es válido (0 - 255), se convierte automáticamente de int a char cuando se asigna
									// Guardar en la dirección
		return 1;					// Éxito
}

int scan_int(FILE *f, va_list ap)
{
    int *ptr = va_arg(ap, int*);		//obtenemos el puntero destino.
    int result = 0;						//número final.
	int c = fgetc(f);					//lee ('1', '-', 'a', etc)
	
	// El primer caracter, si no es un dígito
      if (!isdigit(c)) 
	  {
          ungetc(c, f);		// "Devuelve" el carácter al archivo para que otro lo lea
          return 0;			// indica que es un fallo (no es un digito)
      }

	// Bucle: construir número dígito a dígito
      while (isdigit(c)) 
	  {
          result = result * 10 + (c - '0');  // Agregar dígito
          c = fgetc(f);						// lee el siguiente caracter (c = num actual y el cursor se pone en el siguiente caracter automáticamente)
      }
	
	ungetc(c, f);  		// Devolver último carácter no-dígito, para la siguiente lectura, por si se necesita en otro sitio.
	*ptr = result;		//guarda el núm final en la dirección que nos dio el usuario
      return 1;			// éxito.
}

int scan_string(FILE *f, va_list ap)
{
	char *ptr = va_arg(ap, char*);		//destino del string.
	int i = 0;							//índice para el array.
    int c = fgetc(f);

	if (c == EOF || isspace(c)) 		//verificar el primer carácter
	{
		ungetc(c, f);					//se devuelve si no es válido para la siguiente comprobación
		return 0;						// fallo, no es válido para esta función
	}

	while (c != EOF && !isspace(c)) 
	{ 
		ptr[i] = c;        // ← Guardar directamente en ptr[i]
		i++;
		c = fgetc(f);
	}
	ptr[i] = '\0';			// Terminar string
	ungetc(c, f);			// Devolver carácter no válido
	return 1;				// Éxito

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
	int nconv = 0;								// Contador de conversiones exitosas

	
	int c = fgetc(f);							// ¿Hay contenido que leer?
	if (c == EOF)								// Si no hay nada...
		return EOF;								// ...devuelve EOF
	ungetc(c, f);								// Si hay algo, devuélvelo

	// Bucle principal: procesa cada carácter del formato
	while (*format)
	{
		if (*format == '%')						// ¿Es una conversión (%d, %c, %s)?
		{
			format++;							// Salta el '%'
			if (match_conv(f, &format, ap) != 1) // ¿Conversión exitosa?
				break;							// Si falla, termina
			else
				nconv++;						// Si funciona, cuenta +1
		}
		else if (isspace(*format))				// ¿Es un espacio en el formato?
		{
			if (match_space(f) == -1)			// Salta espacios del input
				break;							// Si hay error, termina
		}
		else if (match_char(f, *format) != 1)	// ¿Carácter literal (como ',' o '.')?
			break;								// Si no coincide, termina
		format++;								// Avanza al siguiente carácter del formato
	}
	
	// Verificación final
	if (ferror(f))								// ¿Hubo error de lectura?
		return EOF;								// Error
	return nconv;								// Devuelve cuántas conversiones funcionaron
}


int ft_scanf(const char *format, ...)
{
	va_list ap;                    // Declarar// ...
	va_start(ap, format);          // Inicializar
	int ret = ft_vfscanf(stdin, format, ap);
	va_end(ap);                    // Limpiar// ...
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
*//*
int main() {
      int n;
      char c;
      char s[100];
      ft_scanf("%d %c %s", &n, &c, s);
      printf("%d %c %s\n", n, c, s);
      return 0;
  }
*/