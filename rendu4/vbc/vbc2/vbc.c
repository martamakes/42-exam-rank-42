//This file is given at the exam
#include "vbc.h"


node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}


void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

//...
node *parse_add(char **s)
{
    node *left = parse_mul(s);
    if(!left)
        return NULL;
        while(accept(s, '+'))
        {
            node *right = parse_mul(s);
            if(!right)
            {
                destroy_tree(left);
                return NULL;
            }
            node *tmp = new_node((node){.type = ADD, .l = left, .r = right});                                                                                                  
            if (!tmp)                                                                                                                                                            
            {
                destroy_tree(left);                                                                                                                                              
                destroy_tree(right);                                                                                                                                           
                return NULL;
            }
            left = tmp;
    }
    return left;
}
node *parse_mul(char **s)
{
    node *l = parse_factor(**s);
    if(!l) return NULL;
    while(accept(s, '*'))
    {
        node *r = parse_factor(**s);
        if(!r) {destroy_tree(r); return NULL;}
        node *t = new_node((node){.type = MULTI, .l = l, .r = r});
        if(!t)
        {
            destroy_tree(l);
            destroy_tree(r);
            return NULL;
        }
        l = t;
    }
    return l;
}
node *parse_factor(char **s)
{
    
    if(isdigit(**s))
    {
        int val = **s - '0';
        (*s)++;
        return (new_node((node){.type = VAL, .val = val}));
    }
    if(**s == '(')
    {
        (*s)++;
        node *n = parse_add(s);
        if (!n) return NULL;
        if(!expect(s, ')'))
        {
            destroy_tree(n);
            return NULL;
        }
        return n;
    }
    unexpected(**s);
    return NULL;

}


node    *parse_expr(char *s)
{
    node *ret = parse_add(&s);

    if (*s) 
    {
        destroy_tree(ret);
        unexpected(*s);
        return (NULL);
    }
    return (ret);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
