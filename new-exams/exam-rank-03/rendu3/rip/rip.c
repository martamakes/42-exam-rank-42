#include <unistd.h>
#include <stdio.h>

int ft_strlen(char *s)
{
    int i = 0;
    while (s[i])
        i++;
    return i;
}

// Verifica si la cadena tiene paréntesis balanceados
int is_balanced(char *str, int len)
{
    int balance = 0;
    int i = 0;

    while (i < len)
    {
        if (str[i] == '(')
            balance++;
        else if (str[i] == ')')
        {
            balance--;
            if (balance < 0)  // Más ')' que '('
                return 0;
        }
        i++;
    }

    return (balance == 0);
}

// Encuentra el mínimo número de paréntesis a remover
void find_min_removals(char *str, int *min_removals, int index, int current_removals)
{
    // Poda: si ya removimos más que el mínimo conocido, retornar
    if (current_removals > *min_removals)
        return;

    // Si está balanceado, actualizar mínimo
    if (is_balanced(str, ft_strlen(str)))
    {
        if (current_removals < *min_removals)
            *min_removals = current_removals;
        return;
    }

    // Probar remover cada paréntesis
    int i = index;
    while (str[i])
    {
        if (str[i] == '(' || str[i] == ')')
        {
            char saved = str[i];
            str[i] = ' ';  // Remover (reemplazar con espacio)

            find_min_removals(str, min_removals, i + 1, current_removals + 1);

            str[i] = saved;  // Restaurar
        }
        i++;
    }
}

// Genera todas las soluciones removiendo exactamente min_removals paréntesis
void generate_solutions(char *str, int min_removals, int index, int current_removals)
{
    // Poda
    if (current_removals > min_removals)
        return;

    // Si removimos el mínimo exacto y está balanceado, imprimir
    if (is_balanced(str, ft_strlen(str)) && current_removals == min_removals)
    {
        write(1, str, ft_strlen(str));
        write(1, "\n", 1);
        return;
    }

    // Probar remover cada paréntesis
    int i = index;
    while (str[i])
    {
        if (str[i] == '(' || str[i] == ')')
        {
            char saved = str[i];
            str[i] = ' ';

            generate_solutions(str, min_removals, i + 1, current_removals + 1);

            str[i] = saved;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2 || argv[1][0] == '\0')
        return 1;

    // Validar que solo contiene paréntesis
    int i = 0;
    while (argv[1][i])
    {
        if (argv[1][i] != '(' && argv[1][i] != ')')
            return 1;
        i++;
    }

    int min_removals = ft_strlen(argv[1]);  // Inicializar con máximo posible

    // Encontrar mínimo número de remociones
    find_min_removals(argv[1], &min_removals, 0, 0);

    // Generar todas las soluciones válidas
    generate_solutions(argv[1], min_removals, 0, 0);

    return 0;
}
