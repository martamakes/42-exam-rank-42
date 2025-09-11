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
        printf("Unexpected end of input\n");  // Fixed: was "end of file"
}

int accept(char **s, char c)
{
    if (**s == c)  // Fixed: check if current char matches c
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


// Parse a factor: digit or '(' expression ')'
node *parse_factor(char **s)
{
    node *ret;
    
    // Check for digit (0-9)
    if (isdigit(**s))
    {
        ret = new_node((node){VAL, **s - '0', NULL, NULL});
        (*s)++;
        return ret;
    }
    
    // Check for '(' expression ')'
    if (accept(s, '('))
    {
        ret = parse_expression(s);
        if (!ret)
            return NULL;
        if (!expect(s, ')'))
        {
            destroy_tree(ret);
            return NULL;
        }
        return ret;
    }
    
    // Unexpected token
    unexpected(**s);
    return NULL;
}

// Parse a term: factor ('*' factor)*
node *parse_term(char **s)
{
    node *left = parse_factor(s);
    if (!left)
        return NULL;
    
    while (accept(s, '*'))
    {
        node *right = parse_factor(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        left = new_node((node){MULTI, 0, left, right});
        if (!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    
    return left;
}

// Parse an expression: term ('+' term)*
node *parse_expression(char **s)
{
    node *left = parse_term(s);
    if (!left)
        return NULL;
    
    while (accept(s, '+'))
    {
        node *right = parse_term(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        left = new_node((node){ADD, 0, left, right});
        if (!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    
    return left;
}

node    *parse_expr(char *s)
{
    char *ptr = s;
    node *ret = parse_expression(&ptr);
    
    if (!ret)
        return NULL;

    // Check if we've consumed all input
    if (*ptr) 
    {
        unexpected(*ptr);
        destroy_tree(ret);
        return NULL;
    }
    return ret;
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
    return 0;  // Should never reach here
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
    return (0);
}