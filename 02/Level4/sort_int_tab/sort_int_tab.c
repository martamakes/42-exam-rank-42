/*
Assignment name  : sort_int_tab
Expected files   : sort_int_tab.c
Allowed functions: None

Este programa debe implementar una función que ordene un array de enteros
en orden ascendente. Los requisitos son:

1. La función debe ordenar el array 'in-place' (sin usar memoria adicional).
2. Debe preservar los números duplicados.
3. El array de entrada siempre será coherente (válido).
4. La función debe tener la siguiente firma:
   void sort_int_tab(int *tab, unsigned int size);

Donde 'tab' es el array de enteros a ordenar y 'size' es el número de elementos en el array.
*/

void	sort_int_tab(int *tab, unsigned int size)
{
	unsigned int	i = 0;  // Índice para recorrer el array
	int				temp;   // Variable temporal para el intercambio

	// Recorremos el array hasta el penúltimo elemento
	while (i < (size - 1))
	{
		// Si el elemento actual es mayor que el siguiente, los intercambiamos
		if (tab[i] > tab[i + 1])
		{
			// Realizamos el intercambio
			temp = tab[i];
			tab[i] = tab[i + 1];
			tab[i + 1] = temp;

			// Reiniciamos el índice para volver a empezar
			// Esto asegura que comprobemos nuevamente desde el principio
			i = 0;
		}
		else
		{
			// Si no hubo intercambio, avanzamos al siguiente elemento
			i++;
		}
	}
}