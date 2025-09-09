#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>//for bool
#include <errno.h>
#include <unistd.h> // for read

//create functions
// bool	arguement_are_num_checker(char **argv);
void	game_of_life_start(char	**argv);
char**	board_creation(int width, int height);