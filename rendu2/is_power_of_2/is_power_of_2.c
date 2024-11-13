/* 
** CONCEPTOS CLAVE:
**
** 1. Potencias de 2 en binario:
**    - 1    = 00000001  (2^0)
**    - 2    = 00000010  (2^1)
**    - 4    = 00000100  (2^2)
**    - 8    = 00001000  (2^3)
**    - 16   = 00010000  (2^4)
**    Las potencias de 2 siempre tienen UN SOLO bit en 1
**    
** 2. Bitwise Operations:
**    - n & (n-1): Si n es potencia de 2, esta operación da 0
**    Ejemplo con 8 (1000):
**      8  = 1000
**      7  = 0111
**      &  = 0000
**    
** 3. Caso especial:
**    - El 0 no es potencia de 2
**    - El 1 es 2^0, por lo tanto es potencia de 2
*/

int is_power_of_2(unsigned int n)
{
    // Si n es 0, retornamos 0
    if (n == 0)
        return (0);
    
    // Verificamos si solo tiene un bit en 1
    // usando la operación n & (n-1)
    return ((n & (n - 1)) == 0);
}