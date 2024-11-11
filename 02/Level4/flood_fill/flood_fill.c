// Definición de la estructura t_point para representar coordenadas
typedef struct s_point
{
    int x;  // Coordenada x
    int y;  // Coordenada y
} t_point;

// Función recursiva que realiza el relleno
void fill(char **tab, t_point size, t_point cur, char to_fill)
{
    // Comprueba si estamos fuera de los límites del área o si el punto actual
    // no es del color que queremos rellenar
    if (cur.y < 0 || cur.y >= size.y || cur.x < 0 || cur.x >= size.x || tab[cur.y][cur.x] != to_fill)
        return;  // Si es así, terminamos esta llamada recursiva

    // Si llegamos aquí, estamos en un punto válido para rellenar
    tab[cur.y][cur.x] = 'F';  // Rellenamos este punto

    // Llamamos recursivamente a fill para los cuatro puntos adyacentes:

    // Punto a la izquierda
    fill(tab, size, (t_point){cur.x - 1, cur.y}, to_fill);
    
    // Punto a la derecha
    fill(tab, size, (t_point){cur.x + 1, cur.y}, to_fill);
    
    // Punto arriba
    fill(tab, size, (t_point){cur.x, cur.y - 1}, to_fill);
    
    // Punto abajo
    fill(tab, size, (t_point){cur.x, cur.y + 1}, to_fill);
}

// Función principal flood_fill que inicia el proceso de relleno
void flood_fill(char **tab, t_point size, t_point begin)
{
    // Llamamos a fill con el punto de inicio y el color original en ese punto
    fill(tab, size, begin, tab[begin.y][begin.x]);
}