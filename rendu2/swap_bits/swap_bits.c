/**
 * swap_bits - Intercambia las mitades de un byte (4 bits altos y 4 bits bajos)
 * @octet: El byte que queremos manipular
 *
 * Return: El byte resultante después de intercambiar sus mitades
 */
unsigned char swap_bits(unsigned char octet)
{
    // Operación compacta: desplaza los 4 bits altos a la derecha y los 4 bits bajos a la izquierda
    // y los combina con OR bit a bit
    return ((octet >> 4) | (octet << 4));
}

/*
 * EXPLICACIÓN DETALLADA:
 *
 * Este código es mucho más simple que mi solución original, y realiza exactamente 
 * la misma operación, pero de manera más concisa.
 *
 * 1. octet >> 4: Desplaza los 4 bits más significativos (izquierda) hacia la derecha
 *    Ejemplo: Si octet = 0x41 (0100 0001), entonces octet >> 4 = 0x04 (0000 0100)
 *
 * 2. octet << 4: Desplaza los 4 bits menos significativos (derecha) hacia la izquierda
 *    Ejemplo: Si octet = 0x41 (0100 0001), entonces octet << 4 = 0x10 (0001 0000)
 *
 * 3. Finalmente, los combinamos con OR (|):
 *    0000 0100 | 0001 0000 = 0001 0100 (0x14)
 *
 * En un unsigned char (8 bits), este enfoque funciona perfectamente porque:
 * - Al desplazar 4 bits a la derecha, los bits sobrantes se descartan automáticamente
 * - Al desplazar 4 bits a la izquierda, los bits sobrantes también se descartan
 * - Al hacer OR, cada mitad acaba en la posición correcta sin interferir con la otra
 *
 * VENTAJAS DE ESTA SOLUCIÓN:
 * - Es extremadamente concisa (una sola línea)
 * - Es muy eficiente (solo dos operaciones bit a bit)
 * - Es fácil de entender conceptualmente
 * - No necesita máscaras adicionales para un unsigned char de 8 bits
 */