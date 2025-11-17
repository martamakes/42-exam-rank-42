# Explicación Visual del Backtracking - Powerset

## Ejemplo: `./powerset 3 1 2`

```
target = 3
set = [1, 2]
set_size = 2
```

---

## TODOS los caminos explorados

```
═══════════════════════════════════════════════════════════════
CAMINO 1: NO incluir 1, NO incluir 2
═══════════════════════════════════════════════════════════════

┌─────────────────────────────────────┐
│ backtrack(index=0, sum=0)           │  ← Estoy en posición 0
│ Pregunta: ¿Incluyo set[0]=1?        │     (elemento 1)
│ Respuesta: NO                       │
│ subset=[], subset_size=0            │
└─────────────────────────────────────┘
           ↓ llama a backtrack(index=1, sum=0)

┌─────────────────────────────────────┐
│ backtrack(index=1, sum=0)           │  ← Estoy en posición 1
│ Pregunta: ¿Incluyo set[1]=2?        │     (elemento 2)
│ Respuesta: NO                       │
│ subset=[], subset_size=0            │  ← Sigue vacío
└─────────────────────────────────────┘
           ↓ llama a backtrack(index=2, sum=0)

┌─────────────────────────────────────┐
│ backtrack(index=2, sum=0)           │  ← Pasé todos los elementos
│ if (index == set_size) ✓            │     (index=2, set_size=2)
│   → Ya procesé todos los elementos  │
│   → No intento acceder a set[2]     │  ← NO EXISTE set[2]
│   → Verifico si sum==target         │
│   → sum=0 ≠ target=3 ❌              │
│   → RETORNA                         │
│ subset=[], subset_size=0            │
└─────────────────────────────────────┘
           ↓ RETORNA a backtrack(index=1)
           ↓ RETORNA a backtrack(index=0)


═══════════════════════════════════════════════════════════════
CAMINO 2: NO incluir 1, SÍ incluir 2
═══════════════════════════════════════════════════════════════

┌─────────────────────────────────────┐
│ backtrack(index=0, sum=0)           │  ← Estoy en posición 0
│ Pregunta: ¿Incluyo set[0]=1?        │     (elemento 1)
│ Respuesta: NO (ya explorado)       │
│ subset=[], subset_size=0            │
└─────────────────────────────────────┘
           ↓ llama a backtrack(index=1, sum=0)

┌──────────────────────────────────────┐
│ backtrack(index=1, sum=0)            │  ← Estoy en posición 1
│ Ya exploré "NO incluir 2"           │
│ Ahora: ¿Incluyo set[1]=2?           │     (elemento 2)
│ Respuesta: SÍ                        │
│ subset[0] = set[1] = 2               │  ← AGREGA el 2 al subset
│ subset=[2], subset_size=1            │
└──────────────────────────────────────┘
           ↓ llama a backtrack(index=2, sum=0+2=2)

┌─────────────────────────────────────┐
│ backtrack(index=2, sum=2)           │  ← Pasé todos los elementos
│ if (index == set_size) ✓            │     (index=2, set_size=2)
│   → Ya procesé todos los elementos  │
│   → Verifico si sum==target         │
│   → sum=2 ≠ target=3 ❌              │
│   → RETORNA                         │
│ subset=[2], subset_size=1           │
└─────────────────────────────────────┘
           ↓ RETORNA a backtrack(index=1)
           ↓ RETORNA a backtrack(index=0)


═══════════════════════════════════════════════════════════════
CAMINO 3: SÍ incluir 1, NO incluir 2
═══════════════════════════════════════════════════════════════

┌──────────────────────────────────────┐
│ backtrack(index=0, sum=0)            │  ← Estoy en posición 0
│ Ya exploré "NO incluir 1"           │
│ Ahora: ¿Incluyo set[0]=1?           │     (elemento 1)
│ Respuesta: SÍ                        │
│ subset[0] = set[0] = 1               │  ← AGREGA el 1 al subset
│ subset=[1], subset_size=1            │
└──────────────────────────────────────┘
           ↓ llama a backtrack(index=1, sum=0+1=1)

┌─────────────────────────────────────┐
│ backtrack(index=1, sum=1)           │  ← Estoy en posición 1
│ Pregunta: ¿Incluyo set[1]=2?        │     (elemento 2)
│ Respuesta: NO                       │
│ subset=[1], subset_size=1           │  ← Mantiene solo el 1
└─────────────────────────────────────┘
           ↓ llama a backtrack(index=2, sum=1)

┌─────────────────────────────────────┐
│ backtrack(index=2, sum=1)           │  ← Pasé todos los elementos
│ if (index == set_size) ✓            │     (index=2, set_size=2)
│   → Ya procesé todos los elementos  │
│   → Verifico si sum==target         │
│   → sum=1 ≠ target=3 ❌              │
│   → RETORNA                         │
│ subset=[1], subset_size=1           │
└─────────────────────────────────────┘
           ↓ RETORNA a backtrack(index=1)


═══════════════════════════════════════════════════════════════
CAMINO 4: SÍ incluir 1, SÍ incluir 2  ← SOLUCIÓN ✅
═══════════════════════════════════════════════════════════════

┌──────────────────────────────────────┐
│ backtrack(index=0, sum=0)            │  ← Estoy en posición 0
│ Ya exploré "NO incluir 1"           │
│ Ahora: ¿Incluyo set[0]=1?           │     (elemento 1)
│ Respuesta: SÍ (ya explorado)        │
│ subset[0] = set[0] = 1               │
│ subset=[1], subset_size=1            │
└──────────────────────────────────────┘
           ↓ llama a backtrack(index=1, sum=1)

┌──────────────────────────────────────┐
│ backtrack(index=1, sum=1)            │  ← Estoy en posición 1
│ Ya exploré "NO incluir 2"           │
│ Ahora: ¿Incluyo set[1]=2?           │     (elemento 2)
│ Respuesta: SÍ                        │
│ subset[1] = set[1] = 2               │  ← AGREGA el 2 al subset
│ subset=[1,2], subset_size=2          │
└──────────────────────────────────────┘
           ↓ llama a backtrack(index=2, sum=1+2=3)

┌─────────────────────────────────────┐
│ backtrack(index=2, sum=3)           │  ← Pasé todos los elementos
│ if (index == set_size) ✓            │     (index=2, set_size=2)
│   → Ya procesé todos los elementos  │
│   → Verifico si sum==target         │
│   → sum=3 == target=3 ✅             │
│   → IMPRIME "1 2"                   │  ← ¡SOLUCIÓN ENCONTRADA!
│   → found=1                         │
│   → RETORNA                         │
│ subset=[1,2], subset_size=2          │
└─────────────────────────────────────┘
           ↓ RETORNA a backtrack(index=1)
           ↓ RETORNA a backtrack(index=0)
           ↓ RETORNA a main()
```

---

## RESUMEN DE EJECUCIÓN

```
═══════════════════════════════════════════════════════════════
Total de llamadas a backtrack: 7

Caminos explorados:
  1. [] → sum=0 ❌
  2. [2] → sum=2 ❌
  3. [1] → sum=1 ❌
  4. [1,2] → sum=3 ✅ IMPRIME "1 2"

Salida del programa:
1 2
═══════════════════════════════════════════════════════════════
```

---

## Orden cronológico de ejecución

```
1️⃣ backtrack(i=0, sum=0) → NO incluir 1
   2️⃣ backtrack(i=1, sum=0) → NO incluir 2
      3️⃣ backtrack(i=2, sum=0) → ❌ RETORNA
   2️⃣ backtrack(i=1, sum=0) → SÍ incluir 2
      4️⃣ backtrack(i=2, sum=2) → ❌ RETORNA

1️⃣ backtrack(i=0, sum=0) → SÍ incluir 1
   5️⃣ backtrack(i=1, sum=1) → NO incluir 2
      6️⃣ backtrack(i=2, sum=1) → ❌ RETORNA
   5️⃣ backtrack(i=1, sum=1) → SÍ incluir 2
      7️⃣ backtrack(i=2, sum=3) → ✅ IMPRIME "1 2"
```

---

## Árbol de decisiones completo

```
                    []
                  (sum=0)
                 index=0
                /         \
          NO 1 /           \ SÍ 1
              /             \
            []               [1]
          (sum=0)          (sum=1)
          index=1          index=1
          /     \          /     \
    NO 2 /       \ SÍ 2  NO 2/     \ SÍ 2
        /         \        /         \
      []          [2]     [1]       [1,2]
    sum=0       sum=2    sum=1      sum=3
      ❌          ❌       ❌          ✅
```

---

## Conceptos clave

### 1. ¿Por qué index=2 si solo hay índices 0 y 1?

**`index` NO es solo un índice del array, es un contador de posiciones procesadas.**

```
set = [1, 2]
       ↑  ↑
    index=0 index=1

Cuando index=2:
- Ya procesamos set[0] y set[1]
- NO accedemos a set[2] (no existe)
- Solo EVALUAMOS si sum==target
```

### 2. Regla de la suma

```c
// DECISIÓN 1: NO incluir
backtrack(..., index+1, sum);  // ← sum NO CAMBIA

// DECISIÓN 2: SÍ incluir
subset[subset_size] = set[index];
backtrack(..., index+1, sum + set[index]);  // ← sum AUMENTA
```

**Cuando dices NO:** El número no entra en el subset, la suma no cambia
**Cuando dices SÍ:** El número entra en el subset, la suma aumenta

### 3. Orden de ejecución

El algoritmo **SIEMPRE**:
1. Explora primero TODO el camino "NO incluir"
2. Cuando termina ese camino, explora TODO el camino "SÍ incluir"
3. Retorna automáticamente cuando termina ambos caminos

### 4. Flujo del código

```c
void backtrack(..., int index, int current_sum, ...)
{
    // LÍNEA A: Caso base
    if (index == set_size)
    {
        if (current_sum == target)
            print_subset(subset, subset_size);
        return;
    }

    // LÍNEA B: PRIMER backtracking (NO incluir)
    backtrack(..., index + 1, current_sum, ...);
    // ↑ Explora TODA esta rama antes de continuar

    // LÍNEA C: Agregar elemento al subset
    subset[subset_size] = set[index];

    // LÍNEA D: SEGUNDO backtracking (SÍ incluir)
    backtrack(..., index + 1, current_sum + set[index], ...);
    // ↑ Solo se ejecuta después de que LÍNEA B termine
}
```

---

## Tabla de todas las llamadas

| Llamada | index | sum | subset | subset_size | ¿Qué hace? | Resultado |
|---------|-------|-----|--------|-------------|------------|-----------|
| 1 | 0 | 0 | [] | 0 | Procesa set[0]=1 | Llama #2 y #5 |
| 2 | 1 | 0 | [] | 0 | Procesa set[1]=2 (sin el 1) | Llama #3 y #4 |
| 3 | 2 | 0 | [] | 0 | Caso base | ❌ No imprime |
| 4 | 2 | 2 | [2] | 1 | Caso base | ❌ No imprime |
| 5 | 1 | 1 | [1] | 1 | Procesa set[1]=2 (con el 1) | Llama #6 y #7 |
| 6 | 2 | 1 | [1] | 1 | Caso base | ❌ No imprime |
| 7 | 2 | 3 | [1,2] | 2 | Caso base | ✅ **IMPRIME "1 2"** |

---

## Puntos importantes para recordar

1. **El algoritmo avanza siempre de izquierda a derecha**, nunca retrocede para sumar
2. **Cuando dice "NO incluir"**, no suma nada y sigue adelante
3. **Cuando dice "SÍ incluir"**, suma el elemento en ese momento
4. **La suma se acumula** mientras avanza
5. **Solo al llegar al final** (index == set_size) verifica si sum == target
6. **El retroceso es automático** por la recursión
7. **`index` puede ser mayor que los índices del array** porque indica "ya terminé de procesar todos"

---

## Analogía final

Imagina que vas por un buffet lineal con 2 platos: [1, 2]

**No retrocedes en el buffet**. Vas avanzando:
1. Plato 1: ¿Lo tomo? → Pruebo NO tomarlo (sigo adelante)
2. Plato 2: ¿Lo tomo? → Pruebo NO tomarlo (sigo adelante)
3. Llegué al final del buffet → Cuento calorías → 0 ≠ 3 ❌
4. Vuelvo mentalmente a Plato 2: ¿Y si SÍ lo tomo?
5. Tomo Plato 2 → Llegué al final → Cuento: 2 ≠ 3 ❌
6. Vuelvo mentalmente a Plato 1: ¿Y si SÍ lo tomo?
7. Tomo Plato 1 → Plato 2: ¿Lo tomo? → NO → Final → 1 ≠ 3 ❌
8. Plato 2: ¿Y si SÍ? → Tomo Plato 2 → Final → 1+2=3 ✅
