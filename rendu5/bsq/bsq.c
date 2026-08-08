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

/*! PSEUDOCODE — read_map:
 *  1. read the header DIRECTLY off the stream with fscanf("%d %c %c %c",
 *     ...) — "sscanf" is NOT in "Allowed functions and globals" (only
 *     "fscanf" is), so the header can't be getline()'d into a buffer
 *     and parsed with sscanf there; fscanf reads straight from the
 *     FILE* instead, no intermediate buffer needed
 *       - reject if rows < 1, if any two of empty/obstacle/full are
 *         equal, or if any of them isn't printable — checked BY HAND
 *         (32 <= c <= 126, the ASCII printable range) since isprint()
 *         isn't in the allowed list either
 *  2. %c does NOT skip the header's own trailing '\n' the way %d does —
 *     discard characters one at a time (fscanf("%c", &c) in a loop)
 *     until that '\n' (or EOF), so the first getline() below starts
 *     exactly at row 1, not mid-header
 *  3. malloc an array of `rows` char* pointers
 *  4. for each of the `rows` remaining lines:
 *       - getline() it
 *       - find its length up to '\n'; if no '\n' was found -> invalid
 *         (subject: every row ends with a line break)
 *       - the FIRST row read fixes the expected width; every other row
 *         must match it exactly, or the map is invalid
 *       - every char must be `empty` or `obstacle` (never `full` —
 *         that symbol is OUTPUT-only)
 *       - malloc(width + 1), copy the row, terminate with '\0'
 *  5. after reading all `rows` rows, if the stream still has another
 *     non-blank line left -> invalid (extra garbage rows)
 *  6. on ANY failure at any step: free whatever was allocated so far
 *     and return NULL — the caller prints "map error" on stderr
 */
char	**read_map(FILE *stream, t_data *data)
{
	char	*line = NULL;
	size_t	n = 0;
	char	**map;
	int		rows_read = 0;
	int		w;
	int		j;
	char	c;

	/* --- leer y validar encabezado: fscanf DIRECTO sobre el stream,
	   sscanf no esta en la lista de funciones permitidas --- */
	if (fscanf(stream, "%d %c %c %c",
		&data->rows, &data->empty, &data->obstacle, &data->full) != 4
		|| data->rows < 1
		|| data->empty == data->obstacle
		|| data->empty == data->full
		|| data->obstacle == data->full
		|| (unsigned char)data->empty < 32 || (unsigned char)data->empty > 126
		|| (unsigned char)data->obstacle < 32 || (unsigned char)data->obstacle > 126
		|| (unsigned char)data->full < 32 || (unsigned char)data->full > 126)
		goto fail;
	/* consumir el resto de la linea de cabecera (normalmente solo el
	   '\n' final) para que el primer getline() de abajo empiece justo
	   en la fila 1 del mapa, no a mitad de la cabecera */
	while (fscanf(stream, "%c", &c) == 1 && c != '\n')
		;

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

/*! PSEUDOCODE — solve_and_mark (dynamic programming):
 *  1. dp[i][j] = side length of the LARGEST all-empty square whose
 *     BOTTOM-RIGHT corner sits at (i, j)
 *  2. base cases: dp[i][j] = 0 if map[i][j] is an obstacle;
 *     dp[i][j] = 1 if map[i][j] is empty AND (i == 0 or j == 0)
 *     (top row / left column can never fit more than a 1x1 square)
 *  3. recurrence: dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1
 *     (the square can only grow as far as its SMALLEST neighboring
 *     square allows, in all 3 directions)
 *  4. track the best (largest) dp value while filling the table; use
 *     STRICT '>' (never >=) when updating so the FIRST square found —
 *     scanning row by row, left to right — wins ties (subject:
 *     top-most, then left-most)
 *  5. once you have best_size / best_y / best_x (bottom-right corner),
 *     walk back best_size cells up and left and overwrite those cells
 *     with `full` in the map
 */
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

/*! PSEUDOCODE — process_map (one map):
 *  1. read_map(); if NULL -> "map error" to stderr and return (skip
 *     this map, but the caller keeps going to the next one)
 *  2. else: solve_and_mark() + print_map() + free_map()
 */
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

/*! PSEUDOCODE — main:
 *  1. no argv (ac == 1) -> read a single map from stdin
 *  2. argv given -> treat EACH argument as a filename, process_map()
 *     each one in order, printing a blank line BETWEEN maps (never
 *     after the last one)
 *  3. unreadable file (fopen fails) -> "map error" too, keep going,
 *     don't crash
 */
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
