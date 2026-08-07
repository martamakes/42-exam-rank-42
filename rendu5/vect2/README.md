# vect2 — Vector 2D en C++

---

## 1. Qué pide el subject

El ejercicio pide crear una **clase `vect2`** que represente un vector matemático de 2 dimensiones con enteros (`int`).

Piénsalo como un punto en un plano: tiene coordenada X y coordenada Y. Por ejemplo, `vect2(3, 5)` es el vector `{3, 5}`.

### Lo que tienes que implementar

| Qué | Cómo se usa | Resultado |
|-----|------------|-----------|
| Constructor por defecto | `vect2 v` | `{0, 0}` |
| Constructor con parámetros | `vect2(2, 3)` | `{2, 3}` |
| Constructor copia | `vect2 b(a)` | copia de `a` |
| Suma | `v1 + v2` | suma componente a componente |
| Resta | `v1 - v2` | resta componente a componente |
| Multiplicación por escalar | `v * 3` o `3 * v` | multiplica x e y por 3 |
| Acceso `[]` | `v[0]` → x, `v[1]` → y | sin comprobación de límites |
| Impresión `<<` | `cout << v` | imprime `{x, y}` |
| Igualdad/desigualdad | `v1 == v2`, `v1 != v2` | compara x e y |
| Incremento/decremento | `++v`, `v++`, `--v`, `v--` | suma/resta 1 a x e y |
| `+=`, `-=`, `*=` | `v1 += v2`, `v *= 3` | modifica v1/v en sitio |
| Negación unaria | `-v` | `{-x, -y}` (distinto de `v1 - v2`) |

> **Clave del subject:** `(vect2(2,2) * 2 == vect2(4,4))` debe ser `true`.  
> Y `cout << v` debe producir exactamente `{x, y}` (con llaves, coma y espacio).
>
> **Trampa real, no solo del texto del subject:** el `main.cpp` que da el examen usa `v1 *= 42` y `-v2` — sin `operator*=` y sin el `operator-` **unario** (un solo operando, distinto de la resta binaria `v1 - v2`), esto **no compila**. Se encontró exactamente este fallo revisando esta solución contra el `main.cpp` real (ver `05/level-1/vect2/grademe/test.sh`).

---

## 2. Casos extremos a probar en el main

### Casos básicos (los que ya están en el main)
```
vect2 v1;           → {0, 0}   ← constructor por defecto
vect2 v2(1, 2);     → {1, 2}   ← constructor con parámetros
const vect2 v3(v2); → {1, 2}   ← copia (const: solo lectura)
vect2 v4 = v2;      → {1, 2}   ← asignación
```

### Casos extremos que hay que vigilar

#### Post-incremento vs pre-incremento
```cpp
vect2 v(1, 2);
v++   // devuelve el valor ANTIGUO {1,2}, luego v se convierte en {2,3}
++v   // primero incrementa, devuelve el valor NUEVO {3,4}
```
**Trampa:** `v++` debe retornar una COPIA del valor antes de incrementar, no una referencia.

#### Multiplicación en los dos sentidos
```cpp
vect2(2,2) * 2   // obj * escalar  → {4,4}  ← dentro de la clase
2 * vect2(2,2)   // escalar * obj  → {4,4}  ← función global fuera de la clase
```
**Trampa:** `2 * v` no puede implementarse como método de la clase (el `2` no es un `vect2`). Necesita una **función libre** `operator*(int, const vect2&)`.

#### Acceso con `[]` en objeto const
```cpp
const vect2 v3(1, 2);
cout << v3[0];   // necesita la versión CONST del operador[]
// v3[0] = 5;   // esto NO debe compilar
```
**Trampa:** Necesitas DOS versiones de `[]`: una que devuelve `int&` (para escribir) y otra `const int&` (para leer en objetos const).

#### Negativos
```cpp
vect2 v(0, 0);
v -= vect2(2, 4);   // {0-2, 0-4} = {-2, -4}
cout << v;          // debe imprimir {-2, -4}
```

#### `*=` (multiplicación compuesta) — fácil de olvidar
```cpp
vect2 v1(-2, -4);
v1 *= 42;   // {-2*42, -4*42} = {-84, -168}
```
**Trampa:** es tentador implementar solo `operator*(int)` (que sí devuelve un nuevo `vect2`) y darlo por completo, pero el `main.cpp` real usa `v1 *= 42` — necesitas el compuesto `operator*=(int)` aparte, igual que `+=`/`-=`.

#### Negación unaria `-v` — NO es la resta binaria
```cpp
vect2 v2(20, 40);
cout << -v2;   // {-20, -40}
```
**Trampa grande:** `operator-(const vect2&) const` (la resta, `v1 - v2`, DOS operandos) y `operator-() const` (la negación, `-v`, UN operando) son overloads **distintos** — el compilador elige uno u otro según cuántos operandos ve. Tener la resta no te da la negación gratis: si el `main.cpp` usa `-v2` y solo implementaste la resta, el error de compilación es "no viable overloaded operator-" y cuesta ver por qué a primera vista.

#### Expresión compuesta
```cpp
v2 = 3 * (v3 + v3 * 2);
// v3 = {1,2}
// v3 * 2 = {2,4}
// v3 + {2,4} = {3,6}
// 3 * {3,6} = {9,18}
```
**Trampa:** El orden de operaciones debe respetarse. Asegúrate de que `operator+` y `operator*` devuelven nuevos objetos (no modifican los existentes).

#### Formato exacto del `<<`
```cpp
cout << vect2(1, 2);   // debe imprimir: {1, 2}
                        // NO: (1,2) ni [1, 2] ni {1,2}
```
Hay un **espacio** después de la coma: `{x, y}`.

---

## 3. Implementación simple para el examen

### `vect2.hpp`

```cpp
#ifndef VECT2_HPP
#define VECT2_HPP

#include <iostream>

class vect2 {
private:
    int x;
    int y;

public:
    // Constructores
    vect2();
    vect2(int x, int y);
    vect2(const vect2& other);
    vect2& operator=(const vect2& other);
    ~vect2();   // forma canonica completa (destructor explicito, cuerpo vacio)

    // Suma
    vect2  operator+(const vect2& other) const;
    vect2& operator+=(const vect2& other);
    vect2& operator++();      // ++v
    vect2  operator++(int);   // v++

    // Resta
    vect2  operator-(const vect2& other) const;
    vect2& operator-=(const vect2& other);
    vect2& operator--();      // --v
    vect2  operator--(int);   // v--

    // Multiplicación
    vect2 operator*(int scalar) const;   // v * 3
    vect2& operator*=(int scalar);       // v *= 3

    // Negación unaria (distinta de la resta binaria de arriba)
    vect2 operator-() const;             // -v

    // Acceso
    int&       operator[](int index);         // escritura: v[0] = 5
    const int& operator[](int index) const;   // lectura:   cout << v[0]

    // Getters (para usar en operator<<)
    int getX() const;
    int getY() const;

    // Comparación
    bool operator==(const vect2& other) const;
    bool operator!=(const vect2& other) const;
};

// Fuera de la clase: 3 * v
vect2 operator*(int scalar, const vect2& vec);

// Impresión: cout << v
std::ostream& operator<<(std::ostream& os, const vect2& vec);

#endif
```

### `vect2.cpp`

```cpp
#include "vect2.hpp"

// --- Constructores ---

vect2::vect2() : x(0), y(0) {}

vect2::vect2(int x, int y) : x(x), y(y) {}

vect2::vect2(const vect2& other) : x(other.x), y(other.y) {}

vect2& vect2::operator=(const vect2& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;
}

vect2::~vect2() {}   // nada que liberar, pero se declara explicito

// --- Suma ---

vect2 vect2::operator+(const vect2& other) const {
    return vect2(x + other.x, y + other.y);
}

vect2& vect2::operator+=(const vect2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

vect2& vect2::operator++() {       // ++v: incrementa y devuelve el nuevo valor
    x += 1;
    y += 1;
    return *this;
}

vect2 vect2::operator++(int) {     // v++: guarda copia, incrementa, devuelve copia antigua
    vect2 copy = *this;
    ++(*this);
    return copy;
}

// --- Resta ---

vect2 vect2::operator-(const vect2& other) const {
    return vect2(x - other.x, y - other.y);
}

vect2& vect2::operator-=(const vect2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

vect2& vect2::operator--() {
    x -= 1;
    y -= 1;
    return *this;
}

vect2 vect2::operator--(int) {
    vect2 copy = *this;
    --(*this);
    return copy;
}

// --- Multiplicación ---

vect2 vect2::operator*(int scalar) const {   // v * 3
    return vect2(x * scalar, y * scalar);
}

vect2 operator*(int scalar, const vect2& vec) {  // 3 * v (función global)
    return vec * scalar;
}

vect2& vect2::operator*=(int scalar) {   // v *= 3 (mismo patrón que += y -=)
    x *= scalar;
    y *= scalar;
    return *this;
}

// --- Negación unaria ---

vect2 vect2::operator-() const {   // -v  (UN operando; no confundir con la resta binaria)
    return vect2(-x, -y);
}

// --- Acceso con [] ---

int& vect2::operator[](int index) {              // v[0] = 5
    return (index == 0) ? x : y;
}

const int& vect2::operator[](int index) const {  // cout << v[0]
    return (index == 0) ? x : y;
}

// --- Getters ---

int vect2::getX() const { return x; }
int vect2::getY() const { return y; }

// --- Comparación ---

bool vect2::operator==(const vect2& other) const {
    return (x == other.x && y == other.y);
}

bool vect2::operator!=(const vect2& other) const {
    return !(*this == other);
}

// --- Impresión ---

std::ostream& operator<<(std::ostream& os, const vect2& vec) {
    os << "{" << vec.getX() << ", " << vec.getY() << "}";
    return os;
}
```

### Compilar y probar

```bash
g++ -std=c++98 -Wall -Wextra -o vect2 main.cpp vect2.cpp && ./vect2
```

---

## Conceptos clave explicados para alguien que los ve por primera vez

### ¿Qué es un operador?

Un operador (`+`, `-`, `*`, `[]`, `<<`) es una función especial en C++. Cuando escribes `v1 + v2`, en realidad estás llamando a `v1.operator+(v2)`. C++ te deja **definir qué hace ese `+`** para tus propias clases.

### ¿Por qué hay dos versiones de `[]`?

```cpp
// Versión "escritora" — devuelve referencia para poder modificar
int& operator[](int i);         // v[0] = 42;  ← funciona

// Versión "lectora" — para objetos que no se pueden modificar (const)
const int& operator[](int i) const;   // const vect2 v(1,2); cout << v[0]; ← funciona
```

Si solo tuvieras la versión escritora, el compilador se quejaría cuando intentas leer de un `const vect2`.

### ¿Por qué `v++` y `++v` son diferentes?

- `++v` (prefijo): incrementa primero, luego devuelve el valor nuevo. Devuelve **referencia** (`vect2&`).
- `v++` (postfijo): guarda una copia, incrementa, devuelve la copia antigua. Devuelve **copia** (`vect2`).

El `int` en `operator++(int)` es solo una señal al compilador para distinguirlo del prefijo. No se usa para nada.

### ¿Por qué `3 * v` necesita una función fuera de la clase?

Cuando escribes `3 * v`, C++ busca `int::operator*(vect2)`. Pero `int` es un tipo del sistema y no puedes modificarlo. Por eso necesitas una **función libre**:

```cpp
vect2 operator*(int scalar, const vect2& vec) {
    return vec * scalar;  // esto sí llama al método de la clase
}
```

### ¿Por qué `operator<<` también va fuera de la clase?

El operador `<<` pertenece a `std::ostream` (el tipo de `cout`), no a `vect2`. Como no puedes modificar `ostream`, la función debe ser libre:

```cpp
std::ostream& operator<<(std::ostream& os, const vect2& vec) {
    os << "{" << vec.getX() << ", " << vec.getY() << "}";
    return os;  // importante: devolver os para poder encadenar (cout << a << b)
}
```

### ¿Por qué `-v` y `v1 - v2` son operadores DISTINTOS aunque usen el mismo símbolo?

C++ decide qué `operator-` llamar según **cuántos operandos ve**, no según el símbolo:

```cpp
vect2 operator-(const vect2& other) const;  // BINARIO: v1 - v2  (2 operandos: *this y other)
vect2 operator-() const;                    // UNARIO:  -v       (1 operando: solo *this)
```

Son dos funciones con el mismo nombre pero distinta cantidad de parámetros — como cualquier sobrecarga de función normal. Definir una no te da la otra: si el `main.cpp` usa `-v2` y solo tienes la resta binaria, no compila.

---

## Resumen rápido para el examen

1. Crea `vect2.hpp` con la clase y las declaraciones.
2. Crea `vect2.cpp` con las implementaciones.
3. Recuerda las **dos funciones libres** fuera de la clase: `operator*(int, vect2)` y `operator<<`. `operator-()` (unario) es distinto: va DENTRO de la clase, como método.
4. Recuerda las **dos versiones de `[]`**: una normal y una `const`.
5. No olvides `operator*=(int)` y `operator-()` (unario) — el `main.cpp` real los usa (`v1 *= 42`, `-v2`) aunque no sean obvios solo leyendo el texto del subject.
6. El formato de impresión es exactamente `{x, y}` (espacio tras la coma, llaves).
7. Compila con `g++ -std=c++98`.
