#ifndef LIFE_H
#define LIFE_H

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

char	**create_grid(int w, int h);
void	free_grid(char **grid, int h);
void	parse_input(char **g, int w, int h);
int		count_neighbors(char **g, int y, int x, int w, int h);
char	**next_generation(char **g1, int w, int h);
void	print_grid(char **grid, int w, int h);

#endif