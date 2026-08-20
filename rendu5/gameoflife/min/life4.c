#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static int cell_index(int y, int x, int w){
    return (y * w + x);
}

void print_state(unsigned char *cur, int w, int h)
{
    for(int i = 0; i < h; i++)
    {
        for(int j= 0; j < w; j++)
            putchar(cur[cell_index(i, j, w)] ? '0' : ' ');
        putchar('\n');
    }
}

int count_n(unsigned char *cur, int y, int x, int w, int h)
{
    int n = 0, ny, nx, dy, dx;
    
    for(dy = -1; dy <= 1; dy++ )
    {
        for(dx = -1; dx <=1; dx++)
        {
            if(dy == 0 && dx == 0)
                continue;
            ny = y + dy;
            nx = x + dx;
            if(ny >= 0 && ny < h && nx >= 0 && nx < w)
                n += cur[cell_index(ny, nx, w)];
        }
    }
    return n;


}

void	evolve(unsigned char *cur, unsigned char *next, int w, int h)
{
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int	index = cell_index(i, j, w);
			int	neighbours = count_n(cur, i, j, w, h);

			next[index] = (neighbours == 3) || (cur[index] && neighbours == 2);
		}
	}
}

void draw_initial_state(unsigned char *cur, int w, int h)
{
    char c;
    int x=0, y=0;
    bool down = false;

    while(read(0, &c, 1) > 0)
    {
        switch (c)
        {
            case 'w':
                if(y>0) y--;
                break;
            case 's':
                if(y < h - 1) y++;
                break;
            case 'a':
                if(x>0) x--;
                break;
            case 'd':
                if(x < w - 1) x++;
                break;
            case 'x':
                down = !down;
                break;
            default:
                break;
        }
        if(down) 
            cur[cell_index(y, x, w)] = 1;
            
    }
}

static unsigned char *new_state (int w, int h)
{
    unsigned char *state;
    state = calloc(w * h, sizeof(*state));
    return state;
}

int main(int ac, char **av)
{
    if(ac !=4)
        return 1;
    int w, h, iter;
    w = atoi(av[1]);
    h = atoi(av[2]);
    iter = atoi(av[3]);

    if(w <=0 || h <=0 || iter < 0) return 1;

    unsigned char *cur, *next, *tmp;
    cur = new_state(w, h);
    next = new_state(w,h);
    if(!cur || !next)
    {
        free(cur);
        free(next);
        return 1;
    }

    draw_initial_state(cur, w, h);

    for(int gen=0; gen < iter; gen++)
    {
        evolve(cur, next, w, h);
        tmp = cur;
        cur = next;
        next = tmp;
    }
    print_state(cur, w, h);
    free(cur);
    free(next);
    return 0;
}