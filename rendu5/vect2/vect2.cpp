#include "vect2.hpp"

// ============================================================
// CONSTRUCTORES
// ============================================================

// Constructor por defecto: vect2 v;  →  {0, 0}
// Los ":" después del nombre es la "lista de inicialización" — forma corta de asignar.
vect2::vect2() : x(0), y(0) {}

// Constructor con parámetros: vect2 v(3, 5);  →  {3, 5}
vect2::vect2(int x, int y) : x(x), y(y) {}

// Constructor copia: vect2 b(a);  →  copia exacta de a
vect2::vect2(const vect2& other) : x(other.x), y(other.y) {}

// Operador de asignación: b = a;
// "this" es un puntero al objeto actual (al que le estamos asignando).
// Comprobamos "this != &other" para no copiarnos a nosotros mismos.
vect2& vect2::operator=(const vect2& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;  // devolvemos el objeto actual para poder encadenar: a = b = c;
}

// Destructor: no hay recursos que liberar (solo dos ints), cuerpo vacío.
// Se declara explícito para dejar la forma canónica completa.
vect2::~vect2() {}

// ============================================================
// SUMA
// ============================================================

// v1 + v2  →  nuevo vect2 con la suma de componentes
// "const" al final = este método NO modifica el objeto.
vect2 vect2::operator+(const vect2& other) const {
    return vect2(x + other.x, y + other.y);
}

// v1 += v2  →  modifica v1 en sitio y devuelve referencia a v1
vect2& vect2::operator+=(const vect2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

// ++v  →  incrementa primero, luego devuelve el NUEVO valor
// Devuelve referencia (&) porque el objeto ya está modificado.
vect2& vect2::operator++() {
    x += 1;
    y += 1;
    return *this;
}

// v++  →  devuelve el valor ANTIGUO, luego incrementa
// Truco: guardamos una copia antes de modificar, y devolvemos esa copia.
// El "int" en el parámetro es solo para que el compilador distinga ++v de v++.
vect2 vect2::operator++(int) {
    vect2 copy = *this;  // guardamos cómo éramos ANTES
    ++(*this);           // ahora nos incrementamos (llama a operator++() de arriba)
    return copy;         // devolvemos cómo éramos antes
}

// ============================================================
// RESTA
// ============================================================

// v1 - v2
vect2 vect2::operator-(const vect2& other) const {
    return vect2(x - other.x, y - other.y);
}

// v1 -= v2
vect2& vect2::operator-=(const vect2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

// --v
vect2& vect2::operator--() {
    x -= 1;
    y -= 1;
    return *this;
}

// v--
vect2 vect2::operator--(int) {
    vect2 copy = *this;
    --(*this);
    return copy;
}

// ============================================================
// MULTIPLICACIÓN POR ESCALAR
// ============================================================

// v * 3  →  {x*3, y*3}
vect2 vect2::operator*(int scalar) const {
    return vect2(x * scalar, y * scalar);
}

// v *= 3  →  modifica v en sitio (igual patrón que += y -=)
vect2& vect2::operator*=(int scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

// 3 * v  →  igual resultado, pero el 3 va PRIMERO.
// Como el 3 no es un vect2, esta función NO puede ser un método de la clase.
// Va fuera, como función libre.
vect2 operator*(int scalar, const vect2& vec) {
    return vec * scalar;  // llama a operator*(int) de arriba, que sí es método
}

// -v  →  {-x, -y}  (negación unaria, un solo operando)
vect2 vect2::operator-() const {
    return vect2(-x, -y);
}

// ============================================================
// ACCESO CON []
// v[0] → x,  v[1] → y
// ============================================================

// Versión ESCRITORA: devuelve referencia → podemos hacer v[0] = 42;
int& vect2::operator[](int index) {
    return (index == 0) ? x : y;
}

// Versión LECTORA (const): devuelve const referencia.
// Se usa cuando el objeto es const: const vect2 v(1,2); cout << v[0];
// Sin esta versión, el compilador se quejaría al leer un objeto const.
const int& vect2::operator[](int index) const {
    return (index == 0) ? x : y;
}

// ============================================================
// GETTERS
// ============================================================

int vect2::getX() const { return x; }
int vect2::getY() const { return y; }

// ============================================================
// COMPARACIÓN
// ============================================================

// v1 == v2  →  true si x e y son iguales en ambos
bool vect2::operator==(const vect2& other) const {
    return (x == other.x && y == other.y);
}

// v1 != v2  →  lo contrario de ==
// Reutilizamos operator== para no repetir código.
bool vect2::operator!=(const vect2& other) const {
    return !(*this == other);
}

// ============================================================
// IMPRESIÓN con cout
// ============================================================

// cout << v  produce exactamente:  {x, y}
// (llave apertura, x, coma, espacio, y, llave cierre)
//
// Esta función va FUERA de la clase porque el primer operando
// es ostream (el cout), no vect2. No podemos modificar ostream.
//
// Devolvemos "os" para poder encadenar: cout << v1 << v2;
std::ostream& operator<<(std::ostream& os, const vect2& vec) {
    os << "{" << vec.getX() << ", " << vec.getY() << "}";
    return os;
}
