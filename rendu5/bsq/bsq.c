#define _POSIX_C_SOURCE 200809L
#include "bsq.h"

int	min3(int a, int b, int c)
{
	if (b < a)
		a = b;
	if (c < a)
		a = c;
	return (a);
}

void	free_map(char **map, int rows)
{
	int	i = 0;

	while (i < rows)
		free(map[i++]);
	free(map);
}

char	**read_map(FILE *stream, t_data *data)
{
	char	*line = NULL;
	size_t	n = 0;
	char	**map;
	int		rows_read = 0;
	int		w;
	int		j;

	/* --- leer y validar encabezado --- */
	if (getline(&line, &n, stream) == -1)
		goto fail;
	if (sscanf(line, "%d %c %c %c",
		&data->rows, &data->empty, &data->obstacle, &data->full) != 4
		|| data->rows < 1
		|| data->empty == data->obstacle
		|| data->empty == data->full
		|| data->obstacle == data->full
		|| !isprint((unsigned char)data->empty)
		|| !isprint((unsigned char)data->obstacle)
		|| !isprint((unsigned char)data->full))
		goto fail;

	/* --- reservar array de filas --- */
	map = malloc(sizeof(char *) * data->rows);
	if (!map)
		goto fail;

	/* --- leer filas del mapa --- */
	data->width = -1;
	while (rows_read < data->rows)
	{
		if (getline(&line, &n, stream) == -1)
			goto fail_map;

		/* calcular ancho sin '\n' */
		w = 0;
		while (line[w] && line[w] != '\n')
			w++;

		/* subject: "At each end of line, there's a line break" —
		   si llegamos al final del buffer sin encontrar '\n', esta
		   fila no terminaba en salto de linea (p.ej. EOF sin '\n'
		   final) y el mapa es invalido. */
		if (line[w] != '\n')
			goto fail_map;

		/* primera fila fija el ancho */
		if (data->width == -1)
			data->width = w;
		if (data->width == 0 || w != data->width)
			goto fail_map;

		/* validar caracteres */
		j = 0;
		while (j < w)
		{
			if (line[j] != data->empty && line[j] != data->obstacle)
				goto fail_map;
			j++;
		}

		/* guardar fila */
		map[rows_read] = malloc(w + 1);
		if (!map[rows_read])
			goto fail_map;
		j = 0;
		while (j < w)
		{
			map[rows_read][j] = line[j];
			j++;
		}
		map[rows_read][w] = '\0';
		rows_read++;
	}
	/* verificar que no hay filas extra con contenido */
	if (getline(&line, &n, stream) != -1
		&& line[0] != '\n' && line[0] != '\0')
		goto fail_map;

	free(line);
	return (map);

fail_map:
	while (rows_read-- > 0)
		free(map[rows_read]);
	free(map);
fail:
	free(line);
	return (NULL);
}

void	solve_and_mark(char **map, t_data data)
{
	int	**dp;
	int	rows_alloc = 0;
	int	i;
	int	j;
	int	best_size = 0;
	int	best_y = 0;
	int	best_x = 0;

	/* --- reservar tabla dp --- */
	dp = malloc(sizeof(int *) * data.rows);
	if (!dp)
		return ;
	while (rows_alloc < data.rows)
	{
		dp[rows_alloc] = malloc(sizeof(int) * data.width);
		if (!dp[rows_alloc])
		{
			while (rows_alloc-- > 0)
				free(dp[rows_alloc]);
			free(dp);
			return ;
		}
		rows_alloc++;
	}

	/* --- rellenar tabla dp y buscar mejor cuadrado --- */
	i = 0;
	while (i < data.rows)
	{
		j = 0;
		while (j < data.width)
		{
			if (map[i][j] == data.obstacle)
				dp[i][j] = 0;
			else if (i == 0 || j == 0)
				dp[i][j] = 1;
			else
				dp[i][j] = min3(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1;

			/* > estricto: conserva el primero (más arriba, más izquierda) */
			if (dp[i][j] > best_size)
			{
				best_size = dp[i][j];
				best_y    = i;
				best_x    = j;
			}
			j++;
		}
		i++;
	}

	/* --- liberar dp --- */
	i = 0;
	while (i < data.rows)
		free(dp[i++]);
	free(dp);

	/* --- marcar cuadrado en el mapa --- */
	i = best_y - best_size + 1;
	while (i <= best_y)
	{
		j = best_x - best_size + 1;
		while (j <= best_x)
			map[i][j++] = data.full;
		i++;
	}
}

void	print_map(char **map, t_data data)
{
	int	i = 0;

	while (i < data.rows)
	{
		fputs(map[i], stdout);
		fputs("\n", stdout);
		i++;
	}
}

void	process_map(FILE *stream)
{
	t_data	data;
	char	**map;

	data.rows     = 0;
	data.width    = 0;
	data.empty    = 0;
	data.obstacle = 0;
	data.full     = 0;

	map = read_map(stream, &data);
	if (!map)
	{
		fputs("map error\n", stderr);
		return ;
	}
	solve_and_mark(map, data);
	print_map(map, data);
	free_map(map, data.rows);
}

int	main(int ac, char *av[])
{
	int		i;
	FILE	*f;

	if (ac == 1)
	{
		process_map(stdin);
		return (0);
	}
	i = 1;
	while (i < ac)
	{
		f = fopen(av[i], "r");
		if (!f)
			fputs("map error\n", stderr);
		else
		{
			process_map(f);
			fclose(f);
		}
		if (i + 1 < ac)
			fputs("\n", stdout);
		i++;
	}
	return (0);
}
