/* 
** CONCEPTOS CLAVE:
**
** 1. Sistema hexadecimal:
**    - Base 16 usa dígitos: 0-9 y a-f
**    - Cada dígito representa 4 bits
**    - Conversión decimal a hex: dividir por 16 repetidamente
**    
** 2. ASCII:
**    - Números 0-9: ASCII 48-57
**    - Letras a-f: ASCII 97-102
**    
** 3. Manejo de argumentos:
**    - Convertir string a número
**    - Verificar que sea número válido
**    - Manejar caso sin argumentos
*/

#include <unistd.h>

// Función auxiliar para escribir un caracter
void    ft_putchar(char c)
{
    write(1, &c, 1);
}

// Función para convertir string a número
int     ft_atoi(char *str)
{
    int num;
    int i;

    num = 0;
    i = 0;
    while (str[i] >= '0' && str[i] <= '9')
    {
        num = num * 10 + (str[i] - '0');
        i++;
    }
    return (num);
}

// Función para imprimir en hexadecimal
void    print_hex_recursive(int num)
{
    char hex_digits[] = "0123456789abcdef";
    
    if (num >= 16)
        print_hex_recursive(num / 16);
    ft_putchar(hex_digits[num % 16]);
}

int     main(int argc, char **argv)
{
    // Si no hay exactamente 1 argumento
    if (argc != 2)
    {
        write(1, "\n", 1);
        return (0);
    }
    
    // Convertir string a número y mostrar en hex
    print_hex_recursive(ft_atoi(argv[1]));
    write(1, "\n", 1);
    return (0);
}