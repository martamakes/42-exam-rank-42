#include <stdio.h>

// Declaraciones de las funciones
unsigned int gcd(unsigned int a, unsigned int b);
unsigned int lcm(unsigned int a, unsigned int b);

int main()
{
    // Array de pares de números para probar
    unsigned int test_cases[][2] = {
        {12, 18},    // Caso común
        {5, 7},      // Números primos
        {0, 5},      // Un número es cero
        {0, 0},      // Ambos números son cero
        {1, 1},      // Ambos números son uno
        {100, 75},   // Números más grandes
        {3, 7},      // Números primos entre sí
        {24, 36}     // Números con factores comunes
    };

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    // Ejecutar pruebas
    for (int i = 0; i < num_tests; i++)
    {
        unsigned int a = test_cases[i][0];
        unsigned int b = test_cases[i][1];
        unsigned int result = lcm(a, b);

        printf("LCM de %u y %u es: %u\n", a, b, result);
    }

    return 0;
}

/*
Explicación detallada:
Función gcd:
Esta función implementa el algoritmo de Euclides para calcular el Máximo Común Divisor (GCD).
Es una implementación recursiva del algoritmo.
El caso base es cuando b es 0, en cuyo caso el GCD es a.
En cada llamada recursiva, a se convierte en b, y b se convierte en el residuo de a / b.
Este proceso continúa hasta que el residuo es 0, momento en el que hemos encontrado el GCD.

Función lcm:
Primero, manejamos el caso especial donde alguno de los números es 0. En este caso, el LCM se define como 0.
Luego, calculamos el GCD de a y b usando la función gcd.
Calculamos el producto de a y b. No necesitamos preocuparnos por el valor absoluto ya que estamos trabajando con unsigned int, que siempre son positivos.
Finalmente, aplicamos la fórmula LCM(a, b) = |a * b| / GCD(a, b).
Dividimos el producto por el GCD para obtener el LCM.

Eficiencia y matemáticas:
Este método es más eficiente que calcular múltiplos de cada número hasta encontrar uno común.
Utiliza la relación matemática entre LCM y GCD: LCM(a, b) * GCD(a, b) = |a * b|.
El algoritmo de Euclides para el GCD es muy eficiente, con una complejidad logarítmica.

Uso de tipos unsigned:
Al usar unsigned int, nos aseguramos de que todos los números sean positivos.
Esto simplifica el cálculo al eliminar la necesidad de manejar valores absolutos.

Ausencia de bucles for y operadores ternarios:
La recursión en gcd elimina la necesidad de un bucle explícito.
Las comprobaciones se hacen con simples declaraciones if, evitando operadores ternarios.



*/