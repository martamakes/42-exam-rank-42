/*
 * EJERCICIO: POWERSET
 * 
 * DESCRIPCIÓN:
 * Encontrar todos los subconjuntos de un conjunto de enteros cuya suma sea igual a un target.
 * 
 * CONCEPTOS CLAVE:
 * 1. BACKTRACKING: Explorar todas las combinaciones posibles
 * 2. SUBSET SUM: Problema clásico de suma de subconjuntos
 * 3. DECISIÓN BINARIA: Para cada elemento, incluir o no incluir
 * 4. ORDEN PRESERVADO: Los elementos deben mantener el orden original
 * 
 * ALGORITMO:
 * 1. Para cada elemento, tomar dos decisiones: incluir o no incluir
 * 2. Mantener suma actual y subset actual
 * 3. Si suma == target, imprimir subset
 * 4. Continuar recursivamente con resto de elementos
 */

#include <stdio.h>
#include <stdlib.h>

// Función para imprimir un subset
void print_subset(int *subset, int size)
{
    /*
     * FORMATO DE SALIDA:
     * - Si size == 0, imprimir línea vacía (subset vacío)
     * - Imprimir elementos separados por espacio
     * - Terminar con newline
     */
    if (size == 0)
    {
        printf("\n");
        return;
    }
    
    int i = 0;
    while (i < size)
    {
        if (i == size - 1)
            printf("%d", subset[i]);
        else
            printf("%d ", subset[i]);
        i++;
    }
    printf("\n");
}

// Función recursiva de backtracking
void backtrack(int *set, int *subset, int set_size, int subset_size, 
               int index, int current_sum, int target, int *found)
{
    /*
     * PARÁMETROS:
     * - set: Array original de números
     * - subset: Array actual que estamos construyendo
     * - set_size: Tamaño del array original
     * - subset_size: Tamaño actual del subset
     * - index: Posición actual en el array original
     * - current_sum: Suma actual del subset
     * - target: Suma objetivo
     * - found: Flag para indicar si encontramos alguna solución
     * 
     * LÓGICA:
     * En cada llamada, tenemos dos opciones:
     * 1. NO incluir el elemento actual (solo avanzar index)
     * 2. SÍ incluir el elemento actual (agregarlo al subset y actualizar suma)
     */
    
    // Caso base: hemos procesado todos los elementos
    if (index == set_size)
    {
        if (current_sum == target)
        {
            print_subset(subset, subset_size);
            *found = 1;
        }
        return;
    }
    
    /*
     * DECISIÓN 1: NO incluir el elemento actual
     * - No modificamos subset ni current_sum
     * - Solo avanzamos al siguiente elemento
     */
    backtrack(set, subset, set_size, subset_size, index + 1, current_sum, target, found);
    
    /*
     * DECISIÓN 2: SÍ incluir el elemento actual
     * - Agregar elemento al subset
     * - Actualizar suma
     * - Incrementar tamaño del subset
     */
    subset[subset_size] = set[index];
    backtrack(set, subset, set_size, subset_size + 1, index + 1, 
              current_sum + set[index], target, found);
    
    // Nota: No necesitamos "deshacer" explícitamente porque
    // subset[subset_size] se sobreescribirá en la próxima llamada
}

// Función para validar argumentos numéricos
int check_args(int argc, char **argv)
{
    /*
     * VALIDACIÓN DE ARGUMENTOS:
     * - Verificar que todos los argumentos son números válidos
     * - Permitir números negativos (comenzando con '-')
     * - Permitir números positivos (opcionalmente con '+')
     */
    int i = 1;
    while (i < argc)
    {
        int j = 0;
        
        // Saltar signo inicial si existe
        if (argv[i][j] == '+' || argv[i][j] == '-')
            j++;
        
        // Verificar que el resto son dígitos
        while (argv[i][j])
        {
            if (argv[i][j] < '0' || argv[i][j] > '9')
                return 0; // No es un número válido
            j++;
        }
        i++;
    }
    return 1; // Todos los argumentos son válidos
}

int main(int argc, char **argv)
{
    /*
     * CASOS ESPECIALES:
     * - argc == 1: Solo el nombre del programa
     * - target == 0: El subset vacío siempre suma 0
     * - Argumentos inválidos: Retornar error
     */
    
    // Caso especial: target = 0, imprimir línea vacía y salir
    if (argc == 1 || (argc >= 2 && argv[1][0] == '0' && argv[1][1] == '\0'))
    {
        printf("\n");
        return 0;
    }

    // Validar argumentos
    if (!check_args(argc, argv))
        return 1;

    int target = atoi(argv[1]);
    int set_size = argc - 2;  // Excluir programa y target
    
    if (set_size <= 0)
        return 1;

    // Alocar arrays para el conjunto y el subset actual
    int *set = malloc(set_size * sizeof(int));
    int *subset = malloc(set_size * sizeof(int));
    
    if (!set || !subset)
    {
        free(set);
        free(subset);
        return 1;
    }

    // Llenar el array con los números del comando
    int i = 0;
    int j = 2; // Comenzar después del target
    while (j < argc)
    {
        set[i] = atoi(argv[j]);
        i++;
        j++;
    }

    int found = 0;
    
    // Iniciar backtracking
    backtrack(set, subset, set_size, 0, 0, 0, target, &found);
    
    // Si no se encontró ninguna solución, imprimir línea vacía
    if (!found)
        printf("\n");

    free(set);
    free(subset);
    return 0;
}

/*
 * EJEMPLO DE EJECUCIÓN:
 * 
 * ./powerset 3 1 0 2 4 5 3
 * 
 * PROCESO:
 * - Target: 3
 * - Set: [1, 0, 2, 4, 5, 3]
 * 
 * Subsets que suman 3:
 * - [3] → suma = 3 ✓
 * - [0, 3] → suma = 3 ✓  
 * - [1, 2] → suma = 3 ✓
 * - [1, 0, 2] → suma = 3 ✓
 * 
 * Salida:
 * 3
 * 0 3
 * 1 2
 * 1 0 2
 */

/*
 * ÁRBOL DE DECISIONES PARA [1, 0, 2] CON TARGET 3:
 * 
 *                    Root
 *                   /    \
 *            No incluir 1   Incluir 1
 *               /    \        /    \
 *        No inc 0   Inc 0   No inc 0   Inc 0
 *         /  \      /  \      /  \      /  \
 *    No inc 2  Inc 2  No inc 2  Inc 2  No inc 2  Inc 2
 *       |      |      |      |      |      |
 *    suma=0  suma=2  suma=0  suma=2  suma=1  suma=3 ✓
 * 
 * Solo el último caso (Inc 1, Inc 0, Inc 2) suma 3
 */

/*
 * OPTIMIZACIONES POSIBLES:
 * 
 * 1. PODA TEMPRANA:
 *    - Si current_sum > target y todos los elementos restantes son positivos, podar
 *    - Si current_sum + sum_remaining < target, podar
 * 
 * 2. ORDENAMIENTO:
 *    - Ordenar elementos puede mejorar la poda
 *    - Pero se debe mantener orden original en la salida
 * 
 * 3. PROGRAMACIÓN DINÁMICA:
 *    - Para conjuntos grandes, usar DP
 *    - Tabla dp[i][sum] = posible alcanzar suma con primeros i elementos
 */

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. PROBLEMA SUBSET SUM:
 *    - Problema NP-completo clásico
 *    - Backtracking es la solución exacta más directa
 *    - Complejidad O(2^n) en el peor caso
 * 
 * 2. ORDEN DE ELEMENTOS:
 *    - CRUCIAL mantener el orden original
 *    - No usar permutaciones, solo combinaciones
 *    - El subset [1,2] es diferente de [2,1] según el problema
 * 
 * 3. CASOS ESPECIALES:
 *    - Target = 0: Subset vacío siempre es solución
 *    - Sin elementos: Solo subset vacío posible
 *    - Números negativos: Permitidos y manejar correctamente
 * 
 * 4. GESTIÓN DE MEMORIA:
 *    - Arrays dinámicos para set y subset
 *    - Verificar malloc antes de usar
 *    - Liberar memoria al finalizar
 * 
 * 5. FORMATO DE SALIDA:
 *    - Línea vacía para subset vacío
 *    - Espacios entre números, no al final
 *    - Una línea por subset encontrado
 */