#ifndef VBC_H
#define VBC_H

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

node *parse_expr(char **s);
node *parse_factor(char **s);
node *parse_val(char **s);

int expect(char **s, char c);
int accept(char **s, char c);
void    unexpected(char c);
void    destroy_tree(node *n);
node    *new_node(node n);

#endif