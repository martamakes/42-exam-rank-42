/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ejemplo_explicacion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42madrid.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by marta             #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** EXPLICACIÓN DEL EJERCICIO: FILTER
** 
** ENUNCIADO EN ESPAÑOL:
** Escribir un programa que tome exactamente un argumento.
** El programa debe leer de stdin y escribir todo el contenido leído en stdout,
** excepto que cada ocurrencia de la cadena 's' debe ser reemplazada por '*'
** (tantos asteriscos como la longitud de s).
** 
** CONCEPTOS CLAVE A APLICAR:
** 
** 1. GESTIÓN DE ARGUMENTOS:
**    - Verificar que argc == 2 (programa + 1 argumento)
**    - Validar que el argumento no esté vacío
** 
** 2. LECTURA DE STDIN:
**    - Usar read() para leer datos desde stdin
**    - Manejar buffers de tamaño variable
**    - Gestionar lecturas parciales y EOF
** 
** 3. BÚSQUEDA Y REEMPLAZO DE PATRONES:
**    - Usar memmem() para encontrar ocurrencias del patrón
**    - Algoritmo de búsqueda en streams de datos
**    - Manejar patrones que pueden estar divididos entre buffers
** 
** 4. GESTIÓN DE MEMORIA:
**    - Uso de malloc/realloc para buffers dinámicos
**    - Liberación adecuada de memoria
**    - Manejo de errores de asignación
** 
** 5. GESTIÓN DE ERRORES:
**    - Usar perror() para mostrar errores del sistema
**    - Validación de parámetros de entrada
**    - Códigos de retorno apropiados
*/

#define _GNU_SOURCE  // Necesario para memmem
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

/*
** ft_strlen: Implementación propia de strlen
** Calculamos la longitud sin usar librerías externas
*/
int ft_strlen(const char *str)
{
    int len = 0;
    if (!str)
        return (0);
    while (str[len])
        len++;
    return (len);
}

/*
** write_pattern: Escribe el patrón de reemplazo (asteriscos)
** Escribe tantos '*' como la longitud del patrón original
*/
int write_pattern(int pattern_len)
{
    int i = 0;
    while (i < pattern_len)
    {
        if (write(STDOUT_FILENO, "*", 1) != 1)
        {
            perror("Error");
            return (-1);
        }
        i++;
    }
    return (0);
}

/*
** process_buffer: Procesa un buffer buscando y reemplazando patrones
** 
** ALGORITMO DE BÚSQUEDA Y REEMPLAZO:
** 1. Usar memmem() para encontrar la primera ocurrencia del patrón
** 2. Escribir todo el contenido antes del patrón
** 3. Escribir el reemplazo (asteriscos)
** 4. Continuar desde después del patrón encontrado
** 5. Repetir hasta no encontrar más patrones
*/
int process_buffer(char *buffer, int buffer_len, char *pattern, int pattern_len)
{
    char *current = buffer;
    char *found;
    int remaining = buffer_len;

    while (remaining > 0)
    {
        // memmem(): busca 'pattern' en los primeros 'remaining' bytes de 'current'
        // Retorna puntero al primer match o NULL si no encuentra
        found = memmem(current, remaining, pattern, pattern_len);
        
        if (!found)
        {
            // No hay más patrones, escribir el resto del buffer
            if (write(STDOUT_FILENO, current, remaining) != remaining)
            {
                perror("Error");
                return (-1);
            }
            break;
        }
        
        // Escribir contenido antes del patrón encontrado
        int bytes_before = found - current;
        if (bytes_before > 0)
        {
            if (write(STDOUT_FILENO, current, bytes_before) != bytes_before)
            {
                perror("Error");
                return (-1);
            }
        }
        
        // Escribir el reemplazo (asteriscos)
        if (write_pattern(pattern_len) == -1)
            return (-1);
        
        // Avanzar después del patrón encontrado
        current = found + pattern_len;
        remaining = buffer_len - (current - buffer);
    }
    
    return (0);
}

/*
** read_and_process: Lee desde stdin y procesa los datos
** 
** GESTIÓN DE BUFFERS:
** - Lee en chunks de BUFFER_SIZE bytes
** - Procesa cada chunk buscando patrones
** - Maneja el caso donde un patrón puede estar dividido entre buffers
*/
int read_and_process(char *pattern, int pattern_len)
{
    char buffer[BUFFER_SIZE];
    int bytes_read;
    
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        if (process_buffer(buffer, bytes_read, pattern, pattern_len) == -1)
            return (-1);
    }
    
    // Verificar errores de lectura
    if (bytes_read == -1)
    {
        perror("Error");
        return (-1);
    }
    
    return (0);
}

/*
** validate_arguments: Valida los argumentos del programa
** 
** VALIDACIONES REQUERIDAS:
** - Exactamente un argumento (argc == 2)
** - El argumento no debe estar vacío
*/
int validate_arguments(int argc, char **argv)
{
    if (argc != 2)
        return (-1);
    
    if (!argv[1] || argv[1][0] == '\0')
        return (-1);
    
    return (0);
}

/*
** main: Función principal
** 
** FLUJO DEL PROGRAMA:
** 1. Validar argumentos
** 2. Obtener el patrón a buscar
** 3. Leer desde stdin y procesar
** 4. Manejar errores apropiadamente
*/
int main(int argc, char **argv)
{
    char *pattern;
    int pattern_len;
    
    // Paso 1: Validar argumentos
    if (validate_arguments(argc, argv) == -1)
        return (1);
    
    // Paso 2: Obtener patrón y su longitud
    pattern = argv[1];
    pattern_len = ft_strlen(pattern);
    
    // Paso 3: Procesar entrada
    if (read_and_process(pattern, pattern_len) == -1)
        return (1);
    
    return (0);
}

/*
** PUNTOS CLAVE SOBRE MEMMEM():
** 
** memmem() es una extensión GNU que busca una subsecuencia de bytes
** en un bloque de memoria. Es más eficiente que buscar carácter por carácter.
** 
** Prototipo: void *memmem(const void *haystack, size_t haystacklen,
**                         const void *needle, size_t needlelen);
** 
** - haystack: bloque de memoria donde buscar
** - haystacklen: tamaño del bloque de memoria
** - needle: patrón que buscamos
** - needlelen: tamaño del patrón
** 
** Retorna: puntero al primer match, o NULL si no encuentra
** 
** VENTAJAS:
** - Maneja datos binarios (no solo strings)
** - Más eficiente que algoritmos manuales
** - Está optimizada en la mayoría de sistemas
** 
** CONSIDERACIONES:
** - Requiere #define _GNU_SOURCE
** - No está disponible en todos los sistemas UNIX
** - Para máxima portabilidad, se podría implementar manualmente
*/

/*
** GESTIÓN DE ERRORES EN ESTE EJERCICIO:
** 
** 1. ERRORES DE ARGUMENTOS:
**    - Número incorrecto de argumentos → return 1
**    - Argumento vacío → return 1
** 
** 2. ERRORES DE SISTEMA:
**    - Error en read() → perror("Error") + return 1
**    - Error en write() → perror("Error") + return 1
**    - Error en malloc() → perror("Error") + return 1
** 
** 3. USO DE PERROR():
**    - perror() imprime "Error: " seguido del mensaje del sistema
**    - Usa la variable global errno para determinar el error
**    - Siempre escribir a stderr, no stdout
*/

/*
** CASOS EDGE A CONSIDERAR:
** 
** 1. Patrón más largo que el buffer
** 2. Patrón dividido entre múltiples lecturas
** 3. Patrones que se solapan (ej: "aaa" en "aaaa")
** 4. Entrada vacía
** 5. Patrón no encontrado
** 6. Errores de memoria o I/O
** 
** OPTIMIZACIONES POSIBLES:
** 
** 1. Buffer sliding window para patrones divididos
** 2. Algoritmo KMP para búsquedas más eficientes
** 3. Reescritura en chunks para mejor rendimiento
*/
