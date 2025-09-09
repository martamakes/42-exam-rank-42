## Guía para implementar ft_printf

Esta guía te ayudará a entender cómo implementar `ft_printf` desde cero.

### 1. Entendiendo el prototipo

```c
int ft_printf(const char *format, ...);
```

- `const char *format`: String de formato que puede contener texto normal y especificadores de formato (%d, %s, %x)
- `...`: Argumentos variables que corresponden a los especificadores

### 2. Uso de la biblioteca stdarg.h

Esta biblioteca proporciona mecanismos para trabajar con funciones que aceptan un número variable de argumentos:

```c
#include <stdarg.h>

int ft_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);  // Inicializa la lista de argumentos
    
    // Procesar formato y argumentos...
    
    va_end(args);  // Limpia la lista de argumentos
    return (caracteres_impresos);
}
```

### 3. Procesamiento básico

Para implementar ft_printf, necesitas:

1. Recorrer la string de formato carácter a carácter
2. Si encuentras un carácter normal (no '%'), imprímelo
3. Si encuentras '%', examina el siguiente carácter y procesa según corresponda

### 4. Implementación paso a paso

```c
int ft_printf(const char *format, ...)
{
    va_list args;
    int count = 0;  // Contador de caracteres impresos
    
    va_start(args, format);
    
    while (*format)
    {
        if (*format == '%' && *(format + 1))
        {
            format++;  // Avanza al carácter después de '%'
            
            if (*format == 's')
                count += print_string(va_arg(args, char *));
            else if (*format == 'd')
                count += print_decimal(va_arg(args, int));
            else if (*format == 'x')
                count += print_hex(va_arg(args, unsigned int));
            else if (*format == '%')
                count += write(1, "%", 1);
        }
        else
            count += write(1, format, 1);
            
        format++;  // Avanza al siguiente carácter
    }
    
    va_end(args);
    return (count);
}
```

### 5. Funciones auxiliares

#### Impresión de string:
```c
int print_string(char *str)
{
    int count = 0;
    
    if (!str)
        str = "(null)";
        
    while (*str)
    {
        count += write(1, str, 1);
        str++;
    }
    
    return (count);
}
```

#### Impresión de decimal:
```c
int print_decimal(int n)
{
    int count = 0;
    long num = n;  // Usar long para manejar INT_MIN
    
    if (num < 0)
    {
        count += write(1, "-", 1);
        num = -num;
    }
    
    if (num >= 10)
        count += print_decimal(num / 10);
        
    char digit = '0' + (num % 10);
    count += write(1, &digit, 1);
    
    return (count);
}
```

#### Impresión de hexadecimal:
```c
int print_hex(unsigned int n)
{
    int count = 0;
    char *hex = "0123456789abcdef";
    
    if (n >= 16)
        count += print_hex(n / 16);
        
    count += write(1, &hex[n % 16], 1);
    
    return (count);
}
```

### 6. Considera problemas de borde

- String NULL: verificar y manejar correctamente
- Números grandes y negativos: asegúrate de manejar INT_MIN correctamente
- Casos recursivos: evita desbordamiento de pila con números muy grandes

### 7. Optimizaciones

- Utiliza funciones auxiliares para mantener el código limpio
- Ten cuidado con el recuento preciso de caracteres
- Maneja los errores de write (aunque en el examen esto no es crítico)

Recuerda que la clave está en entender bien cómo funcionan los argumentos variables y cómo procesar el formato adecuadamente.