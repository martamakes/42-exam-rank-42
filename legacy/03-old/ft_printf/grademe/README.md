SUBJECT 1: ft_printf
Assignment name  : ft_printf
Expected files   : ft_printf.c
Allowed functions: malloc, free, write, va_start, va_arg, va_copy, va_end
--------------------------------------------------------------------------------

Write a function named ft_printf that will mimic the real printf but it will manage only the following conversions:

%s (string)
%d (decimal) 
%x (lowercase hexadecimal)

Your function must be declared as follows:

int ft_printf(const char *, ... );

Examples of the function output:

call: ft_printf("%s\n", "toto");
out: toto$

call: ft_printf("Magic %s is %d", "number", 42);
out: Magic number is 42%

call: ft_printf("Hexadecimal for %d is %x\n", 42, 42);
out: Hexadecimal for 42 is 2a$





# Tester para ft_printf

Este tester comprueba la implementación de `ft_printf` para el Exam Rank 03.

## Requisitos del ejercicio

- Tu función debe implementar correctamente las conversiones: `%s`, `%d` y `%x`
- Debe devolver el número correcto de caracteres impresos
- Debe manejar casos básicos y extremos

## Cómo usar este tester

1. Asegúrate de que tu archivo `ft_printf.c` está en el directorio adecuado:
   ```
   ../../../rendu/ft_printf/ft_printf.c
   ```

2. Ejecuta el script de test:
   ```
   chmod +x test.sh  # Solo la primera vez para hacerlo ejecutable
   ./test.sh
   ```

3. Para comprobar fugas de memoria (requiere Valgrind):
   ```
   ./test.sh --valgrind
   ```

4. Para ver todas las opciones disponibles:
   ```
   ./test.sh --help
   ```

## Casos testeados

1. **Casos básicos del subject**:
   - String simple: `%s\n` con "toto"
   - String con entero: `Magic %s is %d` con "number", 42
   - Entero y hex: `Hexadecimal for %d is %x\n` con 42, 42

2. **Casos adicionales**:
   - String vacía
   - String NULL
   - Enteros positivos y negativos grandes
   - Hexadecimales (0 y grandes)
   - Conversiones múltiples en una sola llamada

## Interpretación de resultados

- ✅ Verde [OK]: El test ha pasado correctamente
- ❌ Rojo [KO]: El test ha fallado
- Se muestran los valores esperados vs. obtenidos para ayudar en la depuración
