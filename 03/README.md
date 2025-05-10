# 42 Exam Rank 03 - Guía de Práctica

Este directorio contiene los ejercicios y materiales para practicar el Exam Rank 03 de 42.

## Estructura del Examen

El Exam Rank 03 consta de dos posibles ejercicios, de los cuales se te asignará uno durante el examen:

1. **ft_printf**: Implementar una versión simplificada de la función printf.
2. **get_next_line**: Implementar una función que lea una línea de un archivo descriptor.

## Cómo usar este sistema de práctica

1. Navega a los directorios `ft_printf` o `get_next_line` para ver el subject y los recursos disponibles.
2. Crea tu solución siguiendo las instrucciones del subject.
3. Usa el script `exam.sh` para seleccionar un ejercicio, practicarlo y validarlo.

## Directorios importantes

- `03/ft_printf/`: Ejercicio de ft_printf con subject, guías y tests.
- `03/get_next_line/`: Ejercicio de get_next_line con subject, guías y tests.
- `03/rendu/`: **Este es el directorio donde debes crear tus soluciones**.

## Estructura de tus soluciones

Para que los test automáticos funcionen correctamente, debes colocar tus archivos de la siguiente manera:

### Para ft_printf:
```
03/rendu/ft_printf/ft_printf.c
```

### Para get_next_line:
```
03/rendu/get_next_line/get_next_line.c
03/rendu/get_next_line/get_next_line.h
```

## Guías y recursos adicionales

Cada ejercicio incluye:
- Archivos de guía paso a paso (nombrados como `*guide*` o `*steps*`)
- Ejemplos de implementaciones
- Tests automáticos para validar tu solución

Puedes acceder a estos recursos a través del menú principal del `exam.sh`.

## Nota importante sobre los test

Los tests verifican que tu implementación:
1. Compila correctamente sin errores ni warnings
2. Funciona correctamente con casos de prueba básicos y avanzados
3. No tiene fugas de memoria (si se usa la opción `--valgrind`)

Para get_next_line, los tests se ejecutan con diferentes valores de BUFFER_SIZE para verificar que tu solución funciona en todas las condiciones.

---

¡Buena suerte con tu práctica!
