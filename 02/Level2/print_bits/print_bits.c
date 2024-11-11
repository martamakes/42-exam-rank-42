#include <unistd.h>

void print_bits(unsigned char octet) {
    for (int i = 7; i >= 0; i--) { // Iteramos desde el bit más significativo al menos significativo
        char bit = ((octet >> i) & 1) + '0'; // Desplazamos el bit a la posición menos significativa y convertimos a carácter
        write(1, &bit, 1); // Escribimos el bit en la salida estándar
    }
}