#include "life.h"
#include <stdlib.h>
#include <unistd.h>

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
