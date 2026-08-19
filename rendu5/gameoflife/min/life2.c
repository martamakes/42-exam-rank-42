#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*convierte la coordenada 2D (x,y) en posición array lineal de 1 dimensión*/
static int	idx(int x, int y, int w)
{
	return (y * w + x);
}

static char	*create_board(int w, int h)
{
	char	*board;
	int		i;

	board = malloc(sizeof(char) * w * h);
	if (!board)
		return (NULL);
	i = 0;
	while (i < w * h)
		board[i++] = ' ';
	return (board);
}

static void	parse_input(char *board, int w, int h)
{
	char	c;
	int		pen_x;
	int		pen_y;
	bool	pen_down;

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
			pen_x--;
		else if (c == 'd' && pen_x < w - 1)
			pen_x++;
		else if (c == 'x')
			pen_down = !pen_down;
		if (pen_down)
			board[idx(pen_x, pen_y, w)] = '0';
	}
}

static int	count_neighbors(char *board, int x, int y, int w, int h)
{
	int	count;
	int	dy;
	int	dx;
	int	nx;
	int	ny;

	count = 0;
	dy = -1;
	while (dy <= 1)
	{
		dx = -1;
		while (dx <= 1)
		{
			if (dy != 0 || dx != 0)
			{
				nx = x + dx;
				ny = y + dy;
				if (nx >= 0 && nx < w && ny >= 0 && ny < h)
					if (board[idx(nx, ny, w)] == '0')
						count++;
			}
			dx++;
		}
		dy++;
	}
	return (count);
}

static void	next_generation(char *cur, char *nxt, int w, int h)
{
	int	x;
	int	y;
	int	n;

	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			n = count_neighbors(cur, x, y, w, h);
			// regla "B3/S23" (Born on 3, Survive on 2-3)
			if (cur[idx(x, y, w)] == '0')
				nxt[idx(x, y, w)] = (n == 2 || n == 3) ? '0' : ' ';
			else
				nxt[idx(x, y, w)] = (n == 3) ? '0' : ' ';
			x++;
		}
		y++;
	}
}

static void	print_grid(char *board, int w, int h)
{
	int	x;
	int	y;

	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			putchar(board[idx(x, y, w)]);
			x++;
		}
		putchar('\n');
		y++;
	}
}

int	main(int ac, char **av)
{
	int		w;
	int		h;
	int		iter;
	int		i;
	char	*current;
	char	*next;
	char	*tmp;

	if (ac != 4)
		return (1);
	w = atoi(av[1]);
	h = atoi(av[2]);
	iter = atoi(av[3]);
	current = create_board(w, h);
	next = create_board(w, h);
	if (!current || !next)
	{
		free(current);
		free(next);
		return (1);
	}
	parse_input(current, w, h);
	i = 0;
	while (i < iter)
	{
		next_generation(current, next, w, h);
		tmp = current;
		current = next;
		next = tmp;
		i++;
	}
	print_grid(current, w, h);
	free(current);
	free(next);
	return (0);
}
