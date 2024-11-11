/*
Programa: print_hex

Este programa convierte un número decimal positivo (o cero) a su representación
hexadecimal y lo imprime en minúsculas. Acepta un único argumento de línea de
comandos. Si no se proporciona un argumento o si se proporcionan más de uno,
simplemente imprime un salto de línea.

Funcionamiento:
1. Verifica el número de argumentos.
2. Convierte el argumento de cadena a entero.
3. Convierte el entero a hexadecimal y lo imprime.
4. Siempre termina con un salto de línea.
*/

#include <unistd.h>

// Función para imprimir un número en hexadecimal
void print_hex(int n)
{
    // Array de caracteres para mapear valores decimales a hexadecimales
    char hex_digits[] = "0123456789abcdef";
    
    // Caso recursivo: si n es 16 o mayor, primero imprimimos los dígitos más significativos
    if (n >= 16)
        print_hex(n / 16);
    
    // Imprimimos el dígito hexadecimal actual
    write(1, &hex_digits[n % 16], 1);
}

// Función para convertir una cadena a entero (versión simplificada de atoi)
int ft_atoi(char *str)
{
    int result = 0;
    
    // Convertimos cada dígito de la cadena a su valor numérico
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

int main(int argc, char **argv)
{
    // Verificamos si hay exactamente un argumento (además del nombre del programa)
    if (argc == 2)
    {
        // Convertimos el argumento a entero
        int num = ft_atoi(argv[1]);
        
        // Caso especial: si el número es 0, imprimimos "0" directamente
        if (num == 0)
            write(1, "0", 1);
        else
            // Para otros números, llamamos a print_hex
            print_hex(num);
    }
    
    // Siempre imprimimos un salto de línea al final
    write(1, "\n", 1);
    return 0;
}