# 🚩 Patrón 12: Uso de Flags (Banderas de Estado)

## 📖 Descripción del Patrón

Los flags son una técnica fundamental para gestionar múltiples estados booleanos de manera eficiente. En lugar de usar múltiples variables, se usa un solo entero donde cada bit representa un estado diferente. Es esencial para programación de sistemas y aparece frecuentemente en exámenes.

## 🎯 Conceptos Clave

### 1. ¿Qué son los Flags?
```c
// En lugar de esto:
int is_uppercase_found = 0;
int is_lowercase_found = 0;
int is_digit_found = 0;
int is_space_found = 0;

// Usamos esto:
int flags = 0;
#define UPPERCASE_FLAG  (1 << 0)  // 0001
#define LOWERCASE_FLAG  (1 << 1)  // 0010
#define DIGIT_FLAG      (1 << 2)  // 0100
#define SPACE_FLAG      (1 << 3)  // 1000
```

### 2. Operaciones Básicas con Flags

#### Establecer un Flag
```c
// Activar un flag específico
void set_flag(int *flags, int flag) {
    *flags |= flag;
}

// Ejemplo de uso:
int status = 0;
set_flag(&status, UPPERCASE_FLAG);  // Marca que se encontró mayúscula
set_flag(&status, DIGIT_FLAG);      // Marca que se encontró dígito
// status ahora es 0101 (5)
```

#### Verificar un Flag
```c
// Comprobar si un flag está activo
int is_flag_set(int flags, int flag) {
    return (flags & flag) != 0;
}

// Ejemplo de uso:
if (is_flag_set(status, UPPERCASE_FLAG)) {
    printf("Se encontró al menos una mayúscula\n");
}
```

#### Limpiar un Flag
```c
// Desactivar un flag específico
void clear_flag(int *flags, int flag) {
    *flags &= ~flag;
}

// Ejemplo de uso:
clear_flag(&status, DIGIT_FLAG);  // Desactiva el flag de dígitos
```

#### Alternar un Flag
```c
// Cambiar el estado de un flag
void toggle_flag(int *flags, int flag) {
    *flags ^= flag;
}
```

### 3. Patrones Comunes de Flags

#### Flags de Estado de Caracteres
```c
#define CHAR_ALPHA     (1 << 0)   // Es letra
#define CHAR_DIGIT     (1 << 1)   // Es dígito
#define CHAR_UPPER     (1 << 2)   // Es mayúscula
#define CHAR_LOWER     (1 << 3)   // Es minúscula
#define CHAR_SPACE     (1 << 4)   // Es espacio
#define CHAR_PUNCT     (1 << 5)   // Es puntuación

int analyze_char(char c) {
    int flags = 0;
    
    if (c >= 'A' && c <= 'Z') {
        flags |= CHAR_ALPHA | CHAR_UPPER;
    } else if (c >= 'a' && c <= 'z') {
        flags |= CHAR_ALPHA | CHAR_LOWER;
    } else if (c >= '0' && c <= '9') {
        flags |= CHAR_DIGIT;
    } else if (c == ' ' || c == '\t') {
        flags |= CHAR_SPACE;
    }
    
    return flags;
}
```

#### Flags de Procesamiento de Strings
```c
#define FOUND_ALPHA    (1 << 0)   // Encontró letras
#define FOUND_DIGIT    (1 << 1)   // Encontró dígitos
#define FOUND_UPPER    (1 << 2)   // Encontró mayúsculas
#define FOUND_LOWER    (1 << 3)   // Encontró minúsculas
#define FOUND_SPACE    (1 << 4)   // Encontró espacios
#define VALID_FORMAT   (1 << 5)   // Formato válido
#define EMPTY_STRING   (1 << 6)   // String vacío

int analyze_string(char *str) {
    int flags = 0;
    int i = 0;
    
    if (!str || str[0] == '\0') {
        return EMPTY_STRING;
    }
    
    while (str[i]) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            flags |= FOUND_ALPHA | FOUND_UPPER;
        } else if (str[i] >= 'a' && str[i] <= 'z') {
            flags |= FOUND_ALPHA | FOUND_LOWER;
        } else if (str[i] >= '0' && str[i] <= '9') {
            flags |= FOUND_DIGIT;
        } else if (str[i] == ' ') {
            flags |= FOUND_SPACE;
        }
        i++;
    }
    
    // Validar formato (ejemplo: debe tener letras y dígitos)
    if ((flags & FOUND_ALPHA) && (flags & FOUND_DIGIT)) {
        flags |= VALID_FORMAT;
    }
    
    return flags;
}
```

### 4. Flags de Estado de Programa
```c
#define PROGRAM_INIT      (1 << 0)   // Programa inicializado
#define INPUT_VALID       (1 << 1)   // Input válido
#define PROCESSING_DONE   (1 << 2)   // Procesamiento completado
#define OUTPUT_READY      (1 << 3)   // Output listo
#define ERROR_OCCURRED    (1 << 4)   // Ocurrió error
#define DEBUG_MODE        (1 << 5)   // Modo debug activo

typedef struct {
    int flags;
    char *input;
    char *output;
    int error_code;
} program_state_t;

void initialize_program(program_state_t *state) {
    state->flags = PROGRAM_INIT;
    state->input = NULL;
    state->output = NULL;
    state->error_code = 0;
}

int validate_input(program_state_t *state, char *input) {
    if (!input || input[0] == '\0') {
        state->flags |= ERROR_OCCURRED;
        state->error_code = 1;
        return 0;
    }
    
    state->flags |= INPUT_VALID;
    state->input = input;
    return 1;
}
```

### 5. Combinaciones y Máscaras Complejas
```c
// Definir grupos de flags
#define ALL_LETTERS    (FOUND_UPPER | FOUND_LOWER)
#define ALL_CHARS      (FOUND_ALPHA | FOUND_DIGIT | FOUND_SPACE)
#define MIXED_CASE     (FOUND_UPPER | FOUND_LOWER)

// Verificar combinaciones
int has_mixed_case(int flags) {
    return (flags & MIXED_CASE) == MIXED_CASE;
}

int has_letters_and_digits(int flags) {
    return (flags & FOUND_ALPHA) && (flags & FOUND_DIGIT);
}

// Limpiar múltiples flags
void clear_error_flags(int *flags) {
    *flags &= ~(ERROR_OCCURRED | INPUT_INVALID | PROCESSING_FAILED);
}
```

## 🧠 Patrones de Pensamiento

### Diseño de Sistema de Flags

1. **Identifica Estados**: ¿Qué condiciones necesitas rastrear?
2. **Asigna Bits**: Cada estado único = un bit específico
3. **Define Constantes**: Usa `#define` para legibilidad
4. **Planifica Combinaciones**: ¿Qué combinaciones son válidas?

### Estrategias de Uso

#### Para Análisis de Strings
```c
// Patrón: Análisis en una pasada
int analyze_password_strength(char *password) {
    int flags = 0;
    int i = 0;
    
    while (password[i]) {
        // Analizar cada carácter y actualizar flags
        if (password[i] >= 'A' && password[i] <= 'Z')
            flags |= HAS_UPPERCASE;
        // ... más análisis
        i++;
    }
    
    // Evaluar fuerza basada en flags
    if ((flags & (HAS_UPPERCASE | HAS_LOWERCASE | HAS_DIGIT)) == 
        (HAS_UPPERCASE | HAS_LOWERCASE | HAS_DIGIT)) {
        flags |= STRONG_PASSWORD;
    }
    
    return flags;
}
```

#### Para Validación de Formato
```c
// Patrón: Validación progresiva
int validate_email_format(char *email) {
    int flags = 0;
    
    // Verificar presencia de @
    if (strchr(email, '@'))
        flags |= HAS_AT_SYMBOL;
    
    // Verificar presencia de .
    if (strchr(email, '.'))
        flags |= HAS_DOT;
    
    // Verificar que @ viene antes que .
    char *at_pos = strchr(email, '@');
    char *dot_pos = strrchr(email, '.');
    if (at_pos && dot_pos && at_pos < dot_pos)
        flags |= VALID_STRUCTURE;
    
    // Email válido si tiene todos los flags necesarios
    if ((flags & (HAS_AT_SYMBOL | HAS_DOT | VALID_STRUCTURE)) == 
        (HAS_AT_SYMBOL | HAS_DOT | VALID_STRUCTURE)) {
        flags |= VALID_EMAIL;
    }
    
    return flags;
}
```

## 🚀 Estrategia de Resolución

1. **Identifica los Estados**: ¿Qué condiciones necesitas rastrear?
2. **Define los Flags**: Asigna un bit único a cada estado
3. **Implementa Operaciones**: set, clear, check, toggle
4. **Combina Lógicamente**: Usa AND, OR para combinaciones
5. **Optimiza**: Un solo entero puede manejar 32 estados diferentes

## 💡 Consejos para el Examen

### Patrones de Flags Comunes en 42

#### Análisis de Strings
```c
#define FOUND_ALPHA  (1 << 0)
#define FOUND_DIGIT  (1 << 1) 
#define FOUND_SPACE  (1 << 2)
#define VALID_FORMAT (1 << 3)
```

#### Estados de Procesamiento
```c
#define INIT_DONE    (1 << 0)
#define INPUT_VALID  (1 << 1)
#define PROC_DONE    (1 << 2)
#define OUTPUT_READY (1 << 3)
```

#### Flags de Error
```c
#define NO_ERROR     0
#define INPUT_ERROR  (1 << 0)
#define PROC_ERROR   (1 << 1)
#define OUTPUT_ERROR (1 << 2)
```

### Trucos de Optimización
```c
// Verificar si exactamente un flag está activo
int exactly_one_flag(int flags) {
    return flags && !(flags & (flags - 1));
}

// Contar flags activos
int count_active_flags(int flags) {
    int count = 0;
    while (flags) {
        count += flags & 1;
        flags >>= 1;
    }
    return count;
}

// Verificar si al menos uno de múltiples flags está activo
#define ANY_ERROR_FLAG (INPUT_ERROR | PROC_ERROR | OUTPUT_ERROR)
if (flags & ANY_ERROR_FLAG) {
    // Hay al menos un error
}
```

## 📝 Ejercicios en este Patrón

### Básicos:
- **char_flags**: Analizar caracteres y establecer flags apropiados
- **string_validator**: Validar formato de string usando flags
- **state_machine**: Implementar máquina de estados con flags

### Intermedios:
- **password_strength**: Evaluar fuerza de contraseña con flags
- **file_permissions**: Simular permisos de archivo con flags
- **option_parser**: Parser de opciones de línea de comandos

### Avanzados:
- **config_manager**: Gestionar configuración con flags complejos
- **protocol_parser**: Analizar protocolo de comunicación
- **feature_flags**: Sistema de activación de características

## 🎯 Aplicaciones Prácticas

- **Configuración de programa**: Activar/desactivar características
- **Estados de conexión**: TCP, HTTP, etc.
- **Permisos de usuario**: Lectura, escritura, ejecución
- **Análisis léxico**: Tokens en compiladores
- **Validación de datos**: Verificar múltiples condiciones
- **Sistemas embebidos**: Control de hardware eficiente

## 🔧 Ejemplo Práctico: Sistema de Permisos

```c
#define PERM_READ    (1 << 0)  // 001
#define PERM_WRITE   (1 << 1)  // 010  
#define PERM_EXECUTE (1 << 2)  // 100

typedef struct {
    int user_perms;
    int group_perms;
    int other_perms;
} file_permissions_t;

// rwxrw-r--
file_permissions_t perms = {
    .user_perms = PERM_READ | PERM_WRITE | PERM_EXECUTE,  // 111 = 7
    .group_perms = PERM_READ | PERM_WRITE,                // 110 = 6
    .other_perms = PERM_READ                              // 100 = 4
};

int can_execute(file_permissions_t *perms, int user_type) {
    switch (user_type) {
        case USER: return perms->user_perms & PERM_EXECUTE;
        case GROUP: return perms->group_perms & PERM_EXECUTE;
        case OTHER: return perms->other_perms & PERM_EXECUTE;
    }
    return 0;
}
```

¡Los flags son una herramienta poderosa para escribir código eficiente y elegante! 🎯
