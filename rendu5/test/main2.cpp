// main2.cpp — prueba CADA operador por separado, aislado, con el resultado
// esperado escrito al lado de cada print. Compilar y ejecutar con:
//   c++ -Wall -Wextra -Werror main2.cpp ../vect2/vect2.cpp -o main2 && ./main2
//
// Objetivo: no probar "todo mezclado" como el main.cpp del subject, sino
// UNA cosa a la vez, para saber exactamente qué método falla si algo falla.

#include "../vect2/vect2.hpp"
#include <iostream>

// ============================================================
static void test_addition()
{
    std::cout << "\n-- Addition (operator+, operator+=) --\n";

    vect2 a(1, 2);
    vect2 b(10, 20);

    std::cout << "a + b = " << (a + b) << std::endl;      // esperado: {11, 22}
    std::cout << "a (sin tocar) = " << a << std::endl;     // esperado: {1, 2}  <- operator+ NO muta a

    a += b;
    std::cout << "a += b -> a = " << a << std::endl;       // esperado: {11, 22} <- ahora SÍ muta a
}

// ============================================================
static void test_subtraction()
{
    std::cout << "\n-- Subtraction (operator-, operator-=) --\n";

    vect2 a(10, 20);
    vect2 b(1, 2);

    std::cout << "a - b = " << (a - b) << std::endl;       // esperado: {9, 18}
    std::cout << "a (sin tocar) = " << a << std::endl;     // esperado: {10, 20}

    a -= b;
    std::cout << "a -= b -> a = " << a << std::endl;       // esperado: {9, 18}
}

// ============================================================
static void test_scalar_multiplication()
{
    std::cout << "\n-- Scalar multiplication (operator*, operator*=, 3 * v) --\n";

    vect2 a(2, 3);

    std::cout << "a * 3 = " << (a * 3) << std::endl;       // esperado: {6, 9}   <- método, vect2 a la izquierda
    std::cout << "3 * a = " << (3 * a) << std::endl;       // esperado: {6, 9}   <- función libre, int a la izquierda
    std::cout << "a (sin tocar) = " << a << std::endl;     // esperado: {2, 3}

    a *= 3;
    std::cout << "a *= 3 -> a = " << a << std::endl;       // esperado: {6, 9}
}

// ============================================================
static void test_unary_minus()
{
    std::cout << "\n-- Unary minus (operator-() , cero argumentos) --\n";

    vect2 a(2, -3);

    std::cout << "-a = " << (-a) << std::endl;             // esperado: {-2, 3}
    std::cout << "a (sin tocar) = " << a << std::endl;     // esperado: {2, -3}  <- unario tampoco muta
}

// ============================================================
static void test_increment_decrement()
{
    std::cout << "\n-- Increment / decrement (prefijo vs. postfijo) --\n";

    vect2 a(5, 5);

    std::cout << "a++ = " << (a++) << std::endl;           // esperado: {5, 5}  <- postfijo devuelve el valor VIEJO
    std::cout << "a (tras a++) = " << a << std::endl;      // esperado: {6, 6}  <- pero a ya quedó incrementado

    std::cout << "++a = " << (++a) << std::endl;           // esperado: {7, 7}  <- prefijo devuelve el valor NUEVO

    std::cout << "a-- = " << (a--) << std::endl;           // esperado: {7, 7}  <- postfijo: valor viejo
    std::cout << "a (tras a--) = " << a << std::endl;      // esperado: {6, 6}

    std::cout << "--a = " << (--a) << std::endl;           // esperado: {5, 5}  <- prefijo: valor nuevo
}

// ============================================================
static void test_access()
{
    std::cout << "\n-- Access (operator[] no-const y const) --\n";

    vect2 a(7, 8);
    const vect2 b(9, 10);

    std::cout << "a[0] = " << a[0] << std::endl;           // esperado: 7
    std::cout << "a[1] = " << a[1] << std::endl;           // esperado: 8

    a[0] = 100;                                            // solo compila porque operator[] no-const devuelve int&
    std::cout << "a[0] = 100 -> a = " << a << std::endl;   // esperado: {100, 8}

    std::cout << "b[0] = " << b[0] << std::endl;           // esperado: 9  <- b es const -> usa la versión const
    // b[0] = 42;  // <- si lo descomentas, NO compila: const int& no admite escritura
}

// ============================================================
static void test_comparison()
{
    std::cout << "\n-- Comparison (operator==, operator!=) --\n";

    vect2 a(1, 2);
    vect2 b(1, 2);
    vect2 c(9, 9);

    std::cout << "a == b : " << (a == b) << std::endl;     // esperado: 1 (true)  <- mismos componentes
    std::cout << "a == c : " << (a == c) << std::endl;     // esperado: 0 (false)
    std::cout << "a != c : " << (a != c) << std::endl;     // esperado: 1 (true)
    std::cout << "a != b : " << (a != b) << std::endl;     // esperado: 0 (false)
}

// ============================================================
int main()
{
    test_addition();
    test_subtraction();
    test_scalar_multiplication();
    test_unary_minus();
    test_increment_decrement();
    test_access();
    test_comparison();
    return (0);
}
