/* ************************************************************************** */
/*                          GET_NEXT_LINE.H - HEADER                         */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

/*
 * INCLUDES OBLIGATORIOS:
 * - unistd.h: para read()
 * - stdlib.h: para malloc() y free()
 */
# include <unistd.h>
# include <stdlib.h>

/*
 * BUFFER_SIZE:
 * - Se define en tiempo de compilación con -D BUFFER_SIZE=xx
 * - Si no se define, usar valor por defecto
 * - Debe ser mayor que 0 para funcionar correctamente
 */
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

/*
 * PROTOTIPO DE LA FUNCIÓN:
 * - Exactamente como especifica el subject
 * - Retorna char* (línea leída) o NULL (EOF/error)
 * - Parámetro int fd (file descriptor)
 */
char	*get_next_line(int fd);

#endif

/*
 * NOTAS IMPORTANTES DEL HEADER:
 * 
 * 1. OBLIGATORIO EN EXAMEN:
 *    - El subject especifica que debe incluir .h
 *    - Sin header = 0 puntos automáticamente
 * 
 * 2. PROTECCIÓN DOBLE INCLUSIÓN:
 *    - #ifndef/#define/#endif evita inclusiones múltiples
 *    - Usa nombre único: GET_NEXT_LINE_H
 * 
 * 3. BUFFER_SIZE:
 *    - Se compila con: gcc -D BUFFER_SIZE=1024 ...
 *    - Si no se define, usa valor por defecto
 *    - Valor típico para examen: 1, 42, 1024
 * 
 * 4. INCLUDES MÍNIMOS:
 *    - Solo las librerías estrictamente necesarias
 *    - unistd.h para read()
 *    - stdlib.h para malloc()/free()
 */