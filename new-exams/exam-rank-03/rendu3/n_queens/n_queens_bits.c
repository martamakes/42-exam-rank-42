#include "n_queens_bits.h"

typedef unsigned long	mask;

void	disp(int *q, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		printf("%d", q[i]);
		if (i < n - 1)
			printf(" ");
		i++;
	}
	printf("\n");
}

void	bt(int *q, int col, int n, mask rows, mask d1, mask d2)
{
	int	r;

	if (col == n)
	{
		disp(q, n);
		return ;
	}
	r = 0;
	while (r < n)
	{
		mask	rm = 1UL << r;
		mask	d1m = 1UL << (col + r);
		mask	d2m = 1UL << (r - col + n);
		if (!(rows & rm) && !(d1 & d1m) && !(d2 & d2m))
		{
			q[col] = r;
			bt(q, col + 1, n, rows | rm, d1 | d1m, d2 | d2m);
		}
		r++;
	}
}
/* Limitación a 32 
int	main(int ac, char **av)
{
	int	n;                      // Tamaño del tablero (N reinas)
	int	q[32];                  // Array de solución (máximo 32 reinas = queens 'q')

	if (ac != 2)
		return (0);
	n = atoi(av[1]);
	bt(q, 0, n, 0, 0, 0);
	return (0);
}
*/
int main(int ac, char **av)
{
    int n;
    int *q;  // ← CAMBIO: Puntero en vez de array fijo
    
    if (ac != 2)
        return (0);
    n = atoi(av[1]);
    
    if (n <= 0 || n > 32)  // ← AÑADIR: Validación
        return (0);
    
    q = malloc(sizeof(int) * n);  // ← AÑADIR: Reservar memoria
    if (!q)
        return (0);
    
    bt(q, 0, n, 0, 0, 0);  // ← Igual que antes
    
    free(q);  // ← AÑADIR: Liberar memoria
    return (0);
}