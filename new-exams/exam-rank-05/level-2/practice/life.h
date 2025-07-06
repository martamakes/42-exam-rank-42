#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>//for read
#include <stdbool.h>// for bool

#define LIVE_CELL '0'
#define DEAD_CELL ' '

//create functions
void    game_of_life(int width, int height, int iteration);
