#ifndef BSQ_H
# define BSQ_H

# define _POSIX_C_SOURCE 200809L

# include <stdio.h>
# include <stdlib.h>
/* NOTE: no <ctype.h> — isprint() is not in "Allowed functions and
   globals" for this subject either, the printable-char check in
   read_map is done by hand. */

/* NOTE (vs the multi-map variant in rendu5/bsq/): "stderr" is NOT in this
   subject's "Allowed functions and globals" list. Every message this
   program prints — valid solution AND errors — goes through fputs/fprintf
   to stdout only. Never touch stderr here. */

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
