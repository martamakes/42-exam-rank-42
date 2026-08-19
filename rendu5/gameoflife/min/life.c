#include <unistd.h>
#include <stdlib.h>

typedef struct s_life
{
	int		width;
	int		height;
	char	*current;
	char	*next;
}	t_life;

static int	is_valid_dimensions(int width, int height, int iterations)
{
	if (width <= 0 || height <= 0 || iterations < 0)
		return (0);
	return (1);
}

static int	index_of(t_life *life, int x, int y)
{
	return (y * life->width + x);
}

static void	move_pen(char command, int *x, int *y, t_life *life)
{
	if (command == 'w' && *y > 0)
		(*y)--;
	else if (command == 's' && *y < life->height - 1)
		(*y)++;
	else if (command == 'a' && *x > 0)
		(*x)--;
	else if (command == 'd' && *x < life->width - 1)
		(*x)++;
}

static void	read_drawing(t_life *life)
{
	char	command;
	int		x;
	int		y;
	int		drawing;

	x = 0;
	y = 0;
	drawing = 0;
	while (read(0, &command, 1) > 0)
	{
		if (command == 'x')
		{
			drawing = !drawing;
			if (drawing)
				life->current[index_of(life, x, y)] = 1;
		}
		else if (command == 'w' || command == 'a'
			|| command == 's' || command == 'd')
		{
			move_pen(command, &x, &y, life);
			if (drawing)
				life->current[index_of(life, x, y)] = 1;
		}
	}
}

static int	count_neighbours(t_life *life, int x, int y)
{
	int	dx;
	int	dy;
	int	nx;
	int	ny;
	int	count;

	count = 0;
	dy = -1;
	while (dy <= 1)
	{
		dx = -1;
		while (dx <= 1)
		{
			nx = x + dx;
			ny = y + dy;
			if (!(dx == 0 && dy == 0)
				&& nx >= 0 && nx < life->width
				&& ny >= 0 && ny < life->height)
				count += life->current[index_of(life, nx, ny)];
			dx++;
		}
		dy++;
	}
	return (count);
}

static void	compute_generation(t_life *life)
{
	int		x;
	int		y;
	int		neighbours;
	char	*tmp;

	y = 0;
	while (y < life->height)
	{
		x = 0;
		while (x < life->width)
		{
			neighbours = count_neighbours(life, x, y);
			if (life->current[index_of(life, x, y)])
				life->next[index_of(life, x, y)]
					= (neighbours == 2 || neighbours == 3);
			else
				life->next[index_of(life, x, y)] = (neighbours == 3);
			x++;
		}
		y++;
	}
	tmp = life->current;
	life->current = life->next;
	life->next = tmp;
}

static void	print_board(t_life *life)
{
	int	x;
	int	y;

	y = 0;
	while (y < life->height)
	{
		x = 0;
		while (x < life->width)
		{
			if (life->current[index_of(life, x, y)])
				putchar('0');
			else
				putchar(' ');
			x++;
		}
		putchar('\n');
		y++;
	}
}

int	main(int argc, char **argv)
{
	t_life	life;
	int		iterations;
	int		i;
	int		size;

	if (argc != 4)
		return (1);
	life.width = atoi(argv[1]);
	life.height = atoi(argv[2]);
	iterations = atoi(argv[3]);
	if (!is_valid_dimensions(life.width, life.height, iterations))
		return (1);
	size = life.width * life.height;
	life.current = calloc(size, sizeof(char));
	life.next = calloc(size, sizeof(char));
	if (!life.current || !life.next)
	{
		free(life.current);
		free(life.next);
		return (1);
	}
	read_drawing(&life);
	i = 0;
	while (i < iterations)
	{
		compute_generation(&life);
		i++;
	}
	print_board(&life);
	free(life.current);
	free(life.next);
	return (0);
}