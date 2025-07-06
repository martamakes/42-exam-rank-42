/*
 * EJERCICIO: PERMUTATIONS
 * 
 * DESCRIPCIÓN:
 * Generar todas las permutaciones de un string dado en orden alfabético.
 * 
 * CONCEPTOS CLAVE:
 * 1. BACKTRACKING: Algoritmo de exploración exhaustiva
 * 2. ORDENAMIENTO: Asegurar orden alfabético
 * 3. RECURSIÓN: Generar permutaciones nivel por nivel
 * 4. ESTADO: Manejar qué caracteres ya se han usado
 * 
 * ALGORITMO:
 * 1. Ordenar el string original alfabéticamente
 * 2. Para cada posición, probar cada carácter no usado
 * 3. Marcar carácter como usado y recursar
 * 4. Desmarcar carácter al retornar (backtrack)
 * 5. Imprimir cuando se complete una permutación
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Función auxiliar para calcular longitud
int ft_strlen(char *s)
{
    int i = 0;
    while (s[i])
        i++;
    return i;
}

// Función auxiliar para verificar si es alfabético
int ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

// Función para intercambiar dos caracteres
void ft_swap(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

// Función para verificar si carácter ya está en uso
int ft_strchr(const char *s, char c)
{
    int i = 0;
    while (s[i])
    {
        if (s[i] == c)
            return 1; // Encontrado
        i++;
    }
    return 0; // No encontrado
}

// Función para ordenar string alfabéticamente (bubble sort)
char *order_string(char *s)
{
    /*
     * ORDENAMIENTO BURBUJA:
     * - Comparar caracteres adyacentes
     * - Intercambiar si están en orden incorrecto
     * - Repetir hasta que no haya intercambios
     * - Garantiza orden alfabético para las permutaciones
     */
    int len = ft_strlen(s);
    int swapped = 1;
    
    while (swapped)
    {
        swapped = 0;
        int i = 0;
        while (i < len - 1)
        {
            if (s[i] > s[i + 1])
            {
                ft_swap(&s[i], &s[i + 1]);
                swapped = 1;
            }
            i++;
        }
    }
    return s;
}

// Función recursiva para generar permutaciones
void generate_permutations(char *source, char *result, int pos)
{
    /*
     * ALGORITMO BACKTRACKING PARA PERMUTACIONES:
     * 
     * CASO BASE: Si pos == longitud, tenemos una permutación completa
     * - Imprimir la permutación actual
     * - Retornar para probar otras opciones
     * 
     * CASO RECURSIVO: Para cada carácter en source:
     * - Si no está usado en result, probarlo
     * - Agregarlo a result[pos]
     * - Recursar para la siguiente posición
     * - Remover carácter (backtrack) para probar otros
     */
    
    int source_len = ft_strlen(source);
    
    // Caso base: permutación completa
    if (pos == source_len)
    {
        write(1, result, source_len);
        write(1, "\n", 1);
        return;
    }
    
    // Probar cada carácter de source
    int i = 0;
    while (i < source_len)
    {
        // Verificar si el carácter ya está usado
        if (!ft_strchr(result, source[i]))
        {
            result[pos] = source[i];        // Usar carácter
            generate_permutations(source, result, pos + 1);  // Recursar
            result[pos] = '\0';             // Backtrack: remover carácter
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    /*
     * VALIDACIÓN Y PROCESAMIENTO:
     * - Verificar argumentos válidos
     * - Validar que solo contiene letras
     * - Ordenar string alfabéticamente
     * - Inicializar buffer para resultado
     * - Generar todas las permutaciones
     */
    
    if (argc != 2)
        return 1;
    
    // Verificar string vacío o solo espacios
    if (ft_strlen(argv[1]) == 0 || (argv[1][0] == ' ' && !argv[1][1]))
        return 0;
    
    // Validar que solo contiene caracteres alfabéticos
    int i = 0;
    while (argv[1][i])
    {
        if (!ft_isalpha(argv[1][i]))
            return 0;
        i++;
    }
    
    int len = ft_strlen(argv[1]);
    
    // Alocar buffer para el resultado (permutación actual)
    char *result = calloc(len + 1, 1);
    if (!result)
        return 1;
    
    // Ordenar el string alfabéticamente
    char *source = order_string(argv[1]);
    
    // Generar todas las permutaciones
    generate_permutations(source, result, 0);
    
    free(result);
    return 0;
}

/*
 * EJEMPLO DE EJECUCIÓN:
 * 
 * ./permutations abc
 * abc
 * acb  
 * bac
 * bca
 * cab
 * cba
 * 
 * PROCESO PARA "abc":
 * 1. Ordenar: "abc" (ya está ordenado)
 * 2. pos=0: Probar 'a', 'b', 'c'
 *    - Elegir 'a': result="a"
 *      - pos=1: Probar 'b', 'c' (no 'a' porque está usado)
 *        - Elegir 'b': result="ab"
 *          - pos=2: Probar 'c': result="abc" → IMPRIMIR
 *        - Elegir 'c': result="ac"  
 *          - pos=2: Probar 'b': result="acb" → IMPRIMIR
 *    - Elegir 'b': result="b"
 *      - Similar proceso...
 */

/*
 * VERSIÓN ALTERNATIVA CON ALGORITMO DE HEAP:
 * (Más eficiente para strings largos)
 */
void heap_permute(char *str, int size)
{
    // Caso base
    if (size == 1)
    {
        puts(str);
        return;
    }
    
    int i;
    for (i = 0; i < size; i++)
    {
        heap_permute(str, size - 1);
        
        // Si size es impar, intercambiar primer y último
        // Si size es par, intercambiar i-ésimo y último
        if (size % 2 == 1)
            ft_swap(&str[0], &str[size - 1]);
        else
            ft_swap(&str[i], &str[size - 1]);
    }
}

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. ORDEN ALFABÉTICO:
 *    - CRUCIAL ordenar el string antes de generar permutaciones
 *    - Sin ordenamiento, no se garantiza orden alfabético
 *    - Usar algoritmo de ordenamiento simple pero correcto
 * 
 * 2. BACKTRACKING EFICIENTE:
 *    - Usar array de caracteres usados para verificación O(n)
 *    - Marcar/desmarcar caracteres correctamente
 *    - No olvidar limpiar estado al retornar
 * 
 * 3. GESTIÓN DE MEMORIA:
 *    - Alocar buffer para permutación actual
 *    - Usar calloc() para inicializar a '\0'
 *    - Liberar memoria al finalizar
 * 
 * 4. VALIDACIÓN ROBUSTA:
 *    - Verificar número de argumentos
 *    - Validar que solo contiene letras
 *    - Manejar casos especiales (string vacío, espacios)
 * 
 * 5. OPTIMIZACIONES:
 *    - Para strings con caracteres duplicados, usar algoritmo diferente
 *    - Para strings muy largos, considerar algoritmo de Heap
 *    - Evitar regenerar permutaciones duplicadas
 */