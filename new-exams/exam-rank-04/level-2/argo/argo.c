/*
 * EJERCICIO: ARGO (JSON Parser)
 * 
 * DESCRIPCIÓN:
 * Implementar un parser de JSON que construya un AST (Abstract Syntax Tree)
 * y pueda serializar de vuelta a formato JSON.
 * 
 * CONCEPTOS CLAVE:
 * 1. PARSING: Análisis sintáctico de texto estructurado
 * 2. AST: Representación en árbol del código/datos
 * 3. RECURSIVE DESCENT: Técnica de parsing top-down
 * 4. TOKENIZACIÓN: Reconocer elementos básicos (números, strings, etc.)
 * 5. GESTIÓN DE MEMORIA: Structures dinámicas enlazadas
 * 
 * FORMATO JSON SOPORTADO:
 * - Números enteros: 42, -17
 * - Strings: "hello", "escape \" test"  
 * - Maps/Objects: {"key": value, "key2": value2}
 * - Escape sequences: \\ y \" solamente
 */

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Definición de estructuras para el AST
typedef struct json {
    enum {
        MAP,
        INTEGER,
        STRING
    } type;
    union {
        struct {
            struct pair *data;
            size_t size;
        } map;
        int integer;
        char *string;
    };
} json;

typedef struct pair {
    char *key;
    json value;
} pair;

// Declaraciones de funciones
int argo(json *dst, FILE *stream);
int parser(json *dst, FILE *stream);
int parse_int(json *dst, FILE *stream);
int parse_string(json *dst, FILE *stream);
int parse_map(json *dst, FILE *stream);
void free_json(json j);

/*
 * FUNCIONES AUXILIARES PARA PARSING:
 */

// Función para mirar el siguiente carácter sin consumirlo
int peek(FILE *stream)
{
    int c = getc(stream);
    ungetc(c, stream);
    return c;
}

// Función para reportar errores de parsing
void unexpected(FILE *stream)
{
    if (peek(stream) != EOF)
        printf("unexpected token '%c'\n", peek(stream));
    else
        printf("unexpected end of input\n");
}

// Función para aceptar un carácter específico
int accept(FILE *stream, char c)
{
    if (peek(stream) == c)
    {
        (void)getc(stream);
        return 1;
    }
    return 0;
}

// Función para requerir un carácter específico
int expect(FILE *stream, char c)
{
    if (accept(stream, c))
        return 1;
    unexpected(stream);
    return 0;
}

/*
 * FUNCIÓN PRINCIPAL DE PARSING:
 * Determina qué tipo de valor JSON estamos parseando
 */
int parser(json *dst, FILE *stream)
{
    /*
     * DISPATCHER DE TIPOS:
     * - '"': String
     * - Dígito o '-': Número entero
     * - '{': Object/Map
     * - Cualquier otra cosa: Error
     */
    int c = peek(stream);
    
    if (c == '"')
        return parse_string(dst, stream);
    else if (isdigit(c) || c == '-')
        return parse_int(dst, stream);
    else if (c == '{')
        return parse_map(dst, stream);
    else
    {
        unexpected(stream);
        return -1;
    }
}

/*
 * PARSER DE NÚMEROS ENTEROS:
 */
int parse_int(json *dst, FILE *stream)
{
    /*
     * PARSING DE ENTEROS:
     * - Usar fscanf para leer entero con signo
     * - fscanf maneja automáticamente '+', '-' y dígitos
     * - Si fscanf retorna 1, el parsing fue exitoso
     */
    int n;
    
    if (fscanf(stream, "%d", &n) == 1)
    {
        dst->type = INTEGER;
        dst->integer = n;
        return 1;
    }
    
    unexpected(stream);
    return -1;
}

/*
 * PARSER DE STRINGS:
 */
int parse_string(json *dst, FILE *stream)
{
    /*
     * PARSING DE STRINGS:
     * 1. Esperar comilla de apertura '"'
     * 2. Leer caracteres hasta comilla de cierre
     * 3. Manejar escape sequences: \" y \\
     * 4. Alocar memoria y copiar string
     */
    char buffer[4096];  // Buffer temporal para el string
    char c;
    int i = 0;
    
    // Consumir comilla de apertura
    if (!expect(stream, '"'))
        return -1;
    
    while (1)
    {
        c = getc(stream);
        
        if (c == EOF)
        {
            unexpected(stream);
            return -1;
        }
        
        if (c == '"')  // Comilla de cierre
            break;
        
        if (c == '\\')  // Escape sequence
        {
            c = getc(stream);
            if (c == EOF)
            {
                unexpected(stream);
                return -1;
            }
            // Solo manejar \\ y \"
            // Cualquier otro escape se incluye literalmente
        }
        
        buffer[i++] = c;
        
        // Prevenir buffer overflow
        if (i >= sizeof(buffer) - 1)
        {
            unexpected(stream);
            return -1;
        }
    }
    
    buffer[i] = '\0';
    
    // Alocar memoria para el string
    dst->type = STRING;
    dst->string = strdup(buffer);
    if (!dst->string)
        return -1;
    
    return 1;
}

/*
 * PARSER DE MAPS/OBJECTS:
 */
int parse_map(json *dst, FILE *stream)
{
    /*
     * PARSING DE OBJECTS:
     * 1. Esperar llave de apertura '{'
     * 2. Parsear pares key:value separados por comas
     * 3. Keys deben ser strings
     * 4. Values pueden ser cualquier tipo JSON válido
     * 5. Esperar llave de cierre '}'
     */
    pair *items = NULL;
    size_t size = 0;
    json key;
    
    // Consumir llave de apertura
    if (!expect(stream, '{'))
        return -1;
    
    // Manejar object vacío
    if (accept(stream, '}'))
    {
        dst->type = MAP;
        dst->map.size = 0;
        dst->map.data = NULL;
        return 1;
    }
    
    // Parsear pares key:value
    while (1)
    {
        // Expandir array de items
        items = realloc(items, sizeof(pair) * (size + 1));
        if (!items)
            return -1;
        
        // Parsear key (debe ser string)
        if (parse_string(&key, stream) == -1)
        {
            free(items);
            return -1;
        }
        
        // Esperar ':'
        if (!expect(stream, ':'))
        {
            free(key.string);
            free(items);
            return -1;
        }
        
        // Parsear value (cualquier tipo JSON)
        if (parser(&items[size].value, stream) == -1)
        {
            free(key.string);
            free(items);
            return -1;
        }
        
        // Guardar key
        items[size].key = key.string;
        size++;
        
        // Verificar si hay más elementos
        if (accept(stream, ','))
        {
            // Hay más elementos, continuar
            continue;
        }
        else if (peek(stream) == '}')
        {
            // Fin del object
            break;
        }
        else
        {
            // Token inesperado
            unexpected(stream);
            
            // Liberar memoria allocada
            for (size_t j = 0; j < size; j++)
            {
                free(items[j].key);
                free_json(items[j].value);
            }
            free(items);
            return -1;
        }
    }
    
    // Consumir llave de cierre
    if (!expect(stream, '}'))
    {
        // Liberar memoria en caso de error
        for (size_t j = 0; j < size; j++)
        {
            free(items[j].key);
            free_json(items[j].value);
        }
        free(items);
        return -1;
    }
    
    // Configurar resultado
    dst->type = MAP;
    dst->map.size = size;
    dst->map.data = items;
    
    return 1;
}

/*
 * FUNCIÓN PRINCIPAL DEL PARSER:
 */
int argo(json *dst, FILE *stream)
{
    return parser(dst, stream);
}

/*
 * FUNCIÓN PARA LIBERAR MEMORIA DEL AST:
 */
void free_json(json j)
{
    switch (j.type)
    {
        case MAP:
            for (size_t i = 0; i < j.map.size; i++)
            {
                free(j.map.data[i].key);
                free_json(j.map.data[i].value);
            }
            free(j.map.data);
            break;
        
        case STRING:
            free(j.string);
            break;
        
        case INTEGER:
            // No hay memoria que liberar
            break;
    }
}

/*
 * FUNCIÓN PARA SERIALIZAR AST DE VUELTA A JSON:
 */
void serialize(json j)
{
    switch (j.type)
    {
        case INTEGER:
            printf("%d", j.integer);
            break;
            
        case STRING:
            putchar('"');
            for (int i = 0; j.string[i]; i++)
            {
                // Escapar caracteres especiales
                if (j.string[i] == '\\' || j.string[i] == '"')
                    putchar('\\');
                putchar(j.string[i]);
            }
            putchar('"');
            break;
            
        case MAP:
            putchar('{');
            for (size_t i = 0; i < j.map.size; i++)
            {
                if (i != 0)
                    putchar(',');
                
                // Serializar key como string
                json key_json = {.type = STRING, .string = j.map.data[i].key};
                serialize(key_json);
                
                putchar(':');
                
                // Serializar value
                serialize(j.map.data[i].value);
            }
            putchar('}');
            break;
    }
}

/*
 * FUNCIÓN MAIN PARA TESTING:
 */
int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;
    
    char *filename = argv[1];
    FILE *stream = fopen(filename, "r");
    if (!stream)
        return 1;
    
    json file;
    
    if (argo(&file, stream) != 1)
    {
        free_json(file);
        fclose(stream);
        return 1;
    }
    
    serialize(file);
    printf("\n");
    
    free_json(file);
    fclose(stream);
    return 0;
}

/*
 * EJEMPLOS DE USO:
 * 
 * Input: "42"
 * Output: 42
 * 
 * Input: "\"hello world\""
 * Output: "hello world"
 * 
 * Input: "{\"name\":\"John\",\"age\":30}"
 * Output: {"name":"John","age":30}
 * 
 * Input: "{\"nested\":{\"value\":42}}"
 * Output: {"nested":{"value":42}}
 */

/*
 * TÉCNICAS DE PARSING EMPLEADAS:
 * 
 * 1. RECURSIVE DESCENT:
 *    - Cada tipo de elemento tiene su función de parsing
 *    - Las funciones se llaman recursivamente para elementos anidados
 *    - Estructura natural que refleja la gramática JSON
 * 
 * 2. LOOKAHEAD:
 *    - peek() para mirar el siguiente carácter sin consumirlo
 *    - Permite decidir qué función de parsing llamar
 *    - Essential para parsing sin backtracking
 * 
 * 3. ERROR RECOVERY:
 *    - unexpected() para reportar errores específicos
 *    - Liberación de memoria en caso de error
 *    - Propagación de errores hacia arriba en la recursión
 */

/*
 * GESTIÓN DE MEMORIA:
 * 
 * 1. STRINGS:
 *    - Cada string se alloca dinámicamente con strdup()
 *    - Debe liberarse en free_json()
 * 
 * 2. MAPS:
 *    - Array de pairs se expande con realloc()
 *    - Cada key es un string allocado
 *    - Cada value puede contener más allocaciones
 * 
 * 3. CLEANUP EN ERRORES:
 *    - Crucial liberar memoria parcialmente allocada
 *    - Evitar memory leaks cuando parsing falla
 */

/*
 * LIMITACIONES DE ESTA IMPLEMENTACIÓN:
 * 
 * 1. NO MANEJA:
 *    - Arrays JSON: [1, 2, 3]
 *    - Booleanos: true, false
 *    - null values
 *    - Números flotantes: 3.14
 *    - Unicode escapes: \u1234
 *    - Whitespace entre tokens
 * 
 * 2. SOLO MANEJA:
 *    - Enteros: 42, -17
 *    - Strings con escapes básicos: "hello \"world\""
 *    - Objects: {"key": value}
 */

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. ESTRUCTURA DEL AST:
 *    - Union para diferentes tipos de datos
 *    - Enum para identificar el tipo
 *    - Punteros para datos de tamaño variable
 * 
 * 2. PARSING RECURSIVO:
 *    - Cada función parsea un tipo específico
 *    - parser() es el dispatcher principal
 *    - Recursión natural para elementos anidados
 * 
 * 3. MANEJO DE ERRORES:
 *    - Verificar retornos de todas las funciones
 *    - Liberar memoria en caso de error
 *    - Reportar errores descriptivos
 * 
 * 4. SERIALIZACIÓN:
 *    - Recorrer AST y generar JSON válido
 *    - Manejar escape sequences correctamente
 *    - Mantener formato correcto (comas, llaves, etc.)
 * 
 * 5. GESTIÓN DE MEMORIA:
 *    - Toda memoria dinámica debe liberarse
 *    - free_json() debe ser recursiva
 *    - Cuidado con double-free y use-after-free
 */