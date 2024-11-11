/* ====================================================
 * FIZZBUZZ
 * Conceptos clave:
 * 1. Operador módulo (%):
 *    - Devuelve el resto de una división
 *    - Si n % x == 0, significa que n es múltiplo de x
 * 
 * 2. Write:
 *    - Prototipo: ssize_t write(int fd, const void *buf, size_t count)
 *    - fd 1 es la salida estándar
 *    - Devuelve el número de bytes escritos
 * ==================================================== */

#include <unistd.h>

// Función auxiliar para escribir números
void	ft_putnbr(int n)
{
    char	c;

    // Si el número es mayor que 9, llamamos recursivamente
    // para escribir primero los dígitos más significativos
    if (n > 9)
        ft_putnbr(n / 10);
    
    // Convertimos el último dígito a char y lo escribimos
    c = (n % 10) + '0';
    write(1, &c, 1);
}

int	main(void)
{
    int	i;

    i = 1;
    // Iteramos del 1 al 100
    while (i <= 100)
    {
        // Comprobamos primero si es múltiplo de ambos (3 y 5)
        // ya que es el caso más específico
        if (i % 3 == 0 && i % 5 == 0)
            write(1, "fizzbuzz", 8);
        // Si no es múltiplo de ambos, comprobamos si es múltiplo de 3
        else if (i % 3 == 0)
            write(1, "fizz", 4);
        // Si no es múltiplo de 3, comprobamos si es múltiplo de 5
        else if (i % 5 == 0)
            write(1, "buzz", 4);
        // Si no es múltiplo de ninguno, escribimos el número
        else
            ft_putnbr(i);
        
        // Después de cada número o palabra, escribimos un salto de línea
        write(1, "\n", 1);
        i++;
    }
    return (0);
}