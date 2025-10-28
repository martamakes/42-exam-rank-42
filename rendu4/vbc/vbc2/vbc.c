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
    return 0; //nunca debería llegar aquí
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    
    // ✅ CORRECCIÓN: Usar variable auxiliar para el puntero
    char *input = argv[1];
    node *tree = parse_expr(&input);
    
    if (!tree)
        return (1);
    
    // ✅ VALIDACIÓN DEL FINAL: Debe estar AQUÍ, no en parse_expr
    if (*input)
    {
        unexpected(*input);
        destroy_tree(tree);
        return (1);
    }
    
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    
    return (0);
}

