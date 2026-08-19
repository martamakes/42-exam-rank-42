#include "life.h"

void	free_grid(char **grid, int h)
{
	int	i;

	i = 0;
	while (i < h)
		free(grid[i++]);
	free(grid);
}

int	count_neighbors(char **g, int y, int x, int w, int h)
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
				if (ny >= 0 && ny < h && nx >= 0 && nx < w)
					if (g[ny][nx] == '0')
						count++;
			}
			dx++;
		}
		dy++;
	}
	return (count);
}

char	**next_generation(char **g1, int w, int h)
{
	char	**g2;
	int		i;
	int		j;
	int		n;

	g2 = create_grid(w, h);
	if (!g2)
		return (NULL);
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			n = count_neighbors(g1, i, j, w, h);
			// regla "B3/S23" (Born on 3, Survive on 2-3)
			if (g1[i][j] == '0')
				g2[i][j] = (n == 2 || n == 3) ? '0' : ' ';
			else
				g2[i][j] = (n == 3) ? '0' : ' ';
			j++;
		}
		i++;
	}
	return (g2);
}

void	parse_input(char **g, int w, int h)
{
	char	c;
	int		pen_x;
	int		pen_y;
	bool	pen_down;

	if (!g || !*g)
		return ;
	pen_x = 0;
	pen_y = 0;
	pen_down = false;
	while (read(0, &c, 1) > 0)
	{
		if (c == 'w' && pen_y > 0)
			pen_y--;
		else if (c == 's' && pen_y < h - 1)
			pen_y++;
		else if (c == 'a' && pen_x > 0)
			--pen_x;
		else if (c == 'd' && pen_x < w - 1)
			pen_x++;
		else if (c == 'x')
			pen_down = !pen_down;
		if (pen_down)
			g[pen_y][pen_x] = '0';
	}
}

char	**create_grid(int w, int h)
{
	char	**grid;
	int		i;
	int		j;

	grid = calloc(h + 1, sizeof(char *));
	if (!grid)
		return (NULL);
	i = 0;
	while (i < h)
	{
		grid[i] = calloc(w + 1, sizeof(char));
		if (!grid[i])
		{
			free_grid(grid, h);
			return (NULL);
		}
		j = 0;
		while (j < w)
			grid[i][j++] = ' ';
		grid[i][j] = '\0';
		i++;
	}
	grid[i] = NULL;
	return (grid);
}

void	print_grid(char **grid, int w, int h)
{
	int	i;
	int	j;

	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
			putchar(grid[i][j++]);
		putchar('\n');
		i++;
	}
}

int	main(int ac, char *av[])
{
	int		width;
	int		height;
	int		iter;
	int		i;
	char	**grid;
	char	**next;

	if (ac != 4)
		return (1);
	width = atoi(av[1]);
	height = atoi(av[2]);
	iter = atoi(av[3]);
	grid = create_grid(width, height);
	if (!grid)
		return (1);
	parse_input(grid, width, height);
	i = 0;
	while (i < iter)
	{
		next = next_generation(grid, width, height);
		if (!next)
		{
			free_grid(grid, height);
			return (1);
		}
		free_grid(grid, height);
		grid = next;
		i++;
	}
	print_grid(grid, width, height);
	free_grid(grid, height);
	return (0);
}
