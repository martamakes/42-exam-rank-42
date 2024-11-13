// flood_fill.h

#ifndef FLOOD_FILL_H
# define FLOOD_FILL_H

typedef struct  s_point
{
    int           x;
    int           y;
}               t_point;

void  flood_fill(char **tab, t_point size, t_point begin);

#endif