#include "powerset.h"

int main (int ac, char **av)
{
	int k = ac - 2;
	unsigned long total = 1UL << k;
	int target = atoi(av[1]);

	if (ac < 2)
		return 0;

	for (unsigned long mask = 0; mask < total; mask++)
	{
		int sum = 0;
		for (int i = 0; i < k; i++)
		{
			if (mask & (1UL << i))
				sum += atoi(av[2 + i]);
		}
		if (sum == target)
		{
			int first = 1;
			for (int i = 0; i < k; i++)
			{
				if (mask & (1UL << i))
				{
					if(!first)
						printf(" ");
					printf ("%s", av[2 + i]);
					first = 0;
				}
			
			}
			if(!first)
				printf("\n");
		}
		
	}
	return 0;
}