# Apuntes - Permutations

Ejercicio de generación de permutaciones en orden alfabético.

---

## Enfoque: Backtracking con contador de frecuencias

Este algoritmo **cuenta cuántas veces aparece cada carácter** y va "gastando" esos caracteres para construir permutaciones en orden alfabético.

---

## Estructura del algoritmo

### 1. Preparación en `main()`

```c
int cnt[256] = {0};  // Array contador para cada carácter ASCII posible
```

**Contar frecuencias de cada carácter:**
```c
i = 0;
while (i < n)
{
    ++cnt[(unsigned char)av[1][i]];  // Incrementa contador del carácter
    i++;
}
```

**Ejemplo con "abc":**
```
cnt[97] = 1  // 'a'
cnt[98] = 1  // 'b'
cnt[99] = 1  // 'c'
cnt[resto] = 0
```

**Ejemplo con "aab":**
```
cnt[97] = 2  // 'a' aparece 2 veces
cnt[98] = 1  // 'b'
cnt[resto] = 0
```

---

### 2. Generación recursiva con `perm()`

```c
void perm(int *cnt, int n, int depth, char *buf)
```

**Parámetros:**
- `cnt`: Array de frecuencias (cuántos de cada carácter quedan disponibles)
- `n`: Longitud total de la cadena
- `depth`: Posición actual que estamos llenando en el buffer
- `buf`: Buffer donde construimos la permutación

**Caso base:**
```c
if (depth == n)  // Si llenamos todas las posiciones
{
    buf[n] = '\0';
    puts(buf);   // Imprimimos la permutación completa
    return;
}
```

**Caso recursivo:**
```c
c = 0;
while (c < 256)  // Recorre TODOS los caracteres ASCII posibles (0-255)
{
    if (cnt[c])  // Si tenemos al menos uno de este carácter disponible
    {
        buf[depth] = (char)c;           // Lo colocamos en la posición actual
        --cnt[c];                       // "Gastamos" uno (decrementamos)
        perm(cnt, n, depth + 1, buf);   // Recursión: llenamos siguiente posición
        ++cnt[c];                       // BACKTRACK: lo devolvemos (restauramos)
    }
    c++;
}
```

---

## Explicación detallada de `++cnt[(unsigned char)av[1][i]]`

Esta línea es crucial para contar la frecuencia de cada carácter.

### Desglose paso a paso:

```c
++cnt[(unsigned char)av[1][i]];
```

**1. `av[1][i]`** - Acceder al carácter
```c
av[1]     // Es la cadena argumento (ej: "abc")
av[1][i]  // Es el carácter en la posición i (ej: 'a', 'b', 'c')
```

**2. `(unsigned char)av[1][i]`** - Cast a unsigned char
```c
// av[1][i] es de tipo char (puede ser signed: -128 a 127)
// Lo convertimos a unsigned char (0 a 255)
```

**¿Por qué?** Para evitar índices negativos en el array.

**3. `cnt[...]`** - Usar el carácter como índice
```c
cnt[(unsigned char)av[1][i]]
// Accede al array cnt en la posición del código ASCII del carácter
```

**4. `++cnt[...]`** - Incrementar el contador
```c
++cnt[(unsigned char)av[1][i]]
// Incrementa en 1 el contador de ese carácter
```

---

### Ejemplo práctico con "abc"

```c
int cnt[256] = {0};  // Array inicializado a ceros
char *str = "abc";
```

**Iteración 1: `i = 0`, carácter = 'a'**
```c
av[1][i] = 'a'
(unsigned char)'a' = 97  // Código ASCII de 'a'
cnt[97]                  // Accedemos a la posición 97
++cnt[97]                // cnt[97] pasa de 0 a 1
```

**Iteración 2: `i = 1`, carácter = 'b'**
```c
av[1][i] = 'b'
(unsigned char)'b' = 98
cnt[98]
++cnt[98]  // cnt[98] pasa de 0 a 1
```

**Iteración 3: `i = 2`, carácter = 'c'**
```c
av[1][i] = 'c'
(unsigned char)'c' = 99
cnt[99]
++cnt[99]  // cnt[99] pasa de 0 a 1
```

**Estado final de `cnt`:**
```c
cnt[0...96] = 0
cnt[97] = 1    // 'a' aparece 1 vez
cnt[98] = 1    // 'b' aparece 1 vez
cnt[99] = 1    // 'c' aparece 1 vez
cnt[100...255] = 0
```

---

### ¿Por qué `unsigned char` y no solo `char`?

**Problema con `char` signed:**
```c
char c = -50;  // Posible en sistemas donde char es signed
cnt[c]         // cnt[-50] ← ¡ÍNDICE NEGATIVO! Crash o comportamiento indefinido
```

**Solución con `unsigned char`:**
```c
unsigned char c = 206;  // Siempre entre 0-255
cnt[c]                  // cnt[206] ← Índice válido ✅
```

---

### Resumen visual

```c
++cnt[(unsigned char)av[1][i]];
  ↑   ↑              ↑      ↑
  │   │              │      └─ Posición en la cadena
  │   │              └──────── Cadena argumento
  │   └─────────────────────── Array de contadores (256 posiciones)
  └─────────────────────────── Incrementar en 1
```

**En palabras simples:**
> "Incrementa el contador del carácter que está en la posición `i` de la cadena"

Es como tener 256 contadores (uno para cada carácter ASCII posible) y cada vez que encuentras un carácter, aumentas su contador.

---

## Ejemplo de ejecución completa con "abc"

### Estado inicial:
```
cnt[97] = 1 ('a')
cnt[98] = 1 ('b')
cnt[99] = 1 ('c')
buf = "___"
depth = 0
```

### Árbol de recursión (simplificado):

```
depth=0: Prueba 'a' (c=97)
  cnt[97]=0, buf="a__"

  depth=1: Prueba 'b' (c=98)
    cnt[98]=0, buf="ab_"

    depth=2: Prueba 'c' (c=99)
      cnt[99]=0, buf="abc"
      depth=3 → IMPRIME "abc" ✅
      cnt[99]=1 (backtrack)

    cnt[98]=1 (backtrack)

  depth=1: Prueba 'c' (c=99)
    cnt[99]=0, buf="ac_"

    depth=2: Prueba 'b' (c=98)
      cnt[98]=0, buf="acb"
      depth=3 → IMPRIME "acb" ✅
      cnt[98]=1 (backtrack)

    cnt[99]=1 (backtrack)

  cnt[97]=1 (backtrack)

depth=0: Prueba 'b' (c=98)
  cnt[98]=0, buf="b__"

  depth=1: Prueba 'a' (c=97)
    cnt[97]=0, buf="ba_"

    depth=2: Prueba 'c' (c=99)
      cnt[99]=0, buf="bac"
      depth=3 → IMPRIME "bac" ✅

... (continúa con bca, cab, cba)
```

---

## ¿Por qué genera en orden alfabético?

**Porque recorre los caracteres en orden ASCII creciente:**

```c
c = 0;
while (c < 256)  // 0, 1, 2, ..., 97('a'), 98('b'), 99('c'), ...
```

Como va de menor a mayor código ASCII, automáticamente genera las permutaciones en orden alfabético sin necesidad de ordenar después.

---

## Ventajas de este enfoque

✅ **Genera permutaciones en orden alfabético automáticamente**
✅ **Simple de entender** (no hay swaps complicados)
✅ **Maneja duplicados naturalmente** (aunque el ejercicio no los tiene)
✅ **Backtracking claro**: decrementa → recurre → incrementa
✅ **Código corto y elegante**

---

## Código completo

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void perm(int *cnt, int n, int depth, char *buf)
{
    int c;

    if (depth == n)
    {
        buf[n] = '\0';
        puts(buf);
        return;
    }
    c = 0;
    while (c < 256)
    {
        if (cnt[c])
        {
            buf[depth] = (char)c;
            --cnt[c];
            perm(cnt, n, depth + 1, buf);
            ++cnt[c];
        }
        c++;
    }
}

int main(int ac, char **av)
{
    if (ac != 2 || !av[1][0])
    {
        puts("");
        return (0);
    }

    int i;
    int n;
    int cnt[256] = {0};
    char *buf;

    n = strlen(av[1]);
    buf = malloc(n + 1);
    if (!buf)
        return (1);

    i = 0;
    while (i < n)
    {
        ++cnt[(unsigned char)av[1][i]];
        i++;
    }
    perm(cnt, n, 0, buf);
    free(buf);

    return (0);
}
```

---

## Puntos clave para recordar

1. **Array de frecuencias**: `int cnt[256] = {0}` cuenta cada carácter ASCII
2. **Cast a unsigned char**: Evita índices negativos
3. **Backtracking**: Decrementa, recurre, incrementa
4. **Orden automático**: Recorrer de 0 a 255 garantiza orden alfabético
5. **Caso base**: Cuando `depth == n`, imprimimos la permutación
