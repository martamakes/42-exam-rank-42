/*
** DESCRIPCIÓN: Calcula el Mínimo Común Múltiplo (MCM) de dos números.
**
** PARÁMETROS:
** - a: Primer número entero positivo
** - b: Segundo número entero positivo
**
** RETORNO:
** - El MCM de los dos números, o 0 si alguno es 0
**
** EJEMPLO:
** lcm(12, 18) retorna 36 porque:
** - Múltiplos de 12: 12, 24, 36, 48, ...
** - Múltiplos de 18: 18, 36, 54, ...
** - 36 es el primer múltiplo común
*/

/* 
** CONCEPTOS CLAVE:
** 1. Algoritmo de Euclides para MCD:
**    - Se basa en que MCD(a,b) = MCD(b, a % b)
**    - Se repite hasta que el resto sea 0
**    Ejemplo: MCD(48,18)
**    48 = 2 * 18 + 12
**    18 = 1 * 12 + 6
**    12 = 2 * 6 + 0
**    MCD = 6
**
** 2. Relación MCM y MCD:
**    MCM(a,b) = |a * b| / MCD(a,b)
**    Esta fórmula es más eficiente que calcular múltiplos
*/

// Función auxiliar para calcular el MCD usando el algoritmo de Euclides
unsigned int    get_gcd(unsigned int a, unsigned int b)
{
    unsigned int    temp;

    // Aplicamos el algoritmo de Euclides
    while (b != 0)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return (a);
}

unsigned int    lcm(unsigned int a, unsigned int b)
{
    unsigned int    gcd;

    // Si alguno de los números es 0, el MCM es 0
    if (a == 0 || b == 0)
        return (0);

    // Calculamos el MCD
    gcd = get_gcd(a, b);

    // Calculamos el MCM usando la fórmula: MCM = |a * b| / MCD
    // Para evitar desbordamiento, primero dividimos y luego multiplicamos
    return ((a / gcd) * b);
}