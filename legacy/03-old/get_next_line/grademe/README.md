# Tester para get_next_line

Este tester comprueba la implementación de `get_next_line` para el Exam Rank 03.

## Requisitos del ejercicio

- Tu función debe leer una línea (terminada en `\n` o EOF) desde un descriptor de archivo
- Debe incluir el `\n` si está presente al final de la línea
- Debe manejar múltiples llamadas para leer archivos línea por línea
- No debe tener fugas de memoria
- Debe funcionar con diferentes valores de BUFFER_SIZE

## Cómo usar este tester

1. Asegúrate de que tus archivos están en el directorio adecuado:
   ```
   ../../../rendu/get_next_line/get_next_line.c
   ../../../rendu/get_next_line/get_next_line.h
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

4. Para probar solo con un BUFFER_SIZE específico:
   ```
   ./test.sh --buffer=42
   ```

5. Para ver todas las opciones disponibles:
   ```
   ./test.sh --help
   ```

## Casos testeados

1. **Diferentes tipos de archivos**:
   - Archivo normal con múltiples líneas
   - Archivo vacío
   - Archivo sin salto de línea al final
   - Archivo con líneas largas
   - Archivo con una única línea

2. **Pruebas adicionales**:
   - Lectura desde pipe
   - Descriptor de archivo inválido
   - Múltiples descriptores de archivo simultáneos

3. **Diferentes BUFFER_SIZE**:
   - BUFFER_SIZE=1 (lectura byte a byte)
   - BUFFER_SIZE=5 (tamaño pequeño)
   - BUFFER_SIZE=42 (tamaño medio)
   - BUFFER_SIZE=9999 (tamaño grande)

## Interpretación de resultados

- ✅ Verde [OK]: El test ha pasado correctamente
- ❌ Rojo [KO]: El test ha fallado
- Se muestra un análisis detallado en caso de errores
- El resumen final muestra el resultado para cada BUFFER_SIZE probado
