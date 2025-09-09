#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>// free
#include <stdbool.h>// for bool

	extern ssize_t		map_array;
	extern char		    empty;
	extern char		    obstacle;
	extern char	    	full;


//focus functions->
// calloc, free, fopen, fclose, getline, fprintf

void	ft_bsq_from_stdin(FILE *stream);
void	ft_bsq(char **argv);
