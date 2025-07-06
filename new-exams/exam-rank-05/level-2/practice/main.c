#include "life.h"

//clear && gcc main.c life.c life.h && valgrind ./a.out 3 3 0 | cat -e
//clear && gcc main.c life.c life.h && echo 'dxss' | valgrind ./a.out 3 3 0 | cat -e
//clear && gcc main.c life.c life.h && echo 'sdxssdxsssssssssdwwwwwwwwwwdsssssssssdddddddddaaaaaaaaaaaaxddddddwwwwwwwwaaaaaassssssss' | valgrind ./a.out 3 3 0 | cat -e



/*
1) store the 3 values of atoi
2) create an empty board
3) add in read and the living cells
*/

int     main(int argc, char **argv)
{
    if (argc != 4)
        return (1);
    
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int iteration = atoi(argv[3]);

    game_of_life(width, height, iteration);
    return (0);
}
