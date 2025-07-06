/*
 * EJERCICIO: VBC (Very Basic Compiler)
 * 
 * DESCRIPCIÓN:
 * Implementar un compilador básico que evalúe expresiones matemáticas
 * con operadores +, *, paréntesis y números del 0-9.
 * 
 * CONCEPTOS CLAVE:
 * 1. PARSING: Análisis sintáctico de expresiones
 * 2. AST: Árbol de sintaxis abstracta
 * 3. PRECEDENCIA: * tiene mayor precedencia que +
 * 4. ASOCIATIVIDAD: Operadores son asociativos por la izquierda
 * 5. RECURSIVE DESCENT: Parsing top-down recursivo
 * 
 * GRAMÁTICA:
 * expression  → addition
 * addition    → multiplication (('+') multiplication)*
 * multiplication → primary (('*') primary)*
 * primary     → NUMBER | '(' expression ')'
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Definición del AST
typedef struct node {
    enum {
        ADD,      // Nodo de suma
        MULTI,    // Nodo de multiplicación  
        VAL       // Nodo de valor (hoja)
    } type;
    int val;           // Valor para nodos VAL
    struct node *l;    // Hijo izquierdo
    struct node *r;    // Hijo derecho
} node;

// Declaraciones de funciones
node *parse_expression(char **s);
node *parse_addition(char **s);
node *parse_multiplication(char **s);
node *parse_primary(char **s);
node *new_node(node n);
void destroy_tree(node *n);
int eval_tree(node *tree);
void unexpected(char c);
int accept(char **s, char c);
int expect(char **s, char c);

/*
 * FUNCIÓN PARA CREAR NUEVOS NODOS:
 */
node *new_node(node n)
{
    node *ret = calloc(1, sizeof(node));
    if (!ret)
        return NULL;
    *ret = n;
    return ret;
}

/*
 * FUNCIÓN PARA LIBERAR MEMORIA DEL AST:
 */
void destroy_tree(node *n)
{
    if (!n)
        return;
    
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

/*
 * FUNCIONES AUXILIARES PARA PARSING:
 */
void unexpected(char c)
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
        return 1;
    }
    return 0;
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return 1;
    unexpected(**s);
    return 0;
}

/*
 * PARSER DE ELEMENTOS PRIMARIOS:
 * primary → NUMBER | '(' expression ')'
 */
node *parse_primary(char **s)
{
    /*
     * ELEMENTOS PRIMARIOS:
     * 1. Números de un solo dígito (0-9)
     * 2. Expresiones entre paréntesis
     */
    node *res;
    node tmp;
    
    if (**s == '(')
    {
        /*
         * EXPRESIÓN ENTRE PARÉNTESIS:
         * - Consumir '('
         * - Parsear expresión recursivamente
         * - Esperar ')'
         */
        (*s)++;  // Consumir '('
        res = parse_addition(s);  // Parsear expresión interna
        
        if (!res || **s != ')')
        {
            destroy_tree(res);
            unexpected(**s);
            return NULL;
        }
        
        (*s)++;  // Consumir ')'
        return res;
    }
    
    if (isdigit(**s))
    {
        /*
         * NÚMERO DE UN DÍGITO:
         * - Convertir carácter a valor numérico
         * - Crear nodo VAL
         * - Avanzar puntero
         */
        tmp.type = VAL;
        tmp.val = **s - '0';  // Convertir '0'-'9' a 0-9
        tmp.l = NULL;
        tmp.r = NULL;
        
        res = new_node(tmp);
        (*s)++;  // Consumir dígito
        return res;
    }
    
    // Token inesperado
    unexpected(**s);
    return NULL;
}

/*
 * PARSER DE MULTIPLICACIÓN:
 * multiplication → primary (('*') primary)*
 */
node *parse_multiplication(char **s)
{
    /*
     * PARSING DE MULTIPLICACIÓN:
     * - Parsear primer operando (primary)
     * - Mientras haya '*': parsear siguiente operando y crear nodo MULTI
     * - Asociatividad izquierda: ((a * b) * c) * d
     */
    node *left;
    node *right;
    node tmp;
    
    left = parse_primary(s);
    if (!left)
        return NULL;
    
    while (**s == '*')
    {
        (*s)++;  // Consumir '*'
        
        right = parse_primary(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        
        // Crear nodo de multiplicación
        tmp.type = MULTI;
        tmp.l = left;
        tmp.r = right;
        tmp.val = 0;  // No usado en nodos operadores
        
        left = new_node(tmp);
        if (!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    
    return left;
}

/*
 * PARSER DE SUMA:
 * addition → multiplication (('+') multiplication)*
 */
node *parse_addition(char **s)
{
    /*
     * PARSING DE SUMA:
     * - Parsear primer operando (multiplication)
     * - Mientras haya '+': parsear siguiente operando y crear nodo ADD
     * - Precedencia: multiplication se evalúa antes que addition
     * - Asociatividad izquierda: ((a + b) + c) + d
     */
    node *left;
    node *right;
    node tmp;
    
    left = parse_multiplication(s);
    if (!left)
        return NULL;
    
    while (**s == '+')
    {
        (*s)++;  // Consumir '+'
        
        right = parse_multiplication(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        
        // Crear nodo de suma
        tmp.type = ADD;
        tmp.l = left;
        tmp.r = right;
        tmp.val = 0;  // No usado en nodos operadores
        
        left = new_node(tmp);
        if (!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    
    return left;
}

/*
 * PARSER DE EXPRESIÓN COMPLETA:
 */
node *parse_expression(char **s)
{
    /*
     * PARSER PRINCIPAL:
     * - Parsear expresión completa
     * - Verificar que no queden caracteres sin parsear
     * - Manejar errores de parsing
     */
    node *ret = parse_addition(s);
    
    if (!ret)
        return NULL;
    
    // Verificar que se consumió toda la entrada
    if (**s)
    {
        destroy_tree(ret);
        unexpected(**s);
        return NULL;
    }
    
    return ret;
}

/*
 * EVALUADOR DEL AST:
 */
int eval_tree(node *tree)
{
    /*
     * EVALUACIÓN RECURSIVA:
     * - Para nodos VAL: retornar el valor
     * - Para nodos ADD: evaluar hijos y sumar
     * - Para nodos MULTI: evaluar hijos y multiplicar
     */
    switch (tree->type)
    {
        case ADD:
            return eval_tree(tree->l) + eval_tree(tree->r);
            
        case MULTI:
            return eval_tree(tree->l) * eval_tree(tree->r);
            
        case VAL:
            return tree->val;
    }
    
    return 0;  // No debería llegar aquí
}

/*
 * FUNCIÓN MAIN:
 */
int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;
    
    char *input = argv[1];
    node *tree = parse_expression(&input);
    
    if (!tree)
        return 1;
    
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    return 0;
}

/*
 * EJEMPLOS DE PARSING Y EVALUACIÓN:
 * 
 * INPUT: "3+4*5"
 * AST:
 *       +
 *      / \
 *     3   *
 *        / \
 *       4   5
 * EVALUACIÓN: 3 + (4 * 5) = 3 + 20 = 23
 * 
 * INPUT: "(3+4)*5"  
 * AST:
 *       *
 *      / \
 *     +   5
 *    / \
 *   3   4
 * EVALUACIÓN: (3 + 4) * 5 = 7 * 5 = 35
 * 
 * INPUT: "2+3*4+5"
 * AST:
 *         +
 *        / \
 *       +   5
 *      / \
 *     2   *
 *        / \
 *       3   4
 * EVALUACIÓN: ((2 + (3 * 4)) + 5) = (2 + 12) + 5 = 19
 */

/*
 * PRECEDENCIA Y ASOCIATIVIDAD:
 * 
 * 1. PRECEDENCIA:
 *    - * tiene mayor precedencia que +
 *    - Se logra parseando multiplication antes que addition
 *    - parse_addition() llama a parse_multiplication()
 * 
 * 2. ASOCIATIVIDAD IZQUIERDA:
 *    - Operadores del mismo nivel se evalúan de izquierda a derecha
 *    - Se logra con el bucle while en cada función de parsing
 *    - "1+2+3" se parsea como "((1+2)+3)"
 * 
 * 3. PARÉNTESIS:
 *    - Tienen la mayor precedencia
 *    - Se manejan en parse_primary()
 *    - Permiten cambiar el orden de evaluación
 */

/*
 * TÉCNICAS DE COMPILACIÓN EMPLEADAS:
 * 
 * 1. LEXING IMPLÍCITO:
 *    - No hay tokenización explícita
 *    - Los caracteres se procesan directamente
 *    - Funciona para esta gramática simple
 * 
 * 2. RECURSIVE DESCENT PARSING:
 *    - Una función por cada regla de la gramática
 *    - Llamadas recursivas para reglas anidadas
 *    - Manejo natural de precedencia
 * 
 * 3. AST CONSTRUCTION:
 *    - Construcción del árbol durante el parsing
 *    - Estructura refleja la precedencia de operadores
 *    - Fácil de evaluar recursivamente
 * 
 * 4. ERROR HANDLING:
 *    - Detección de tokens inesperados
 *    - Liberación de memoria en caso de error
 *    - Mensajes de error descriptivos
 */

/*
 * LIMITACIONES:
 * 
 * 1. SOLO NÚMEROS DE UN DÍGITO:
 *    - Números multi-dígito requerirían lexing más sofisticado
 *    - Fácil extensión parseando secuencias de dígitos
 * 
 * 2. SOLO DOS OPERADORES:
 *    - División y resta requerirían nuevas reglas de gramática
 *    - División necesitaría manejo de división por cero
 * 
 * 3. SIN WHITESPACE:
 *    - No se manejan espacios en blanco
 *    - Requerirían función para saltar espacios
 * 
 * 4. SIN OPTIMIZACIÓN:
 *    - El AST se evalúa directamente
 *    - Un compilador real generaría código máquina
 */

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. PRECEDENCIA DE OPERADORES:
 *    - * antes que + se logra con el orden de parsing
 *    - parse_addition() → parse_multiplication() → parse_primary()
 *    - Cada nivel maneja operadores de una precedencia
 * 
 * 2. CONSTRUCCIÓN DEL AST:
 *    - Nodos binarios para operadores
 *    - Nodos hoja para valores
 *    - Estructura refleja orden de evaluación
 * 
 * 3. MANEJO DE PARÉNTESIS:
 *    - Se manejan en el nivel de mayor precedencia (primary)
 *    - Permiten "resetear" la precedencia
 *    - Parsing recursivo de la expresión interna
 * 
 * 4. GESTIÓN DE MEMORIA:
 *    - Crear nodos dinámicamente
 *    - Liberar árbol completo al finalizar
 *    - Cleanup en caso de errores de parsing
 * 
 * 5. EVALUACIÓN RECURSIVA:
 *    - Recorrer AST en post-order
 *    - Evaluar hijos antes que padre
 *    - Aplicar operador a resultados de hijos
 */