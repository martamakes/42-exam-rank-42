#ifndef BSQ_H
# define BSQ_H

# define _POSIX_C_SOURCE 200809L

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>

typedef struct s_data
{
	int		rows;
	char	empty;
	char	obstacle;
	char	full;
	int		width;
}	t_data;

int		min3(int a, int b, int c);
void	free_map(char **map, int rows);
char	**read_map(FILE *stream, t_data *data);
void	solve_and_mark(char **map, t_data data);
void	print_map(char **map, t_data data);
void	process_map(FILE *stream);

#endif
