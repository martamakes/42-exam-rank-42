#include "life.h"

//clear && gcc main.c life.c life.h && echo 'sdxddssaaww' | valgrind --leak-check=full --show-leak-kinds=all ./a.out 5 5 0 && rm ./a.out life.h.gch

//clear && gcc main.c life.c life.h && echo 'sdxssdswdxdddxsaddawxwdxwaa' | valgrind --leak-checkfull --show-leak-kinds=all ./a.out 10 6 0 && rm ./a.out life.h.gch

//clear && gcc main.c life.c life.h && echo 'dxss' | valgrind --leak-check=full --show-leak-kinds=all ./a.out 3 3 1 && rm ./a.out life.h.gch

/*
Plan:
1) get the 3 values immediately

2) create an empty board with calloc, filled with spaces ' '.
3) use Read to get the location of living cells and add them in

4) if (iteration > 0), get the new_board and get the '8' neighbours.
5) update the board and free the boards you aren't using

6) ensure you have a print board and a free board.

Finally,
free everything ensure nothing is left. Check valgrind
*/

int	main(int argc, char **argv)
{
	if (argc == 4)
		game_of_life_start(argv);//convert the char into size_t
	return (0);
}