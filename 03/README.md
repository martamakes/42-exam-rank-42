# Modos Guiados para Exam Rank 03

Este directorio contiene modos guiados interactivos para ayudarte a implementar las funciones requeridas en el Exam Rank 03 de 42:
- `ft_printf`
- `get_next_line`

## ¿Qué son los modos guiados?

Estos modos guiados son programas interactivos que te guían paso a paso en la implementación de las funciones requeridas. Te explican los conceptos clave, te muestran ejemplos de código y te permiten probar tu implementación en tiempo real.

A diferencia de simplemente ver una solución completa, los modos guiados te ayudan a entender el proceso de desarrollo y a crear tu propia implementación. Esto es especialmente útil para prepararte para el examen, donde tendrás que escribir el código desde cero.

## Cómo usar los modos guiados

### Requisitos
- Compilador GCC
- Make

### 1. Estructura de archivos

Crearás tus soluciones en el directorio `rendu/` siguiendo esta estructura:

```
03/
├── rendu/
│   ├── ft_printf/
│   │   └── ft_printf.c         # Tu solución para ft_printf
│   └── get_next_line/
│       ├── get_next_line.c     # Tu solución para get_next_line
│       └── get_next_line.h     # Header para get_next_line
```

### 2. Comandos básicos

Para compilar ambos modos guiados:
```bash
make
```

Para iniciar el modo guiado de ft_printf:
```bash
make printf_guide
```

Para iniciar el modo guiado de get_next_line:
```bash
make gnl_guide
```

Para limpiar los archivos temporales:
```bash
make clean
```

Para limpiar completamente y recompilar:
```bash
make re
```

### 3. Flujo de trabajo

1. Compila los modos guiados con `make`
2. Inicia el modo guiado que quieras seguir (`make printf_guide` o `make gnl_guide`)
3. Sigue las instrucciones paso a paso y crea/modifica los archivos en el directorio `rendu/`
4. El sistema comprobará automáticamente tu código en cada paso

## Estructura de ft_printf

El modo guiado de ft_printf te ayuda a implementar una versión simplificada de la función printf que maneje solo las conversiones %s, %d y %x. El proceso se divide en 6 pasos:

1. **Entendiendo ft_printf**: Introducción al enunciado y los conceptos clave.
2. **Estructura Básica**: Creación de una estructura básica para ft_printf.
3. **Manejo de Especificadores de Formato**: Implementación de la detección de especificadores.
4. **Manejo de Strings (%s)**: Implementación de la impresión de strings.
5. **Manejo de Enteros (%d y %x)**: Implementación de la impresión de enteros en decimal y hexadecimal.
6. **Prueba Final**: Pruebas completas de tu implementación.

## Estructura de get_next_line

El modo guiado de get_next_line te ayuda a implementar una función que lea una línea de un descriptor de archivo. El proceso se divide en 6 pasos:

1. **Entendiendo get_next_line**: Introducción al enunciado y los conceptos clave.
2. **Creando el Header**: Creación del archivo de cabecera con las definiciones necesarias.
3. **Implementación Básica**: Implementación simple que lee carácter por carácter.
4. **Probando get_next_line**: Pruebas básicas de la implementación.
5. **Optimizando get_next_line**: Optimización para usar BUFFER_SIZE de manera eficiente.
6. **Prueba Final**: Pruebas completas con diferentes BUFFER_SIZE y casos especiales.

## Consejos para el examen

### Para ft_printf:
- Asegúrate de entender bien cómo funcionan las funciones variádicas (va_list, va_start, va_arg, va_end).
- Presta atención al valor de retorno, que debe ser el número de caracteres impresos.
- Maneja correctamente los casos especiales como NULL para %s.

### Para get_next_line:
- Asegúrate de que tu implementación funcione con cualquier BUFFER_SIZE positivo.
- No olvides manejar EOF, errores y líneas sin salto de línea al final.
- Cuida las fugas de memoria: asegúrate de hacer free() cuando sea necesario.
- Para el examen, incluso una implementación más simple (carácter por carácter) puede ser suficiente si es correcta.

## Notas importantes

- Los modos guiados están diseñados para ayudarte a entender y a implementar las funciones por ti mismo, no para darte una solución lista para copiar.
- En el examen, tendrás que escribir el código desde cero, así que asegúrate de entender cada paso y concepto.
- Practica varias veces implementando las funciones por tu cuenta, sin mirar los ejemplos de código.

¡Buena suerte en tu examen!
