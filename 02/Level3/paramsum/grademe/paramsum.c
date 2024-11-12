/* 
** paramsum.c
**
** CONCEPTOS CLAVE:
**
** 1. argc y argv:
**    - argc: número de argumentos (incluyendo el nombre del programa)
**    - argv: array de strings con los argumentos
**    - argv[0] es siempre el nombre del programa
**
** 2. write:
**    - Función de bajo nivel para escribir en un file descriptor
**    - write(fd, buffer, count)
**      fd: file descriptor (1 para stdout)
**      buffer: datos a escribir
**      count: número de bytes a escribir
**
** 3. Conversion de int a char:
**    - Necesitamos convertir números a caracteres para mostrarlos
**    - '0' + n convierte un dígito n a su carácter correspondiente
*/

#include <unistd.h>

// Función auxiliar para escribir un número
void    ft_putnbr(int n)
{
    char    c;

    if (n >= 10)
        ft_putnbr(n / 10);
    c = '0' + (n % 10);
    write(1, &c, 1);
}

int main(int argc, char **argv)
{
    (void)argv;  // Evitar warning de variable no usada
    if(argc < 2)
        ft_putnbr(0);
    else if(argc >= 2)
    {
    // argc incluye el nombre del programa, así que restamos 1
        ft_putnbr(argc - 1);
    }
     write(1, "\n", 1);
    return (0);
}