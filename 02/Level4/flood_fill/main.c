#include <stdio.h>
#include <stdlib.h>
#include "flood_fill.h"

char** make_area(char** zone, t_point size)
{
    char** new = malloc(sizeof(char*) * size.y);
    for (int i = 0; i < size.y; ++i)
    {
        new[i] = malloc(size.x + 1);
        for (int j = 0; j < size.x; ++j)
            new[i][j] = zone[i][j];
        new[i][size.x] = '\0';
    }
    return new;
}

void print_area(char **area, t_point size)
{
    for (int i = 0; i < size.y; ++i)
        printf("%s\n", area[i]);
    printf("\n");
}

void free_area(char **area, t_point size)
{
    for (int i = 0; i < size.y; ++i)
        free(area[i]);
    free(area);
}

int main(void)
{
    t_point size = {8, 5};
    char *zone[] = {
        "11111111",
        "10001001",
        "10010001",
        "10110001",
        "11100001",
    };

    char **area = make_area(zone, size);
    
    printf("Before flood_fill:\n");
    print_area(area, size);

    t_point begin = {7, 4};
    flood_fill(area, size, begin);

    printf("After flood_fill:\n");
    print_area(area, size);

    free_area(area, size);
    return (0);
}