#include "powerset_bits.h"

#include <stdio.h>
 #include <stdlib.h>
int main (int ac, char **av)
{
    if (ac < 2)
        return 0;

    int k = ac - 2;
    int target = atoi(av [1]);
    unsigned long total = 1UL << k;
    

    for (unsigned long mask = 0; mask < total; mask++)
    {
        int sum = 0;
        for (int i = 0; i < k; i++)
        {
            if (mask & (1UL << i))
                sum += atoi(av[2+i]);
        }
        if (sum == target)
        {
            int first = 1;
            for (int i = 0; i < k; i++)
            {
                if (mask & (1UL << i))
                {
                    if (!first)
                        printf(" ");
                    printf ("%s", av[2+i]);
                    first = 0;
                }
            }
            printf("\n");
        }
    }
    return 0;
}

/*********************
 * MANTRA

	"k elementos, 2^k máscaras"			1UL << k	
	"Bit activo → incluir"				mask & (1UL << i)	
	"saltamos programa y target"		av[i + 2]	
	"first controla espacios"			if (!first) printf(" ")	

 * 
 * 
 * 
 * 
 * ESTRUCTURA MENTAL RÁPIDA
 * 
	Valida argumentos.
	┌─────────────────────────────────┐
	│ SETUP: k, target, total (2^k)   │
	└────────────┬────────────────────┘
				 │
		┌────────▼────────┐
		│ FOR cada MÁSCARA│ ◄─── Externo (2^k veces)
		└────────┬────────┘			for (unsigned long mask = 0; mask < total; mask++)
				 │
		   ┌─────▼─────┐
		   │ sum = 0   │
		   │ FOR cada i│ ◄───── Interno (k veces)
		   │  sum? +=  │				for (int i = 0; i < k; i++)
		   └─────┬─────┘				if (mask & (1UL << i))
		  	     │
		   ┌─────▼─────┐
		   │ sum==targ?│
		   │  SI: PRINT│ ◄───── Interno (k veces)
		   └───────────┘				for (int i = 0; i < k; i++)
 * 										if (mask & (1UL << i))
 
 * 
 * 
 */

/*

int	main(int ac, char **av)
{
	if (ac < 2)
		return (0);
	
	int				k = ac - 2;
	int				target = atoi(av[1]);
	unsigned long	total = 1UL << k;
	int				found = 0;

	unsigned long	mask = 1;
	while (mask < total)
	{
		int			sum = 0;
		int			i = 0;
		while (i < k)
		{
			if (mask & (1UL << i))
				sum += atoi(av[i + 2]); //i+2 -> salta los 1ªs args (./a.out y target);
			++i;
		}

		if (sum == target)
		{
			int		first = 1;
			i = 0;
			while (i < k)
			{
				if (mask & (1UL << i))
				{
					if (!first)
						printf(" ");
					printf("%s", av[i + 2]);
					first = 0;
				}
				++i;
			}
			printf("\n");
			found = 1;
		}
		++mask;
	}
	if (!found)
		printf("\n");
	return (0);
}
	*/