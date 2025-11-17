/*
 * EJERCICIO: FT_SCANF
 * 
 * DESCRIPCIÓN:
 * Implementar una versión simplificada de scanf que maneje solo %s, %d y %c.
 * 
 * CONCEPTOS CLAVE:
 * 1. ARGUMENTOS VARIABLES: va_list, va_start, va_arg, va_end
 * 2. PARSING DE FORMATO: Analizar string de formato carácter por carácter
 * 3. LECTURA DE ARCHIVO: fgetc(), ungetc() para control de flujo
 * 4. CONVERSIONES: Convertir strings a números, manejar espacios en blanco
 * 
 * FORMATO SOPORTADO:
 * - %s: string (hasta el primer espacio en blanco)
 * - %d: entero decimal (con signo opcional)
 * - %c: un solo carácter
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

// Función para saltar espacios en blanco en el stream
int match_space(FILE *f)
{
    /*
     * MANEJO DE ESPACIOS EN BLANCO:
     * - Leer caracteres mientras sean espacios
     * - Devolver el primer carácter no-espacio al stream
     * - Retornar -1 en caso de error
     */
    int ch = fgetc(f);
    if (ch == EOF && ferror(f))
        return -1;
    
    while (ch != EOF)
    {
        if (!isspace(ch))
        {
            ungetc(ch, f); // Devolver carácter no-espacio
            break;
        }
        ch = fgetc(f);
    }
    
    if (ferror(f))
        return -1;
    return 1;
}

// Función para verificar un carácter específico
int match_char(FILE *f, char c)
{
    /*
     * COINCIDENCIA DE CARACTERES LITERALES:
     * - Leer un carácter del stream
     * - Verificar si coincide con el esperado
     * - Devolver al stream si no coincide
     */
    int ch = fgetc(f);
    if (ch == c)
        return 1;
    if (ch != EOF)
        ungetc(ch, f);
    return -1;
}

// Función para leer un carácter (%c)
int scan_char(FILE *f, va_list ap)
{
    /*
     * CONVERSIÓN %c:
     * - Leer exactamente un carácter
     * - No saltar espacios en blanco
     * - Almacenar en el puntero proporcionado
     */
    int ch = fgetc(f);
    char *cp = va_arg(ap, char *);

    if (ch == EOF)
        return -1;
    
    *cp = (char)ch;
    return 1;
}

// Función para leer un entero (%d)
int scan_int(FILE *f, va_list ap)
{
    /*
     * CONVERSIÓN %d:
     * - Saltar espacios en blanco iniciales
     * - Leer signo opcional (+/-)
     * - Leer dígitos y construir el número
     * - Devolver último carácter no-dígito al stream
     */
    int sign = 1;
    int value = 0;
    int ch = fgetc(f);
    int *ip = va_arg(ap, int *);
    int count = 0;

    if (ch == EOF)
        return -1;
    
    // Saltar espacios en blanco
    while (isspace(ch))
        ch = fgetc(f);
    
    // Manejar signo
    if (ch == '-')
    {
        sign = -1;
        ch = fgetc(f);
    }
    else if (ch == '+')
    {
        ch = fgetc(f);
    }
    
    // Verificar que el primer carácter sea un dígito
    if (!isdigit(ch))
    {
        ungetc(ch, f);
        return -1;
    }
    
    // Leer dígitos y construir el número
    while (isdigit(ch))
    {
        value = value * 10 + (ch - '0');
        count++;
        ch = fgetc(f);
    }
    
    // Devolver último carácter no-dígito
    if (ch != EOF)
        ungetc(ch, f);

    if (count == 0)
        return -1;
    
    *ip = value * sign;
    return 1;
}

// Función para leer un string (%s)
int scan_string(FILE *f, va_list ap)
{
    /*
     * CONVERSIÓN %s:
     * - Saltar espacios en blanco iniciales
     * - Leer caracteres hasta encontrar espacio en blanco
     * - Terminar el string con '\0'
     * - Devolver último carácter de espacio al stream
     */
    int ch = fgetc(f);
    char *sp = va_arg(ap, char *);
    int i = 0;

    // Saltar espacios en blanco iniciales
    while (ch != EOF && isspace(ch))
        ch = fgetc(f);
    
    if (ch == EOF)
        return -1;

    // Leer caracteres hasta espacio en blanco
    do
    {
        sp[i] = ch;
        i++;
        ch = fgetc(f);
    } while (ch != EOF && !isspace(ch));
    
    sp[i] = '\0'; // Terminar string

    // Devolver carácter de espacio al stream
    if (ch != EOF)
        ungetc(ch, f);
    
    if (i == 0)
        return -1;
    return 1;
}

// Función para manejar conversiones de formato
int match_conv(FILE *f, const char **format, va_list ap)
{
    /*
     * DISPATCHER DE CONVERSIONES:
     * - Analizar el carácter de conversión
     * - Llamar a la función apropiada
     * - Manejar espacios para %d y %s automáticamente
     */
    switch (**format)
    {
        case 'c':
            return scan_char(f, ap);
        case 'd':
            match_space(f); // %d salta espacios automáticamente
            return scan_int(f, ap);
        case 's':
            match_space(f); // %s salta espacios automáticamente
            return scan_string(f, ap);
        case '\0':
            return -1;
        default:
            return -1; // Conversión no soportada
    }
}

// Función principal de scanf
int ft_vfscanf(FILE *f, const char *format, va_list ap)
{
    /*
     * LÓGICA PRINCIPAL DE SCANF:
     * - Analizar string de formato carácter por carácter
     * - Manejar caracteres literales y conversiones (%)
     * - Contar conversiones exitosas
     * - Parar en el primer error
     */
    int nconv = 0; // Número de conversiones exitosas

    // Verificar que hay datos disponibles
    int c = fgetc(f);
    if (c == EOF)
        return EOF;
    ungetc(c, f);

    while (*format)
    {
        if (*format == '%')
        {
            // Conversión encontrada
            format++;
            if (match_conv(f, &format, ap) != 1)
                break; // Error en conversión
            else
                nconv++; // Conversión exitosa
        }
        else if (isspace(*format))
        {
            // Espacio en blanco en formato: saltar espacios en input
            if (match_space(f) == -1)
                break;
        }
        else
        {
            // Carácter literal: debe coincidir exactamente
            if (match_char(f, *format) != 1)
                break;
        }
        format++;
    }
    
    // Verificar errores de archivo
    if (ferror(f))
        return EOF;
    
    return nconv; // Retornar número de conversiones exitosas
}

// Función wrapper para scanf estándar
int ft_scanf(const char *format, ...)
{
    /*
     * WRAPPER PARA ARGUMENTOS VARIABLES:
     * - Inicializar va_list
     * - Llamar a la función principal con stdin
     * - Limpiar va_list
     */
    va_list ap;

    va_start(ap, format);
    int ret = ft_vfscanf(stdin, format, ap);
    va_end(ap);
    
    return ret;
}


/*
  
  int main(void)
  {
      int x;
      char str[100];
      char c;
  
      // Leer: número, espacio, string, espacio, carácter
      int converted = ft_scanf("%d %s %c", &x, str, &c);
    
     printf("Convertidos: %d\n", converted);
     printf("Número: %d, String: %s, Carácter: %c\n", x, str, c);
    
    return 0;
 }
 */

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. ARGUMENTOS VARIABLES:
 *    - va_start(ap, last_param) para inicializar
 *    - va_arg(ap, type) para obtener siguiente argumento
 *    - va_end(ap) para limpiar
 * 
 * 2. CONTROL DE FLUJO DE ARCHIVO:
 *    - fgetc() para leer carácter
 *    - ungetc() para devolver carácter al stream
 *    - ferror() para verificar errores
 * 
 * 3. MANEJO DE ESPACIOS:
 *    - %c NO salta espacios en blanco
 *    - %d y %s SÍ saltan espacios en blanco
 *    - Espacios en formato coinciden con cualquier whitespace
 * 
 * 4. VALOR DE RETORNO:
 *    - Número de conversiones exitosas
 *    - EOF si error de archivo o EOF antes de conversiones
 *    - Parar en primera conversión fallida
 */