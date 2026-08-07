# GUIA BSQ — Exam Rank 05

> Para alguien que sabe C pero no conoce el ejercicio.
> Todo lo necesario para reproducirlo de memoria.

---

## 0. QUÉ ES EL EJERCICIO

Dado un mapa rectangular de caracteres, encontrar el **cuadrado más grande** que quepa sin tocar obstáculos. Reemplazar ese cuadrado con el carácter "lleno" e imprimir el mapa resultante.

```
Entrada (test2.map):       Salida:
5 . o x                    .....
.....                      ..o..
..o..                      ..xxx
.....                      .oxxx
.o...                      ..xxx
.....
```

El encabezado `5 . o x` dice:
- `5` → el mapa tiene 5 filas
- `.` → carácter vacío (libre)
- `o` → carácter obstáculo
- `x` → carácter con el que se rellena el cuadrado

---

## 1. LOS ARCHIVOS QUE TIENES QUE CREAR

```
bsq.h    → struct t_data + prototipos
bsq.c    → toda la lógica + main
```

Funciones permitidas: `malloc calloc realloc free fopen fclose getline fscanf fputs fprintf stderr stdout stdin errno`

> NO tienes `printf` ni `putchar`. Para imprimir usa `fputs(str, stdout)` o `fprintf(stdout, "%c", c)`.

---

## 2. LA ESTRUCTURA — `t_data`

```c
typedef struct s_data
{
    int     rows;       /* número de filas del mapa */
    char    empty;      /* carácter libre (ej: '.') */
    char    obstacle;   /* carácter obstáculo (ej: 'o') */
    char    full;       /* carácter relleno (ej: 'x') */
    int     width;      /* ancho de cada fila (calculado al leer) */
}   t_data;
```

---

## 3. EL ALGORITMO CLAVE — PROGRAMACIÓN DINÁMICA (DP)

Esta es la parte más difícil. Merece toda tu atención.

### El problema

Necesitamos encontrar el cuadrado más grande de celdas libres.
Fuerza bruta: probar todos los cuadrados posibles = muy lento.
DP: lo resuelve en una sola pasada.

### La idea

Creamos una tabla `dp[filas][columnas]` de enteros.

Para cada celda `(i, j)`:
- Si es **obstáculo**: `dp[i][j] = 0`
- Si está en la primera fila o primera columna: `dp[i][j] = 1` (solo puede haber un cuadrado 1×1)
- En cualquier otro caso (celda libre):

```
dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1
              ↑ arriba       ↑ izquierda   ↑ diagonal
```

`dp[i][j]` significa: **el lado del cuadrado más grande cuya esquina inferior-derecha es (i, j)**.

### Ejemplo visual (mapa 3×3 todo libre)

```
Mapa:          dp resultante:
. . .          1  1  1
. . .          1  2  2
. . .          1  2  3
```

`dp[2][2] = 3` → hay un cuadrado 3×3 con esquina inferior-derecha en (2,2).

### Ejemplo con obstáculos

```
Mapa:          dp:
. . . . .      1 1 1 1 1
. . o . .      1 2 0 1 2
. . . . .      1 2 0 1 2   ← el obstáculo "rompe" la cadena
. o . . .      1 0 0 1 2
. . . . .      1 0 1 2 3   ← máximo: 3 en (4,4)
```

Cuadrado 3×3 con esquina inferior-derecha en (4,4) → cubre filas [2..4], columnas [2..4]. ¡Coincide con el output esperado!

### Por qué funciona

Para que quepa un cuadrado k×k con esquina inferior-derecha en (i,j), necesitas:
- Un cuadrado (k-1)×(k-1) en (i-1, j) → que venga de arriba
- Un cuadrado (k-1)×(k-1) en (i, j-1) → que venga de la izquierda
- Un cuadrado (k-1)×(k-1) en (i-1, j-1) → que venga de la diagonal

Los tres deben caber. Por eso usas el **mínimo** de los tres.

```
┌─────────┐
│ diag ←  │ arriba
│         │
│  ← izq  │   ← dp[i][j]
└─────────┘
```

---

## 4. DESEMPATE: MÁS ARRIBA, LUEGO MÁS A LA IZQUIERDA

Si hay varios cuadrados del mismo tamaño máximo, el subject pide el más cercano a la parte superior, luego el más a la izquierda.

**Regla para el código**: actualiza el mejor solo con `>` estricto (no `>=`).

```c
if (dp[i][j] > best_size)   // ESTRICTO — no >=
{
    best_size = dp[i][j];
    best_y = i;
    best_x = j;
}
```

Con `>` estricto y recorrido de arriba a abajo, izquierda a derecha, el primer cuadrado máximo que encuentras es siempre el más arriba y luego más a la izquierda.

---

## 5. MARCAR EL CUADRADO EN EL MAPA

Una vez tienes `best_size`, `best_y` (fila esquina inferior-derecha), `best_x` (columna esquina inferior-derecha):

```
esquina superior-izquierda: fila = best_y - best_size + 1
                            col  = best_x - best_size + 1
esquina inferior-derecha:   fila = best_y
                            col  = best_x
```

```c
i = best_y - best_size + 1;
while (i <= best_y)
{
    j = best_x - best_size + 1;
    while (j <= best_x)
        map[i][j++] = data.full;
    i++;
}
```

Si `best_size == 0` (todos obstáculos): `i = 0 - 0 + 1 = 1 > 0 = best_y` → el bucle no se ejecuta. Safe.

---

## 6. VALIDACIÓN DEL MAPA — REGLAS

El mapa es inválido si:

| Condición | Error |
|-----------|-------|
| No se pueden leer los 4 campos del encabezado | map error |
| `rows < 1` (cero líneas) | map error |
| Dos caracteres del encabezado son iguales (e.g. `3 . . x`) | map error |
| Alguna fila tiene distinto ancho que las demás | map error |
| Una fila contiene un carácter que no es empty ni obstacle | map error |
| El número de filas leídas no coincide con el declarado | map error |
| Ancho de fila = 0 (línea vacía) | map error |

Si hay error → `fputs("map error\n", stderr)` y continuar con el siguiente mapa.

---

## 7. LEER EL MAPA — `read_map`

```c
char **read_map(FILE *stream, t_data *data)
{
    char    *line = NULL;
    size_t  n = 0;
    char    **map;
    int     rows_read = 0;
    int     w;
    int     j;

    /* Leer encabezado completo con getline para no dejar bytes sueltos */
    if (getline(&line, &n, stream) == -1)
        goto fail;
    if (sscanf(line, "%d %c %c %c",
        &data->rows, &data->empty, &data->obstacle, &data->full) != 4
        || data->rows < 1
        || data->empty == data->obstacle
        || data->empty == data->full
        || data->obstacle == data->full)
        goto fail;

    map = malloc(sizeof(char *) * data->rows);
    if (!map)
        goto fail;

    data->width = -1;
    while (rows_read < data->rows)
    {
        if (getline(&line, &n, stream) == -1)
            goto fail_map;

        /* calcular ancho (sin el '\n') */
        w = 0;
        while (line[w] && line[w] != '\n')
            w++;

        /* primer fila fija el ancho */
        if (data->width == -1)
            data->width = w;
        if (data->width == 0 || w != data->width)
            goto fail_map;

        /* validar caracteres */
        j = 0;
        while (j < w)
        {
            if (line[j] != data->empty && line[j] != data->obstacle)
                goto fail_map;
            j++;
        }

        /* guardar fila */
        map[rows_read] = malloc(w + 1);
        if (!map[rows_read])
            goto fail_map;
        j = 0;
        while (j < w)
        {
            map[rows_read][j] = line[j];
            j++;
        }
        map[rows_read][w] = '\0';
        rows_read++;
    }
    free(line);
    return map;

fail_map:
    while (rows_read-- > 0)
        free(map[rows_read]);
    free(map);
fail:
    free(line);
    return NULL;
}
```

### Por qué `getline` para el encabezado y no `fscanf`

`fscanf` deja el `'\n'` en el buffer. Tienes que hacer un `getline` extra para consumirlo. Con `getline` + `sscanf` lees la línea entera y la parseas, sin sorpresas.

### `goto` en C — ¿es válido?

Sí. En C es el patrón estándar para gestión de errores cuando tienes múltiples recursos que liberar. Es limpio si los labels están al final de la función.

---

## 8. RESOLVER Y MARCAR — `solve_and_mark`

```c
void solve_and_mark(char **map, t_data data)
{
    int **dp;
    int rows_alloc = 0;
    int i;
    int j;
    int best_size = 0;
    int best_y = 0;
    int best_x = 0;

    dp = malloc(sizeof(int *) * data.rows);
    if (!dp)
        return;
    while (rows_alloc < data.rows)
    {
        dp[rows_alloc] = malloc(sizeof(int) * data.width);
        if (!dp[rows_alloc])
        {
            while (rows_alloc-- > 0)
                free(dp[rows_alloc]);
            free(dp);
            return;
        }
        rows_alloc++;
    }

    i = 0;
    while (i < data.rows)
    {
        j = 0;
        while (j < data.width)
        {
            if (map[i][j] == data.obstacle)
                dp[i][j] = 0;
            else if (i == 0 || j == 0)
                dp[i][j] = 1;
            else
                dp[i][j] = min3(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1;
            if (dp[i][j] > best_size)
            {
                best_size = dp[i][j];
                best_y    = i;
                best_x    = j;
            }
            j++;
        }
        i++;
    }

    i = 0;
    while (i < data.rows)
        free(dp[i++]);
    free(dp);

    /* marcar cuadrado */
    i = best_y - best_size + 1;
    while (i <= best_y)
    {
        j = best_x - best_size + 1;
        while (j <= best_x)
            map[i][j++] = data.full;
        i++;
    }
}
```

---

## 9. IMPRIMIR — `print_map`

```c
void print_map(char **map, t_data data)
{
    int i = 0;
    while (i < data.rows)
    {
        fputs(map[i], stdout);
        fputs("\n", stdout);
        i++;
    }
}
```

Recordatorio: `fputs` imprime el string **sin** añadir `'\n'`. Hay que añadirlo a mano.

---

## 10. LIBERAR MEMORIA — `free_map`

```c
void free_map(char **map, int rows)
{
    int i = 0;
    while (i < rows)
        free(map[i++]);
    free(map);
}
```

---

## 11. ORQUESTADOR — `process_map`

```c
void process_map(FILE *stream)
{
    t_data  data;
    char    **map;

    data.rows = 0;
    data.width = 0;
    data.empty = 0;
    data.obstacle = 0;
    data.full = 0;

    map = read_map(stream, &data);
    if (!map)
    {
        fputs("map error\n", stderr);
        return;
    }
    solve_and_mark(map, data);
    print_map(map, data);
    free_map(map, data.rows);
}
```

---

## 12. MAIN

```c
int main(int ac, char *av[])
{
    int     i;
    FILE    *f;

    if (ac == 1)
    {
        process_map(stdin);
        return (0);
    }
    i = 1;
    while (i < ac)
    {
        f = fopen(av[i], "r");
        if (!f)
            fputs("map error\n", stderr);
        else
        {
            process_map(f);
            fclose(f);
        }
        if (i + 1 < ac)
            fputs("\n", stdout);
        i++;
    }
    return (0);
}
```

El `fputs("\n", stdout)` entre mapas es el salto de línea que pide el subject cuando hay múltiples argumentos.

---

## 13. EL HEADER — `bsq.h`

```c
#ifndef BSQ_H
# define BSQ_H

# include <stdio.h>
# include <stdlib.h>

typedef struct s_data
{
    int     rows;
    char    empty;
    char    obstacle;
    char    full;
    int     width;
}   t_data;

char    **read_map(FILE *stream, t_data *data);
void    solve_and_mark(char **map, t_data data);
void    print_map(char **map, t_data data);
void    free_map(char **map, int rows);
void    process_map(FILE *stream);
int     min3(int a, int b, int c);

#endif
```

---

## 14. ERRORES COMUNES EN EL EXAMEN

### Error 1: Usar printf
```c
// MAL — no está en la lista de funciones permitidas
printf("%s\n", map[i]);

// BIEN
fputs(map[i], stdout);
fputs("\n", stdout);
```

### Error 2: Usar fscanf para el encabezado y olvidar el \n sobrante
```c
// MAL — deja el '\n' en el buffer, getline siguiente lee línea vacía
fscanf(stream, "%d %c %c %c", ...);
getline(...);  // esto solo consume el '\n' del encabezado, no el mapa

// BIEN — leer línea completa y parsearla
getline(&line, &n, stream);
sscanf(line, "%d %c %c %c", ...);
```

### Error 3: Usar >= en lugar de > para el desempate
```c
// MAL — te quedas con el último cuadrado de ese tamaño, no el primero
if (dp[i][j] >= best_size) { best_size = dp[i][j]; ... }

// BIEN — te quedas con el primero (más arriba y a la izquierda)
if (dp[i][j] > best_size) { best_size = dp[i][j]; ... }
```

### Error 4: Confundir filas y columnas en dp
```c
// dp[FILA][COLUMNA] — fila primero, columna segundo
dp[i][j] = min3(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1;
//              ↑ arriba     ↑ izquierda  ↑ diagonal arriba-izquierda
```

### Error 5: Liberar dp antes de marcar el mapa
```c
// MAL
free_dp(dp, data.rows);
// ... usar dp para marcar  ← uso after free

// BIEN
// calcular best_y, best_x, best_size
// LUEGO liberar dp
// LUEGO marcar map[][] con data.full
```

### Error 6: No validar caracteres del mapa
```c
// Cada carácter debe ser empty o obstacle, nada más
if (line[j] != data.empty && line[j] != data.obstacle)
    goto fail_map;  // carácter inválido → map error
```

### Error 7: No detectar ancho inconsistente entre filas
```c
// Todas las filas deben tener el mismo ancho
if (w != data->width)
    goto fail_map;
```

---

## 15. VERIFICACIÓN CON LOS TESTS DEL SUBJECT

```bash
gcc -Wall -o bsq bsq.c

# Test 1: cuadrado 3x3 simple
./bsq tests/test1.map
# xxx
# xxx
# xxx

# Test 2: cuadrado 3x3 con obstáculos
./bsq tests/test2.map
# .....
# ..o..
# ..xxx
# .oxxx
# ..xxx

# Test 3: mapa 1x1
./bsq tests/test3.map
# x

# Validaciones de error (deben imprimir "map error" en stderr)
./bsq tests/t_dup_header 2>&1       # header con chars duplicados
./bsq tests/t_bad_char 2>&1         # char inválido en el mapa
./bsq tests/t_wrong_width 2>&1      # filas de distinto ancho
./bsq tests/t_too_few 2>&1          # menos filas que las declaradas
./bsq tests/t_too_many 2>&1         # más filas que las declaradas
./bsq tests/t_zero_lines 2>&1       # 0 líneas declaradas
```

---

## 16. ORDEN DE IMPLEMENTACIÓN EN EL EXAMEN

1. **`bsq.h`** — struct + includes + prototipos (3 min)
2. **`min3`** — 5 líneas (1 min)
3. **`free_map`** — libera el mapa (2 min)
4. **`read_map`** — lee y valida el mapa (15 min) ← más tiempo aquí
5. **`solve_and_mark`** — DP + marcar (10 min) ← segundo más difícil
6. **`print_map`** — `fputs` por fila (2 min)
7. **`process_map`** — une todo (3 min)
8. **`main`** — args o stdin (3 min)
9. **Compilar y testear** con los casos del subject (5 min)

**Total estimado: ~45 minutos**

---

## 17. EL TRUCO MENTAL PARA EL DP

Memoriza esto:

```
Celda vacía, no en borde:
dp[i][j] = min( arriba, izquierda, diagonal ) + 1

Celda vacía, en borde (fila 0 o columna 0):
dp[i][j] = 1

Obstáculo:
dp[i][j] = 0
```

Y para el marcado:

```
top-left:  (best_y - best_size + 1,  best_x - best_size + 1)
bot-right: (best_y,                  best_x)
```

El `+1` viene de que el índice empieza en 0: si el cuadrado tiene lado `s` y la esquina inferior-derecha está en fila `r`, la esquina superior-izquierda está en fila `r - s + 1`.

---

## 18. EJEMPLO PASO A PASO CON TEST5

```
Mapa (test5):   Tabla dp:
..ooo           1 1 0 0 0
..ooo           1 2 0 0 0   → best=2 en (1,1) 
ooooo           0 0 0 0 0
oo...           0 0 1 1 1
oo...           0 0 1 2 2   → dp[4][3]=min(dp[3][3]=1,dp[4][2]=1,dp[3][2]=1)+1=2
                             → dp[4][4]=min(dp[3][4]=1,dp[4][3]=2,dp[3][3]=1)+1=2
                             → NO supera best=2 (tenemos >=, usamos > → no se actualiza)
```

Resultado: best_size=2, best_y=1, best_x=1.
Marca filas [0..1], cols [0..1]:

```
Salida:
xxooo
xxooo
ooooo
oo...
oo...
```

✓ Coincide con `test5.out`.
