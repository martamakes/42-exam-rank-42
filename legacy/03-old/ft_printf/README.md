# ft_printf

## Enunciado

Escribe una función llamada `ft_printf` que imite el comportamiento real de printf pero solo gestione las siguientes conversiones:

- `%s` (string)
- `%d` (decimal)
- `%x` (hexadecimal en minúsculas)

## Prototipo

```c
int ft_printf(const char *format, ...);
```

## Restricciones

- Tu función debe manejar el formato de la misma manera que printf
- No es necesario reproducir el buffer de printf
- Debes usar la librería `<stdarg.h>`

## Retorno

La función debe devolver el número de caracteres impresos (excluyendo el '\0' final).

## Ejemplos

```c
ft_printf("Hello %s\n", "world"); // Imprime: Hello world
ft_printf("%d\n", 42);          // Imprime: 42
ft_printf("%x\n", 42);          // Imprime: 2a
```

## Consejos

- Familiarízate con la biblioteca `<stdarg.h>` y sus funciones (va_list, va_start, va_arg, va_end)
- Separa la lógica para cada tipo de conversión
- Utiliza la función write para imprimir caracteres
- Recuerda contar correctamente los caracteres impresos para el valor de retorno