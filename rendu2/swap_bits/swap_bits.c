/*
** swap_bits.c
** 
** CONCEPTOS CLAVE:
** 
** 1. Bitwise Operations (Operaciones a nivel de bits):
**    & (AND): Compara bit a bit, devuelve 1 solo si ambos bits son 1
**    | (OR): Compara bit a bit, devuelve 1 si al menos un bit es 1
**    >> (right shift): Desplaza bits a la derecha
**    << (left shift): Desplaza bits a la izquierda
**
** 2. Máscaras de bits:
**    0xF0 = 11110000 en binario (para obtener los 4 bits más significativos)
**    0x0F = 00001111 en binario (para obtener los 4 bits menos significativos)
**
** 3. Ejemplo visual:
**    Entrada: 0100 0001
**    Salida:  0001 0100
*/

unsigned char swap_bits(unsigned char octet)
{
    // Obtenemos los 4 bits más significativos y los desplazamos a la derecha
    unsigned char high_nibble = (octet & 0xF0) >> 4;
    
    // Obtenemos los 4 bits menos significativos y los desplazamos a la izquierda
    unsigned char low_nibble = (octet & 0x0F) << 4;
    
    // Combinamos ambas partes usando OR
    return (high_nibble | low_nibble);
}