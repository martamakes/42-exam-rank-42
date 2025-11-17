#ifndef N_QUEENS_BITS_H
# define N_QUEENS_BITS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned long	mask;

void	disp(int *q, int n);
void	bt(int *q, int col, int n, mask rows, mask d1, mask d2);

#endif