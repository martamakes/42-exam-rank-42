#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	cell_index(int i, int j, int width)
{
	return (i * width + j);
}

static unsigned char	*new_state(int width, int height)
{
	unsigned char	*state;

	state = calloc(width * height, sizeof(*state));
	return (state);
}

static void	draw_initial_state(unsigned char *state, int width, int height)
{
	char	c;
	int		x = 0, y = 0;
	bool	down = false;

	while (read(0, &c, 1) > 0)
	{
		switch (c)
		{
			case 'w':
				if (y > 0) y--;
				break;
			case 's':
				if (y < height - 1) y++;
				break;
			case 'a':
				if (x > 0) x--;
				break;
			case 'd':
				if (x < width - 1) x++;
				break;
			case 'x':
				down = !down;
				break;
			default:
				break; //pen do nothing in case of invalid command
		}
		if (down)
			state[cell_index(y, x, width)] = 1;
	}
}

/*counts the number of live cells around, give a specific cell (i, j). The program cells alived are '1' and dead '0' so they can be added up */
static int	nearest_neighbours(unsigned char *state,
    int i, int j, int width, int height)
{
    int	neighbours = 0;
    int	ni;
    int	nj;

    for (int di = -1; di <= 1; di++)
    {
        for (int dj = -1; dj <= 1; dj++)
        {
            if (di == 0 && dj == 0)
                continue;
            ni = i + di;
            nj = j + dj;
            if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                neighbours += state[cell_index(ni, nj, width)];
        }
    }
    return (neighbours);
}
/*
 * Conway's Game of Life rule table: rule[current_state][live_neighbours].
 *
 * Row 0 represents a dead cell:
 * - It becomes alive only when it has exactly 3 live neighbours.
 *
 * Row 1 represents a live cell:
 * - It survives only when it has 2 or 3 live neighbours.
 *
 * Each row has 9 values because a cell can have from 0 to 8 neighbours.
 * The stored value is the cell state for the next generation:
 * 0 = dead, 1 = alive.
 */
static void	evolve(unsigned char *prev, unsigned char *next,
    int width, int height)
{
    //rule B3/S23. 
static const unsigned char	rule[2][9] = {
    {0, 0, 0, 1, 0, 0, 0, 0, 0}, //if alive and 3 keep alive ->1
    {0, 0, 1, 1, 0, 0, 0, 0, 0} //if dead and 2/3 evolves to alive ->1
};

for (int i = 0; i < height; i++)
{
    for (int j = 0; j < width; j++)
    {
        int index = cell_index(i, j, width);
        int neighbours = nearest_neighbours(prev, i, j, width, height);

        next[index] = rule[prev[index]][neighbours];
    }
}
}
static void	display_state(unsigned char *state, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			putchar(state[cell_index(i, j, width)] ? '0' : ' ');
		putchar('\n');
	}
}
int	main(int ac, char **av)
{
	int				width, height, iterations;
	unsigned char	*prev, *next, *tmp;

	if (ac != 4)
		return (1);
	width = atoi(av[1]);
	height = atoi(av[2]);
	iterations = atoi(av[3]);
	if (width <= 0 || height <= 0 || iterations < 0)
		return (1);
	prev = new_state(width, height);
	next = new_state(width, height);
	if (!prev || !next)
	{
		free(prev);
		free(next);
		return (1);
	}
	draw_initial_state(prev, width, height);
	for (int generation = 0; generation < iterations; generation++)
	{
		evolve(prev, next, width, height);
		tmp = prev;
		prev = next;
		next = tmp;
	}
	display_state(prev, width, height);
	free(prev);
	free(next);
	return (0);
}
