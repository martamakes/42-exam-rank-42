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
| `+=`, `-=` | `v1 += v2` | modifica v1 en sitio |

> **Clave del subject:** `(vect2(2,2) * 2 == vect2(4,4))` debe ser `true`.  
> Y `cout << v` debe producir exactamente `{x, y}` (con llaves, coma y espacio).

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

---

## Resumen rápido para el examen

1. Crea `vect2.hpp` con la clase y las declaraciones.
2. Crea `vect2.cpp` con las implementaciones.
3. Recuerda las **dos funciones libres** fuera de la clase: `operator*(int, vect2)` y `operator<<`.
4. Recuerda las **dos versiones de `[]`**: una normal y una `const`.
5. El formato de impresión es exactamente `{x, y}` (espacio tras la coma, llaves).
6. Compila con `g++ -std=c++98`.
