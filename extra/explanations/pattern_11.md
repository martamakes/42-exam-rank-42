# 🔢 Patrón 11: Operaciones Bitwise (Manipulación a Nivel de Bits)

## 📖 Descripción del Patrón

Este patrón se enfoca en la manipulación directa de bits, una habilidad fundamental en programación de bajo nivel. Las operaciones bitwise son extremadamente eficientes y aparecen frecuentemente en exámenes de 42 School.

## 🎯 Conceptos Clave

### 1. Operadores Bitwise Fundamentales
```c
// AND (&): Solo 1 si ambos bits son 1
// 1010 & 1100 = 1000
int and_operation = a & b;

// OR (|): 1 si al menos uno de los bits es 1  
// 1010 | 1100 = 1110
int or_operation = a | b;

// XOR (^): 1 si los bits son diferentes
// 1010 ^ 1100 = 0110
int xor_operation = a ^ b;

// NOT (~): Invierte todos los bits
// ~1010 = 0101 (en complemento a 2)
int not_operation = ~a;

// Shift Left (<<): Desplaza bits hacia la izquierda
// 1010 << 2 = 101000 (multiplica por 2^n)
int left_shift = a << 2;

// Shift Right (>>): Desplaza bits hacia la derecha  
// 1010 >> 1 = 0101 (divide por 2^n)
int right_shift = a >> 1;
```

### 2. Técnicas Esenciales con Bits

#### Verificar si un Bit está Establecido
```c
// Verificar si el bit en posición 'n' está en 1
int is_bit_set(int number, int position) {
    return (number >> position) & 1;
}

// Ejemplo: verificar si el bit 3 está establecido en 12 (1100)
// is_bit_set(12, 3) = (12 >> 3) & 1 = 1 & 1 = 1 ✓
```

#### Establecer un Bit
```c
// Poner en 1 el bit en posición 'n'
int set_bit(int number, int position) {
    return number | (1 << position);
}

// Ejemplo: establecer el bit 1 en 8 (1000)
// set_bit(8, 1) = 8 | (1 << 1) = 1000 | 0010 = 1010 = 10
```

#### Limpiar un Bit
```c
// Poner en 0 el bit en posición 'n'
int clear_bit(int number, int position) {
    return number & ~(1 << position);
}

// Ejemplo: limpiar el bit 2 en 12 (1100)
// clear_bit(12, 2) = 12 & ~(1 << 2) = 1100 & 1011 = 1000 = 8
```

#### Alternar un Bit
```c
// Cambiar el estado del bit en posición 'n'
int toggle_bit(int number, int position) {
    return number ^ (1 << position);
}
```

### 3. Patrones Avanzados

#### Verificar si es Potencia de 2
```c
// Un número es potencia de 2 si tiene exactamente un bit en 1
int is_power_of_2(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// Ejemplo: 8 (1000) & 7 (0111) = 0000 ✓
//          6 (0110) & 5 (0101) = 0100 ≠ 0 ✗
```

#### Contar Bits Establecidos (Population Count)
```c
// Contar cuántos bits están en 1
int count_bits(int n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

// Versión optimizada (Brian Kernighan's algorithm)
int count_bits_fast(int n) {
    int count = 0;
    while (n) {
        n &= n - 1;  // Elimina el bit menos significativo
        count++;
    }
    return count;
}
```

#### Invertir Bits
```c
// Invertir el orden de los bits en un byte
unsigned char reverse_bits(unsigned char b) {
    unsigned char result = 0;
    int i;
    
    for (i = 0; i < 8; i++) {
        if (b & (1 << i))
            result |= (1 << (7 - i));
    }
    return result;
}
```

#### Intercambiar Mitades de un Byte
```c
// Intercambiar los 4 bits superiores con los 4 inferiores
unsigned char swap_bits(unsigned char b) {
    return ((b & 0x0F) << 4) | ((b & 0xF0) >> 4);
}

// Ejemplo: 11010011 -> 00111101
//          Upper: 1101, Lower: 0011
//          Result: 0011 1101
```

### 4. Representación Binaria
```c
// Imprimir la representación binaria de un número
void print_bits(unsigned char octet) {
    int i = 8;
    while (i--) {
        if (octet & (1 << i))
            write(1, "1", 1);
        else
            write(1, "0", 1);
    }
}
```

## 🧠 Patrones de Pensamiento

### Estrategias para Problemas Bitwise

1. **Visualiza en Binario**: Siempre convierte a binario mentalmente
2. **Identifica el Patrón**: ¿Necesitas establecer, limpiar, o alternar bits?
3. **Usa Máscaras**: Crea máscaras para aislar bits específicos
4. **Piensa en Potencias de 2**: 1, 2, 4, 8, 16, 32, 64, 128...

### Máscaras Útiles
```c
#define BIT_0  0x01  // 00000001
#define BIT_1  0x02  // 00000010  
#define BIT_2  0x04  // 00000100
#define BIT_3  0x08  // 00001000
#define BIT_4  0x10  // 00010000
#define BIT_5  0x20  // 00100000
#define BIT_6  0x40  // 01000000
#define BIT_7  0x80  // 10000000

#define LOWER_NIBBLE  0x0F  // 00001111
#define UPPER_NIBBLE  0xF0  // 11110000
```

## 🚀 Estrategia de Resolución

1. **Convierte a Binario**: Visualiza los números en su representación binaria
2. **Identifica la Operación**: ¿Qué bits necesitas manipular?
3. **Crea la Máscara**: Diseña la máscara apropiada
4. **Aplica la Operación**: Usa el operador bitwise correcto
5. **Verifica el Resultado**: Convierte de vuelta y verifica

## 💡 Consejos para el Examen

### Trucos de Memorización
- **& para verificar**: `n & (1 << pos)` - ¿está el bit establecido?
- **| para establecer**: `n | (1 << pos)` - poner bit en 1
- **^ para alternar**: `n ^ (1 << pos)` - cambiar estado del bit
- **& ~ para limpiar**: `n & ~(1 << pos)` - poner bit en 0

### Optimizaciones Comunes
```c
// Multiplicar por 2^n
result = number << n;

// Dividir por 2^n (números positivos)
result = number >> n;

// Verificar si es par
if ((number & 1) == 0) // Es par

// Verificar si es impar  
if (number & 1) // Es impar

// Obtener el bit menos significativo
lsb = number & -number;
```

## 📝 Ejercicios en este Patrón

### Básicos:
- **print_bits**: Mostrar representación binaria de un byte
- **reverse_bits**: Invertir el orden de bits en un byte
- **swap_bits**: Intercambiar mitades de un byte
- **is_power_of_2**: Verificar si un número es potencia de 2

### Intermedios:
- **count_bits**: Contar bits establecidos
- **clear_bit**: Limpiar un bit específico
- **set_bit**: Establecer un bit específico
- **toggle_bit**: Alternar un bit específico

### Avanzados:
- **bit_manipulation**: Manipulación compleja de bits
- **binary_operations**: Operaciones binarias avanzadas
- **bit_patterns**: Reconocimiento de patrones en bits

## 🎯 Aplicaciones Prácticas

- **Optimización de memoria**: Almacenar múltiples booleanos en un entero
- **Criptografía básica**: XOR para cifrado simple
- **Flags de estado**: Múltiples estados en una variable
- **Algoritmos de compresión**: Manipulación eficiente de datos
- **Programación de sistemas**: Control de hardware y registros

¡Dominar las operaciones bitwise te dará una ventaja significativa en el examen! 🎯
