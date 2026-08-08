# GUIA BSQ_1MAP — variante "un solo mapa" (Exam Rank 05)

> Esta guía asume que ya conoces `rendu5/bsq/GUIA.md` (la variante multi-mapa).
> El algoritmo DP (`solve_and_mark`) y la validación del mapa (`read_map`)
> son **exactamente los mismos** en las dos variantes — esto es una copia
> **verificada** de ese código, no una implementación distinta. Esta guía
> solo cubre lo que **cambia** entre las dos.

## 0. Cómo distinguir las dos variantes en el examen

Mira la lista **"Allowed functions and globals"** del subject:

| | `bsq` (multi-mapa) | `bsq_1map` (esta) |
|---|---|---|
| `stderr` en la lista | ✅ sí | ❌ **no** |
| "Program will..." | recibe varios mapas por argv | **"recieve one map as an argument"** |
| Mensaje de error | `"map error"` | `"Error: invalid map"` |
| Canal del error | **stderr** | **stdout** |
| Separador entre mapas | línea en blanco entre soluciones | no aplica (un solo mapa) |

Si `stderr` NO aparece en "Allowed functions and globals", **es esta
variante** — y usar `fprintf(stderr, ...)` en el examen sería una
violación de la lista de funciones permitidas, no solo un detalle
cosmético. Revisa la lista ANTES de escribir una sola línea.

## 1. Lo que NO cambia (reutilizado tal cual de `rendu5/bsq/`)

- `min3` — mínimo de 3 enteros.
- `read_map` — parseo de cabecera + filas, todas las reglas de validación.
- `solve_and_mark` — el DP: `dp[i][j] = min(arriba, izquierda,
  diagonal) + 1`, desempate top-left con `>` estricto.
- `print_map` / `free_map`.

Si tienes dudas de POR QUÉ funciona el DP, la explicación completa con
dibujos está en `rendu5/bsq/GUIA.md`, secciones 3 y 4.

## 2. Lo que SÍ cambia

### `process_map` — el mensaje de error

```c
map = read_map(stream, &data);
if (!map)
{
    fputs("Error: invalid map\n", stdout);   /* NO stderr, NO "map error" */
    return ;
}
```

El subject fija el texto EXACTO para este caso: `"Error: invalid map"`.
Para "cualquier otro error" (fichero no abre, demasiados argumentos...)
solo exige el prefijo `"Error "` + un mensaje — el texto no está fijado,
así que puedes elegir el que quieras mientras empiece por `"Error"` y
vaya a stdout.

### `main` — un solo argumento, sin bucle

```c
if (ac == 1)
{
    process_map(stdin);
    return (0);
}
/* ya NO hay "while (i < ac) { ... }" recorriendo varios ficheros */
f = fopen(av[1], "r");
if (!f)
{
    fputs("Error: cannot open file\n", stdout);
    return (1);
}
process_map(f);
fclose(f);
return (0);
```

No hay `fputs("\n", stdout)` entre mapas (esa lógica solo existía para
separar varias soluciones en la variante multi-mapa) — aquí nunca hay
"el siguiente mapa".

### El header sin espacios ya funciona sin tocar nada

El subject de esta variante quita la coletilla "(space separated)" del
enunciado y su propio ejemplo usa `9.ox` en vez de `9 . o x`. **No hace
falta cambiar el `sscanf`**: el formato `"%d %c %c %c"` ya tiene un
espacio delante de cada `%c`, y en la familia `scanf` un espacio en el
formato significa "salta CERO o más espacios en blanco" — así que
`sscanf("9.ox", "%d %c %c %c", ...)` funciona exactamente igual que con
espacios. Verificado con el edge test `test1 (cabecera sin espacios)`.

## 3. Orden para escribirlo en el examen

1. `bsq.h` — mismo struct `t_data` que la variante multi-mapa (2 min)
2. `min3` + `free_map` (2 min)
3. `read_map` (10 min) — igual que siempre
4. `solve_and_mark` (10 min) — el DP, igual que siempre
5. `print_map` (2 min)
6. `process_map` — **usa `fputs(..., stdout)`, nunca stderr** (2 min)
7. `main` — **un solo `av[1]`, sin bucle sobre `ac`** (3 min)

## 4. Checklist antes de entregar

- [ ] `grep stderr *.c *.h` no devuelve nada fuera de comentarios
- [ ] mapa inválido → exactamente `Error: invalid map\n` en **stdout**
- [ ] mapa válido → se sigue imprimiendo en stdout, sin cambios
- [ ] sin argumentos → lee stdin (igual que siempre)
- [ ] un argumento inexistente → algo que empiece por `"Error"`, sin
      crashear, sin tocar stderr
