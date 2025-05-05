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
