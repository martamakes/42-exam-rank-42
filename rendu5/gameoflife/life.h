#ifndef LIFE_H
# define LIFE_H

# include <stdio.h>

typedef struct s_data
{
	int	width;
	int	height;
	int	iterations;
	int	pen_x;
	int	pen_y;
	int	pen_down;
}	t_data;

char	**init_grid(t_data data);
void	parse_input(char **grid, t_data *data);
int		count_neighbors(char **grid, int y, int x, t_data data);
void	next_generation(char **grid, char **next, t_data data);
void	print_grid(char **grid, t_data data);
void	free_grid(char **grid, t_data data);

#endif
