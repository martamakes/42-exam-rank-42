#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef struct	json {
	enum {
		MAP,
		INTEGER,
		STRING
	} type;
	union {
		struct {
			struct pair	*data;
			size_t		size;
		} map;
		int	integer;
		char	*string;
	};
}	json;

typedef struct	pair {
	char	*key;
	json	value;
}	pair;

// Funciones auxiliares dadas
int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

void	free_json(json j)
{
	switch (j.type)
	{
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break ;
		case STRING:
			free(j.string);
			break ;
		default:
			break ;
	}
}


// Forward declarations
int parse_value(json *dst, FILE *stream);

// Parse integer
int parse_integer(json *dst, FILE *stream)
{
	int value = 0;
	bool negative = false;
	
	// Handle negative numbers
	if (accept(stream, '-'))
		negative = true;
	
	if (!isdigit(peek(stream)))
	{
		unexpected(stream);
		return -1;
	}
	
	// Parse digits
	while (isdigit(peek(stream)))
	{
		value = value * 10 + (getc(stream) - '0');
	}
	
	dst->type = INTEGER;
	dst->integer = negative ? -value : value;
	return 1;
}

// Parse string with escaping
int parse_string(json *dst, FILE *stream)
{
	if (!expect(stream, '"'))
		return -1;
	
	char *str = malloc(1024); // Reasonable buffer size
	if (!str)
		return -1;
	
	int pos = 0;
	int c;
	
	while ((c = peek(stream)) != '"' && c != EOF)
	{
		if (c == '\\')
		{
			getc(stream); // consume backslash
			c = getc(stream);
			if (c == EOF)
			{
				free(str);
				unexpected(stream);
				return -1;
			}
			// Only handle quote and backslash escapes
			if (c == '"' || c == '\\')
				str[pos++] = c;
			else
			{
				// Other escapes not handled, treat as literal
				str[pos++] = '\\';
				str[pos++] = c;
			}
		}
		else
		{
			str[pos++] = getc(stream);
		}
		
		if (pos >= 1023) // Prevent buffer overflow
		{
			free(str);
			unexpected(stream);
			return -1;
		}
	}
	
	if (!expect(stream, '"'))
	{
		free(str);
		return -1;
	}
	
	str[pos] = '\0';
	dst->type = STRING;
	dst->string = str;
	return 1;
}

// Parse object/map
int parse_object(json *dst, FILE *stream)
{
	if (!expect(stream, '{'))
		return -1;
	
	dst->type = MAP;
	dst->map.data = NULL;
	dst->map.size = 0;
	
	// Empty object
	if (accept(stream, '}'))
		return 1;
	
	while (1)
	{
		// Parse key (must be string)
		json key_json;
		if (parse_string(&key_json, stream) != 1)
		{
			free_json(*dst);
			return -1;
		}
		
		// Expect colon
		if (!expect(stream, ':'))
		{
			free_json(key_json);
			free_json(*dst);
			return -1;
		}
		
		// Parse value
		json value_json;
		if (parse_value(&value_json, stream) != 1)
		{
			free_json(key_json);
			free_json(*dst);
			return -1;
		}
		
		// Add pair to array
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		if (!dst->map.data)
		{
			free_json(key_json);
			free_json(value_json);
			return -1;
		}
		
		dst->map.data[dst->map.size].key = key_json.string;
		dst->map.data[dst->map.size].value = value_json;
		dst->map.size++;
		
		// Check for continuation or end
		if (accept(stream, '}'))
			break;
		
		if (!expect(stream, ','))
		{
			free_json(*dst);
			return -1;
		}
	}
	
	return 1;
}

// Parse any JSON value
int parse_value(json *dst, FILE *stream)
{
	int c = peek(stream);
	
	if (c == '"')
		return parse_string(dst, stream);
	else if (isdigit(c) || c == '-')
		return parse_integer(dst, stream);
	else if (c == '{')
		return parse_object(dst, stream);
	else
	{
		unexpected(stream);
		return -1;
	}
}

// Main parsing function
int argo(json *dst, FILE *stream)
{
	if (!dst || !stream)
		return -1;
	
	return parse_value(dst, stream);
}

