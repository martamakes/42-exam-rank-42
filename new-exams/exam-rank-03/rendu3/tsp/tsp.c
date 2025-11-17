/*
 * EJERCICIO: TSP (Traveling Salesman Problem)
 * 
 * DESCRIPCIÓN:
 * Encontrar el camino más corto que visite todas las ciudades exactamente una vez
 * y regrese al punto de partida (ciclo hamiltoniano).
 * 
 * CONCEPTOS CLAVE:
 * 1. PROBLEMA NP-COMPLETO: No hay solución polinomial conocida
 * 2. FUERZA BRUTA: Con ≤11 ciudades es factible (11! permutaciones)
 * 3. DISTANCIA EUCLIDIANA: sqrt((x2-x1)² + (y2-y1)²)
 * 4. PERMUTACIONES: Generar todos los órdenes posibles de visita
 * 5. OPTIMIZACIÓN: Fijar primera ciudad para reducir cálculos
 * 
 * ALGORITMO:
 * 1. Leer coordenadas de ciudades desde stdin
 * 2. Generar todas las permutaciones de ciudades
 * 3. Calcular distancia total para cada permutación (incluyendo vuelta)
 * 4. Encontrar la permutación con menor distancia
 * 5. Imprimir resultado con 2 decimales
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// Estructura para representar una ciudad
typedef struct {
    float x, y;
} City;

// Función para calcular distancia euclidiana entre dos ciudades
float calculate_distance(City a, City b)
{
    /*
     * DISTANCIA EUCLIDIANA:
     * d = √[(x2-x1)² + (y2-y1)²]
     * 
     * Usar sqrtf() para mayor eficiencia con floats
     * Recordar compilar con -lm para enlazar biblioteca matemática
     */
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

// Función para calcular distancia total de un recorrido completo
float calculate_total_distance(City *cities, int *path, int n)
{
    /*
     * DISTANCIA TOTAL DEL CICLO:
     * - Sumar distancias entre ciudades consecutivas en el path
     * - IMPORTANTE: Agregar distancia de vuelta (última → primera)
     *   para completar el ciclo hamiltoniano
     */
    float total = 0.0f;
    int i;
    
    // Distancias entre ciudades consecutivas
    for (i = 0; i < n - 1; i++)
    {
        total += calculate_distance(cities[path[i]], cities[path[i + 1]]);
    }
    
    // Distancia de vuelta al inicio (cerrar el ciclo)
    total += calculate_distance(cities[path[n - 1]], cities[path[0]]);
    
    return total;
}

// Función para intercambiar dos elementos
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Función recursiva para generar permutaciones y encontrar mínimo
void find_shortest_path(City *cities, int *path, int n, int pos, float *min_distance)
{
    /*
     * ALGORITMO DE HEAP PARA PERMUTACIONES:
     * 
     * CASO BASE: pos == n
     * - Hemos generado una permutación completa
     * - Calcular distancia total del recorrido
     * - Actualizar mínimo si es menor
     * 
     * CASO RECURSIVO: pos < n
     * - Para cada posición restante i >= pos:
     *   - Intercambiar path[pos] con path[i]
     *   - Recursar para la siguiente posición
     *   - Restaurar intercambio (backtrack)
     */
    
    if (pos == n)
    {
        // Permutación completa generada
        float current_distance = calculate_total_distance(cities, path, n);
        
        if (current_distance < *min_distance)
            *min_distance = current_distance;
        
        return;
    }
    
    // Generar todas las permutaciones intercambiando elementos
    for (int i = pos; i < n; i++)
    {
        swap(&path[pos], &path[i]);              // Intercambiar
        find_shortest_path(cities, path, n, pos + 1, min_distance);  // Recursar
        swap(&path[pos], &path[i]);              // Restaurar (backtrack)
    }
}

int main(void)
{
    /*
     * PROCESAMIENTO PRINCIPAL:
     * 1. Leer coordenadas desde stdin hasta EOF
     * 2. Validar número mínimo de ciudades
     * 3. Inicializar path y distancia mínima
     * 4. Aplicar optimización: fijar primera ciudad
     * 5. Encontrar camino más corto
     * 6. Imprimir resultado
     */
    
    City cities[12];  // Máximo 11 ciudades + margen
    int n = 0;
    
    // Leer coordenadas desde stdin
    while (n < 11 && fscanf(stdin, "%f, %f", &cities[n].x, &cities[n].y) == 2)
    {
        n++;
    }
    
    // Casos especiales
    if (n < 2)
    {
        printf("0.00\n");
        return 0;
    }
    
    // Inicializar path: [0, 1, 2, ..., n-1]
    int path[12];
    for (int i = 0; i < n; i++)
    {
        path[i] = i;
    }
    
    float min_distance = FLT_MAX;
    
    /*
     * OPTIMIZACIÓN CRUCIAL:
     * 
     * Como buscamos un CICLO, todas las rotaciones circulares
     * dan la misma distancia total:
     * [0,1,2,3] ≡ [1,2,3,0] ≡ [2,3,0,1] ≡ [3,0,1,2]
     * 
     * Por tanto, podemos FIJAR la primera ciudad en posición 0
     * y solo permutar las demás. Esto reduce la complejidad de
     * n! a (n-1)!, una mejora significativa.
     * 
     * Para 11 ciudades: de 39,916,800 a 3,628,800 permutaciones
     */
    
    // Generar permutaciones solo para las ciudades 1..n-1
    // (manteniendo la ciudad 0 fija en la primera posición)
    find_shortest_path(cities, path, n, 1, &min_distance);
    
    // Imprimir resultado con 2 decimales
    printf("%.2f\n", min_distance);
    
    return 0;
}

/*
 * EJEMPLO DE EJECUCIÓN:
 * 
 * Input:
 * 0, 0
 * 1, 0  
 * 1, 1
 * 0, 1
 * 
 * Ciudades:
 * (0,0) → (1,0) → (1,1) → (0,1) → vuelta a (0,0)
 * 
 * Distancias:
 * (0,0)→(1,0): 1.00
 * (1,0)→(1,1): 1.00  
 * (1,1)→(0,1): 1.00
 * (0,1)→(0,0): 1.00
 * Total: 4.00
 * 
 * Output: 4.00
 */

/*
 * COMPLEJIDAD Y OPTIMIZACIONES:
 * 
 * 1. COMPLEJIDAD TEMPORAL:
 *    - Fuerza bruta: O(n!)
 *    - Con optimización: O((n-1)!)
 *    - Para n=11: ~3.6 millones de permutaciones
 * 
 * 2. OPTIMIZACIONES ADICIONALES:
 *    - Programación dinámica con bitmasks: O(n²2ⁿ)
 *    - Algoritmo de Christofides: aproximación 1.5x óptimo
 *    - Heurísticas: nearest neighbor, 2-opt, etc.
 * 
 * 3. PODA TEMPRANA:
 *    - Si distancia parcial > min_actual, podar rama
 *    - Usar cota inferior (MST) para poda agresiva
 * 
 * 4. PRECISIÓN NUMÉRICA:
 *    - Usar float para eficiencia
 *    - Cuidar errores de redondeo en comparaciones
 */

/*
 * IMPLEMENTACIÓN ALTERNATIVA CON PODA:
 * 
 * void find_shortest_with_pruning(City *cities, int *path, int n, int pos, 
 *                                 float current_dist, float *min_dist)
 * {
 *     // Poda: si distancia actual ya supera el mínimo
 *     if (current_dist >= *min_dist)
 *         return;
 *     
 *     if (pos == n)
 *     {
 *         // Agregar distancia de vuelta
 *         float total = current_dist + calculate_distance(cities[path[n-1]], cities[path[0]]);
 *         if (total < *min_dist)
 *             *min_dist = total;
 *         return;
 *     }
 *     
 *     for (int i = pos; i < n; i++)
 *     {
 *         swap(&path[pos], &path[i]);
 *         
 *         float next_dist = current_dist;
 *         if (pos > 0)
 *             next_dist += calculate_distance(cities[path[pos-1]], cities[path[pos]]);
 *         
 *         find_shortest_with_pruning(cities, path, n, pos + 1, next_dist, min_dist);
 *         
 *         swap(&path[pos], &path[i]);
 *     }
 * }
 */

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. LECTURA DE ENTRADA:
 *    - Formato: "x, y" por línea
 *    - Usar fscanf(stdin, "%f, %f", &x, &y)
 *    - Leer hasta EOF o máximo 11 ciudades
 * 
 * 2. DISTANCIA EUCLIDIANA:
 *    - Fórmula: sqrt((x2-x1)² + (y2-y1)²)
 *    - Usar sqrtf() para floats
 *    - Compilar con -lm
 * 
 * 3. CICLO HAMILTONIANO:
 *    - CRUCIAL: agregar distancia de vuelta al inicio
 *    - Sin esto, el problema sería "shortest path" no TSP
 * 
 * 4. OPTIMIZACIÓN DE PERMUTACIONES:
 *    - Fijar primera ciudad reduce factorial
 *    - Fundamental para que el algoritmo termine en tiempo razonable
 * 
 * 5. CASOS ESPECIALES:
 *    - 0 o 1 ciudades: distancia 0.00
 *    - 2 ciudades: 2 × distancia entre ellas
 *    - Entrada inválida: manejar graciosamente
 */