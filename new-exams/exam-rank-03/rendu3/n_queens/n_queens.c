#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Escribir un byte con todas sus combinaciones*/

/*
void	imprimir(int *pos, int n)
{

	for (int i = 0; i < n; i++)
    {
        if (i > 0)
            printf(" ");
        printf("%d", pos[i]);
    }
    printf("\n");
}
	*/

int	pos_libre(int *pos, int fila, int columna)
{
	for (int i = 0; i < fila; i++)
	{
		if(pos[i] == columna || pos[i] - i == columna - fila || pos[i] + i == columna + fila)
			return(0);
	}
	return 1;
}

void	resolver(int *pos, int n, int fila)
{
	if (fila == n)
	{
		//imprimir(pos, n);
		//return;

		//directamnte:
		for (int i = 0; i < n; i++)
    	{
			if (i > 0)
				printf(" ");
			printf("%d", pos[i]);
    	}
   		printf("\n");
		return;
	}

	for (int columna = 0; columna < n; columna++)
	{
		if (pos_libre(pos, fila, columna))
		{
			pos[fila] = columna;
			resolver(pos, n, fila + 1);
		}
	}
}

int	main(int argc, char **argv)			//Validar → Crear → Backtracking → Liberar
{
	if (argc != 2)
		return 1;

	int n = atoi(argv[1]);
	if (n <= 0)
		return 1;

	int *pos = malloc (sizeof(int) * n);
	if (!pos)
	 return 1;
	
	resolver(pos, n, 0);
	free(pos);
	return(0);
}

/**
 * resolver(pos, n, 0);
            ↑    ↑  ↑
            │    │  └─ fila inicial (empezamos en fila 0), ¿en que fila estamos trabajando?
            │    └──── tamaño del tablero (N×N)
        	└───────── array donde guardamos posiciones, la solución.

			Las columnas se prueban TODAS dentro de la función.

	                resolver(pos, 4, 0)  ← fila = 0
                    /    |    |    \
               col=0  col=1 col=2 col=3  ← columnas (bucle for)
                 ↓      ↓     ↓     ↓
            resolver(pos, 4, 1)          ← fila = 1 (parámetro)
            /    |    |    \
       col=0  col=1 col=2 col=3          ← columnas (bucle for)
         ↓      ↓     ↓     ↓
    resolver(pos, 4, 2)                  ← fila = 2 (parámetro)


LLAMADA INICIAL:
resolver(pos, 4, 0)
         └────────┘
         Parámetros de ENTRADA

DENTRO DE LA FUNCIÓN:
for (columna = 0; columna < n; columna++)
     └─────────────────────────────────┘
     Variable LOCAL (no se pasa)

LLAMADA RECURSIVA:
resolver(pos, n, fila + 1)
                 └──────┘
                 Solo cambia FILA





 */