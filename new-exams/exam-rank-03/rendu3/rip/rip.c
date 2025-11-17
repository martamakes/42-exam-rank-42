/*
 * EJERCICIO: RIP
 * 
 * DESCRIPCIÓN:
 * Dada una cadena de paréntesis desbalanceada, remover el mínimo número de paréntesis
 * para hacer que la expresión esté balanceada. Imprimir todas las soluciones posibles.
 * 
 * CONCEPTOS CLAVE:
 * 1. VALIDACIÓN DE PARÉNTESIS: Algoritmo clásico con contador
 * 2. BACKTRACKING: Explorar todas las combinaciones de remoción
 * 3. OPTIMIZACIÓN: Encontrar primero el mínimo número de remociones
 * 4. GENERACIÓN: Producir todas las soluciones con el mínimo encontrado
 * 
 * ALGORITMO:
 * 1. Encontrar el mínimo número de paréntesis a remover
 * 2. Generar todas las combinaciones removiendo exactamente esa cantidad
 * 3. Verificar cuáles resultan en expresiones balanceadas
 * 4. Imprimir todas las soluciones válidas
 */

#include <unistd.h>
#include <stdio.h>

// Función auxiliar para calcular longitud
int ft_strlen(char *s)
{
    int i = 0;
    while (s[i])
        i++;
    return i;
}

// Función para verificar si una cadena está balanceada
int is_balanced(char *str, int len)
{
    /*
     * ALGORITMO DE VALIDACIÓN DE PARÉNTESIS:
     * - Usar contador para paréntesis abiertos
     * - Incrementar por '(', decrementar por ')'
     * - Si contador < 0 en cualquier momento: desbalanceado
     * - Al final, contador debe ser 0
     */
    int balance = 0;
    int i = 0;
    
    while (i < len)
    {
        if (str[i] == '(')
            balance++;
        else if (str[i] == ')')
        {
            balance--;
            if (balance < 0)  // Más ')' que '(' hasta ahora
                return 0;
        }
        // Ignorar espacios (caracteres removidos)
        i++;
    }
    
    return (balance == 0);  // Balanceado si contador final es 0
}

// Función para encontrar el mínimo número de remociones necesarias
void find_min_removals(char *str, int *min_removals, int index, int current_removals)
{
    /*
     * BÚSQUEDA DEL MÍNIMO:
     * - Probar remover cada paréntesis uno por uno
     * - Si la cadena resultante está balanceada, actualizar mínimo
     * - Usar backtracking para explorar todas las combinaciones
     * - Podar si current_removals ya es mayor que el mínimo conocido
     */
    
    // Poda: si ya removimos más que el mínimo conocido, no continuar
    if (current_removals > *min_removals)
        return;
    
    // Verificar si la cadena actual está balanceada
    if (is_balanced(str, ft_strlen(str)))
    {
        if (current_removals < *min_removals)
            *min_removals = current_removals;
        return;
    }
    
    // Probar remover cada paréntesis desde la posición index
    int i = index;
    while (str[i])
    {
        // Solo intentar remover paréntesis, no espacios
        if (str[i] == '(' || str[i] == ')')
        {
            char saved = str[i];
            str[i] = ' ';  // "Remover" reemplazando con espacio
            
            find_min_removals(str, min_removals, i + 1, current_removals + 1);
            
            str[i] = saved;  // Restaurar para probar otras opciones
        }
        i++;
    }
}

// Función para generar todas las soluciones válidas
void generate_solutions(char *str, int min_removals, int index, int current_removals)
{
    /*
     * GENERACIÓN DE SOLUCIONES:
     * - Solo explorar combinaciones que remuevan exactamente min_removals
     * - Verificar si cada combinación está balanceada
     * - Imprimir las que sí lo estén
     * - Usar backtracking para explorar sistemáticamente
     */
    
    // Poda: si ya removimos más que el mínimo, no continuar
    if (current_removals > min_removals)
        return;
    
    // Si removimos exactamente el mínimo, verificar si está balanceada
    if (is_balanced(str, ft_strlen(str)) && current_removals == min_removals)
    {
        // Imprimir solución válida
        write(1, str, ft_strlen(str));
        write(1, "\n", 1);
        return;
    }
    
    // Probar remover cada paréntesis desde la posición index
    int i = index;
    while (str[i])
    {
        // Solo intentar remover paréntesis, no espacios
        if (str[i] == '(' || str[i] == ')')
        {
            char saved = str[i];
            str[i] = ' ';  // "Remover" reemplazando con espacio
            
            generate_solutions(str, min_removals, i + 1, current_removals + 1);
            
            str[i] = saved;  // Restaurar para probar otras opciones
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    /*
     * VALIDACIÓN Y PROCESAMIENTO:
     * - Verificar argumentos válidos
     * - Validar que solo contiene paréntesis
     * - Encontrar número mínimo de remociones
     * - Generar todas las soluciones con ese mínimo
     */
    
    if (argc != 2 || argv[1][0] == '\0')
        return 1;
    
    // Validar que el string solo contiene paréntesis
    int i = 0;
    while (argv[1][i])
    {
        if (argv[1][i] != '(' && argv[1][i] != ')')
            return 1;
        i++;
    }
    
    int min_removals = ft_strlen(argv[1]);  // Inicializar con máximo posible
    
    // Encontrar el mínimo número de remociones necesarias
    find_min_removals(argv[1], &min_removals, 0, 0);
    
    // Generar todas las soluciones con el mínimo número de remociones
    generate_solutions(argv[1], min_removals, 0, 0);
    
    return 0;
}

/*
 * EJEMPLO DE EJECUCIÓN:
 * 
 * ./rip '(()'
 * 
 * PROCESO:
 * 1. String original: "(()"
 * 2. Verificar si está balanceado: NO (balance = 1 al final)
 * 3. Encontrar mínimo:
 *    - Remover primer '(': "())" → balance = -1 → NO válido
 *    - Remover segundo '(': "())" → balance = -1 → NO válido  
 *    - Remover ')': "((" → balance = 2 → NO válido
 *    - Remover primer '(' y ')': " ()" → balance = 0 → VÁLIDO (1 remoción)
 *    - Remover segundo '(' y ')': "( )" → balance = 0 → VÁLIDO (1 remoción)
 * 4. Mínimo = 1
 * 5. Generar soluciones con 1 remoción:
 *    - " ()" → Imprimir
 *    - "( )" → Imprimir
 * 
 * Salida:
 *  ()
 * ( )
 */

/*
 * ALGORITMO OPTIMIZADO (para casos complejos):
 * 
 * void rip_optimized(char *str)
 * {
 *     // Contar paréntesis desbalanceados
 *     int open = 0, close = 0;
 *     
 *     // Primera pasada: contar ')' excedentes
 *     for (int i = 0; str[i]; i++)
 *     {
 *         if (str[i] == '(')
 *             open++;
 *         else if (str[i] == ')' && open > 0)
 *             open--;
 *         else if (str[i] == ')')
 *             close++;  // ')' sin '(' correspondiente
 *     }
 *     
 *     // open = '(' excedentes, close = ')' excedentes
 *     // Total a remover = open + close
 *     
 *     generate_solutions_optimized(str, 0, 0, open, close);
 * }
 */

/*
 * CASOS ESPECIALES:
 * 
 * 1. String ya balanceado: "()" → No remover nada
 * 2. Solo paréntesis abiertos: "(((" → Remover todos
 * 3. Solo paréntesis cerrados: ")))" → Remover todos
 * 4. String vacío: "" → Ya balanceado
 * 5. Un solo paréntesis: "(" o ")" → Remover el único
 */

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. VALIDACIÓN DE PARÉNTESIS:
 *    - Algoritmo clásico con contador
 *    - balance++ para '(', balance-- para ')'
 *    - balance < 0 en cualquier momento = inválido
 *    - balance == 0 al final = válido
 * 
 * 2. BACKTRACKING EN DOS FASES:
 *    - Fase 1: Encontrar mínimo número de remociones
 *    - Fase 2: Generar todas las soluciones con ese mínimo
 *    - Usar misma estructura recursiva para ambas
 * 
 * 3. REPRESENTACIÓN DE REMOCIÓN:
 *    - Reemplazar con espacio ' ' en lugar de eliminar
 *    - Más fácil que manejar strings de longitud variable
 *    - Restaurar carácter original después de cada prueba
 * 
 * 4. PODA EFICIENTE:
 *    - Parar si current_removals > min_removals
 *    - Evita explorar ramas innecesarias
 *    - Crucial para performance en strings largos
 * 
 * 5. MÚLTIPLES SOLUCIONES:
 *    - Pueden existir varias formas de remover el mínimo
 *    - Generar todas sistemáticamente
 *    - No permitir duplicados (orden de exploración importante)
 */