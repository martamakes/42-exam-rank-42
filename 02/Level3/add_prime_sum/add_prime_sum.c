/*
Asignación: add_prime_sum

Este programa toma un número entero positivo como argumento y muestra la suma
de todos los números primos menores o iguales a él, seguido de un salto de línea.

Si el número de argumentos no es 1, o si el argumento no es un número positivo,
muestra 0 seguido de un salto de línea.

Ejemplos:
./add_prime_sum 5
10
suma ( 2 + 3 + 5)
./add_prime_sum 7
17 (2 + 3 + 5 + 7)
./add_prime_sum
0
*/

#include <unistd.h>

// Función para escribir un número en stdout
void ft_putnbr(int n)
{
    char digit;

    if (n >= 10)
        ft_putnbr(n / 10);
    digit = n % 10 + '0';
    write(1, &digit, 1);
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

// Función para convertir una cadena a un entero
int ft_atoi(char *str)
{
    int result = 0;

    while (*str >= '0' && *str <= '9') // Convertir caracteres numéricos a entero
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

int main(int argc, char **argv)
{
    int num;
    int sum = 0;
    int i = 2;

    // Comprobar si hay exactamente un argumento
    if (argc != 2)
    {
        write(1, "0\n", 2); // Si no hay un argumento válido, imprimir 0
        return 0;
    }

    // Convertir el argumento a entero
    num = ft_atoi(argv[1]);

    // Comprobar si el número es positivo
    if (num <= 0)
    {
        write(1, "0\n", 2); // Si no es positivo, imprimir 0
        return 0;
    }

    // Calcular la suma de los números primos
    while (i <= num)
    {
        if (is_prime(i)) // Verificar si i es primo
            sum += i; // Si es primo, agregarlo a la suma
        i++;
    }

    // Mostrar el resultado
    ft_putnbr(sum); // Imprimir la suma de los primos
    write(1, "\n", 1); // Imprimir un salto de línea

    return 0;
}