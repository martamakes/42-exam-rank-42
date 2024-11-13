/*
** flood_fill.c
**
** CONCEPTOS CLAVE:
**
** 1. Recursividad:
**    - La función se llama a sí misma para rellenar áreas adyacentes
**    - Cada llamada procesa una celda y sus vecinos
**
** 2. Flood Fill:
**    - Algoritmo de relleno por inundación
**    - Se expande desde un punto inicial
**    - Solo se mueve horizontal y verticalmente
**
** 3. Límites:
**    - Verificar bordes del array
**    - Verificar caracteres diferentes
**    - No rellenar en diagonal
*/

#include "flood_fill.h"

void flood_fill(char **tab, t_point size, t_point begin)
{
    // Verificar límites del array
    if (begin.y < 0 || begin.y >= size.y || begin.x < 0 || begin.x >= size.x)
        return;

    // Guardar el carácter original que debemos reemplazar
    static char target = 0;
    
    // Si es la primera llamada, guardamos el carácter objetivo
    if (target == 0)
        target = tab[begin.y][begin.x];
    
    // Si no es el carácter que buscamos o ya está rellenado, retornar
    if (tab[begin.y][begin.x] != target || tab[begin.y][begin.x] == 'F')
        return;

    // Rellenar la posición actual
    tab[begin.y][begin.x] = 'F';

    // Rellenar recursivamente los puntos adyacentes
    flood_fill(tab, size, (t_point){begin.x - 1, begin.y}); // izquierda
    flood_fill(tab, size, (t_point){begin.x + 1, begin.y}); // derecha
    flood_fill(tab, size, (t_point){begin.x, begin.y - 1}); // arriba
    flood_fill(tab, size, (t_point){begin.x, begin.y + 1}); // abajo
}