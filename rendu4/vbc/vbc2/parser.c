#include "vbc.h"

/*
 * parse_val: Parsea valores individuales o expresiones entre paréntesis
 * 
 * GRAMÁTICA: val → digit | '(' expr ')'
 * 
 * Esta función maneja:
 * 1. Dígitos individuales (0-9)
 * 2. Expresiones completas entre paréntesis
 */
node *parse_val(char **s)
{
    node *ret = NULL;

    // CASO 1: Es un dígito
    if (isdigit(**s))
    {
        ret = new_node((node){VAL, **s - '0', NULL, NULL});
        (*s)++;
    }
    // CASO 2: Es una expresión entre paréntesis
    else if (accept(s, '('))
    {
        ret = parse_expr(s);
        if (!ret || !expect(s, ')'))
        {
            destroy_tree(ret);
            return NULL;
        }
    }
    // CASO 3: Token inesperado
    else
    {
        unexpected(**s);
        return NULL;
    }
    return ret;
}

/*
 * parse_factor: Parsea multiplicaciones
 * 
 * GRAMÁTICA: factor → val (('*') val)*
 * 
 * Procesa todas las multiplicaciones de izquierda a derecha.
 * Ejemplo: 2*3*4 se convierte en (2*3)*4
 */
node *parse_factor(char **s)
{
    node *left = parse_val(s);
    if (!left)
        return NULL;

    // Mientras haya multiplicaciones
    while (**s == '*')
    {
        (*s)++;
        node *right = parse_val(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        
        // Crear nodo MULTI y actualizar left
        node new = {MULTI, 0, left, right};
        left = new_node(new);
        if (!left)
        {
            destroy_tree(right);  // Importante: liberar right también
            return NULL;
        }
    }
    return left;
}

/*
 * parse_expr: Parsea sumas
 * 
 * GRAMÁTICA: expr → factor (('+') factor)*
 * 
 * Procesa todas las sumas de izquierda a derecha.
 * Como llama a parse_factor, respeta la precedencia (* antes que +)
 * 
 * IMPORTANTE: NO valida el final de la cadena aquí porque
 * esta función puede ser llamada recursivamente desde parse_val
 * cuando hay paréntesis.
 */
node *parse_expr(char **s)
{
    node *left = parse_factor(s);
    if (!left)
        return NULL;

    // Mientras haya sumas
    while (**s == '+')
    {
        (*s)++;
        node *right = parse_factor(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        
        // Crear nodo ADD y actualizar left
        node new = {ADD, 0, left, right};
        left = new_node(new);
        if (!left)
        {
            destroy_tree(right);  // Importante: liberar right también
            return NULL;
        }
    }
    
    // ❌ NO PONGAS LA VALIDACIÓN AQUÍ
    // La validación del final debe estar en main, no aquí
    
    return left;
}