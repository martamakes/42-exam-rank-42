#include "argo.h"

// ─────────────────────────────────────────────────────────────
// PASO 1: parsear un string  →  "hola"  →  guarda: hola
// ─────────────────────────────────────────────────────────────
static int	parse_string(char **out, FILE *stream)
{
	size_t	len;
	size_t	cap;
	char	*buf;
	char	*tmp;
	int		c;

	// 1. Consumir la comilla de apertura
	if (!accept(stream, '"'))
	{
		unexpected(stream);
		return -1;
	}

	// 2. Preparar buffer dinámico
	len = 0;
	cap = 16;
	buf = malloc(cap);
	if (!buf)
		return -1;

	// 3. Leer chars hasta la comilla de cierre
	while ((c = getc(stream)) != '"')
	{
		// Error: fin de archivo sin cerrar el string
		if (c == EOF)
		{
			free(buf);
			unexpected(stream);
			return -1;
		}

		// Escape: si leemos '\', el siguiente char es el carácter real
		if (c == '\\')
		{
			c = getc(stream);
			// Solo se permiten \" y \\ como escapes
			if (c != '"' && c != '\\')
			{
				free(buf);
				unexpected(stream);
				return -1;
			}
		}

		// 4. Crecer el buffer si está lleno
		// (+2 para tener hueco para c y para el '\0' final)
		if (len + 2 > cap)
		{
			cap = cap * 2;
			tmp = realloc(buf, cap);
			if (!tmp)
			{
				free(buf);
				return -1;
			}
			buf = tmp;
		}

		// 5. Guardar el char
		buf[len] = c;
		len++;
	}

	// 6. Terminar el string y devolver
	buf[len] = '\0';
	*out = buf;
	return 1;
}

// ─────────────────────────────────────────────────────────────
// PASO 2: parsear un número  →  42  →  guarda: 42
// ─────────────────────────────────────────────────────────────
static int	parse_integer(json *dst, FILE *stream)
{
	dst->type = INTEGER;
	if (fscanf(stream, "%d", &dst->integer) != 1)
	{
		unexpected(stream);
		return -1;
	}
	return 1;
}

// ─────────────────────────────────────────────────────────────
// PASO 3: parsear un mapa  →  {"clave":valor, "clave2":valor2}
// ─────────────────────────────────────────────────────────────
static int	parse_map(json *dst, FILE *stream)
{
	pair	*tmp;

	// 1. Consumir '{'
	getc(stream);
	dst->type = MAP;
	dst->map.data = NULL;
	dst->map.size = 0;

	// 2. Caso especial: mapa vacío {}
	if (accept(stream, '}'))
		return 1;

	// 3. Leer pares "clave":valor mientras haya comas
	while (1)
	{
		// 3a. Hacer sitio para un par más
		tmp = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		if (!tmp)
			return -1;
		dst->map.data = tmp;

		// 3b. Parsear la clave (siempre es un string)
		if (parse_string(&dst->map.data[dst->map.size].key, stream) != 1)
			return -1;

		// 3c. Consumir ':'
		if (!expect(stream, ':'))
			return -1;

		// 3d. Parsear el valor (llama a argo → puede ser cualquier tipo)
		if (argo(&dst->map.data[dst->map.size].value, stream) != 1)
			return -1;

		// 3e. Par guardado
		dst->map.size++;

		// 3f. ¿Hay más pares? Si no hay ',', salimos
		if (!accept(stream, ','))
			break ;
	}

	// 4. Consumir '}' final
	if (!expect(stream, '}'))
		return -1;
	return 1;
}

// ─────────────────────────────────────────────────────────────
// ENTRADA PRINCIPAL: mira el primer char y decide qué parsear
// ─────────────────────────────────────────────────────────────
int	argo(json *dst, FILE *stream)
{
	int	c;

	c = peek(stream); // ver el siguiente char SIN consumirlo

	if (c == '"')
	{
		dst->type = STRING;
		dst->string = NULL;
		return parse_string(&dst->string, stream);
	}
	if (isdigit(c) || c == '-')
		return parse_integer(dst, stream);
	if (c == '{')
		return parse_map(dst, stream);

	// Ningún tipo reconocido → error
	unexpected(stream);
	return -1;
}
