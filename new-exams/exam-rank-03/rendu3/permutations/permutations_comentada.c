#include "permutations_comentada.h"

void perm(int *cnt, int n, int pos, char *buf)
{
    if (pos == n)
    {
        buf[n] = '\0';
        puts(buf);
        return;
    }

    for (int c = 0; c < 256; c++)
    {
        if (cnt[c])
        {
            buf[pos] = c;
            cnt[c]--;
            perm(cnt, n, pos + 1, buf);
            cnt[c]++;  // Backtrack
        }
    }
}

int main(int ac, char **av)
{
    if (ac != 2 || !av[1][0])
        return (1);
    
    int cnt[256] = {0};
    int n = 0;
    
    while (av[1][n])
        cnt[(unsigned char)av[1][n++]]++;
    
    char *buf = malloc(n + 1);
    if (!buf)
        return (1);
    
    perm(cnt, n, 0, buf);
    free(buf);
    return (0);
}





/************************
 * Generación recurasiva:
 * cnt: Array de frecuencias (cuántos de cada carácter quedan)
 * n: Longitud total de la cadena
 * pos: Posición actual que estamos llenando
 * buf: Buffer donde construimos la permutación
 ******************************************************/
/*
int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}
void	perm(int *cnt, int n, int pos, char *buf)
{
	int	c;

	if (pos == n)
	{
		buf[n] = '\0';
		puts(buf);
		return ;
	}
	c = 0;
	while (c < 256)							// Recorre TODOS los caracteres ASCII posibles
	{
		if (cnt[c])							// Si tenemos al menos uno de este carácter disponible
		{
			buf[pos] = (char)c;				// Lo colocamos en la posición actual
			--cnt[c];						// "Gastamos" uno
			perm(cnt, n, pos + 1, buf);	    // Recursión: siguiente posición
			++cnt[c];						// BACKTRACK: lo devolvemos (restauramos)
		}
		c++;
	}
}

int	main(int ac, char **av)
{
	if (ac == 2 && av[1][0])
	{
		int		i;
		int		n;
		int		cnt[256] = {0};				// Array contador para cada carácter ASCII posible
		char	*buf;

		n = ft_strlen(av[1]);
		buf = malloc(n + 1);
		if (!buf)
			return (1);
		
		i = 0;
		while (i < n)
		{
			++cnt[(unsigned char)av[1][i]];	// Incrementa contador del carácter que está en la posición i de la cadena.
			i++;
		}
		perm(cnt, n, 0, buf);
		free(buf);
	}
	return (0);
}
*/