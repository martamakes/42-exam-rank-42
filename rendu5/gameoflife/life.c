#include "life.h"
#include <stdlib.h>
#include <unistd.h>

/*! PSEUDOCODE — init_grid:
 *  1. malloc (height + 1) row pointers (+1 slot for a NULL sentinel)
 *  2. for each row i in [0, height):
 *       - malloc (width + 1) chars
 *       - fill all `width` cells with ' ' (dead)
 *       - put '\0' at the end (never printed, just a safety habit)
 *  3. on any malloc failure: free everything allocated so far, return NULL
 *  4. grid[height] = NULL
 */
char	**init_grid(t_data data)
{
	int		i;
	int		j;
	char	**grid;

	grid = calloc(data.height + 1, sizeof(char *));
	if (!grid)
		return (NULL);
	i = 0;
	while (i < data.height)
	{
		grid[i] = calloc(data.width + 1, sizeof(char));
		if (!grid[i])
		{
			while (--i >= 0)
				free(grid[i]);
			free(grid);
			return (NULL);
		}
		j = 0;
		while (j < data.width)
			grid[i][j++] = ' ';
		grid[i][j] = '\0';
		i++;
	}
	grid[i] = NULL;
	return (grid);
}

/*! PSEUDOCODE — parse_input:
 *  1. read one char at a time with read(0, &c, 1) until it returns <= 0
 *  2. track pen position (pen_x, pen_y) and pen_down state in *data
 *  3. for w/a/s/d:
 *       - only move if the new position stays INSIDE the board
 *         (subject: "pen no move outside board and stays still")
 *       - if the move happened AND pen is down, light the NEW cell
 *  4. for x: toggle pen_down; if it just went DOWN, light the CURRENT
 *     cell (the pen "starts drawing" from where it already is)
 *  5. any other char: do nothing (subject: "pen do nothing in case of
 *     invalid command") — this is also what makes the trailing '\n'
 *     from `echo` harmless
 */
void	parse_input(char **grid, t_data *data)
{
	char	c;

	while (read(0, &c, 1) > 0)
	{
		if (c == 'w' && data->pen_y > 0)
		{
			data->pen_y--;
			if (data->pen_down)
				grid[data->pen_y][data->pen_x] = '0';
		}
		else if (c == 's' && data->pen_y < data->height - 1)
		{
			data->pen_y++;
			if (data->pen_down)
				grid[data->pen_y][data->pen_x] = '0';
		}
		else if (c == 'a' && data->pen_x > 0)
		{
			data->pen_x--;
			if (data->pen_down)
				grid[data->pen_y][data->pen_x] = '0';
		}
		else if (c == 'd' && data->pen_x < data->width - 1)
		{
			data->pen_x++;
			if (data->pen_down)
				grid[data->pen_y][data->pen_x] = '0';
		}
		else if (c == 'x')
		{
			data->pen_down = !data->pen_down;
			if (data->pen_down)
				grid[data->pen_y][data->pen_x] = '0';
		}
	}
}

/*! PSEUDOCODE — count_neighbors:
 *  1. loop dy in [-1, 1], dx in [-1, 1]
 *  2. skip (dy == 0 && dx == 0) — that's the cell itself, not a neighbor
 *  3. for each (ny, nx): only count it if it's INSIDE the board
 *     (subject: "each cell outside of the array will be considered dead")
 *  4. count += 1 if grid[ny][nx] == '0'
 */
int		count_neighbors(char **grid, int y, int x, t_data data)
{
	int	count;
	int	dy;
	int	dx;
	int	ny;
	int	nx;

	count = 0;
	dy = -1;
	while (dy <= 1)
	{
		dx = -1;
		while (dx <= 1)
		{
			if (dy != 0 || dx != 0)
			{
				ny = y + dy;
				nx = x + dx;
				if (ny >= 0 && ny < data.height && nx >= 0 && nx < data.width)
					if (grid[ny][nx] == '0')
						count++;
			}
			dx++;
		}
		dy++;
	}
	return (count);
}

/*! PSEUDOCODE — next_generation:
 *  1. for every cell (i, j):
 *       - n = count_neighbors(grid, i, j)
 *       - if cell is ALIVE: survives when (n == 2 || n == 3), else dies
 *       - if cell is DEAD:  born only when (n == 3), else stays dead
 *  2. write the result into `next`, NEVER into `grid` while still
 *     reading it (mutating in place would corrupt the neighbor counts
 *     of cells not visited yet)
 *  3. caller (main) swaps grid <-> next afterwards — a pointer swap,
 *     not a copy
 */
void	next_generation(char **grid, char **next, t_data data)
{
	int	i;
	int	j;
	int	n;

	i = 0;
	while (i < data.height)
	{
		j = 0;
		while (j < data.width)
		{
			n = count_neighbors(grid, i, j, data);
			if (grid[i][j] == '0')
				next[i][j] = (n == 2 || n == 3) ? '0' : ' ';
			else
				next[i][j] = (n == 3) ? '0' : ' ';
			j++;
		}
		i++;
	}
}

void	print_grid(char **grid, t_data data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data.height)
	{
		j = 0;
		while (j < data.width)
			putchar(grid[i][j++]);
		putchar('\n');
		i++;
	}
}

void	free_grid(char **grid, t_data data)
{
	int	i;

	i = 0;
	while (i < data.height)
		free(grid[i++]);
	free(grid);
}

/*! PSEUDOCODE — main:
 *  1. parse argv: width, height, iterations = atoi(av[1..3]) (ac must be 4)
 *  2. grid = init_grid()
 *  3. parse_input(grid, &data) — reads stdin, draws the initial state
 *  4. next = init_grid() — second board for double-buffering
 *  5. loop `iterations` times:
 *       - next_generation(grid, next, data)
 *       - swap grid and next (pointers, O(1), no copying)
 *  6. print_grid(grid, data)
 *  7. free BOTH boards — after swapping, `grid` and `next` still point
 *     to the two distinct original allocations, so freeing only one
 *     leaks the other
 */
int		main(int ac, char *av[])
{
	t_data	data;
	char	**grid;
	char	**next;
	char	**tmp;
	int		k;

	if (ac != 4)
		return (1);
	data.width      = atoi(av[1]);
	data.height     = atoi(av[2]);
	data.iterations = atoi(av[3]);
	data.pen_x      = 0;
	data.pen_y      = 0;
	data.pen_down   = 0;
	grid = init_grid(data);
	if (!grid)
		return (1);
	parse_input(grid, &data);
	next = init_grid(data);
	if (!next)
	{
		free_grid(grid, data);
		return (1);
	}
	k = 0;
	while (k < data.iterations)
	{
		next_generation(grid, next, data);
		tmp  = grid;
		grid = next;
		next = tmp;
		k++;
	}
	print_grid(grid, data);
	free_grid(grid, data);
	free_grid(next, data);
	return (0);
}
