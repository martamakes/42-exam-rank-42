# 🔤 Patrón 01: Manipulación Básica de Caracteres

## 📖 Descripción del Patrón

Este patrón cubre las operaciones fundamentales con caracteres individuales y transformaciones simples de cadenas. Es la base sobre la cual se construyen conceptos más avanzados.

## 🎯 Conceptos Clave

### 1. Verificación de Caracteres
```c
// Verificar si un carácter es letra
int is_alpha(char c) { 
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); 
}

// Verificar si es dígito
int is_digit(char c) { 
    return (c >= '0' && c <= '9'); 
}
```

### 2. Conversión de Casos
```c
// Convertir a mayúscula
char to_upper(char c) { 
    return (c >= 'a' && c <= 'z') ? c - 32 : c; 
}

// Convertir a minúscula
char to_lower(char c) { 
    return (c >= 'A' && c <= 'Z') ? c + 32 : c; 
}
```

### 3. Operaciones con Bits (Nivel Avanzado)
```c
// Invertir todos los bits
unsigned char invert_bits(unsigned char c) {
    return ~c;
}

// Obtener bit en posición i (0-7)
int get_bit(unsigned char c, int i) {
    return (c >> i) & 1;
}

// Establecer bit en posición i
unsigned char set_bit(unsigned char c, int i) {
    return c | (1 << i);
}
```

## 🧠 Patrones de Pensamiento

### ROT13 y Rotaciones
- **Concepto**: Desplazar letras en el alfabeto
- **Clave**: Manejar el "wrap around" (volver al inicio)
- **Fórmula**: `nueva_pos = (pos_actual + desplazamiento) % 26`

### Manipulación de Bits
- **Concepto**: Trabajar con representación binaria
- **Operadores esenciales**: `&` (AND), `|` (OR), `^` (XOR), `~` (NOT), `<<` (shift left), `>>` (shift right)
- **Potencias de 2**: Un número es potencia de 2 si `(n & (n-1)) == 0 && n > 0`

## 🚀 Estrategia de Resolución

1. **Analiza el problema**: ¿Qué transformación necesitas?
2. **Identifica el patrón**: ¿Es rotación, inversión, o manipulación bit a bit?
3. **Piensa en casos especiales**: ¿Qué pasa con espacios, números, símbolos?
4. **Implementa paso a paso**: Una transformación a la vez
5. **Prueba con ejemplos**: Casos normales y extremos

## 💡 Consejos para el Examen

- **Memoriza las conversiones ASCII**: 'A' = 65, 'a' = 97, diferencia = 32
- **Domina las operaciones bit a bit**: Muy comunes en exámenes
- **Practica el patrón ROT**: Aparece frecuentemente
- **No olvides el carácter nulo**: Siempre termina strings con '\0'

## 📝 Ejercicios en este Patrón

- **ft_putstr**: Función básica para escribir strings
- **ft_swap**: Intercambiar valores de variables
- **rot_13**: Cifrado César con desplazamiento 13
- **rotone**: Desplazamiento de 1 en el alfabeto
- **ulstr**: Invertir mayúsculas/minúsculas
- **alpha_mirror**: Espejo alfabético (a↔z, b↔y, etc.)
- **print_bits**: Mostrar representación binaria
- **reverse_bits**: Invertir orden de bits
- **swap_bits**: Intercambiar mitades de un byte
- **is_power_of_2**: Verificar potencia de 2

¡Domina estos conceptos y tendrás una base sólida para el resto de patrones! 🎯
