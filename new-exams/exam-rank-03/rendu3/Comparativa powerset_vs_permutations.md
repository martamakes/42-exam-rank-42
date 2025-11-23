# 💡 PLANTILLAS MENTALES PARA EL EXAMEN

## 🟦 POWERSET (Máscaras)

**Señales en el enunciado:**
- "suma de elementos"
- "subconjuntos"
- Primer argumento es número

**Patrón de código:**
```c
int k = ac - 2;                    // Cantidad de números
int target = atoi(av[1]);          // Objetivo a sumar
unsigned long total = 1UL << k;    // 2^k combinaciones
unsigned long mask = 1;

while (mask < total)
{
    // 1. Calcular suma con mask
    // 2. Si sum == target, imprimir con misma mask
    ++mask;
}
```

**Frase clave:** *"Pruebo todas las MÁSCARAS para encontrar SUMAS"*

---

## 🟩 PERMUTATIONS (Recursión)

**Señales en el enunciado:**
- "permutaciones"
- "orden alfabético"
- Un solo argumento string

**Patrón de código:**
```c
int cnt[256] = {0};          // Contador de frecuencias
char *buf = malloc(n + 1);   // Buffer para construir

// Llenar cnt[] con frecuencias
perm(cnt, n, 0, buf);        // Llamada recursiva

void perm(int *cnt, int n, int pos, char *buf)
{
    if (pos == n)            // Caso base: imprime
        puts(buf);

    for (c = 0; c < 256; c++)
    {
        if (cnt[c])          // Si hay disponible
        {
            buf[pos] = c;    // Usa
            --cnt[c];        // Gasta
            perm(...);       // Recursión
            ++cnt[c];        // Backtrack (devuelve)
        }
    }
}
```

**Frase clave:** *"RECURSIÓN que COLOCA cada carácter y hace BACKTRACK"*

---

# 🎓 CHECKLIST PARA EL EXAMEN

Cuando veas el ejercicio, pregúntate:

### ✅ Pregunta 1: ¿Cuántos argumentos?
- **Muchos números** → POWERSET
- **Un string** → PERMUTATIONS

### ✅ Pregunta 2: ¿Qué busco?
- **Subconjuntos que SUMEN** → POWERSET (máscaras)
- **REORDENAR caracteres** → PERMUTATIONS (recursión)

### ✅ Pregunta 3: ¿Qué técnica?
- **"mask & (1UL << i)"** → POWERSET
- **"función recursiva + backtrack"** → PERMUTATIONS

---

# 🚀 CÓDIGO ESQUELETO RÁPIDO

## Para POWERSET:
```c
int k = ac - 2;
int target = atoi(av[1]);
unsigned long total = 1UL << k;
unsigned long mask = 1;

while (mask < total) {
    // Calcular suma
    // Si suma == target, imprimir
    ++mask;
}
```

## Para PERMUTATIONS:
```c
int cnt[256] = {0};
// Contar frecuencias
void perm(int *cnt, int n, int pos, char *buf) {
    if (pos == n) { puts(buf); return; }
    for (int c = 0; c < 256; c++) {
        if (cnt[c]) {
            buf[pos] = c;
            --cnt[c];
            perm(cnt, n, pos+1, buf);
            ++cnt[c];
        }
    }
}
```

---

# 🎯 RESUMEN ULTRA-RÁPIDO

| Si ves... | Es... | Usa... |
|-----------|-------|--------|
| `./exe 5 1 2 3` | POWERSET | `mask & (1UL << i)` |
| `./exe "abc"` | PERMUTATIONS | Recursión + backtrack |
| Palabra "suma" | POWERSET | Bucle while masks |
| Palabra "permut" | PERMUTATIONS | Función recursiva |

---

---

# 🔄 PERMUTATIONS: `--cnt[c]` y `++cnt[c]` (BACKTRACKING)

## El problema que resuelve

Imagina que tienes la cadena **"abc"**. Necesitas generar todas las permutaciones, pero **cada carácter solo puede usarse UNA vez** por permutación.

```c
void perm(int *cnt, int n, int pos, char *buf)
{
    if (pos == n) { puts(buf); return; }

    for (int c = 0; c < 256; c++)
    {
        if (cnt[c])              // ¿Tengo este carácter disponible?
        {
            buf[pos] = c;        // 1. Lo USO
            --cnt[c];            // 2. Lo MARCO como usado (decremento)
            perm(cnt, n, pos+1, buf);  // 3. RECURSIÓN con un carácter menos
            ++cnt[c];            // 4. BACKTRACK: lo DEVUELVO (incremento)
        }
    }
}
```

## Ejemplo visual con "ab"

```c
cnt['a'] = 1, cnt['b'] = 1
buf = [?, ?]

┌─ Nivel 0 (pos=0) ─────────────────────┐
│                                        │
│  Pruebo 'a':                           │
│  buf[0] = 'a'  →  buf = [a, ?]        │
│  --cnt['a']    →  cnt['a'] = 0  ✓     │
│                                        │
│  ┌─ Nivel 1 (pos=1) ─────────────┐   │
│  │                                 │   │
│  │  Pruebo 'b':                    │   │
│  │  buf[1] = 'b'  →  buf = [a, b] │   │
│  │  --cnt['b']    →  cnt['b'] = 0 │   │
│  │                                 │   │
│  │  ┌─ Nivel 2 (pos=2) ──────┐   │   │
│  │  │  pos == n → IMPRIME "ab" │   │   │
│  │  └─────────────────────────┘   │   │
│  │                                 │   │
│  │  ++cnt['b']  ← BACKTRACK ✓     │   │
│  │  cnt['b'] = 1 (restaurado)     │   │
│  └─────────────────────────────────┘   │
│                                        │
│  ++cnt['a']  ← BACKTRACK ✓             │
│  cnt['a'] = 1 (restaurado)             │
└────────────────────────────────────────┘

┌─ Nivel 0 (pos=0) ─────────────────────┐
│                                        │
│  Pruebo 'b':                           │
│  buf[0] = 'b'  →  buf = [b, ?]        │
│  --cnt['b']    →  cnt['b'] = 0  ✓     │
│                                        │
│  ┌─ Nivel 1 (pos=1) ─────────────┐   │
│  │                                 │   │
│  │  Pruebo 'a':                    │   │
│  │  buf[1] = 'a'  →  buf = [b, a] │   │
│  │  --cnt['a']    →  cnt['a'] = 0 │   │
│  │                                 │   │
│  │  ┌─ Nivel 2 (pos=2) ──────┐   │   │
│  │  │  pos == n → IMPRIME "ba" │   │   │
│  │  └─────────────────────────┘   │   │
│  │                                 │   │
│  │  ++cnt['a']  ← BACKTRACK ✓     │   │
│  └─────────────────────────────────┘   │
│                                        │
│  ++cnt['b']  ← BACKTRACK ✓             │
└────────────────────────────────────────┘

Salida:
ab
ba
```

## ¿Por qué --cnt y ++cnt?

### `--cnt[c]` (Decrementar)
**"Estoy USANDO este carácter ahora, ya no está disponible"**

```c
cnt['a'] = 1  →  --cnt['a']  →  cnt['a'] = 0
// Ahora 'a' NO puede usarse en niveles más profundos
```

### `++cnt[c]` (Incrementar - BACKTRACK)
**"YA TERMINÉ de explorar con este carácter, lo DEVUELVO al pool"**

```c
cnt['a'] = 0  →  ++cnt['a']  →  cnt['a'] = 1
// Ahora 'a' está disponible de nuevo para otras ramas
```

## Sin backtracking, ¿qué pasaría?

```c
// CÓDIGO ROTO (sin ++cnt[c]):
void perm_roto(int *cnt, int n, int pos, char *buf)
{
    if (pos == n) { puts(buf); return; }

    for (int c = 0; c < 256; c++)
    {
        if (cnt[c])
        {
            buf[pos] = c;
            --cnt[c];
            perm_roto(cnt, n, pos+1, buf);
            // ❌ NO HAY ++cnt[c] aquí
        }
    }
}

// Con "ab":
// Imprime "ab"
// Pero NO imprime "ba" porque 'a' ya no está disponible (cnt['a']=0)
```

**Resultado:** Solo imprimiría **"ab"**, faltaría **"ba"**.

---

# ⬆️ POWERSET: `++mask` (SIMPLE CONTADOR)

## El propósito completamente diferente

En powerset, **NO hay backtracking** porque no es recursivo. Es simplemente un **contador** que avanza linealmente:

```c
unsigned long mask = 1;
while (mask < total)  // Bucle ITERATIVO
{
    // Procesar máscara actual
    ++mask;  // Siguiente número
}
```

## ¿Por qué `++mask`?

**"Paso a la siguiente combinación"**

```
mask=1  →  procesa  →  ++mask  →  mask=2
mask=2  →  procesa  →  ++mask  →  mask=3
mask=3  →  procesa  →  ++mask  →  mask=4
...
```

Es simplemente un **contador normal** de 1 a 31 (si k=5).

## NO es backtracking

```c
// En powerset NO hay:
--mask;  // ❌ No retrocedemos
++mask;  // ✅ Solo avanzamos linealmente
```

**No necesita "devolver" nada** porque cada iteración es **independiente**. No hay llamadas recursivas que compartan estado.

---

# 📊 TABLA COMPARATIVA

| Aspecto | **PERMUTATIONS** (`--cnt[c]` / `++cnt[c]`) | **POWERSET** (`++mask`) |
|---------|---------------------------------------------|-------------------------|
| **Propósito** | BACKTRACKING (usar/devolver recurso) | CONTADOR simple |
| **Contexto** | Recursión (estado compartido) | Iteración (estados independientes) |
| **--cnt[c]** | "Uso este carácter AHORA" | N/A |
| **++cnt[c]** | "DEVUELVO el carácter para otras ramas" | N/A |
| **++mask** | N/A | "Siguiente número en secuencia" |
| **Necesidad** | CRÍTICO (sin él, fallan permutaciones) | OBVIO (solo avanza el bucle) |

---

# 🎯 ANALOGÍAS PARA RECORDAR

## PERMUTATIONS es como prestar libros

```
Biblioteca: cnt[] = {libro_a: 1, libro_b: 1}

1. Tomo libro_a  →  --cnt['a']  (ya no disponible)
2. Tomo libro_b  →  --cnt['b']
3. Leo: "ab"
4. Devuelvo libro_b  →  ++cnt['b']  (disponible de nuevo)
5. Devuelvo libro_a  →  ++cnt['a']  (disponible de nuevo)
6. Ahora puedo formar "ba" porque devolví los libros
```

**Sin devolverlos (`++cnt`), no podrías formar otras permutaciones.**

## POWERSET es como contar ovejas

```
Oveja 1, oveja 2, oveja 3, oveja 4...
No necesitas "devolver" las ovejas, solo cuentas.
++mask es simplemente el conteo: 1, 2, 3, 4...
```

---

# 🧠 RESUMEN MENTAL

### **PERMUTATIONS: `--cnt[c]` / `++cnt[c]`**
- **--cnt[c]**: "Tomo prestado este carácter"
- **++cnt[c]**: "Devuelvo el carácter (BACKTRACK)"
- **Razón**: Necesito reutilizar caracteres en diferentes ramas de recursión

### **POWERSET: `++mask`**
- **++mask**: "Siguiente número en la secuencia"
- **Razón**: Es un simple contador de 1 a 2^k-1
- **NO es backtracking**, es un bucle lineal

---

# 📁 ARCHIVOS DE REFERENCIA

- **POWERSET solución:** [rendu3/powerset/powerset_backtraking.c](../rendu3/powerset/powerset_backtraking.c)
- **PERMUTATIONS solución:** [rendu3/permutations/permutations_bits.c](../rendu3/permutations/permutations_bits.c)
- **Subject POWERSET:** [level-2/powerset/subject-es.txt](../level-2/powerset/subject-es.txt)
- **Subject PERMUTATIONS:** [level-2/permutations/subject-es.txt](../level-2/permutations/subject-es.txt)
