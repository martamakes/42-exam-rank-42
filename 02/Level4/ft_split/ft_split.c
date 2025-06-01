#include <stdlib.h>

/*
DESCRIPCIÓN DEL EJERCICIO:
- Dividir un string en palabras usando espacios/tabs/newlines como separadores
- Retornar array de strings terminado en NULL
- Solo malloc permitido
- Gestión de memoria crítica: si falla malloc intermedio, liberar todo

CONCEPTOS CLAVE:
1. GESTIÓN DINÁMICA DE MEMORIA:
   - malloc para array de punteros: sizeof(char *) * (num_palabras + 1)
   - malloc para cada palabra: sizeof(char) * (longitud_palabra + 1) 
   - +1 siempre para el terminador NULL

2. ALGORITMO DE PARSING:
   - Contar palabras primero para saber cuanta memoria asignar
   - Identificar separadores: ' ', '\t', '\n'
   - Extraer cada palabra carácter por carácter

3. MANEJO DE ERRORES:
   - Si input es NULL -> retornar NULL o array con [NULL]
   - Si malloc falla -> liberar toda memoria previamente asignada
   - Evitar memory leaks a toda costa

4. CASOS LÍMITE:
   - String vacío ""
   - Solo separadores "   \t\n  "
   - NULL input
   - Una sola palabra
*/

// Función auxiliar: Verifica si un carácter es separador
// SEPARADORES EN EL EXAMEN: solo space, tab, newline
static int is_separator(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

// Función auxiliar: Cuenta el número de palabras en el string
// ALGORITMO: recorre string, cuenta transiciones de separador->no_separador
static int count_words(char *str)
{
    int count = 0;
    int in_word = 0;  // Flag: 1 si estamos dentro de una palabra, 0 si no

    // Si el string es NULL, no hay palabras
    if (!str)
        return (0);

    while (*str)
    {
        if (is_separator(*str))
        {
            in_word = 0;  // Salimos de la palabra (si estábamos en una)
        }
        else if (in_word == 0)  // Si no estábamos en palabra y encontramos carácter válido
        {
            in_word = 1;  // Entramos en una nueva palabra
            count++;      // Incrementamos contador
        }
        str++;  // Avanzamos al siguiente carácter
    }
    return (count);
}

// Función auxiliar: Calcula la longitud de una palabra desde la posición actual
// IMPORTANTE: No incluye el '\0' en el conteo
static int word_len(char *str)
{
    int len = 0;
    
    // Contamos hasta encontrar separador o final de string
    while (str[len] && !is_separator(str[len]))
        len++;
    return (len);
}

// Función auxiliar: Libera toda la memoria asignada en caso de error
// CRÍTICO PARA EL EXAMEN: evitar memory leaks
static void free_all(char **result, int up_to)
{
    int i = 0;
    
    // Liberamos todas las palabras asignadas hasta el punto de fallo
    while (i < up_to)
    {
        free(result[i]);
        i++;
    }
    // Liberamos el array principal
    free(result);
}

// Función principal: ft_split
char **ft_split(char *str)
{
    // MANEJO DE NULL INPUT: crítico en el examen
    if (!str)
    {
        // Opción 1: Retornar NULL
        // return (NULL);
        
        // Opción 2: Retornar array vacío (más seguro para el examen)
        char **result = malloc(sizeof(char *) * 1);
        if (!result)
            return (NULL);
        result[0] = NULL;
        return (result);
    }

    // Paso 1: Contar palabras para saber cuanta memoria necesitamos
    int words = count_words(str);
    
    // Paso 2: Asignar memoria para array de punteros
    // +1 para el NULL final que termina el array
    char **result = malloc(sizeof(char *) * (words + 1));
    if (!result)
        return (NULL);  // Si falla malloc, retornamos NULL

    int i = 0;  // Índice para rellenar el array result

    // Paso 3: Procesar cada palabra
    while (*str && i < words)
    {
        // Saltamos todos los separadores iniciales
        while (*str && is_separator(*str))
            str++;

        // Si llegamos al final del string, terminamos
        if (*str == '\0')
            break;

        // Calculamos longitud de la palabra actual
        int len = word_len(str);
        
        // Asignamos memoria para esta palabra (+1 para '\0')
        result[i] = malloc(sizeof(char) * (len + 1));
        if (!result[i])
        {
            // GESTIÓN DE ERROR: si falla malloc, liberamos todo lo anterior
            free_all(result, i);
            return (NULL);
        }

        // Copiamos la palabra carácter por carácter
        int j = 0;
        while (j < len)
        {
            result[i][j] = str[j];
            j++;
        }
        // Añadimos terminador de string
        result[i][j] = '\0';

        // Avanzamos a la siguiente palabra
        i++;
        str += len;  // Movemos puntero str al final de la palabra actual
    }

    // Paso 4: Terminar el array con NULL (OBLIGATORIO)
    result[i] = NULL;

    return (result);
}

/*
PUNTOS CRÍTICOS PARA EL EXAMEN:

1. **GESTIÓN DE MEMORIA**:
   - Siempre verificar que malloc no retorne NULL
   - Si falla malloc intermedio, liberar TODO lo anterior
   - No olvidar +1 para '\0' en cada palabra
   - Array final debe terminar en NULL

2. **CASOS LÍMITE**:
   - NULL input: manejar sin segfault
   - String vacío: retornar array con solo NULL
   - Solo separadores: retornar array con solo NULL
   - Una palabra: funcionar correctamente

3. **SEPARADORES**:
   - Solo space (' '), tab ('\t'), newline ('\n')
   - TODO lo demás es parte de palabra válida

4. **ALGORITMO**:
   - Contar palabras primero
   - Procesar una por una
   - Copiar carácter por carácter
   - Terminar cada palabra con '\0'

5. **DEBUGGING**:
   - Si segfault: verificar NULL checks
   - Si memory leaks: verificar free_all en errores
   - Si palabras cortadas: verificar longitudes y '\0'

ERRORES COMUNES:
- No manejar NULL input
- Olvidar liberar memoria en caso de error
- No terminar array con NULL
- No terminar palabras con '\0'
- Confundir sizeof(char *) con sizeof(char)
*/