#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct {
    float x, y;
} City;

// Calcula distancia euclidiana entre dos ciudades
float calculate_distance(City a, City b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

// Calcula distancia total de un recorrido completo (incluyendo vuelta)
float calculate_total_distance(City *cities, int *path, int n)
{
    float total = 0.0f;
    int i;

    // Distancias entre ciudades consecutivas
    for (i = 0; i < n - 1; i++)
    {
        total += calculate_distance(cities[path[i]], cities[path[i + 1]]);
    }

    // Distancia de vuelta al inicio (cerrar el ciclo)
    total += calculate_distance(cities[path[n - 1]], cities[path[0]]);

    return total;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Genera todas las permutaciones y encuentra el camino más corto
void find_shortest_path(City *cities, int *path, int n, int pos, float *min_distance)
{
    if (pos == n)
    {
        float current_distance = calculate_total_distance(cities, path, n);

        if (current_distance < *min_distance)
            *min_distance = current_distance;

        return;
    }

    // Generar permutaciones intercambiando elementos
    for (int i = pos; i < n; i++)
    {
        swap(&path[pos], &path[i]);              // Intercambiar
        find_shortest_path(cities, path, n, pos + 1, min_distance);
        swap(&path[pos], &path[i]);              // Restaurar
    }
}

int main(void)
{
    City cities[12];  // Máximo 11 ciudades
    int n = 0;

    // Leer coordenadas desde stdin
    while (n < 11 && fscanf(stdin, "%f, %f", &cities[n].x, &cities[n].y) == 2)
    {
        n++;
    }

    if (n < 2)
    {
        printf("0.00\n");
        return 0;
    }

    // Inicializar path: [0, 1, 2, ..., n-1]
    int path[12];
    for (int i = 0; i < n; i++)
    {
        path[i] = i;
    }

    float min_distance = FLT_MAX;

    // Optimización: fijar primera ciudad y permutar solo las demás
    // Reduce complejidad de n! a (n-1)!
    find_shortest_path(cities, path, n, 1, &min_distance);

    printf("%.2f\n", min_distance);

    return 0;
}
