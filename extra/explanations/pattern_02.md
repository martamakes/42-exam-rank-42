# 📝 Patrón 02: Procesamiento de Cadenas Palabra por Palabra

## 📖 Descripción del Patrón

Este patrón se enfoca en identificar y manipular palabras dentro de cadenas de texto. Es fundamental para muchos ejercicios de examen que requieren procesar texto de manera inteligente.

## 🎯 Conceptos Clave

### 1. Definición de Palabra
En el contexto de 42, una palabra es una secuencia de caracteres separada por espacios (' ') y/o tabulaciones ('\t').

### 2. Patrón Básico de Procesamiento
```c
void process_words(char *str) {
    int i = 0;
    
    // Saltar espacios iniciales
    while (str[i] && (str[i] == ' ' || str[i] == '\t'))
        i++;
    
    while (str[i]) {
        // Procesar la palabra actual
        while (str[i] && str[i] != ' ' && str[i] != '\t') {
            // Hacer algo con str[i] (carácter de la palabra)
            write(1, &str[i], 1);  // Ejemplo: imprimir
            i++;
        }
        
        // Saltar espacios entre palabras
        while (str[i] && (str[i] == ' ' || str[i] == '\t'))
            i++;
        
        // Si hay más palabras, agregar separador
        if (str[i])
            write(1, " ", 1);
    }
}
```

### 3. Conversiones de Formato

#### CamelCase ↔ snake_case
```c
// camelCase -> snake_case: "helloWorld" -> "hello_world"
void camel_to_snake(char *str) {
    int i = 0;
    while (str[i]) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            if (i > 0) write(1, "_", 1);  // Agregar underscore antes
            str[i] += 32;  // Convertir a minúscula
        }
        write(1, &str[i], 1);
        i++;
    }
}

// snake_case -> camelCase: "hello_world" -> "helloWorld"
void snake_to_camel(char *str) {
    int i = 0;
    int capitalize_next = 0;
    
    while (str[i]) {
        if (str[i] == '_') {
            capitalize_next = 1;
        } else {
            if (capitalize_next && str[i] >= 'a' && str[i] <= 'z') {
                str[i] -= 32;  // Convertir a mayúscula
                capitalize_next = 0;
            }
            write(1, &str[i], 1);
        }
        i++;
    }
}
```

### 4. Manipulación de Espacios
```c
// Eliminar espacios múltiples y normalizar
void normalize_spaces(char *str) {
    int i = 0;
    int first_word = 1;
    
    // Saltar espacios iniciales
    while (str[i] && (str[i] == ' ' || str[i] == '\t'))
        i++;
    
    while (str[i]) {
        // Si no es la primera palabra, agregar un espacio
        if (!first_word)
            write(1, " ", 1);
        
        // Imprimir la palabra actual
        while (str[i] && str[i] != ' ' && str[i] != '\t') {
            write(1, &str[i], 1);
            i++;
        }
        
        // Saltar espacios múltiples
        while (str[i] && (str[i] == ' ' || str[i] == '\t'))
            i++;
        
        first_word = 0;
    }
}
```

### 5. Capitalización de Palabras
```c
// Capitalizar primera letra de cada palabra
void str_capitalizer(char *str) {
    int i = 0;
    int new_word = 1;
    
    while (str[i]) {
        if (str[i] == ' ' || str[i] == '\t') {
            new_word = 1;
            write(1, &str[i], 1);
        } else {
            if (new_word && str[i] >= 'a' && str[i] <= 'z') {
                str[i] -= 32;  // Convertir a mayúscula
                new_word = 0;
            } else if (!new_word && str[i] >= 'A' && str[i] <= 'Z') {
                str[i] += 32;  // Convertir a minúscula
            } else if (str[i] >= 'a' && str[i] <= 'z') {
                new_word = 0;
            }
            write(1, &str[i], 1);
        }
        i++;
    }
}
```

## 🧠 Patrones de Pensamiento

### Estrategias Comunes

1. **Identificación de Límites**: Siempre identifica dónde empiezan y terminan las palabras
2. **Estado de Proceso**: Mantén seguimiento si estás dentro o fuera de una palabra
3. **Normalización**: Trata espacios y tabs de manera uniforme
4. **Casos Especiales**: Maneja cadenas vacías, solo espacios, y palabras únicas

### Técnicas de Rotación de Palabras
```c
// rostring: mover primera palabra al final
// "hello world 42" -> "world 42 hello"
void rostring(char *str) {
    // 1. Encontrar y guardar primera palabra
    // 2. Procesar resto de palabras
    // 3. Agregar primera palabra al final
}
```

## 🚀 Estrategia de Resolución

1. **Lee el problema cuidadosamente**: ¿Qué se considera una palabra?
2. **Identifica el patrón de transformación**: ¿Rotación? ¿Conversión? ¿Limpieza?
3. **Dibuja ejemplos en papel**: Visualiza la transformación
4. **Implementa el esqueleto básico**: Navegación por palabras
5. **Agrega la lógica específica**: La transformación requerida
6. **Maneja casos especiales**: Strings vacíos, espacios múltiples

## 💡 Consejos para el Examen

- **Memoriza el patrón de navegación**: Es la base de todo
- **Cuidado con los espacios**: Muchos ejercicios fallan por mal manejo de espacios
- **Practica las conversiones**: camelCase ↔ snake_case son muy comunes
- **Atención a first_word y last_word**: Lógica similar pero diferente implementación
- **Siempre verifica**: ¿Qué pasa con strings vacíos?

## 📝 Ejercicios en este Patrón

- **first_word**: Extraer primera palabra
- **last_word**: Extraer última palabra  
- **repeat_alpha**: Repetir caracteres según posición alfabética
- **rev_print**: Imprimir string al revés
- **search_and_replace**: Reemplazar caracteres
- **camel_to_snake**: Conversión camelCase → snake_case
- **snake_to_camel**: Conversión snake_case → camelCase
- **epur_str**: Limpiar espacios múltiples
- **expand_str**: Expandir con espacios
- **str_capitalizer**: Capitalizar primera letra de palabras
- **rstr_capitalizer**: Capitalizar última letra de palabras
- **rostring**: Rotar palabras (primera al final)
- **rev_wstr**: Palabras en orden inverso

¡Este patrón es fundamental para muchos ejercicios de examen! 🎯
