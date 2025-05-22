# 🔄 Patrón 03: Funciones de Conversión de Tipos

## 📖 Descripción del Patrón

Este patrón cubre la conversión entre diferentes tipos de datos: strings ↔ números, cambios de base, y manipulación de arrays. Son funciones fundamentales que aparecen constantemente en exámenes.

## 🎯 Conceptos Clave

### 1. Conversión String → Número (atoi)
```c
int ft_atoi(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;
    
    // Saltar espacios en blanco
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    
    // Manejar signo
    if (str[i] == '-' || str[i] == '+') {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    
    // Convertir dígitos
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    
    return sign * result;
}
```

### 2. Conversión Número → String (itoa)
```c
char *ft_itoa(int n) {
    // 1. Calcular longitud necesaria
    // 2. Manejar caso especial (n = -2147483648)
    // 3. Convertir dígito por dígito desde el final
    // 4. Manejar signo negativo
    
    char *result;
    int len = 0;
    int temp = n;
    int negative = (n < 0);
    
    // Calcular longitud
    if (n == 0) len = 1;
    while (temp) {
        len++;
        temp /= 10;
    }
    if (negative) len++;  // Para el signo '-'
    
    // Asignar memoria
    result = malloc(sizeof(char) * (len + 1));
    if (!result) return NULL;
    
    result[len] = '\0';
    
    // Manejar caso especial
    if (n == 0) {
        result[0] = '0';
        return result;
    }
    
    // Hacer positivo si es negativo
    if (negative) n = -n;
    
    // Llenar desde el final
    while (n > 0) {
        result[--len] = (n % 10) + '0';
        n /= 10;
    }
    
    // Agregar signo si es necesario
    if (negative) result[0] = '-';
    
    return result;
}
```

### 3. Conversión con Bases (atoi_base)
```c
int ft_atoi_base(const char *str, int str_base) {
    int result = 0;
    int sign = 1;
    int i = 0;
    int digit_value;
    
    // Validar base (debe ser 2-16)
    if (str_base < 2 || str_base > 16)
        return 0;
    
    // Saltar espacios
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    
    // Manejar signo
    if (str[i] == '-' || str[i] == '+') {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    
    // Convertir dígitos en la base dada
    while (str[i]) {
        if (str[i] >= '0' && str[i] <= '9')
            digit_value = str[i] - '0';
        else if (str[i] >= 'a' && str[i] <= 'f')
            digit_value = str[i] - 'a' + 10;
        else if (str[i] >= 'A' && str[i] <= 'F')
            digit_value = str[i] - 'A' + 10;
        else
            break;  // Carácter inválido
        
        if (digit_value >= str_base)
            break;  // Dígito inválido para esta base
        
        result = result * str_base + digit_value;
        i++;
    }
    
    return sign * result;
}
```

### 4. Creación de Arrays (ft_range)
```c
int *ft_range(int start, int end) {
    int size;
    int *result;
    int i = 0;
    
    // Calcular tamaño
    if (start <= end)
        size = end - start + 1;
    else
        size = start - end + 1;
    
    // Asignar memoria
    result = malloc(sizeof(int) * size);
    if (!result)
        return NULL;
    
    // Llenar array
    if (start <= end) {
        while (start <= end) {
            result[i++] = start++;
        }
    } else {
        while (start >= end) {
            result[i++] = start--;
        }
    }
    
    return result;
}
```

### 5. División de Strings (ft_split)
```c
char **ft_split(char const *s, char c) {
    // 1. Contar número de palabras
    // 2. Asignar array de punteros
    // 3. Para cada palabra:
    //    - Calcular longitud
    //    - Asignar memoria
    //    - Copiar contenido
    // 4. Terminar con NULL
    
    int word_count = count_words(s, c);
    char **result = malloc(sizeof(char*) * (word_count + 1));
    
    if (!result)
        return NULL;
    
    int i = 0, j = 0;
    while (s[i]) {
        // Saltar separadores
        while (s[i] && s[i] == c)
            i++;
        
        if (s[i]) {
            int start = i;
            // Encontrar final de palabra
            while (s[i] && s[i] != c)
                i++;
            
            // Asignar y copiar palabra
            result[j] = malloc(sizeof(char) * (i - start + 1));
            strncpy(result[j], &s[start], i - start);
            result[j][i - start] = '\0';
            j++;
        }
    }
    
    result[j] = NULL;
    return result;
}
```

## 🧠 Patrones de Pensamiento

### Manejo de Overflow
- **Problema**: ¿Qué pasa si el número es muy grande?
- **Solución**: En exámenes normalmente no se maneja, pero sé consciente

### Casos Especiales de atoi
- Strings vacíos → 0
- Solo espacios → 0  
- Solo signo → 0
- Números con caracteres no válidos → parar en el primer inválido

### Gestión de Memoria
- **Siempre verificar malloc**: `if (!ptr) return NULL;`
- **ft_split requiere liberación compleja**: Cada string + el array de punteros

## 🚀 Estrategia de Resolución

1. **Identifica el tipo de conversión**: String→Número, Número→String, o División
2. **Maneja casos especiales primero**: NULL, vacío, casos límite
3. **Implementa el algoritmo core**: La lógica principal de conversión
4. **Verifica gestión de memoria**: ¿Necesitas malloc? ¿Verificas errores?
5. **Prueba casos extremos**: Números negativos, bases diferentes, strings largos

## 💡 Consejos para el Examen

### Para atoi:
- **Memoriza el patrón**: Espacios → Signo → Dígitos
- **Cuidado con el overflow**: En examen raramente se pide manejarlo
- **Atención a caracteres inválidos**: Para en el primero que no sea dígito

### Para itoa:
- **Caso especial INT_MIN**: -2147483648 no se puede hacer positivo
- **Calcula longitud correctamente**: No olvides el signo y el '\0'
- **Llena desde el final**: Es más fácil

### Para ft_split:
- **Cuenta palabras primero**: Necesitas saber cuántos punteros asignar
- **Maneja separadores múltiples**: "hello,,world" con separador ','
- **Termina con NULL**: Requisito estándar para arrays de strings

## 📝 Ejercicios en este Patrón

- **ft_strlen**: Contar caracteres en string
- **ft_strcpy**: Copiar string
- **ft_atoi**: String → entero (base 10)
- **do_op**: Calculadora simple con strings
- **print_hex**: Número → hexadecimal
- **ft_strdup**: Duplicar string (requiere malloc)
- **ft_atoi_base**: String → entero (cualquier base 2-16)
- **ft_range/ft_rrange**: Crear array de enteros consecutivos
- **ft_itoa**: Entero → string
- **ft_split**: Dividir string en array de palabras

¡Estas conversiones son el pan de cada día en programación C! 🎯
