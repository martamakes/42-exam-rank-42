# Guía para usar el entorno de práctica para el Exam Rank 02/03

## Introducción

Este entorno te ayudará a practicar los ejercicios del examen Rank 02/03 de 42, que incluye:
- ft_printf
- get_next_line

El entorno ofrece diferentes modos de práctica, incluyendo:
- Modo guiado clásico: Instrucciones paso a paso básicas
- Modo guiado mejorado: Sigue el proceso de pensamiento natural de un programador
- Modo de práctica: Verifica tu solución y comprueba si pasa los tests
- Modo simulación: Simula un examen real con selección aleatoria de ejercicios

## Configuración inicial

Para configurar el entorno, ejecuta:

```
bash chmod_setup.sh
bash setup_permissions.sh
```

## Cómo usar el entorno

Ejecuta el script principal:

```
./exam.sh
```

En el menú principal puedes elegir entre varias opciones:

1. **Modo guiado**: Recomendado para principiantes. Te mostrará una guía paso a paso para implementar las funciones, siguiendo un flujo natural de pensamiento.

2. **Practicar ft_printf**: Te permite validar tu implementación de ft_printf y ver ejemplos.

3. **Practicar get_next_line**: Te permite validar tu implementación de get_next_line y ver ejemplos.

4. **Simular examen**: Te asigna un ejercicio aleatorio y mide el tiempo que tardas en completarlo.

5. **Ver estructura de directorios**: Muestra la estructura de carpetas del entorno.

6. **Limpiar directorio rendu**: Elimina tus implementaciones actuales para empezar de nuevo.

## Funcionalidades adicionales

En el modo guiado puedes elegir entre:
- Modo mejorado: Sigue un proceso de pensamiento natural, explicando el razonamiento detrás de cada paso
- Modo clásico: Instrucciones directas y concisas

## Tips para el examen

1. **ft_printf**:
   - Maneja los 3 especificadores requeridos (%s, %d, %x)
   - Asegúrate de que tu función devuelva el número correcto de caracteres impresos
   - Maneja correctamente strings NULL y números negativos

2. **get_next_line**:
   - Recuerda incluir el \n en la línea devuelta, excepto al final del archivo
   - Usa la variable estática para mantener el contenido restante entre llamadas
   - El BUFFER_SIZE puede variar, tu función debe funcionar con cualquier valor

## Buena suerte en tu examen!
