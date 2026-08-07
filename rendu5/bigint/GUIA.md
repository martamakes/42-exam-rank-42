# bigint — Entero de precisión arbitraria en C++

---

## 1. Qué pide el subject

> "In computer science a bignum is an object representing an arbitrary precision number, this is useful when you want to store a number bigger than SIZE_MAX without any loss of precision. [...] Create a class called bigint that will store an arbitrary precision unsigned integer."

Un `unsigned long` normal se desborda al llegar a ~18 dígitos. `bigint` no tiene ese límite: guarda el número como texto (`std::string`), dígito a dígito, así que puede crecer todo lo que haga falta.

### Lo que tienes que implementar

| Qué | Cómo se usa | Resultado |
|-----|------------|-----------|
| Constructor por defecto | `bigint c` | `"0"` |
| Constructor desde entero | `bigint a(42)` | `"42"` |
| Constructor copia | `bigint e(d)` | copia de `d` |
| Suma | `a + b` | suma dígito a dígito, con acarreo |
| `+=` | `c += a` | modifica `c` en sitio |
| Incremento | `++b`, `b++` | suma 1 |
| Comparación | `<`, `>`, `==`, `!=`, `<=`, `>=` | por valor numérico, no alfabético |
| Digitshift izquierda | `a << 3` | añade `n` ceros al final: `42 → 42000` |
| Digitshift derecha | `a >> 2` | quita los últimos `n` dígitos: `1337 → 13` |
| Digitshift compuesto | `<<=`, `>>=` | modifica en sitio |
| Impresión `<<` | `cout << a` | en base 10, **sin ceros a la izquierda** |

> **Clave del subject:** `digitshift` no es `bitshift`. `42 << 3 == 42000` (añade 3 ceros en base 10, no desplaza bits en binario). `1337 >> 2 == 13` (quita los 2 últimos dígitos).
> **Clave del subject:** el resultado impreso **nunca** debe tener ceros a la izquierda — ni `"007"` ni `""` para el cero (debe ser `"0"`).

---

## 2. Casos extremos a probar en el main

### Casos básicos (los que ya están en el main dado)
```
const bigint a(42);
bigint b(21), c, d(1337), e(d);
// a = 42, b = 21, c = 0, d = 1337, e = 1337 (copia de d)
```

### Casos extremos que hay que vigilar

#### Acarreo en cascada (varios 9 seguidos)
```cpp
bigint(999) + bigint(1)   // → "1000", no "0999" ni "1000" con un digito de mas
```
**Trampa:** si sumas dos números de la misma longitud y el resultado necesita un dígito extra (acarreo final), ese nuevo dígito **nunca** es cero — pero si tu bucle de suma tiene un error de índices, puedes acabar con un `"0"` sobrante delante. Prueba también `50 + 50 = 100` y `999999999 + 1 = 1000000000`.

#### Digitshift con una CANTIDAD que es otro bigint, no solo `int`
```cpp
bigint x(12345678), y(5);
x << y;   // shiftear x por "cuánto dice y", no por el literal 5
```
**Trampa:** el `main.cpp` real del examen usa `x << y` donde `y` es un `bigint`, no un entero. Si tu `operator<<` solo acepta `int`, esto compila igual **por conversión implícita** en un sentido (int → bigint sí, pero necesitas que tu operador acepte `const bigint&` como cantidad de desplazamiento, y convertir su texto a un número internamente).

#### Shift total o sobre cero
```cpp
bigint(100) >> bigint(10);   // se queda sin dígitos → debe dar "0", NO ""
bigint(0) << bigint(5);      // shiftear "0" sigue siendo "0", no "000000"
```
**Trampa:** si tu shift-right calcula `longitud - n` y `n >= longitud`, el resultado es negativo o vacío — hay que detectarlo antes y devolver `"0"` explícitamente.

#### Comparación: más dígitos = más grande (nunca alfabética directa)
```cpp
bigint(7) < bigint(42);     // true  (1 digito vs 2: menos digitos = menor)
bigint(100) < bigint(42);   // false (3 digitos vs 2: mas digitos = mayor)
```
**Trampa:** comparar los `std::string` directamente (`"7" < "42"`) da el resultado **al revés** de lo que quieres, porque compara caracter a caracter como texto (`'7' > '4'`). Primero hay que comparar la **longitud**; solo si son iguales, comparar como texto (ahí sí funciona, porque con la misma longitud el orden lexicográfico coincide con el numérico).

#### Post-incremento vs pre-incremento (igual que en vect2)
```cpp
bigint b(21);
b++    // devuelve el valor ANTIGUO "21", luego b se convierte en "22"
++b    // primero incrementa, devuelve el valor NUEVO "22"
```

#### El cero no es un caso especial "vacío"
```cpp
bigint c;             // constructor por defecto → "0", NUNCA ""
cout << c;             // debe imprimir "0", no nada
```
**Trampa:** es tentador inicializar `_digits` como cadena vacía y "rellenar bajo demanda". No lo hagas: un `bigint` recién creado debe imprimir `"0"` inmediatamente.

---

## 3. Implementación de referencia (ya probada — 34/34 tests, ver `grademe/test.sh`)

### `bigint.hpp`

```cpp
#ifndef BIG_INT
#define BIG_INT

#include <string>
#include <iostream>

class bigint
{
    private:
        std::string _digits;

    public:
        bigint( void );
        bigint(unsigned long n);
        bigint(const bigint &other);
        bigint &operator=(const bigint &other);
        ~bigint( void );

        bigint operator+(const bigint &other) const;
        bigint &operator+=(const bigint &other);

        bigint &operator++();
        bigint operator++(int);

        bool operator<(const bigint &other) const;
        bool operator>(const bigint &other) const;
        bool operator==(const bigint &other) const;
        bool operator!=(const bigint &other) const;
        bool operator<=(const bigint &other) const;
        bool operator>=(const bigint &other) const;

        bigint &operator<<=(const bigint &shift);
        bigint &operator>>=(const bigint &shift);
        bigint operator<<(const bigint &shift) const;
        bigint operator>>(const bigint &shift) const;

        const std::string& getDigits() const;
};

std::ostream &operator<<(std::ostream &os, const bigint &bg);

#endif
```

### `bigint.cpp` (lo esencial — suma y digitshift)

```cpp
#include "bigint.hpp"
#include <sstream>
#include <algorithm>

bigint::bigint( void ) : _digits("0") {}

bigint::bigint(unsigned long n) {
    std::ostringstream oss;
    oss << n;
    _digits = oss.str();
}

// --- Suma: dígito a dígito desde el final, con acarreo ---
bigint bigint::operator+(const bigint &other) const {
    std::string resString = "";
    int i = this->_digits.length() - 1;
    int j = other._digits.length() - 1;
    int carry = 0;

    while (i >= 0 || j >= 0 || carry > 0) {
        int d1 = (i >= 0) ? this->_digits[i] - '0' : 0;
        int d2 = (j >= 0) ? other._digits[j] - '0' : 0;
        int sum = d1 + d2 + carry;
        resString += (sum % 10) + '0';
        carry = sum / 10;
        i--; j--;
    }
    std::reverse(resString.begin(), resString.end());
    bigint result;
    result._digits = resString;
    return result;
}

// --- Comparación: longitud primero, texto solo si empatan ---
bool bigint::operator<(const bigint &other) const {
    if (this->_digits.length() != other._digits.length())
        return this->_digits.length() < other._digits.length();
    return this->_digits < other._digits;
}

bool bigint::operator>(const bigint &other) const { return other < *this; }

// --- Digitshift: la cantidad es un bigint, se decodifica a numero real ---
bigint &bigint::operator<<=(const bigint &shift) {
    if (this->_digits == "0" || shift._digits == "0")
        return *this;
    std::istringstream iss(shift.getDigits());
    unsigned long n;
    iss >> n;
    this->_digits.append(n, '0');   // append(count, char) = n ceros al final
    return *this;
}

bigint &bigint::operator>>=(const bigint &shift) {
    std::istringstream iss(shift.getDigits());
    size_t shft;
    iss >> shft;
    if (shft >= _digits.length()) {
        _digits = "0";              // shift total -> "0", nunca ""
        return *this;
    }
    _digits.resize(_digits.length() - shft);
    return *this;
}

std::ostream &operator<<(std::ostream &os, const bigint &bg) {
    os << bg.getDigits();
    return os;
}
```

El resto (`operator+=`, `++`, `!=`, `<=`, `>=`, `<<`/`>>` sin `=`) son una línea cada uno, delegando en los de arriba — están completos en `bigint.cpp` de este directorio.

### Compilar y probar

```bash
g++ -std=c++98 -Wall -Wextra -o bigint main.cpp bigint.cpp && ./bigint
```

---

## Conceptos clave explicados para alguien que los ve por primera vez

### ¿Por qué guardar el número como `std::string` y no como `int`/`long`?

Un `unsigned long` normal tiene un límite fijo (unos 18-20 dígitos). El subject pide precisión **arbitraria**: debe funcionar igual de bien con 5 dígitos que con 500. Guardando cada dígito como un carácter de texto, el tamaño solo está limitado por la memoria disponible, no por el tipo.

### ¿Por qué la suma se hace "desde el final" (`i--`, `j--`)?

Sumar a mano funciona igual: empiezas por las unidades (el dígito más a la derecha) y vas hacia la izquierda, arrastrando el acarreo cuando una columna suma 10 o más. Por eso el bucle recorre los índices `length()-1` hacia `0`, y al final se hace `std::reverse` porque el resultado se construyó "al revés" (unidades primero).

```
  1337
+   21
------
  1358     <- se calcula 7+1=8, 3+2=5, 3+0=3, 1+0=1 -> "8531" -> reverse -> "1358"
```

### ¿Por qué la comparación no puede ser solo `_digits < other._digits`?

Comparar dos `std::string` directamente compara **carácter a carácter**, como si fueran palabras de un diccionario. Eso funciona para números de la **misma longitud** (`"42" < "100"` en texto SÍ da el resultado correcto casualmente aquí, pero `"9" < "10"` en texto da `false`, cuando `9 < 10` en realidad es `true` — porque `'9' > '1'` como caracteres). Por eso siempre se compara la **longitud primero**: más dígitos = número más grande (asumiendo que nunca hay ceros a la izquierda, que es justo el invariante que mantiene esta clase).

### ¿Por qué `digitshift` recibe otro `bigint` como cantidad, y no un `int`?

El subject define `<<`/`>>` para **desplazar dígitos**, igual que `<<`/`>>` en enteros desplazan bits — pero aquí la cantidad de desplazamiento puede venir, en el `main.cpp` real, como **otro `bigint`** (`x << y`, con `y` siendo un `bigint`), no solo como literal entero (`b << 10`). La solución: el operador siempre recibe `const bigint&` como cantidad; si le pasas un `int`, se convierte automáticamente a `bigint` (por el constructor `bigint(unsigned long)`, que no es `explicit`). Dentro del operador, se "decodifica" el texto de la cantidad a un `unsigned long` real con un `istringstream`, porque `std::string::append` necesita un número, no un texto.

### ¿Por qué el shift-right usa `resize()` y no `erase()` de otra forma?

`_digits.resize(nuevaLongitud)` recorta el `std::string` dejando solo los primeros `nuevaLongitud` caracteres — que son los dígitos **más significativos** (los de la izquierda). Es exactamente "quitar los últimos N dígitos": `"1337"` con `resize(2)` se queda en `"13"`.

### ¿Por qué el constructor por defecto pone `"0"` y no `""`?

Porque `bigint c;` en el `main.cpp` dado se imprime inmediatamente (`cout << c`) y el subject exige que la impresión nunca tenga ceros a la izquierda **ni esté vacía** — el cero se representa como el string `"0"`, un único carácter, que es su propia excepción al invariante "nunca ceros a la izquierda" (si no, no podrías representar el cero en absoluto).

---

## Resumen rápido para el examen

1. Crea `bigint.hpp` con la clase y las declaraciones (constructor por defecto → `"0"`, constructor desde entero, copia, `=`, destructor).
2. Crea `bigint.cpp`. Lo más largo es `operator+` (dígito a dígito con acarreo) y los digitshift (`<<=`/`>>=`).
3. Los digitshift reciben **otro `bigint`** como cantidad (no solo `int`) — decodifícalo a un número real con `istringstream` antes de usarlo.
4. La comparación compara **longitud primero**, texto solo si empatan — nunca compares los strings directamente sin mirar la longitud.
5. El cero es siempre `"0"`, nunca `""` ni `"00"`.
6. Compila con `g++ -std=c++98`.
7. Verifica con `05/level-1/bigint/grademe/test.sh` (34 tests unitarios + integración con el `main.cpp` real).
