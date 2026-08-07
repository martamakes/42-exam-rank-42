# Guía polyset — desde cero en C++

> Sabes programar. Sabes lo que es una variable, una función, un puntero.
> Lo que no sabes es C++. Esta guía te lleva de la mano por cada concepto
> nuevo **justo cuando aparece**, con ejemplos en C para comparar.

---

## 0. El mapa completo antes de empezar

Hay 11 ficheros en total. 7 te los **dan**. Tú escribes **6**.

```
LO QUE TE DAN (no tocar)          LO QUE ESCRIBES TÚ
─────────────────────────          ──────────────────
subject/bag.hpp                    searchable_array_bag.hpp
subject/searchable_bag.hpp         searchable_array_bag.cpp
subject/array_bag.hpp              searchable_tree_bag.hpp
subject/array_bag.cpp              searchable_tree_bag.cpp
subject/tree_bag.hpp               set.hpp
subject/tree_bag.cpp               set.cpp
subject/main.cpp
```

---

## 1. Concepto nuevo: ¿qué es una clase?

Una **clase** en C++ es como una `struct` de C, pero puede tener también
**funciones dentro**. Esas funciones se llaman **métodos**.

```c
// C — struct + funciones separadas
typedef struct { int *data; int size; } t_bag;
void   bag_insert(t_bag *b, int v);
void   bag_print(t_bag *b);
```

```cpp
// C++ — todo junto dentro de la clase
class array_bag {
    int *data;
    int  size;
public:
    void insert(int v);
    void print() const;
};
```

La diferencia clave: en C++ cada función "pertenece" a la clase y tiene
acceso directo a sus datos sin que le pases un puntero.

---

## 2. Concepto nuevo: `public` / `protected` / `private`

Controlan desde dónde se puede acceder a cada cosa.

| Palabra     | Quién puede acceder                            |
|-------------|------------------------------------------------|
| `public`    | Cualquiera desde fuera de la clase             |
| `protected` | Solo la clase y sus hijos (herencia)           |
| `private`   | Solo la propia clase                           |

En `array_bag.hpp` ves:
```cpp
class array_bag : virtual public bag {
protected:        // ← tus hijos podrán usar esto
    int *data;    // el array de enteros
    int  size;    // cuántos hay
public:           // ← cualquiera puede llamar a esto
    void insert(int);
    void print() const;
    ...
};
```

`data` y `size` son `protected` porque **tú los necesitas** en tu
`searchable_array_bag` cuando implementas `has()`.

---

## 3. Concepto nuevo: clase abstracta y `= 0`

Una clase abstracta es una clase que **declara funciones pero no las
implementa**. Obliga a que alguien más las implemente.

```cpp
// bag.hpp — clase abstracta pura
class bag {
public:
    virtual void insert(int) = 0;   // = 0 → "debes implementarla tú"
    virtual void print() const = 0;
    virtual void clear() = 0;
};
```

El `= 0` significa: **esta función existe pero no tiene cuerpo aquí**.
Es un contrato: "cualquier clase que herede de mí DEBE implementar esto".

Equivalente en C: sería como una struct con punteros a función que hay
que rellenar obligatoriamente.

No puedes hacer `new bag()` directamente — da error de compilación.
Solo puedes hacer `new array_bag()` o `new searchable_array_bag()`, que
sí implementan todo.

---

## 4. Concepto nuevo: herencia (`:`  y `public`)

La herencia dice "esta clase es una versión extendida de otra".

```cpp
class array_bag : virtual public bag { ... };
//                         ↑
//                 "array_bag hereda de bag"
```

En C lo harías poniendo la struct padre como primer campo:
```c
typedef struct { t_bag base; int *data; int size; } t_array_bag;
```

En C++ la herencia lo hace automáticamente. `array_bag` tiene todo lo
de `bag` más sus propios campos.

La palabra `virtual` antes de `public bag` es para evitar el **problema
del diamante** — lo explico en el punto 6.

---

## 5. Los 7 ficheros que te dan — qué hace cada uno

### `bag.hpp` — el contrato base

```
bag
├── insert(int)          → meter un número
├── insert(int*, int)    → meter un array de números
├── print()              → imprimir el contenido
└── clear()              → vaciarlo todo
```

Todo es `= 0`. Es solo un contrato. Nadie crea un `bag` directamente.

---

### `searchable_bag.hpp` — contrato de búsqueda

```cpp
class searchable_bag : virtual public bag {
public:
    virtual bool has(int) const = 0;   // ← añade solo esto
};
```

Hereda de `bag` y añade **una sola función**: `has(int)`.
También es abstracta — tampoco se instancia directamente.

---

### `array_bag.hpp` + `array_bag.cpp` — bolsa con array

Implementa `bag` usando un **array dinámico** (`int *data`).

- `insert(int v)` → hace `realloc` y añade `v` al final
- `print()` → imprime en orden de inserción: `5 3 8 1`
- `clear()` → libera el array
- Los datos están en orden de inserción, sin ordenar

**No implementa `has()`** — eso es lo que tú añades.

---

### `tree_bag.hpp` + `tree_bag.cpp` — bolsa con árbol BST

Implementa `bag` usando un **árbol binario de búsqueda**.

```
     5
    / \
   3   8
  /
 1
```

- `insert(int v)` → inserta en el lugar correcto del BST
- `print()` → recorre in-order → imprime ordenado: `1 3 5 8`
- `clear()` → destruye todos los nodos

Tienes acceso a `this->tree` (el nodo raíz) porque es `protected`.

**No implementa `has()`** — eso es lo que tú añades.

---

### `main.cpp` — el examinador

Lo que hace el main (no lo tocas, pero debes entenderlo):

```cpp
searchable_bag *t = new searchable_tree_bag;   // árbol
searchable_bag *a = new searchable_array_bag;  // array

// 1. Inserta todos los argumentos en ambos
// 2. Imprime ambos
// 3. Para cada argumento: has(arg) y has(arg-1)
// 4. Limpia ambos
// 5. Crea dos sets que comparten el mismo bag
// 6. Inserta en los sets (sin duplicados)
// 7. Imprime, testea get_bag(), limpia
```

---

## 6. Concepto nuevo: herencia múltiple y el diamante

Tu `searchable_array_bag` hereda de **dos clases a la vez**:

```
         bag
        /   \
array_bag  searchable_bag
        \   /
  searchable_array_bag
```

Esto se llama el **diamante** porque el diagrama tiene esa forma.
El problema: sin `virtual`, `searchable_array_bag` tendría **dos copias
de `bag`** dentro — una de `array_bag` y otra de `searchable_bag`.
Eso causa ambigüedad al compilar.

La solución: `virtual public bag` en ambos padres — le dice al
compilador "compartid una sola copia de `bag`".

```cpp
class array_bag      : virtual public bag { ... };  // ← virtual
class searchable_bag : virtual public bag { ... };  // ← virtual
```

Tú no tocas esas líneas — ya están escritas en los ficheros del subject.
Lo que necesitas saber: **existe, funciona, no rompas el `virtual`**.

---

## 7. Concepto nuevo: forma canónica ortodoxa

El subject dice "all your classes should be under orthodox canonical form".

Esto significa que **toda clase debe tener exactamente estos 4 métodos**:

| Método | Para qué sirve |
|--------|----------------|
| Constructor por defecto | Crear un objeto vacío |
| Constructor de copia | Crear un objeto copiando otro |
| Operador de asignación `=` | Copiar un objeto en otro ya existente |
| Destructor | Limpiar cuando el objeto muere |

```cpp
class mi_clase {
public:
    mi_clase();                              // constructor por defecto
    mi_clase(const mi_clase& src);           // constructor de copia
    mi_clase& operator=(const mi_clase& src); // operador =
    ~mi_clase();                             // destructor
};
```

En C harías lo mismo con funciones:
```c
t_cosa  *cosa_new(void);
t_cosa  *cosa_copy(const t_cosa *src);
void     cosa_assign(t_cosa *dst, const t_cosa *src);
void     cosa_destroy(t_cosa *self);
```

---

## 8. Concepto nuevo: `this`

Dentro de un método, `this` es un puntero al objeto actual.
Es exactamente como el `self` o `self` que pasarías en C:

```c
// C
void bag_insert(t_bag *self, int v) { self->size++; }

// C++ — self se llama "this" y es implícito
void array_bag::insert(int v) { this->size++; }
//                              ↑ equivalente a self->size++
```

---

## 9. Concepto nuevo: `::` (operador de scope)

Cuando defines una función fuera del `.hpp`, debes decirle al compilador
a qué clase pertenece:

```cpp
// INCORRECTO — el compilador no sabe de qué clase es
bool has(int value) const { ... }

// CORRECTO — "esta función pertenece a searchable_array_bag"
bool searchable_array_bag::has(int value) const { ... }
```

Es como si en C todos los nombres de función llevaran el prefijo del
módulo, pero en C++ el compilador lo exige formalmente.

---

## 10. Concepto nuevo: `const` al final de un método

```cpp
bool has(int value) const;
//                  ↑ esto
```

Significa "este método NO modifica el objeto". El compilador te impide
modificar `this->data`, `this->size`, etc. dentro de él.

¿Por qué importa? Porque en el main hay esto:
```cpp
const searchable_array_bag tmp(...);
tmp.has(1);  // solo compila si has() es const
```

Si `tmp` es constante, solo puedes llamar métodos `const` sobre él.

---

## 11. Tu fichero 1: `searchable_array_bag.hpp`

```cpp
#pragma once
#include "array_bag.hpp"
#include "searchable_bag.hpp"

class searchable_array_bag : public array_bag, public searchable_bag
{
public:
    searchable_array_bag();
    searchable_array_bag(const searchable_array_bag& src);
    searchable_array_bag& operator=(const searchable_array_bag& src);
    ~searchable_array_bag();
    bool has(int value) const;
};
```

**Qué dice cada línea:**

`#pragma once` → "incluye este fichero solo una vez aunque se referencie varias veces". Alternativa moderna al `#ifndef / #define / #endif`.

`class searchable_array_bag : public array_bag, public searchable_bag`
→ herencia múltiple. Tiene todo lo de `array_bag` (datos + insert/print/clear) y también implementa la interfaz `searchable_bag` (tiene `has()`).

Los 4 métodos de la forma canónica + `has()` que es la única función nueva.

---

## 12. Tu fichero 2: `searchable_array_bag.cpp`

```cpp
#include "searchable_array_bag.hpp"

// Constructor por defecto
// array_bag() se llama automáticamente antes — inicializa data=0, size=0
searchable_array_bag::searchable_array_bag() {}

// Constructor de copia
// ": array_bag(src)" → llama al constructor de copia del padre
// array_bag sabe copiar su data[] y size — tú no tienes que repetirlo
searchable_array_bag::searchable_array_bag(const searchable_array_bag& src)
    : array_bag(src) {}

// Operador de asignación
searchable_array_bag& searchable_array_bag::operator=(const searchable_array_bag& src)
{
    if (this != &src)          // guarda: si haces "a = a" no hagas nada
        array_bag::operator=(src);  // delega al padre
    return *this;              // permite "a = b = c"
}

// Destructor vacío
// array_bag::~array_bag() se llama automáticamente después
// y ya hace delete[] data — tú no tienes nada que limpiar
searchable_array_bag::~searchable_array_bag() {}

// La única función con lógica real: buscar en el array
bool searchable_array_bag::has(int value) const
{
    for (int i = 0; i < this->size; i++)
        if (this->data[i] == value)   // ← esta línea es la que falta en tu versión
            return true;
    return false;
}
```

### ⚠️ Bug en tu implementación actual

En tu `has()` actual tienes:
```cpp
for (int i = 0; i < this->size; i++)
    return true;   // ← devuelve true sin comprobar nada
```

Le falta la condición. La versión correcta:
```cpp
for (int i = 0; i < this->size; i++)
    if (this->data[i] == value)   // ← añade esta línea
        return true;
```

---

## 13. El truco del copy/op= — por qué delegas al padre

`searchable_array_bag` **no tiene datos propios**.
Sus únicos datos (`data` y `size`) viven en `array_bag`.

Por eso en la copia no haces nada propio — llamas al padre:

```
searchable_array_bag (sin datos propios)
       ↓ hereda
   array_bag (tiene data[] y size)
```

Si tuvieras datos propios, harías:
```cpp
searchable_array_bag::searchable_array_bag(const searchable_array_bag& src)
    : array_bag(src)          // copia los datos del padre
{
    this->mi_dato = src.mi_dato;  // copia tus propios datos
}
```

Como no tienes datos propios, el cuerpo queda vacío.

---

## 14. Tu fichero 3: `searchable_tree_bag.hpp`

```cpp
#pragma once
#include "tree_bag.hpp"
#include "searchable_bag.hpp"

class searchable_tree_bag : public tree_bag, public searchable_bag
{
public:
    searchable_tree_bag();
    searchable_tree_bag(const searchable_tree_bag& src);
    searchable_tree_bag& operator=(const searchable_tree_bag& src);
    ~searchable_tree_bag();
    bool has(int value) const;
private:
    bool search(node* n, int value) const;  // ← helper privado
};
```

Idéntico a `searchable_array_bag.hpp` excepto:
- Hereda de `tree_bag` en vez de `array_bag`
- Tiene un método privado extra: `search()` — el helper recursivo

`search` es `private` porque es un detalle de implementación interno.
Nadie de fuera necesita saber cómo buscas — solo saben que `has()` existe.

---

## 15. Tu fichero 4: `searchable_tree_bag.cpp`

```cpp
#include "searchable_tree_bag.hpp"

// Forma canónica: exactamente igual que array_bag pero delegando a tree_bag
searchable_tree_bag::searchable_tree_bag() {}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag& src)
    : tree_bag(src) {}

searchable_tree_bag& searchable_tree_bag::operator=(const searchable_tree_bag& src)
{
    if (this != &src)
        tree_bag::operator=(src);
    return *this;
}

searchable_tree_bag::~searchable_tree_bag() {}

// Helper recursivo: busca en el BST
bool searchable_tree_bag::search(node* n, int value) const
{
    if (!n)                        // llegué a una hoja sin encontrarlo
        return false;
    if (n->value == value)         // lo encontré
        return true;
    if (value < n->value)          // BST: si es menor, busca a la izquierda
        return search(n->l, value);
    return search(n->r, value);    // si es mayor, busca a la derecha
}

// La función pública: arranca la búsqueda desde la raíz
bool searchable_tree_bag::has(int value) const
{
    return search(this->tree, value);
    //            ↑
    // this->tree es el nodo raíz, heredado de tree_bag (protected)
}
```

### La lógica del BST para `has()`

Un árbol BST tiene una regla: **izquierda < nodo ≤ derecha**.

```
Árbol con 5, 3, 8, 1:

         5
        / \
       3   8
      /
     1

has(3): 3 < 5 → voy izquierda → nodo es 3 → ¡encontrado! → true
has(6): 6 > 5 → voy derecha  → nodo es 8 → 6 < 8 → voy izquierda → NULL → false
has(1): 1 < 5 → izquierda → 1 < 3 → izquierda → nodo es 1 → true
```

---

## 16. La diferencia entre `array` y `tree`

Esto es lo único que cambia entre los dos. **Todo lo demás es copy-paste.**

| | `searchable_array_bag` | `searchable_tree_bag` |
|---|---|---|
| Padre de datos | `array_bag` | `tree_bag` |
| Delega copia a | `array_bag(src)` | `tree_bag(src)` |
| Delega `op=` a | `array_bag::operator=` | `tree_bag::operator=` |
| `has()` | bucle lineal en `data[]` | recursión en BST |
| Helper privado | ninguno | `search(node*, int)` |
| Print | orden de inserción | ordenado |

---

## 17. Tu fichero 5 y 6: `set.hpp` y `set.cpp`

### ¿Qué es un set?

Un **set** es una bolsa donde **no puede haber duplicados**.
Si intentas meter el `5` dos veces, la segunda vez se ignora.

El set no almacena los datos él mismo — **envuelve** una `searchable_bag`
que ya existe y le añade la lógica de "no duplicados".

### Concepto nuevo: composición (tener vs ser)

Hasta ahora hemos usado herencia: "searchable_array_bag **es** un array_bag".

`set` usa **composición**: "set **tiene** una searchable_bag".

```cpp
class set {
private:
    searchable_bag& bag;   // ← tiene una referencia a un bag externo
    ...
};
```

### Concepto nuevo: referencia (`&`)

Una referencia es como un puntero, pero:
- No puede ser NULL
- No se puede redirigir a otro objeto después de crearse
- No necesita `*` para acceder a lo que apunta

```c
// C — puntero
t_bag *bag;
bag->insert(5);

// C++ — referencia
searchable_bag& bag;  // & al declarar = referencia
bag.insert(5);        // sin * ni -> para acceder
```

En `set`, la referencia apunta a un `searchable_bag` que viene de fuera.
**Set no es dueño del bag** — solo lo usa. Por eso no hace `delete` en
el destructor.

### `set.hpp`

```cpp
#ifndef SET_HPP
#define SET_HPP
#include "searchable_bag.hpp"

class set
{
private:
    searchable_bag& bag;   // referencia al bag que envuelve

    // Forma canónica: copia y asignación NO se pueden hacer
    // porque no se puede reasignar una referencia en C++
    set();
    set(const set& src);
    set& operator=(const set& src);

public:
    set(searchable_bag& s_bag);   // único constructor válido
    ~set();

    bool has(int value) const;
    void insert(int value);
    void insert(int* data, int size);
    void print() const;
    void clear();
    const searchable_bag& get_bag();
};

#endif
```

¿Por qué `set()`, `set(const set&)` y `operator=` están en `private`
sin implementación?

Porque en C++ una referencia **no se puede reasignar**. Una vez que
`bag` apunta a un objeto, no puede apuntar a otro. Esto hace imposible
implementar correctamente el `operator=`. Al declararlo en `private` sin
cuerpo, el compilador da error si alguien intenta copiar un `set`.

### `set.cpp`

```cpp
#include "set.hpp"

// Constructor: recibe el bag externo y guarda la referencia
set::set(searchable_bag& s_bag) : bag(s_bag) {}
//                                 ↑
//                  ": bag(s_bag)" inicializa la referencia
//                  DEBE estar aquí, no dentro del cuerpo {}

set::~set() {}   // no hacemos delete bag — no somos dueños

// Delegamos todo al bag interno:
bool set::has(int value) const  { return bag.has(value); }
void set::print() const         { bag.print(); }
void set::clear()               { bag.clear(); }
const searchable_bag& set::get_bag() { return bag; }

// insert con lógica de set: solo insertar si no existe ya
void set::insert(int value)
{
    if (!has(value))       // ← aquí está TODA la magia del set
        bag.insert(value);
}

// insert array: llamar al insert de uno en uno
void set::insert(int* data, int size)
{
    for (int i = 0; i < size; i++)
        insert(data[i]);   // usa el insert de arriba, que ya comprueba duplicados
}
```

### Por qué `: bag(s_bag)` va fuera del cuerpo

Las referencias **deben inicializarse en la lista de inicialización**,
antes del `{`. No se pueden asignar dentro del cuerpo:

```cpp
// INCORRECTO — las referencias no se asignan en el cuerpo
set::set(searchable_bag& s_bag) {
    bag = s_bag;   // error: no se puede reasignar una referencia
}

// CORRECTO — se inicializa en la lista ": bag(s_bag)"
set::set(searchable_bag& s_bag) : bag(s_bag) {}
```

---

## 18. Pasos para escribirlo de memoria en el examen

### PASO 1 — `searchable_array_bag.hpp` (2 min)

```
1. #pragma once
2. includes: array_bag.hpp, searchable_bag.hpp
3. class ... : public array_bag, public searchable_bag
4. public: 4 forma canónica + has()
```

### PASO 2 — `searchable_array_bag.cpp` (3 min)

```
1. #include header
2. constructor()    → cuerpo vacío
3. copia(src)      → : array_bag(src) {}
4. operator=(src)  → if (this != &src) array_bag::operator=(src); return *this;
5. destructor()    → cuerpo vacío
6. has(value)      → for + if data[i]==value + return false
```

### PASO 3 — `searchable_tree_bag.hpp` (1 min)

Copia `searchable_array_bag.hpp` y cambia:
- `array_bag` → `tree_bag`
- Añade `private: bool search(node* n, int value) const;`

### PASO 4 — `searchable_tree_bag.cpp` (4 min)

Copia `searchable_array_bag.cpp` y cambia:
- `array_bag` → `tree_bag` en todas las delegaciones
- `has()` → `return search(this->tree, value);`
- Añade función `search()` recursiva antes de `has()`

### PASO 5 — `set.hpp` (2 min)

```
1. #ifndef SET_HPP / #define / #endif
2. include searchable_bag.hpp
3. class set { private: searchable_bag& bag; }
4. private (sin cuerpo): set(); set(const set&); set& operator=(...)
5. public: constructor(searchable_bag&), destructor
6. public: has, insert(int), insert(int*,int), print, clear, get_bag
```

### PASO 6 — `set.cpp` (3 min)

```
1. constructor  → : bag(s_bag) {}
2. destructor   → {}
3. has/print/clear/get_bag → delegan a bag
4. insert(int)  → if (!has(value)) bag.insert(value)
5. insert(arr)  → for + insert(data[i])
```

---

## 19. Cómo compilar y testar

```bash
# Desde tu directorio de trabajo (con los ficheros del subject al lado)
g++ -std=c++98 -Wall \
    searchable_array_bag.cpp \
    searchable_tree_bag.cpp \
    set.cpp \
    subject/array_bag.cpp \
    subject/tree_bag.cpp \
    subject/main.cpp \
    -o polyset

./polyset 5 3 8 1
```

Salida esperada:
```
1 3 5 8       ← tree imprime ordenado (BST in-order)
5 3 8 1       ← array imprime en orden de inserción
1             ← tree.has(5)  → 1 (true)
1             ← array.has(5) → 1
0             ← tree.has(4)  → 0 (false)
0             ← array.has(4) → 0
...           (mismo patrón para 3, 8, 1)

              ← línea vacía (tmp.print() sobre bag vacío)
5 3 8 1       ← sa.print()
5 3 8 1       ← sa.get_bag().print()
5 3 8 1       ← st.print()
```
