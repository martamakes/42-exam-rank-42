/*
 * EJERCICIO: FILTER
 * 
 * DESCRIPCIÓN: 
 * Leer de stdin y escribir a stdout, pero reemplazando todas las ocurrencias
 * de una cadena dada por asteriscos (*) de la misma longitud.
 * 
 * CONCEPTOS CLAVE:
 * 1. LECTURA DINÁMICA: read() con buffer variable
 * 2. BÚSQUEDA DE PATRONES: strstr() o memmem()
 * 3. GESTIÓN DE MEMORIA: realloc() para buffer dinámico
 * 4. MANEJO DE ERRORES: perror() para errores de sistema
 * 
 * ALGORITMO:
 * 1. Leer todo el contenido de stdin en un buffer dinámico
 * 2. Buscar todas las ocurrencias del patrón
 * 3. Reemplazar cada ocurrencia con asteriscos
 * 4. Escribir el resultado a stdout
 */

#define _GNU_SOURCE  // Para memmem()
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Función para encontrar y reemplazar todas las ocurrencias
void ft_filter(char *buffer, const char *target)
{
    int i = 0;
    int target_len = strlen(target);
    int j, k;

    /*
     * ALGORITMO DE BÚSQUEDA Y REEMPLAZO:
     * - Recorrer el buffer carácter por carácter
     * - En cada posición, verificar si coincide con el patrón
     * - Si coincide, escribir asteriscos y saltar la longitud del patrón
     * - Si no coincide, escribir el carácter original
     */
    
    while (buffer[i])
    {
        j = 0;
        // Verificar si hay coincidencia desde la posición actual
        while (target[j] && (buffer[i + j] == target[j]))
            j++;
        
        if (j == target_len) // Coincidencia completa encontrada
        {
            // Escribir asteriscos en lugar del patrón
            k = 0;
            while (k < target_len)
            {
                write(1, "*", 1);
                k++;
            }
            i += target_len; // Saltar el patrón completo
        }
        else
        {
            // No hay coincidencia, escribir carácter original
            write(1, &buffer[i], 1);
            i++;
        }
    }
}

int main(int argc, char **argv)
{
    /*
     * VALIDACIÓN DE ARGUMENTOS:
     * - Debe haber exactamente 1 argumento
     * - El argumento no puede estar vacío
     */
    if (argc != 2 || argv[1][0] == '\0')
        return 1;

    /*
     * LECTURA DINÁMICA DE STDIN:
     * - Usar buffer temporal para leer chunks
     * - Usar realloc() para expandir el buffer principal
     * - Mantener seguimiento del total leído
     */
    char temp[BUFFER_SIZE];
    char *result = NULL;
    char *buffer;
    int total_read = 0;
    ssize_t bytes;

    // Leer de stdin hasta EOF
    while ((bytes = read(0, temp, BUFFER_SIZE)) > 0)
    {
        // Expandir el buffer principal para acomodar los nuevos datos
        buffer = realloc(result, total_read + bytes + 1);
        if (!buffer)
        {
            free(result);
            perror("realloc");
            return 1;
        }
        
        result = buffer;
        
        // Copiar los nuevos datos al buffer principal
        memmove(result + total_read, temp, bytes);
        total_read += bytes;
        result[total_read] = '\0'; // Asegurar terminación
    }

    // Verificar errores de lectura
    if (bytes < 0)
    {
        perror("read");
        free(result);
        return 1;
    }

    // Si no se leyó nada, salir sin error
    if (!result)
        return 0;

    // Procesar el buffer y aplicar el filtro
    ft_filter(result, argv[1]);
    
    // Liberar memoria
    free(result);
    return 0;
}

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. GESTIÓN DE MEMORIA:
 *    - Siempre verificar el retorno de malloc/realloc
 *    - Liberar memoria en caso de error
 *    - Usar memmove() en lugar de memcpy() para solapamientos
 * 
 * 2. MANEJO DE ERRORES:
 *    - Usar perror() para errores de sistema
 *    - Retornar códigos de error apropiados
 *    - Validar argumentos antes de usarlos
 * 
 * 3. LECTURA DINÁMICA:
 *    - El buffer puede llenarse con cualquier cantidad de datos
 *    - Usar realloc() para expandir según sea necesario
 *    - Mantener un terminador nulo válido
 * 
 * 4. ALGORITMO EFICIENTE:
 *    - Búsqueda simple carácter por carácter
 *    - Evitar usar funciones no permitidas
 *    - Escribir directamente a stdout sin almacenar el resultado
 */