# GUIA GAME OF LIFE — Exam Rank 05

> Escrita para alguien que sabe C pero no ha visto el ejercicio nunca.
> Todo lo que necesitas para reproducirlo de memoria en el examen.

---

## 0. QUÉ ES EL EJERCICIO

El programa simula el **Juego de la Vida de Conway** en una cuadrícula de caracteres.

Recibes:
1. Un tablero de `width × height` celdas, inicialmente vacío (espacios).
2. Comandos por `stdin` que mueven un "bolígrafo" para pintar celdas vivas (`'0'`).
3. Un número de iteraciones para simular las reglas del juego.
4. Al final imprimes el tablero resultante.

⏺ El Juego de la Vida de Conway                                         
                                                                        
  La idea en una frase                                                  
                                                                        
  Imagina un tablero de ajedrez infinito. Cada casilla puede estar viva 
  (0) o muerta (espacio). Cada turno, todas las casillas cambian de     
  estado al mismo tiempo siguiendo 4 reglas simples.                    
                                                                        
  ---                                                                   
  Las 4 reglas                                                        
              
  Cada casilla mira a sus 8 vecinos (los que la rodean en todas las
  direcciones):                                                         
  
  [ ][ ][ ]                                                             
  [ ][X][ ]   ← X mira a sus 8 vecinos                      
  [ ][ ][ ]                                                             
                                                                      
  ┌─────────────┬───────────────┬────────────────────────┐              
  │ Estado de X │ Vecinos vivos │       Resultado        │  
  ├─────────────┼───────────────┼────────────────────────┤              
  │ Viva        │ < 2           │ Muere (soledad)        │            
  ├─────────────┼───────────────┼────────────────────────┤            
  │ Viva        │ 2 o 3         │ Sobrevive              │              
  ├─────────────┼───────────────┼────────────────────────┤
  │ Viva        │ > 3           │ Muere (superpoblación) │              
  ├─────────────┼───────────────┼────────────────────────┤              
  │ Muerta      │ exactamente 3 │ Nace                   │            
  └─────────────┴───────────────┴────────────────────────┘              
                                                            
  Nadie más cambia. Solo esas condiciones.                              
                                                                      
  ---                                                                   
  Lo fascinante: de reglas simples emergen patrones complejos
                                                                        
  Con solo esas 4 reglas aparecen patrones que se comportan de formas 
  inesperadas:                                                          
                                                            
  Blinker (oscilador): 3 células en fila → alternan entre horizontal y  
  vertical cada turno. Es exactamente el ejemplo del subject (dxss con 3
   iteraciones).                                                        
                                                                      
  Iter 0:   Iter 1:   Iter 2 (= 0):                                     
   0         000       0                                              
   0                   0                                                
   0                   0                                    
                                                                        
  Glider: un patrón de 5 células que "camina" por el tablero sin        
  destruirse.                                                         
                                                                        
  El juego lo inventó el matemático John Conway en 1970. Nadie esperaba 
  que 4 reglas tan simples pudieran generar comportamiento tan complejo
  — incluso se ha demostrado que el Juego de la Vida es Turing completo 
  (puedes construir un ordenador dentro de él).             
                                                                      
  ---                                                                 
  Qué hace tu implementación
                                                                        
  Tu código tiene 5 partes bien separadas:
                                                                        
  1. init_grid — crea el tablero vacío                      
                                                                        
  grid = calloc(data.height + 1, sizeof(char *));                     
  // Cada fila se llena de espacios ' '                                 
  Aloca una matriz de char**. Usa calloc para que empiece limpia.       
                                                                        
  2. parse_input — dibuja el estado inicial con el "lápiz"              
                                                                        
  Comandos:  w=arriba  s=abajo  a=izquierda  d=derecha  x=bajar/levantar
   lápiz                                                                
  El lápiz empieza en (0,0). Cuando está bajado (pen_down=1), cualquier
  movimiento pinta un '0' en la nueva posición. x alterna el estado del 
  lápiz y pinta la celda actual al bajar.                              
                                                                        
  3. count_neighbors — cuenta los 8 vecinos vivos de una celda        
                                                                        
  dy = -1; while (dy <= 1) {        // filas: arriba, misma, abajo    
      dx = -1; while (dx <= 1) {    // columnas: izq, misma, der        
          if (dy != 0 || dx != 0)   // excluye la celda misma           
              if (grid[ny][nx] == '0') count++;                         
  Las celdas fuera del tablero cuentan como muertas (el if (ny >= 0 &&  
  ...) las ignora).                                                     
                                                                      
  4. next_generation — aplica las 4 reglas                              
                                                                        
  if (grid[i][j] == '0')                          // celda viva:      
      next[i][j] = (n == 2 || n == 3) ? '0' : ' '; // sobrevive con 2 o 
  3 vecinos                                                             
  else                                             // celda muerta:     
      next[i][j] = (n == 3) ? '0' : ' ';           // nace solo con     
  exactamente 3                                                         
  Clave: escribe en next (tablero nuevo) para no contaminar el cálculo
  del resto de celdas de la misma generación. Es el error más común si  
  usas un solo tablero.                                               
                                                                        
  5. El bucle principal — swap de punteros                            
                                                                        
  while (k < data.iterations) {                                       
      next_generation(grid, next, data);                                
      tmp  = grid;   // ← truco: en lugar de copiar el tablero,
      grid = next;   //   solo intercambiamos los punteros              
      next = tmp;                                                       
      k++;                                                              
  }                                                                     
  Intercambiar punteros es O(1). Copiar el tablero entero sería O(width
  × height). Este truco es elegante y eficiente.                        
                                                                      
  ---                                                                   
  El flujo completo de tu programa                          
                                                                      
  ./life 5 5 3  (con stdin "sdxddssaaww")                             
        ↓                                                               
  init_grid → tablero 5×5 lleno de espacios                             
        ↓                                                               
  parse_input → dibuja el estado inicial leyendo comandos               
        ↓                                                               
  bucle 3 veces: next_generation + swap de punteros                   
        ↓                                                               
  print_grid → imprime el estado final                      
        ↓                                                               
  free_grid × 2                                                       
                                                                        
  ---                                                                 
  El único "truco difícil" del subject                                  
                                                            
  El comando x no solo baja/sube el lápiz — también pinta la celda    
  actual al bajar:                                                      
   
  else if (c == 'x') {                                                  
      data->pen_down = !data->pen_down;                     
      if (data->pen_down)                      // si acaba de bajar...  
          grid[data->pen_y][data->pen_x] = '0'; // pinta donde está     
  }                                                                     
                                                                        
  Sin eso, el primer x del subject (sdxddssaaww) no pintaría            
  correctamente.     

### Compilación y ejecución

```bash
gcc -o life *.c
echo 'sdxddssaaww' | ./life 5 5 0
```

**Funciones permitidas:** `atoi`, `read`, `putchar`, `malloc`, `calloc`, `realloc`, `free`

> NO puedes usar `printf`, `scanf`, `fprintf`, `fgets`, ni nada de `<stdio.h>` para output.
> Solo `putchar` para imprimir. Solo `read` para leer.

---

## 1. LOS ARCHIVOS QUE TIENES QUE CREAR

```
life.h    → struct t_data + prototipos de funciones
life.c    → toda la lógica
```

Solo son dos archivos. El `main` va en `life.c`.

---

## 2. LA ESTRUCTURA — `t_data`

Necesitas guardar el estado del programa. Crea un `struct` con:

```c
typedef struct s_data
{
    int width;       /* columnas del tablero */
    int height;      /* filas del tablero */
    int iterations;  /* veces que se aplican las reglas */
    int pen_x;       /* posición X actual del bolígrafo (columna) */
    int pen_y;       /* posición Y actual del bolígrafo (fila) */
    int pen_down;    /* 1 = bolígrafo bajado (pinta), 0 = levantado */
}   t_data;
```

### Por qué typedef

En C tienes que escribir `struct s_data var;` siempre.
Con `typedef` puedes escribir solo `t_data var;`. Es un alias.

### Por qué guardar pen_x y pen_y

El bolígrafo empieza en (0,0) = esquina superior izquierda.
Los comandos `w a s d` lo mueven. `x` lo baja/sube.

---

## 3. EL TABLERO — `char **grid`

El tablero es un array de strings (array de arrays de char):

```
grid[fila][columna]

grid[0] = "     "   ← fila 0
grid[1] = " 000 "   ← fila 1
grid[2] = " 0 0 "   ← fila 2
```

### Por qué `char **` y no un array fijo

El tamaño llega en tiempo de ejecución (argumentos del programa).
No puedes hacer `char grid[width][height]` con tamaño variable en C89/C90.
Necesitas `malloc`/`calloc`.

### La convención: `grid[y][x]`

> TRAMPA MENTAL: en arrays 2D, el primer índice es la FILA (Y), el segundo la COLUMNA (X).
> `grid[pen_y][pen_x]` — primero la fila, luego la columna.

---

## 4. CREAR EL TABLERO — `init_grid`

```c
char **init_grid(t_data data)
{
    int     i;
    int     j;
    char    **grid;

    grid = calloc(data.height + 1, sizeof(char *));
    if (!grid)
        return (NULL);
    i = 0;
    while (i < data.height)
    {
        grid[i] = calloc(data.width + 1, sizeof(char));
        if (!grid[i])
        {
            while (--i >= 0)
                free(grid[i]);
            free(grid);
            return (NULL);
        }
        j = 0;
        while (j < data.width)
            grid[i][j++] = ' ';
        grid[i][j] = '\0';
        i++;
    }
    grid[i] = NULL;
    return (grid);
}
```

#### Puntos clave:

- `calloc(n, size)` = `malloc(n * size)` + rellena con ceros. Más seguro.
- Reservamos `height + 1` punteros (el último será `NULL` — centinela).
- Cada fila tiene `width + 1` chars (el último será `'\0'` — fin de string).
- Iniciamos cada celda con `' '` (muerta).
- Si falla a mitad, **liberamos lo ya reservado** antes de retornar NULL.

---

## 5. LEER COMANDOS — `parse_input`

```c
void parse_input(char **grid, t_data *data)
{
    char c;

    while (read(0, &c, 1) > 0)
    {
        if (c == 'w' && data->pen_y > 0)
        {
            data->pen_y--;
            if (data->pen_down)
                grid[data->pen_y][data->pen_x] = '0';
        }
        else if (c == 's' && data->pen_y < data->height - 1)
        {
            data->pen_y++;
            if (data->pen_down)
                grid[data->pen_y][data->pen_x] = '0';
        }
        else if (c == 'a' && data->pen_x > 0)
        {
            data->pen_x--;
            if (data->pen_down)
                grid[data->pen_y][data->pen_x] = '0';
        }
        else if (c == 'd' && data->pen_x < data->width - 1)
        {
            data->pen_x++;
            if (data->pen_down)
                grid[data->pen_y][data->pen_x] = '0';
        }
        else if (c == 'x')
        {
            data->pen_down = !data->pen_down;
            if (data->pen_down)
                grid[data->pen_y][data->pen_x] = '0';
        }
    }
}
```

#### Las reglas del bolígrafo:

| Comando | Acción | Límite |
|---------|--------|--------|
| `w` | mueve arriba (pen_y--) | no sale por arriba (pen_y > 0) |
| `s` | mueve abajo (pen_y++) | no sale por abajo (pen_y < height-1) |
| `a` | mueve izquierda (pen_x--) | no sale por izquierda (pen_x > 0) |
| `d` | mueve derecha (pen_x++) | no sale por derecha (pen_x < width-1) |
| `x` | toggle pen_down | pinta la celda actual si baja |

#### Truco clave con `x`:

Cuando el bolígrafo **baja** (`x` y `pen_down` pasa a 1), la celda actual se pinta **en ese momento**.
Cuando se **mueve con el bolígrafo bajado**, la celda destino se pinta.
La celda de origen NO se borra.

#### Por qué `read(0, &c, 1)`

`0` es el file descriptor de `stdin`.
`&c` es la dirección donde guardar el byte leído.
`1` es cuántos bytes leer.
Retorna > 0 mientras haya datos, 0 en EOF.

---

## 6. LAS REGLAS DEL JUEGO DE LA VIDA

Para cada celda, cuenta sus vecinos vivos (las 8 celdas alrededor):

```
┌───┬───┬───┐
│ v │ v │ v │  ← vecinos de la celda central
├───┼───┼───┤
│ v │ C │ v │  C = celda actual, v = vecino
├───┼───┼───┤
│ v │ v │ v │
└───┴───┴───┘
```

**Reglas:**

| Estado actual | Vecinos vivos | Estado siguiente |
|---------------|---------------|------------------|
| Viva (`'0'`) | < 2 | Muere (soledad) |
| Viva (`'0'`) | 2 o 3 | Sobrevive |
| Viva (`'0'`) | > 3 | Muere (superpoblación) |
| Muerta (`' '`) | exactamente 3 | Nace |
| Muerta (`' '`) | cualquier otro | Sigue muerta |

---

## 7. CONTAR VECINOS — `count_neighbors`

```c
int count_neighbors(char **grid, int y, int x, t_data data)
{
    int count;
    int ny;
    int nx;
    int dy;
    int dx;

    count = 0;
    dy = -1;
    while (dy <= 1)
    {
        dx = -1;
        while (dx <= 1)
        {
            if (dy != 0 || dx != 0)   /* skip la celda central */
            {
                ny = y + dy;
                nx = x + dx;
                if (ny >= 0 && ny < data.height && nx >= 0 && nx < data.width)
                    if (grid[ny][nx] == '0')
                        count++;
            }
            dx++;
        }
        dy++;
    }
    return (count);
}
```

#### Puntos clave:

- Iteramos un cuadrado 3×3 centrado en `(y, x)`.
- Saltamos `(dy==0, dx==0)` porque esa es la celda misma.
- Comprobamos que `ny` y `nx` estén dentro del tablero (las celdas fuera cuentan como muertas).

---

## 8. SIMULAR UNA GENERACIÓN — `next_generation`

```c
void next_generation(char **grid, char **next, t_data data)
{
    int i;
    int j;
    int neighbors;

    i = 0;
    while (i < data.height)
    {
        j = 0;
        while (j < data.width)
        {
            neighbors = count_neighbors(grid, i, j, data);
            if (grid[i][j] == '0')
                next[i][j] = (neighbors == 2 || neighbors == 3) ? '0' : ' ';
            else
                next[i][j] = (neighbors == 3) ? '0' : ' ';
            j++;
        }
        i++;
    }
}
```

#### Por qué dos tableros (grid y next)

Si modificas el tablero mientras calculas, las celdas ya calculadas afectan a las siguientes.
Las reglas se aplican **todas a la vez** sobre el estado anterior.
Solución: calculas en `next` leyendo de `grid`, luego los intercambias.

#### El intercambio (swap de punteros)

```c
char **tmp = grid;
grid = next;
next = tmp;
```

No copias datos. Solo intercambias los punteros. Rápido y limpio.

---

## 9. IMPRIMIR EL TABLERO — `print_grid`

```c
void print_grid(char **grid, t_data data)
{
    int i;
    int j;

    i = 0;
    while (i < data.height)
    {
        j = 0;
        while (j < data.width)
            putchar(grid[i][j++]);
        putchar('\n');
        i++;
    }
}
```

Solo `putchar`. Ni `printf`, ni `puts`. El subject solo permite `putchar` para output.

---

## 10. LIBERAR MEMORIA — `free_grid`

```c
void free_grid(char **grid, t_data data)
{
    int i;

    i = 0;
    while (i < data.height)
        free(grid[i++]);
    free(grid);
}
```

Liberamos cada fila primero, luego el array de punteros.
El orden inverso al de creación: creamos el array → creamos filas; liberamos filas → liberamos el array.

---

## 11. EL MAIN — estructura completa

```c
int main(int ac, char *av[])
{
    t_data  data;
    char    **grid;
    char    **next;
    int     k;

    if (ac != 4)
        return (1);

    data.width      = atoi(av[1]);
    data.height     = atoi(av[2]);
    data.iterations = atoi(av[3]);
    data.pen_x      = 0;
    data.pen_y      = 0;
    data.pen_down   = 0;

    grid = init_grid(data);
    if (!grid)
        return (1);

    parse_input(grid, &data);

    next = init_grid(data);
    if (!next)
    {
        free_grid(grid, data);
        return (1);
    }

    k = 0;
    while (k < data.iterations)
    {
        char **tmp;
        next_generation(grid, next, data);
        tmp  = grid;
        grid = next;
        next = tmp;
        k++;
    }

    print_grid(grid, data);

    free_grid(grid, data);
    free_grid(next, data);
    return (0);
}
```

#### Orden obligatorio de operaciones:

1. Validar argumentos (`ac != 4` → return 1)
2. Leer datos → `data`
3. Crear `grid` (tablero inicial)
4. Procesar input → pintar el tablero
5. Crear `next` (tablero auxiliar para las iteraciones)
6. Bucle de iteraciones → `next_generation` + swap
7. Imprimir
8. Liberar ambos tableros

---

## 12. EL HEADER — `life.h`

```c
#ifndef LIFE_H
# define LIFE_H

typedef struct s_data
{
    int width;
    int height;
    int iterations;
    int pen_x;
    int pen_y;
    int pen_down;
}   t_data;

char    **init_grid(t_data data);
void    parse_input(char **grid, t_data *data);
int     count_neighbors(char **grid, int y, int x, t_data data);
void    next_generation(char **grid, char **next, t_data data);
void    print_grid(char **grid, t_data data);
void    free_grid(char **grid, t_data data);

#endif
```

---

## 13. ERRORES COMUNES EN EL EXAMEN

### Error 1: Usar printf en lugar de putchar
```c
// MAL — printf no está permitido
printf("%s\n", grid[i]);

// BIEN
while (j < data.width)
    putchar(grid[i][j++]);
putchar('\n');
```

### Error 2: Mezclar X e Y
```c
// MAL — confundes filas y columnas
grid[pen_x][pen_y] = '0';

// BIEN — primero fila (Y), luego columna (X)
grid[pen_y][pen_x] = '0';
```

### Error 3: No comprobar límites al mover el bolígrafo
```c
// MAL — el bolígrafo puede salir del tablero
data->pen_y--;
grid[data->pen_y][data->pen_x] = '0';

// BIEN — comprobar antes de mover
if (data->pen_y > 0)
{
    data->pen_y--;
    if (data->pen_down)
        grid[data->pen_y][data->pen_x] = '0';
}
```

### Error 4: Modificar el tablero mientras calculas la siguiente generación
```c
// MAL — modificas grid mientras lo lees
grid[i][j] = new_state;

// BIEN — escribe en next, lee de grid
next[i][j] = new_state;
```

### Error 5: No saltar la celda central en count_neighbors
```c
// MAL — la celda se cuenta a sí misma
for (dy = -1; dy <= 1; dy++)
    for (dx = -1; dx <= 1; dx++)
        if (grid[y+dy][x+dx] == '0') count++;

// BIEN
if (dy != 0 || dx != 0)
    if (grid[ny][nx] == '0') count++;
```

### Error 6: Olvidar que `x` pinta la celda actual al bajar
```c
// MAL — solo cambia el estado
data->pen_down = !data->pen_down;

// BIEN — si baja, pinta donde está
data->pen_down = !data->pen_down;
if (data->pen_down)
    grid[data->pen_y][data->pen_x] = '0';
```

### Error 7: Liberar solo un tablero
```c
// MAL
free_grid(grid, data);

// BIEN — siempre son dos tableros después del paso 5
free_grid(grid, data);
free_grid(next, data);
```

---

## 14. VERIFICACIÓN CON LOS EJEMPLOS DEL SUBJECT

```bash
gcc -o life life.c

# Ejemplo 1: 0 iteraciones (solo imprime lo dibujado)
echo 'sdxddssaaww' | ./life 5 5 0
#      
#  000 
#  0 0 
#  000 
#      

# Ejemplo 2: blinker — 1 iteración (vertical → horizontal)
echo 'dxss' | ./life 3 3 1
#    
# 000
#    

# Ejemplo 3: blinker — 2 iteraciones (vuelve a vertical)
echo 'dxss' | ./life 3 3 2
#  0 
#  0 
#  0 
```

Para verificar con `cat -e` (muestra el $ al final de cada línea):
```bash
echo 'sdxddssaaww' | ./life 5 5 0 | cat -e
#      $
#  000 $
#  0 0 $
#  000 $
#      $
```

---

## 15. ORDEN DE IMPLEMENTACIÓN EN EL EXAMEN

1. **Crea `life.h`** — struct + prototipos (5 min)
2. **Escribe `init_grid`** — alloc + rellena con espacios (5 min)
3. **Escribe `parse_input`** — lee con `read`, switch de comandos (10 min)
4. **Escribe `count_neighbors`** — doble bucle 3×3, skip centro, check bounds (5 min)
5. **Escribe `next_generation`** — aplica reglas leyendo grid, escribiendo next (5 min)
6. **Escribe `print_grid`** — `putchar` por celda + `\n` (2 min)
7. **Escribe `free_grid`** — libera filas, luego array (2 min)
8. **Escribe `main`** — une todo en orden (5 min)
9. **Compila y testa** con los 3 ejemplos del subject (5 min)

**Total estimado: ~45 minutos** si tienes el código memorizado.

---

## 16. EL TRUCO MENTAL PARA NO CONFUNDIRSE

Piensa en el tablero como una hoja de papel con coordenadas:

```
         columna 0   columna 1   columna 2
fila 0  [ grid[0][0] grid[0][1] grid[0][2] ]
fila 1  [ grid[1][0] grid[1][1] grid[1][2] ]
fila 2  [ grid[2][0] grid[2][1] grid[2][2] ]
```

- `w` (up) → fila disminuye → `pen_y--`
- `s` (down) → fila aumenta → `pen_y++`
- `a` (left) → columna disminuye → `pen_x--`
- `d` (right) → columna aumenta → `pen_x++`

Siempre: **`grid[Y][X]`** — fila primero, columna segundo.
