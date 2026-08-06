# Apuntes: broken_gnl - Encontrar y Reparar Errores ⚡

## Resumen del ejercicio
- **Objetivo**: Detectar y reparar errores en una implementación de get_next_line
- **Habilidades**: Debugging, análisis de código, comprensión de punteros y memoria
- **Archivos**: 
  - `broken_gnl.c` - implementación con errores
  - `broken_gnl.h` - header 
  - `reparired/` - versión arreglada para comparar

---

## 🐛 ERRORES ENCONTRADOS Y REPARACIONES

### ❌ ERROR #1: `ft_strchr` - Bucle infinito
**📍 Líneas 6-7**

**🚨 Código roto:**
```c
while (s[i] != c)  // ¡FALTA CONDICIÓN!
    i++;
```

**✅ Código arreglado:**
```c
while (s[i] && s[i] != c)  // O: while (s[i] != '\0' && s[i] != c)
    i++;
```

**🤔 ¿Por qué estaba mal?**
- Si el carácter `c` no existe en la cadena `s`, el bucle nunca termina
- Sigue buscando hasta salirse de la memoria → CRASH

**🎯 Concepto clave:** Siempre proteger contra cadenas sin terminador

---

### ❌ ERROR #2: `ft_memcpy` - Copia incompleta  
**📍 Líneas 16-17**

**🚨 Código roto:**
```c
while (--n > 0)
    ((char *)dest)[n - 1] = ((char *)src)[n - 1];
```

**✅ Soluciones correctas:**
```c
// Opción 1:
while (n-- > 0)
    ((char *)dest)[n] = ((char *)src)[n];

// Opción 2:  
while (n > 0)
{
    n--;
    ((char *)dest)[n] = ((char *)src)[n];
}

// Opción 3:
while (n > 0)
{
    ((char *)dest)[n-1] = ((char *)src)[n-1];
    n--;
}
```

**🤔 ¿Por qué estaba mal?**
- Con `while (--n > 0)` y `[n-1]`: nunca copia la última posición
- Ejemplo con n=4: copia posiciones 2, 1, 0 pero NO la 3

**📚 Seguimiento paso a paso:**
```
Ejemplo: copiar "hola" (n=4)

VERSIÓN ROTA:
n=4 → --n → n=3 → ¿3>0? SÍ → copia [2]='l'  
n=3 → --n → n=2 → ¿2>0? SÍ → copia [1]='o'
n=2 → --n → n=1 → ¿1>0? SÍ → copia [0]='h'  
n=1 → --n → n=0 → ¿0>0? NO → sale
RESULTADO: "hol?" (falta 'a')

VERSIÓN ARREGLADA (opción 1):
n=4 → ¿4>0? SÍ → n-- → n=3 → copia [3]='a'
n=3 → ¿3>0? SÍ → n-- → n=2 → copia [2]='l'  
n=2 → ¿2>0? SÍ → n-- → n=1 → copia [1]='o'
n=1 → ¿1>0? SÍ → n-- → n=0 → copia [0]='h'
n=0 → ¿0>0? NO → sale
RESULTADO: "hola" (completo)
```

---

### ❌ ERROR #3: `str_append_mem` - Crash con NULL
**📍 Línea 34**

**🚨 Código roto:**
```c
size_t size1 = ft_strlen(*s1);  // ¡CRASH si *s1 es NULL!
```

**✅ Código arreglado:**
```c
size_t size1 = (*s1) ? ft_strlen(*s1) : 0;
// Más adelante también:
if (*s1)
    ft_memcpy(tmp, *s1, size1);
```

**🤔 ¿Por qué estaba mal?**
- En `get_next_line`, `ret` se inicializa como `NULL`
- `str_append_mem(&ret, b, size)` llama `ft_strlen(NULL)` → CRASH

**🎯 Concepto:** Siempre validar punteros antes de usarlos

---

### ❌ ERROR #4: `ft_memmove` - Uso incorrecto de ft_strlen
**📍 Línea 57**

**🚨 Código roto:**
```c
size_t i = ft_strlen((char *)src) - 1;  // ¡INCORRECTO!
```

**✅ Código arreglado:**
```c
int i = n - 1;  // Usar parámetro n, no ft_strlen
```

**🤔 ¿Por qué estaba mal?**
- `ft_memmove` debe copiar exactamente `n` bytes
- `ft_strlen()` cuenta hasta `'\0'`, no tiene relación con `n`
- Si src="hola" pero n=10, solo copia 4 bytes en lugar de 10

**🎯 Concepto:** ft_memmove copia memoria bruta, no cadenas

---

### ❌ ERROR #5: `ft_memmove` - Bucle infinito con unsigned
**📍 Línea 58**

**🚨 Código roto:**
```c
size_t i = n - 1;
while (i >= 0)  // ¡INFINITO con unsigned!
```

**✅ Código arreglado:**
```c
int i = n - 1;  // Cambiar a int (signed)
while (i >= 0)
```

**🤔 ¿Por qué estaba mal?**
- `size_t` es unsigned (sin signo)
- Cuando i=0 y haces i--, se convierte en un número gigante
- El bucle nunca termina porque siempre es >= 0

**🎯 Concepto:** Cuidado con tipos unsigned en bucles decrementales

---

### ❌ ERROR #6: `get_next_line` - No maneja EOF
**📍 Líneas 76-80**

**🚨 Código roto:**
```c
int read_ret = read(fd, b, BUFFER_SIZE);
if (read_ret == -1)
    return (NULL);
b[read_ret] = 0;
// ¡No maneja read_ret == 0 (EOF)!
```

**✅ Código arreglado:**
```c
int read_ret = read(fd, b, BUFFER_SIZE);
if (read_ret == -1)
    return (NULL);
if (read_ret == 0)  // Fin de archivo
    break;
b[read_ret] = 0;
```

**🤔 ¿Por qué estaba mal?**
- Cuando `read()` devuelve 0 = fin de archivo
- Sin break, el bucle continúa infinitamente
- Archivo sin `'\n'` final causa bucle infinito

---

### ❌ ERROR #7: `get_next_line` - No actualiza tmp
**📍 Línea 79**

**🚨 Código roto:**
```c
while (!tmp)
{
    // ... lee datos ...
    b[read_ret] = 0;
    // ¡FALTA: tmp = ft_strchr(b, '\n');
}
```

**✅ Código arreglado:**
```c
while (!tmp)
{
    // ... lee datos ...
    b[read_ret] = 0;
    tmp = ft_strchr(b, '\n');  // ¡NECESARIO!
}
```

**🤔 ¿Por qué estaba mal?**
- Después de leer nuevos datos, hay que verificar si ahora contienen `'\n'`
- Sin actualizar `tmp`, el bucle puede ser infinito

---

### ❌ ERROR #8: `get_next_line` - Crash con tmp NULL
**📍 Líneas 81-86**

**🚨 Código roto:**
```c
if (!str_append_mem(&ret, b, tmp - b + 1))  // ¡tmp puede ser NULL!
```

**✅ Código arreglado:**
```c
if (tmp)  // Verificar si tmp no es NULL
{
    if (!str_append_mem(&ret, b, tmp - b + 1))
    {
        free(ret);
        return (NULL);
    }
    ft_memmove(b, tmp + 1, ft_strlen(tmp + 1) + 1);
    return (ret);
}
else  // EOF sin '\n'
{
    b[0] = '\0';
    if (ret && *ret)
        return (ret);
    else
    {
        free(ret);
        return (NULL);
    }
}
```

**🤔 ¿Por qué estaba mal?**
- Si llegas al EOF sin encontrar `'\n'`, tmp es NULL
- `NULL - b + 1` causa segmentation fault
- Necesitas manejar ambos casos: con y sin `'\n'`

**🎯 Concepto:** Gestión del buffer estático para conservar datos entre llamadas

---

## 💡 CONCEPTOS CLAVE QUE SUELO FALLAR

### 1️⃣ Diferencia: Pre vs Post-decremento
```c
int n = 4;

// PRE-decremento (--n):
while (--n > 0)  // 1) n=3, 2) ¿3>0? SÍ

// POST-decremento (n--):  
while (n-- > 0)  // 1) ¿4>0? SÍ, 2) n=3
```

### 2️⃣ Doble función de variable: contador + índice
```c
// n = 4 significa "4 bytes para copiar"
// Pero arrays usan índices [0,1,2,3]
// Por eso: n=4 → n=3 → copiamos índice 3
```

### 3️⃣ Protección contra NULL en funciones auxiliares
```c
// ❌ MAL:
size_t len = ft_strlen(s);

// ✅ BIEN:  
size_t len = s ? ft_strlen(s) : 0;
```

### 4️⃣ Condiciones de bucle con punteros
```c
// ❌ MAL - puede salirse de memoria:
while (s[i] != c)

// ✅ BIEN - para en '\0':
while (s[i] && s[i] != c)
```

---

## 🔄 PATRONES DE RETORNO (RECORDATORIO)

### En main():
- `return 0` = **éxito** (convención del SO)
- `return 1` = **error**

### En funciones auxiliares:
- `return 1` = **éxito** (lógica booleana: true)
- `return 0` = **error** (lógica booleana: false)

**Truco para recordar:**
- main = convención del SO (0 = bien)  
- funciones = lógica booleana (1 = true = bien)

---

## 📊 RESUMEN COMPLETO DE ERRORES ENCONTRADOS

**TOTAL: 8 ERRORES CRÍTICOS REPARADOS**

1. **ft_strchr**: Bucle infinito sin protección `'\0'`
2. **ft_memcpy**: Copia incompleta con condición `--n > 0` 
3. **str_append_mem**: Crash con NULL pointer
4. **ft_memmove**: Uso incorrecto de `ft_strlen()` vs `n`
5. **ft_memmove**: Bucle infinito con `size_t` unsigned
6. **get_next_line**: No maneja EOF (`read_ret == 0`)
7. **get_next_line**: No actualiza `tmp` después de leer
8. **get_next_line**: Crash con `tmp` NULL al final

---

## 📝 METODOLOGÍA PARA ENCONTRAR ERRORES

### 1. Leer función por función
- No mirar la versión arreglada primero
- Simular ejecución mental paso a paso

### 2. Casos críticos a verificar:
- ¿Qué pasa si el input es NULL?
- ¿Qué pasa si no se encuentra lo que buscamos?
- ¿Se copian/procesan TODOS los elementos?
- ¿Los bucles terminan correctamente?
- ¿Se maneja correctamente el EOF?
- ¿Los tipos unsigned pueden causar problemas?

### 3. Seguimiento con ejemplos concretos:
- Usar datos específicos ("hola", n=4)
- Seguir ejecución línea por línea
- Verificar el resultado final
- Probar casos límite (EOF, NULL, cadenas vacías)

### 4. Verificar patrones comunes:
- Bucles con condiciones de parada
- Manejo de punteros NULL
- Operadores pre/post incremento
- Cálculos de índices vs tamaños
- Tipos signed vs unsigned
- Gestión de memoria estática vs dinámica

---

## 🎯 ERRORES QUE SUELO COMETER

### ❌ Confundir `return (s + i)` vs `return (s[i])`
- `s + i` = puntero a posición i ✅
- `s[i]` = valor en posición i ❌

### ❌ Olvidar decrementar en bucles 
```c
while (n > 0)
    codigo;  // ¡INFINITO! Falta n--
```

### ❌ No proteger contra NULL
```c
ft_strlen(*s1);  // ❌ Crash si *s1 es NULL
```

### ❌ Condiciones de bucle incompletas
```c
while (s[i] != c)  // ❌ No para en '\0'
```

### ❌ Tipos unsigned en bucles decrementales
```c
size_t i = n - 1;
while (i >= 0)  // ❌ Infinito: i nunca es < 0
    i--;
```

### ❌ No manejar EOF en read()
```c
if (read_ret == -1) return NULL;
// ❌ Falta: if (read_ret == 0) break;
```

### ❌ No actualizar variables de control
```c
while (!tmp) {
    // ... leer datos ...
    // ❌ Falta: tmp = ft_strchr(b, '\n');
}
```