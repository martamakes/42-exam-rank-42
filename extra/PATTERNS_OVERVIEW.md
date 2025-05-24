# 📋 Sistema Completo de Patrones - 42 School Exam Rank 02

## 🎯 12 Patrones Conceptuales para Dominar el Examen

### ✨ **Nuevos Patrones Especializados**

## 🔢 **Patrón 11: Operaciones Bitwise** 
**Ejercicios**: print_bits, reverse_bits, swap_bits, is_power_of_2

### 🧠 Conceptos Esenciales:
- **Operadores básicos**: & (AND), | (OR), ^ (XOR), ~ (NOT), << (shift left), >> (shift right)
- **Verificar bits**: `(n >> pos) & 1` 
- **Establecer bits**: `n | (1 << pos)`
- **Limpiar bits**: `n & ~(1 << pos)`
- **Potencias de 2**: `n > 0 && (n & (n-1)) == 0`

### 💡 Casos de Uso:
- Optimización de memoria y rendimiento
- Manipulación eficiente de flags múltiples
- Algoritmos de bajo nivel y sistemas embebidos

---

## 🚩 **Patrón 12: Uso de Flags** 
**Ejercicios**: wdmatch, union, inter, hidenp, paramsum, str_capitalizer

### 🧠 Conceptos Esenciales:
- **Múltiples estados en un entero**: En lugar de muchas variables booleanas
- **Flags combinados**: `#define FLAG_A (1 << 0)`, `#define FLAG_B (1 << 1)`
- **Establecer**: `flags |= FLAG_A`
- **Verificar**: `if (flags & FLAG_A)`
- **Limpiar**: `flags &= ~FLAG_A`

### 💡 Casos de Uso:
- Rastrear estados de procesamiento de strings
- Validación de formatos complejos
- Gestión eficiente de configuraciones

---

## 📚 **Lista Completa de los 12 Patrones**

| # | Patrón | Enfoque | Ejercicios Clave |
|---|--------|---------|------------------|
| 🔤 **01** | Manipulación básica de caracteres | ASCII, transformaciones simples | rot_13, ulstr, print_bits |
| 📝 **02** | Procesamiento palabra por palabra | Navegación, conversiones formato | first_word, camelCase, rostring |
| 🔄 **03** | Conversión de tipos | String↔Número, bases, arrays | ft_atoi, ft_itoa, ft_split |
| 🔍 **04** | Algoritmos de búsqueda | Subcadenas, coincidencias | wdmatch, hidenp, strcspn |
| 📊 **05** | Operaciones con conjuntos | Unión, intersección | union, inter |
| 💾 **06** | Memoria y punteros | malloc, gestión dinámica | strdup, ft_range, ft_split |
| 🧮 **07** | Algoritmos matemáticos | Primos, MCM, MCD | add_prime_sum, pgcd, fprime |
| 🔗 **08** | Listas enlazadas | Navegación, modificación | ft_list_size, sort_list |
| 📈 **09** | Algoritmos de ordenación | Bubble sort, optimización | sort_int_tab, sort_list |
| 🌊 **10** | Recursividad avanzada | Flood fill, backtracking | flood_fill |
| 🔢 **11** | Operaciones Bitwise | Manipulación bits, máscaras | print_bits, is_power_of_2 |
| 🚩 **12** | Uso de Flags | Estados múltiples, optimización | wdmatch, union, str_capitalizer |

---

## 🎓 **Estrategia de Estudio Actualizada**

### **Fase 1: Fundamentos (Semanas 1-3)**
- **Patrones 01-03**: Caracteres, palabras, conversiones
- **Patrón 11**: Operaciones bitwise (fundamental para optimización)

### **Fase 2: Algoritmos Core (Semanas 4-5)**
- **Patrones 04-05**: Búsqueda y conjuntos
- **Patrón 12**: Flags (técnica transversal)

### **Fase 3: Memoria y Matemáticas (Semanas 6-7)**
- **Patrones 06-07**: Memoria y algoritmos matemáticos

### **Fase 4: Estructuras Avanzadas (Semanas 8-9)**
- **Patrones 08-09**: Listas y ordenación

### **Fase 5: Recursividad y Repaso (Semana 10)**
- **Patrón 10**: Recursividad avanzada
- **Repaso integral**: Combinando técnicas de todos los patrones

---

## 🚀 **Ventajas de los Nuevos Patrones**

### **Patrón 11 - Operaciones Bitwise**
✅ **Velocidad**: Operaciones más rápidas que aritmética tradicional  
✅ **Memoria**: Almacenar múltiples booleanos en un entero  
✅ **Elegancia**: Soluciones concisas y eficientes  
✅ **Examen**: Aparece frecuentemente en ejercicios avanzados  

### **Patrón 12 - Uso de Flags**
✅ **Organización**: Gestión clara de estados múltiples  
✅ **Escalabilidad**: Fácil agregar nuevos estados  
✅ **Eficiencia**: Menos variables, mejor rendimiento  
✅ **Realismo**: Técnica usada en sistemas reales  

---

## 💡 **Aplicación Combinada de Patrones**

### **Ejemplo: Validador de Contraseña Avanzado**
```c
#define HAS_UPPER    (1 << 0)  // Patrón 12: Flags
#define HAS_LOWER    (1 << 1)
#define HAS_DIGIT    (1 << 2)
#define HAS_SPECIAL  (1 << 3)
#define VALID_LENGTH (1 << 4)
#define STRONG_PASS  (1 << 5)

int validate_password(char *pass) {
    int flags = 0;  // Patrón 12: Sistema de flags
    int i = 0;
    
    // Patrón 02: Procesamiento carácter por carácter
    while (pass[i]) {
        if (pass[i] >= 'A' && pass[i] <= 'Z')
            flags |= HAS_UPPER;  // Patrón 11: Operación bitwise
        else if (pass[i] >= 'a' && pass[i] <= 'z')
            flags |= HAS_LOWER;
        else if (pass[i] >= '0' && pass[i] <= '9')
            flags |= HAS_DIGIT;
        // Patrón 01: Manipulación de caracteres
        i++;
    }
    
    // Patrón 03: Validación de longitud
    if (i >= 8) flags |= VALID_LENGTH;
    
    // Verificar fortaleza combinando múltiples flags
    if ((flags & (HAS_UPPER | HAS_LOWER | HAS_DIGIT | VALID_LENGTH)) == 
        (HAS_UPPER | HAS_LOWER | HAS_DIGIT | VALID_LENGTH)) {
        flags |= STRONG_PASS;
    }
    
    return flags;
}
```

Este ejemplo combina **5 patrones diferentes** en una solución elegante y eficiente!

---

## 🎯 **Comandos Actualizados**

```bash
# Nuevos patrones disponibles
make pattern-11    # Operaciones Bitwise
make pattern-12    # Uso de Flags

# Sistema completo
make start         # Acceso a los 12 patrones
make progress      # Tracking de 12 patrones
make patterns      # Lista completa actualizada
```

---

## 🏆 **Filosofía Expandida**

> **"No memorices 54 ejercicios, domina 12 patrones universales."**

Con estos 12 patrones conceptuales:
- **✅ Cobertura completa**: Todo ejercicio de examen encaja en al menos uno
- **✅ Transferencia de conocimiento**: Aplicas patrones a problemas nuevos  
- **✅ Optimización mental**: Menos información que memorizar
- **✅ Confianza en examen**: Reconoces patrones bajo presión

¡Ahora tienes un sistema completo y optimizado para dominar cualquier ejercicio de 42 School! 🚀
