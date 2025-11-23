/*
 * EJERCICIO: BROKEN_GNL (Get Next Line)
 * 
 * DESCRIPCIÓN:
 * Implementar get_next_line que lee línea por línea de un file descriptor.
 * Esta versión puede tener casos especiales o "bugs" intencionales.
 * 
 * CONCEPTOS CLAVE:
 * 1. BUFFER ESTÁTICO: Mantener estado entre llamadas
 * 2. LECTURA POR CHUNKS: read() con BUFFER_SIZE
 * 3. GESTIÓN DE MEMORIA: malloc/free para líneas dinámicas
 * 4. DETECCIÓN DE NEWLINE: Encontrar '\n' para delimitar líneas
 * 
 * ALGORITMO:
 * 1. Usar buffer estático para persistir datos entre llamadas
 * 2. Leer chunks de BUFFER_SIZE hasta encontrar '\n' o EOF
 * 3. Extraer una línea completa (incluyendo '\n')
 * 4. Guardar el resto para la siguiente llamada
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 100
#endif

// Función auxiliar para calcular longitud de string
int ft_strlen(char *s)
{
    int i = 0;
    if (!s)
        return 0;
    while (s[i])
        i++;
    return i;
}

// Función auxiliar para buscar carácter en string
int ft_strchr(const char *s, char c)
{
    int i = 0;
    if (!s)
        return 0;
    while (s[i])
    {
        if (s[i] == c)
            return 1; // Encontrado
        i++;
    }
    return 0; // No encontrado
}

// Función auxiliar para concatenar dos strings
char *ft_strjoin(char *s1, char *s2)
{
    /*
     * CONCATENACIÓN DINÁMICA:
     * - Calcular longitudes de ambos strings
     * - Alocar memoria para el resultado
     * - Copiar s1 + s2 al nuevo string
     * - Liberar s1 (importante para evitar leaks)
     */
    if (!s1 || !s2)
        return NULL;
    
    int len1 = ft_strlen(s1);
    int len2 = ft_strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    
    if (!result)
        return NULL;
    
    int i = 0, j = 0;
    
    // Copiar s1
    while (s1[i])
    {
        result[i] = s1[i];
        i++;
    }
    
    // Copiar s2
    while (s2[j])
    {
        result[i] = s2[j];
        i++;
        j++;
    }
    
    result[i] = '\0';
    return result;
}

// Función para unir buffer con datos leídos
char *join_and_free(char *saved, char *buffer)
{
    /*
     * UNIÓN CON LIBERACIÓN AUTOMÁTICA:
     * - Crear nuevo string concatenado
     * - Liberar el string anterior (saved)
     * - Retornar el nuevo string o NULL en error
     */
    char *temp = ft_strjoin(saved, buffer);
    if (!temp)
    {
        free(saved);
        return NULL;
    }
    free(saved);
    return temp;
}

// Función para leer hasta encontrar newline o EOF
char *read_until_newline(int fd, char *saved)
{
    /*
     * LECTURA ACUMULATIVA:
     * - Leer chunks de BUFFER_SIZE
     * - Acumular en el buffer saved
     * - Parar cuando se encuentra '\n' o EOF
     * - Manejar errores de read()
     */
    char *buffer;
    int bytes_read;

    // Inicializar saved si es NULL
    if (!saved)
    {
        saved = malloc(1);
        if (!saved)
            return NULL;
        saved[0] = '\0';
    }

    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
    {
        free(saved);
        return NULL;
    }

    bytes_read = 1;
    while (bytes_read > 0)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        
        if (bytes_read == -1)
        {
            free(saved);
            free(buffer);
            return NULL;
        }
        
        buffer[bytes_read] = '\0';
        saved = join_and_free(saved, buffer);
        
        if (!saved)
        {
            free(buffer);
            return NULL;
        }
        
        // Parar si encontramos newline
        if (ft_strchr(buffer, '\n'))
            break;
    }
    
    free(buffer);
    return saved;
}

// Función para extraer una línea del buffer
char *extract_line(char *buffer)
{
    /*
     * EXTRACCIÓN DE LÍNEA:
     * - Encontrar la posición del '\n'
     * - Alocar memoria para la línea (incluyendo '\n')
     * - Copiar caracteres hasta '\n' inclusive
     * - Terminar con '\0'
     */
    if (!buffer || !buffer[0])
        return NULL;
    
    int i = 0;
    // Encontrar longitud hasta '\n' o final
    while (buffer[i] && buffer[i] != '\n')
        i++;
    
    char *line = malloc(i + 2); // +1 para '\n', +1 para '\0'
    if (!line)
        return NULL;
    
    int j = 0;
    // Copiar caracteres
    while (j <= i && buffer[j])
    {
        line[j] = buffer[j];
        j++;
    }
    line[j] = '\0';
    
    return line;
}

// Función para guardar el resto después de la línea extraída
char *save_remainder(char *buffer)
{
    /*
     * GUARDAR RESTO:
     * - Encontrar posición después del '\n'
     * - Si no hay más contenido, liberar y retornar NULL
     * - Si hay contenido, alocar y copiar el resto
     */
    if (!buffer)
        return NULL;
    
    int i = 0;
    // Encontrar posición del '\n'
    while (buffer[i] && buffer[i] != '\n')
        i++;
    
    if (buffer[i] == '\n')
        i++; // Saltar el '\n'
    
    // Si no hay más contenido después del '\n'
    if (!buffer[i])
    {
        free(buffer);
        return NULL;
    }
    
    // Alocar para el resto
    char *remainder = malloc(ft_strlen(buffer) - i + 1);
    if (!remainder)
    {
        free(buffer);
        return NULL;
    }
    
    int j = 0;
    while (buffer[i])
        remainder[j++] = buffer[i++];
    remainder[j] = '\0';
    
    free(buffer);
    return remainder;
}

// Función principal de get_next_line
char *get_next_line(int fd)
{
    /*
     * LÓGICA PRINCIPAL:
     * - Usar buffer estático para persistir entre llamadas
     * - Leer hasta encontrar newline completo
     * - Extraer una línea
     * - Guardar el resto para la siguiente llamada
     */
    static char *saved; // Buffer estático persistente
    char *line;

    // Validar file descriptor y BUFFER_SIZE
    if (fd < 0 || BUFFER_SIZE <= 0)
        return NULL;
    
    // Leer datos hasta tener una línea completa
    saved = read_until_newline(fd, saved);
    if (!saved)
        return NULL;
    
    // Extraer la línea actual
    line = extract_line(saved);
    if (!line)
    {
        free(saved);
        saved = NULL;
        return NULL;
    }
    
    // Guardar el resto para la siguiente llamada
    saved = save_remainder(saved);
    
    return line;
}

/*
 * VERSIÓN ALTERNATIVA MÁS SIMPLE (para casos especiales):
 * Esta versión podría tener algunos "bugs" intencionales
 */
char *get_next_line_simple(int fd)
{
    static char buffer[BUFFER_SIZE];
    static int buffer_read = 0;
    static int buffer_pos = 0;
    char *line;
    int i = 0;
    int line_size = 1000; // Tamaño fijo (posible "bug")

    if (fd < 0 || BUFFER_SIZE <= 0)
        return NULL;
    
    line = malloc(line_size);
    if (!line)
        return NULL;
    
    while (1)
    {
        // Si hemos consumido todo el buffer, leer más
        if (buffer_pos >= buffer_read)
        {
            buffer_read = read(fd, buffer, BUFFER_SIZE);
            buffer_pos = 0;
            
            if (buffer_read == 0) // EOF
                break;
            else if (buffer_read < 0) // Error
            {
                free(line);
                return NULL;
            }
        }
        
        // Copiar carácter actual a la línea
        line[i++] = buffer[buffer_pos++];
        
        // Si encontramos newline, terminar
        if (line[i - 1] == '\n')
            break;
    }
    
    line[i] = '\0';
    
    // Si no leímos nada, retornar NULL
    if (i == 0)
    {
        free(line);
        return NULL;
    }
    
    return line;
}

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. BUFFER ESTÁTICO:
 *    - Mantiene datos entre llamadas a la función
 *    - Se reinicia automáticamente cuando el archivo termina
 *    - Debe manejarse cuidadosamente para evitar leaks
 * 
 * 2. GESTIÓN DE MEMORIA:
 *    - Cada línea retornada debe ser liberada por el caller
 *    - Liberar buffers internos en caso de error
 *    - Usar realloc si es necesario para líneas largas
 * 
 * 3. CASOS ESPECIALES:
 *    - Archivos sin newline final
 *    - Líneas vacías (solo '\n')
 *    - Errores de read()
 *    - BUFFER_SIZE de 1 (caso extremo)
 * 
 * 4. "BUGS" COMUNES (que podrían ser intencionales):
 *    - No manejar líneas más largas que un buffer fijo
 *    - No liberar memoria correctamente en errores
 *    - No manejar BUFFER_SIZE dinámico
 *    - Comportamiento incorrecto con archivos binarios
 */