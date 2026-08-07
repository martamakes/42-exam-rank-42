#ifndef VECT2_HPP
#define VECT2_HPP

// Necesitamos cout/ostream para el operador <<
#include <iostream>

// ============================================================
// CLASE vect2 — vector matemático de 2 dimensiones con ints
// Imagínalo como un punto en un plano: tiene X e Y.
// vect2(3, 5) representa el punto (3, 5).
// ============================================================
class vect2 {

private:
    int x;  // coordenada horizontal
    int y;  // coordenada vertical

public:

    // ----------------------------------------------------------
    // CONSTRUCTORES — cómo se "nace" un objeto vect2
    // ----------------------------------------------------------

    vect2();                    // por defecto:   vect2 v;       → {0, 0}
    vect2(int x, int y);        // con valores:   vect2 v(3, 5); → {3, 5}
    vect2(const vect2& other);  // copia:         vect2 b(a);    → copia exacta de a
    vect2& operator=(const vect2& other);  // asignación: b = a;
    ~vect2();  // destructor: no hay memoria dinámica que liberar, pero se
               // declara explícito para completar la forma canónica ortodoxa
               // (ctor por defecto, copia, operator=, destructor).

    // ----------------------------------------------------------
    // SUMA
    // ----------------------------------------------------------

    vect2  operator+(const vect2& other) const;  // v1 + v2  → nuevo vect2
    vect2& operator+=(const vect2& other);        // v1 += v2 → modifica v1

    // Incremento: suma 1 a x e y
    vect2& operator++();    // ++v  → incrementa PRIMERO, devuelve valor nuevo
    vect2  operator++(int); // v++  → devuelve valor ANTIGUO, luego incrementa

    // ----------------------------------------------------------
    // RESTA
    // ----------------------------------------------------------

    vect2  operator-(const vect2& other) const;  // v1 - v2
    vect2& operator-=(const vect2& other);        // v1 -= v2

    // Decremento: resta 1 a x e y
    vect2& operator--();    // --v
    vect2  operator--(int); // v--

    // ----------------------------------------------------------
    // MULTIPLICACIÓN POR ESCALAR
    // v * 3  →  {x*3, y*3}
    // ----------------------------------------------------------

    vect2 operator*(int scalar) const;  // v * 3
    vect2& operator*=(int scalar);      // v *= 3  → modifica v en sitio

    // Negación unaria: -v  →  {-x, -y}
    // OJO: es DISTINTO del operator- de arriba (esa es la resta binaria,
    // v1 - v2). Aquí solo hay UN operando, así que el compilador elige
    // esta versión cuando ve "-v" y la de arriba cuando ve "v1 - v2".
    // El main.cpp dado usa "-v2", así que sin este operador no compila.
    vect2 operator-() const;

    // ----------------------------------------------------------
    // ACCESO CON []
    // v[0] → x,  v[1] → y
    // Necesitamos DOS versiones: una para leer y otra para escribir.
    // ----------------------------------------------------------

    int&       operator[](int index);        // escritura: v[0] = 42;
    const int& operator[](int index) const;  // lectura:   cout << v[0];  (funciona en const)

    // ----------------------------------------------------------
    // GETTERS — para usar dentro de operator<<
    // ----------------------------------------------------------

    int getX() const;
    int getY() const;

    // ----------------------------------------------------------
    // COMPARACIÓN
    // ----------------------------------------------------------

    bool operator==(const vect2& other) const;  // v1 == v2
    bool operator!=(const vect2& other) const;  // v1 != v2
};

// ----------------------------------------------------------
// FUNCIONES FUERA DE LA CLASE
// No pueden ser métodos porque el primer operando NO es vect2.
// ----------------------------------------------------------

// 3 * v  (el 3 no es vect2, así que no puede ser método de la clase)
vect2 operator*(int scalar, const vect2& vec);

// cout << v  (el cout no es vect2, así que tampoco puede ser método)
// Imprime exactamente:  {x, y}
std::ostream& operator<<(std::ostream& os, const vect2& vec);

#endif
