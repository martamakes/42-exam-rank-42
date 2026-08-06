#include "argo.h"


static int parse_string(char **dst, FILE *stream)
{
	if(!accept(stream, '"'))
	{
		unexpected(stream);
		return -1;
	}

	char *buf = NULL;
	char *tmp;
	int len = 0, buf_size = 42;
	buf = malloc(buf_size);
	if(!buf)
		return -1;
	
	int c;
	while((c = getc(stream)) != '"')
	{
		if(c == EOF)
		{
			free(buf);
			unexpected(stream);
			return -1;
		}
		if (c == '\\')
		{
			c = getc(stream);
			if(c != '\\' && c != '"')
			{
				free(buf);
				unexpected(stream);
				return -1;
			}
		}
		if(len + 2 > buf_size)
		{
			buf_size *= 2;
			tmp = realloc(buf, buf_size);
			if(!tmp)
			{
				free(buf);
				return -1;
			}
			buf = tmp;
		}
		buf[len] = c;
		len++;
	}
	buf[len] = '\0';
	*dst = buf;
	return 1;
}
static int parse_map(json *dst, FILE *stream)
{
	if(!accept(stream, '{'))
	{
		unexpected(stream);
		return -1;
	}
	if(accept(stream, '}')) return 1;
	pair *tmp;
	while(1)
	{
		tmp = realloc(dst->map.data, (dst->map.size + 1)*sizeof(pair));
		if(!tmp)
			return -1;
		dst->map.data = tmp;
		if(parse_string(&dst->map.data[dst->map.size].key, stream) != 1)
			return -1;

		if(!expect(stream, ':')) return -1;

		if(argo(&dst->map.data[dst->map.size].value, stream) != 1)
			return -1;
		dst->map.size++;

		if (!accept(stream, ','))
			break ;

	}
	if (!expect(stream, '}'))
		return -1;
	return 1;
}
int argo(json *dst, FILE *stream)
{
	int c;
	c = peek(stream);
	if (c == '"')
	{
		dst->type = STRING;
		dst->string = NULL;
		return (parse_string(&dst->string, stream));
	}
	if (c == '-' || isdigit(c))
	{
		dst->type = INTEGER;
		if(fscanf(stream, "%d", &dst->integer) != 1)
		{
			unexpected(stream);
			return -1;
		}
		return 1;
	}
	if (c == '{')
	{
		dst->type = MAP;
		dst->map.data = NULL;
		dst->map.size = 0;
		return (parse_map(dst, stream));
	}
	unexpected(stream);
	return -1;
}