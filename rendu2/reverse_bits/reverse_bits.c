/*
** DESCRIPCIÓN: Esta función toma un byte (8 bits) y devuelve el mismo byte
** con los bits en orden inverso.
**
** PARÁMETROS:
** - octet: El byte que queremos invertir
**
** RETORNO:
** - El byte con sus bits en orden inverso
**
** EJEMPLO:
** Input:  0100 0001
** Output: 1000 0010
*/

/* 
** CONCEPTOS CLAVE:
** 1. Bitwise Operations (Operaciones a nivel de bits):
**    - & (AND): Se usa para "enmascarar" bits
**    - << (left shift): Desplaza bits a la izquierda
**    - >> (right shift): Desplaza bits a la derecha
**
** 2. Algoritmo:
**    - Iteramos 8 veces (un byte = 8 bits)
**    - En cada iteración:
**      a) Extraemos el bit menos significativo del byte original
**      b) Desplazamos el resultado a la izquierda
**      c) Añadimos el bit extraído al resultado
**      d) Desplazamos el byte original a la derecha
*/

unsigned char	reverse_bits(unsigned char octet)
{
    unsigned char	result;
    int				i;

    result = 0;
    i = 8;  // Un byte tiene 8 bits
    while (i > 0)
    {
        result = (result << 1) | (octet & 1);  // Desplazamos y añadimos el bit
        octet = octet >> 1;  // Preparamos el siguiente bit
        i--;
    }
    return (result);
}