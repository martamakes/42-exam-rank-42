#define _POSIX_C_SOURCE 200809L
#include "bsq.h"

/* This file mirrors rendu5/bsq/bsq.c's read_map/solve_and_mark/print_map —
   that DP algorithm and map-validation logic doesn't change between the
   two subject variants, only the I/O contract does:
     - multi-map variant : errors -> "map error"\n on STDERR, loops over
       every argv[i] as a separate map, blank line BETWEEN maps.
     - this variant (1 map) : errors -> "Error: invalid map"\n on STDOUT
       ("Error " + a meaningful message for any OTHER kind of error),
       stderr is NOT in "Allowed functions and globals" so it must never
       be touched, and the program only ever receives ONE map. */

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

/*! PSEUDOCODE — read_map (identical rules to the multi-map variant):
 *  1. read the header DIRECTLY off the stream with fscanf("%d %c %c %c",
 *     ...) — "sscanf" is NOT in "Allowed functions and globals" (only
 *     "fscanf" is), so no getline()-into-a-buffer-then-sscanf here
 *       - the spaces in the format string skip ZERO or more whitespace,
 *         so this parses "9 . o x" AND the no-space form "9.ox" the
 *         same way (this subject dropped the "(space separated)"
 *         wording on purpose — don't assume spaces are there)
 *       - reject if rows < 1, any two of empty/obstacle/full are equal,
 *         or any of them isn't printable — checked BY HAND (32 <= c <=
 *         126) since isprint() isn't in the allowed list either
 *  2. %c does NOT skip the header's trailing '\n' — discard characters
 *     one at a time (fscanf("%c", &c) in a loop) until that '\n' (or
 *     EOF), so the first getline() below starts exactly at row 1
 *  3. malloc an array of `rows` char* pointers
 *  4. for each of the `rows` remaining lines:
 *       - getline() it; find length up to '\n' — no '\n' found -> invalid
 *       - first row read FIXES the expected width; any other width -> invalid
 *       - every char must be `empty` or `obstacle` (never `full`)
 *       - malloc(width + 1), copy, '\0'-terminate
 *  5. an extra non-blank line left after `rows` rows -> invalid
 *  6. ANY failure at any step: free what was allocated so far, return NULL
 *     (the caller turns that into "Error: invalid map" on STDOUT)
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
	while (fscanf(stream, "%c", &c) == 1 && c != '\n')
		;
	map = malloc(sizeof(char *) * data->rows);
	if (!map)
		goto fail;
	data->width = -1;
	while (rows_read < data->rows)
	{
		if (getline(&line, &n, stream) == -1)
			goto fail_map;
		w = 0;
		while (line[w] && line[w] != '\n')
			w++;
		if (line[w] != '\n')
			goto fail_map;
		if (data->width == -1)
			data->width = w;
		if (data->width == 0 || w != data->width)
			goto fail_map;
		j = 0;
		while (j < w)
		{
			if (line[j] != data->empty && line[j] != data->obstacle)
				goto fail_map;
			j++;
		}
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

/*! PSEUDOCODE — solve_and_mark (dynamic programming, identical to the
 *  multi-map variant — the algorithm doesn't depend on the I/O contract):
 *  1. dp[i][j] = side length of the LARGEST all-empty square whose
 *     BOTTOM-RIGHT corner sits at (i, j)
 *  2. base cases: dp[i][j] = 0 if obstacle; dp[i][j] = 1 if empty AND
 *     (i == 0 or j == 0)
 *  3. recurrence: dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1
 *  4. track the best (largest) dp value with STRICT '>' so the FIRST
 *     square found (top-to-bottom, left-to-right scan) wins ties
 *     (subject: top-most, then left-most)
 *  5. walk back best_size cells up and left from (best_y, best_x) and
 *     overwrite those cells with `full`
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
	i = 0;
	while (i < data.rows)
		free(dp[i++]);
	free(dp);
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

/*! PSEUDOCODE — process_map (the actual behavioural difference vs bsq/):
 *  1. read_map(); if NULL -> fputs("Error: invalid map\n", stdout) — NOT
 *     stderr, that global isn't in the allowed list for this subject
 *  2. else: solve_and_mark() + print_map() + free_map()
 *  -> no "next map" concept here: this variant only ever gets ONE map
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
		fputs("Error: invalid map\n", stdout);
		return ;
	}
	solve_and_mark(map, data);
	print_map(map, data);
	free_map(map, data.rows);
}

/*! PSEUDOCODE — main:
 *  1. no argv (ac == 1) -> process_map(stdin)
 *  2. exactly one argv (ac == 2) -> fopen(av[1]); can't open it -> "Error: "
 *     + a short meaningful message on STDOUT (subject: "any other error
 *     occurs 'Error ' followed by a meaningfull message" — the exact
 *     wording for THIS case is deliberately not pinned by the subject,
 *     unlike the fixed "Error: invalid map" string); else process_map(f)
 *  3. more than one argv: undefined by the subject ("Program will
 *     recieve one map as an argument") — handled defensively with the
 *     same free-form "Error: ..." on stdout instead of crashing
 */
int	main(int ac, char *av[])
{
	FILE	*f;

	if (ac == 1)
	{
		process_map(stdin);
		return (0);
	}
	if (ac > 2)
	{
		fputs("Error: too many arguments\n", stdout);
		return (1);
	}
	f = fopen(av[1], "r");
	if (!f)
	{
		fputs("Error: cannot open file\n", stdout);
		return (1);
	}
	process_map(f);
	fclose(f);
	return (0);
}
