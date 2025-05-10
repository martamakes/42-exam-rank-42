/* 
** add_prime_sum.c
**
** CONCEPTOS CLAVE:
**
** 1. Números Primos:
**    - Un número primo es divisible solo por 1 y por sí mismo
**    - Optimizaciones para verificar primalidad:
**      * Verificar solo hasta la raíz cuadrada
**      * Saltar múltiplos de 2 y 3
**
** 2. Funciones permitidas:
**    - write(fd, buffer, count): escribir en file descriptor
**      fd: file descriptor (1 para stdout)
**      buffer: datos a escribir
**      count: número de bytes a escribir
**    - exit: terminar el programa
**
** 3. Manejo de argumentos:
**    - argc: número de argumentos (incluyendo nombre del programa)
**    - argv: array de strings con los argumentos
**    - Validación de entrada y conversión a int
**
** 4. Suma acumulativa:
**    - Mantener una suma mientras encontramos primos
**    - Convertir el resultado final a caracteres para output
*/

#include <unistd.h>

// Función auxiliar para escribir un caracter
void    ft_putchar(char c)
{
    write(1, &c, 1);
}

// Función recursiva para escribir un número
void    ft_putnbr(int n)
{
    if (n >= 10)
        ft_putnbr(n / 10);
    ft_putchar(n % 10 + '0');
}

// Convierte string a int y valida la entrada
int     ft_atoi(char *str)
{
    int num = 0;
    int sign = 1;
    
    // Manejar espacios iniciales y signos
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    if (*str == '-')
        return (-1);  // Números negativos no son válidos
    if (*str == '+')
        str++;
    
    // Convertir dígitos a número
    while (*str >= '0' && *str <= '9')
    {
        num = num * 10 + (*str - '0');
        str++;
    }
    
    // Si quedan caracteres, la entrada es inválida
    if (*str != '\0')
        return (-1);
        
    return (num * sign);
}

// Función para verificar si un número es primo
int is_prime(int n)
{
    int i = 2;

    if (n <= 1)
        return 0; // Los números menores o iguales a 1 no son primos
    while (i * i <= n) // Solo verificamos hasta la raíz cuadrada de n
    {
        if (n % i == 0) // Si n es divisible por i, no es primo
            return 0;
        i++;
    }
    return 1; // Si no se encontró ningún divisor, n es primo
}

int     main(int argc, char **argv)
{
    int num;
    int sum;
    int i;

    // Verificar número de argumentos
    if (argc != 2)
    {
        write(1, "0\n", 2);
        return (0);
    }

    // Convertir y validar entrada
    num = ft_atoi(argv[1]);
    if (num <= 0)
    {
        write(1, "0\n", 2);
        return (0);
    }

    // Calcular suma de primos hasta num
    sum = 0;
    i = 2;  // Empezar desde el primer primo
    while (i <= num)
    {
        if (is_prime(i))
            sum += i;
        i++;
    }
    
    // Mostrar resultado
    ft_putnbr(sum);
    ft_putchar('\n');
    return (0);
}