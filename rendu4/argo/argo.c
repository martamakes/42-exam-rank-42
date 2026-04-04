#include "argo.h"

static int	parse_string(char **out, FILE *stream)
{
	size_t	len;
	size_t	cap;
	char	*buf;
	char	*tmp;
	int		c;

	if (!accept(stream, '"'))
		return (unexpected(stream), -1);
	len = 0;
	cap = 16;
	buf = malloc(cap);
	if (!buf)
		return -1;
	while ((c = getc(stream)) != '"')
	{
		if (c == EOF)
			return (free(buf), unexpected(stream), -1);
		if (c == '\\')
		{
			c = getc(stream);
			if (c != '"' && c != '\\')
				return (free(buf), unexpected(stream), -1);
		}
		if (len + 2 > cap)
		{
			cap *= 2;
			tmp = realloc(buf, cap);
			if (!tmp)
				return (free(buf), -1);
			buf = tmp;
		}
		buf[len++] = c;
	}
	buf[len] = '\0';
	*out = buf;
	return 1;
}

int	argo(json *dst, FILE *stream)
{
	int		c;
	pair	*tmp;

	c = peek(stream);
	if (c == '"')
	{
		dst->type = STRING;
		dst->string = NULL;
		return parse_string(&dst->string, stream);
	}
	if (isdigit(c) || c == '-')
	{
		dst->type = INTEGER;
		if (fscanf(stream, "%d", &dst->integer) != 1)
			return (unexpected(stream), -1);
		return 1;
	}
	if (c == '{')
	{
		getc(stream);
		dst->type = MAP;
		dst->map.data = NULL;
		dst->map.size = 0;
		if (accept(stream, '}'))
			return 1;
		do {
			tmp = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
			if (!tmp)
				return -1;
			dst->map.data = tmp;
			if (parse_string(&dst->map.data[dst->map.size].key, stream) != 1)
				return -1;
			if (!expect(stream, ':'))
				return -1;
			if (argo(&dst->map.data[dst->map.size].value, stream) != 1)
				return -1;
			dst->map.size++;
		} while (accept(stream, ','));
		if (!expect(stream, '}'))
			return -1;
		return 1;
	}
	unexpected(stream);
	return -1;
}
