/* 
** CONCEPTOS CLAVE:
**
** 1. Operadores a nivel de bits (Bitwise Operators):
**    & (AND): Compara bit a bit, devuelve 1 solo si ambos bits son 1
**    >> (Right Shift): Desplaza los bits a la derecha
**    
** 2. Representación binaria:
**    - Un byte (unsigned char) tiene 8 bits
**    - Cada bit puede ser 0 o 1
**    - El bit más significativo está a la izquierda
**    
** 3. Máscara de bits:
**    - Usando 1 << i creamos una máscara con un 1 en la posición i
**    - Al hacer AND (&) con esta máscara, aislamos el bit en esa posición
*/

void    print_bits(unsigned char octet)
{
    int     i;
    char    bit;

    // Empezamos por el bit más significativo (posición 7)
    i = 7;
    while (i >= 0)
    {
        // Creamos una máscara con 1 en la posición i
        // y hacemos AND con el byte
        bit = ((octet >> i) & 1) + '0';
        write(1, &bit, 1);
        i--;
    }
}