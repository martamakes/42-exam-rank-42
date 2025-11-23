# Apuntes: ft_scanf - Ejercicio COMPLETADO ✅

## Resumen del ejercicio
- **Objetivo**: Implementar `ft_scanf()` que maneja conversiones %s, %d, %c
- **Estado**: COMPLETADO y funcionando
- **Archivos**: 
  - `ft_scanf.c` - implementación principal
  - `tester/` - directorio con tests para examen

## Conceptos clave implementados

### 1. Argumentos variables (va_list)
```c
va_list ap;           // "Caja" para guardar los argumentos
va_start(ap, format); // "Abrir la caja" desde 'format'
va_end(ap);           // "Cerrar la caja"
```

### 2. Diferencia scanf vs printf
| **printf** | **scanf** |
|------------|-----------|
| **ESCRIBE** datos | **LEE** datos |
| `printf("%d", num)` | `scanf("%d", &num)` |
| Toma **valores** | Toma **direcciones** (con &) |

### 3. Jerarquía de funciones implementadas
```c
ft_scanf() → ft_vfscanf() → match_conv() → {scan_int(), scan_char(), scan_string()}
```

### 4. Funciones auxiliares implementadas
- `match_space()` - Salta espacios en blanco
- `match_char()` - Verifica caracteres literales
- `scan_char()` - Lee un carácter
- `scan_int()` - Lee enteros dígito por dígito
- `scan_string()` - Lee cadenas hasta espacio

## Algoritmos importantes

### Construcción de números dígito por dígito
```c
// Input: "123"
result = 0;
// '1': result = 0 * 10 + 1 = 1
// '2': result = 1 * 10 + 2 = 12  
// '3': result = 12 * 10 + 3 = 123
```

### Conversión char a int
```c
'5' - '0' = 53 - 48 = 5  // ASCII a número
```

## Patrones de retorno
- **1** = éxito/verdadero
- **0** = fallo/falso  
- **-1** = error (EOF)

## Tests implementados
- Test básico de examen con %d %c %s
- Compilación separada sin main() para testing
- Validación automática de resultados

---

## 🔄 Conversión de int a char: `*ptr = c`

### ❓ ¿Por qué `fgetc()` devuelve `int` y no `char`?

**Motivo**: `fgetc()` necesita devolver **dos tipos de valores**:
- 🟢 **Caracteres válidos** (0-255) 
- 🔴 **EOF** (-1) para indicar error/fin

Un `char` solo puede guardar 0-255, pero **EOF es -1**. Por eso usa `int`.

### 🧠 ¿Qué pasa en memoria?

```c
int c = 65;        // c contiene el NÚMERO 65
char letra;        // Variable destino  
char *ptr = &letra;// ptr apunta a letra
*ptr = c;          // ✨ Conversión automática: 65 → 'A'
```

### 💾 En memoria (paso a paso):

```
🔹 Antes:
int c = 65;              // 4 bytes: [0][0][0][65]
char letra = ?;          // 1 byte:  [?]

🔹 Después de *ptr = c:
char letra = 'A';        // 1 byte:  [65] ← se interpreta como 'A'
```

### 🎭 ¿Conversión o Interpretación?

**🚨 CLAVE**: En memoria SIEMPRE se guarda el **número 65** (binario: `01000001`)

```c
char letra = 65;        // Memoria: [01000001]

// 🔍 MISMO valor, diferentes interpretaciones:
printf("Como número: %d\n", letra);      // 65
printf("Como carácter: %c\n", letra);    // A  
printf("Como hexadecimal: %x\n", letra); // 41
```

### 🎯 ¿Quién decide la interpretación?

**El programador** con el formato:
- `%d` = "interpreta estos bits como número"
- `%c` = "interpreta estos bits como carácter ASCII"

### ✅ Resumen:
- **En memoria**: siempre el número 65
- **El compilador/printf**: interpreta según TÚ le digas (`%d` o `%c`)  
- **No hay conversión real**: son el mismo valor visto con "gafas diferentes"

---

## 🎯 Doble Puntero: `const char **format`

### 🤔 ¿Por qué doble puntero `**`?

**Porque necesita MODIFICAR el puntero original** para avanzar en el formato.

### 📖 Analogía del Marcador de Página

Imagina que `format` es como un **marcador de página** en un libro:

```c
const char *format = "%d %c hello";
//                    ^
//                    📍 Marcador aquí
```

### 🚫 Con puntero simple `*format` (NO funciona):

```c
void mala_funcion(const char *format)  // Solo copia el marcador
{
    format++;  // ❌ Mueve MI copia del marcador
    // El marcador original NO se mueve
}
```

### ✅ Con doble puntero `**format` (SÍ funciona):

```c  
void buena_funcion(const char **format)  // Recibe la DIRECCIÓN del marcador
{
    (*format)++;  // ✅ Mueve el marcador ORIGINAL
    // Ahora el marcador original SÍ se movió
}
```

### 🔧 Ejemplo paso a paso:

```c
// 🔹 Situación inicial:
const char *format = "%d %c";
//                    ^📍

// 🔹 Llamada:
match_conv(f, &format, ap);
//            ^^^^^^^
//            "Dame la dirección donde está el marcador"

// 🔹 Dentro de match_conv:
switch (**format)  // **format = 'd'
{
    case 'd':
        (*format)++;        // ✨ Mueve el marcador original
        return scan_int(f, ap);
}

// 🔹 Resultado:
// format ahora apunta a: "%d %c"
//                          ^📍
```

### 🎪 ¿Por qué es necesario?

Para que `ft_vfscanf` sepa **dónde continuar** procesando el formato después de cada conversión.

**Sin esto**: El bucle se quedaría atascado procesando siempre el mismo carácter.

### 🧮 Tipos de punteros según el formato:

```c
char letra;     int numero;     char texto[100];
ft_scanf("%c",  "%d",          "%s", 
         &letra, &numero,      texto);
         ^       ^             ^
      char*    int*         char* (array)
```

Cada `va_arg()` debe usar el tipo correcto:
- `%c` → `va_arg(ap, char*)`
- `%d` → `va_arg(ap, int*)`  
- `%s` → `va_arg(ap, char*)`

---

## 🤔 DUDAS FRECUENTES Y RESOLUCIONES

### 1️⃣ ¿Por qué `fgetc()` dentro del bucle `while`?

**❓ Duda**: "¿No lee ya automáticamente el siguiente carácter?"

**✅ Respuesta**: SÍ avanza automáticamente, pero necesitas **actualizar la variable `c`** para el bucle.

```c
// Input: "123a"
int c = fgetc(f);  // c = '1', cursor → '2'

while (isdigit(c)) {
    result = result * 10 + (c - '0');
    c = fgetc(f);      // ← NECESARIO: actualiza c para próxima vuelta
}
// Sin esto: c siempre sería '1' → bucle infinito
```

### 2️⃣ ¿Por qué arrays no llevan `&` en scanf?

**❓ Duda**: "¿Por qué `palabra` no lleva `&` pero `num` sí?"

**✅ Respuesta**: Porque **los arrays YA SON punteros** (direcciones).

```c
int num;              // Variable individual
char palabra[100];    // Array de 100 caracteres

// En scanf:
ft_scanf("%d %s", &num, palabra);
//                 ^     ^
//              dirección  ya es dirección
```

**Equivalencias**:
```c
palabra == &palabra[0]    // Son exactamente lo mismo
```

**Analogía**:
- `&num` = "Dame la dirección de la casa de num"
- `palabra` = "Ya es la dirección de la primera casa del barrio"

### 3️⃣ ¿Por qué `ungetc()` después de leer?

**❓ Duda**: "¿Para qué devolver el carácter que acabas de leer?"

**✅ Respuesta**: Para **no consumir caracteres** que pertenecen a la siguiente operación.

```c
// Input: "123 456"
// Después de leer "123":
c = ' ';  // ← Este espacio NO es parte del número
ungetc(c, f);  // ← Lo devuelve para el próximo scanf
```

**Sin `ungetc()`**: El espacio se perdería y el próximo `scanf` fallaría.

### 4️⃣ ¿Cómo funciona `result * 10 + (c - '0')`?

**❓ Duda**: "¿Cómo se construye un número desde caracteres?"

**✅ Respuesta**: **Algoritmo posicional** dígito por dígito.

```c
// Input: "123"
result = 0;

// 1er dígito: '1'
result = 0 * 10 + ('1' - '0') = 0 + 1 = 1

// 2do dígito: '2'  
result = 1 * 10 + ('2' - '0') = 10 + 2 = 12

// 3er dígito: '3'
result = 12 * 10 + ('3' - '0') = 120 + 3 = 123
```

**¿Por qué `(c - '0')`?**
```c
'5' - '0' = 53 - 48 = 5  // Convierte carácter a número
```

### 5️⃣ ¿Por qué verificar `isdigit(c)` antes del bucle?

**❓ Duda**: "¿No basta con verificarlo en el `while`?"

**✅ Respuesta**: Para **detectar errores inmediatamente** y no procesar basura.

```c
// Input: "abc"
int c = fgetc(f);  // c = 'a'

if (!isdigit(c)) {     // ← Detecta el error ANTES del bucle
    ungetc(c, f);      // Devuelve 'a' para otros usos
    return 0;          // Falla inmediatamente
}
// Sin esto: entraría al while y se comportaría de forma extraña
```

### 6️⃣ ¿Guardar directamente en `ptr[i]` vs usar variable temporal?

**❓ Duda**: "¿Es mejor usar una variable intermedia para strings?"

**✅ Respuesta**: **NO es necesario**. Puedes guardar directamente en el destino.

```c
// ✅ Directo (mejor):
char *ptr = va_arg(ap, char*);
ptr[i] = c;  // Guarda directamente en el array del usuario

// ❌ Innecesario:
char temp[1000];  // Variable temporal
temp[i] = c;
strcpy(ptr, temp);  // Copia extra innecesaria
```

### 7️⃣ ¿Qué hace exactamente `va_arg()`?

**❓ Duda**: "¿Cómo sabe `va_arg()` qué tipo extraer?"

**✅ Respuesta**: **TÚ se lo dices** con el segundo parámetro.

```c
// El usuario llamó: ft_scanf("%d %c %s", &num, &letra, palabra);
// Los argumentos están en ap en este orden: [&num] [&letra] [palabra]

int *ptr1 = va_arg(ap, int*);   // Extrae &num
char *ptr2 = va_arg(ap, char*); // Extrae &letra  
char *ptr3 = va_arg(ap, char*); // Extrae palabra
```

**Responsabilidad**: El **formato debe coincidir** con los tipos:
- `%d` → `int*`
- `%c` → `char*`
- `%s` → `char*` (array)