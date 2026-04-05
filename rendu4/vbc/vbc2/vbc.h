#ifndef VBC_H
# define VBC_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node *parse_factor(char **s);                                                                         
node *parse_mul(char **s);                                                                            
node *parse_add(char **s);                                                                          
node *parse_expr(char *s);  

#endif