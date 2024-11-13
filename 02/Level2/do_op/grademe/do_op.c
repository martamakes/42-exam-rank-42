/* 
** CONCEPTOS CLAVE:
**
** 1. Operadores aritméticos:
**    + : suma
**    - : resta
**    * : multiplicación
**    / : división
**    % : módulo (resto de división)
**    
** 2. Manejo de argumentos:
**    - Verificar que hay exactamente 3
**    - Convertir strings a números con atoi
**    - Identificar el operador
**    
** 3. Casos especiales:
**    - Números negativos: ya manejados por atoi
**    - División/módulo por cero: no especificado en subject
**    - Resultado debe caber en un int
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int num1;
    int num2;
    
    // Si no hay exactamente 3 argumentos
    if (argc != 4)
    {
        printf("\n");
        return (0);
    }
    
    // Convertir strings a números
    num1 = atoi(argv[1]);
    num2 = atoi(argv[3]);
    
    // Realizar la operación según el operador
    switch (argv[2][0])
    {
        case '+':
            printf("%d\n", num1 + num2);
            break;
        case '-':
            printf("%d\n", num1 - num2);
            break;
        case '*':
            printf("%d\n", num1 * num2);
            break;
        case '/':
            printf("%d\n", num1 / num2);
            break;
        case '%':
            printf("%d\n", num1 % num2);
            break;
        default:
            printf("\n");
    }
    
    return (0);
}