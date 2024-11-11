#include <stdio.h>

// unsigned char swap_bits(unsigned char octet);

// int main() {
//     unsigned char oct = 2; // Valor inicial
//     printf("Valor original: %d\n", (unsigned int)oct); // Imprime el valor original

//     oct = swap_bits(oct); // Llama a la función swap_bits
//     printf("Valor después de swap_bits: %u\n", (unsigned int)oct); // Imprime el valor intercambiado

//     return 0;
// }

unsigned char swap_bits(unsigned char octet) 
{
    return ((octet << 4) | (octet >> 4));
}