#include "permutations_backtracking.h"

int ft_strlen(char *s)
{
    int i = 0;
    while (s[i])
        i++;
    return i;
}

int ft_isalpha(int c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

void ft_swap(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

// Verifica si carácter ya está en uso
int ft_strchr(const char *s, char c)
{
    int i = 0;
    while (s[i])
    {
        if (s[i] == c)
            return 1;
        i++;
    }
    return 0;
}

// Ordena el string alfabéticamente (bubble sort)
char *order_string(char *s)
{
    int len = ft_strlen(s);
    int swapped = 1;

    while (swapped)
    {
        swapped = 0;
        int i = 0;
        while (i < len - 1)
        {
            if (s[i] > s[i + 1])
            {
                ft_swap(&s[i], &s[i + 1]);
                swapped = 1;
            }
            i++;
        }
    }
    return s;
}

// Genera todas las permutaciones usando backtracking
void generate_permutations(char *source, char *result, int pos)
{
    int source_len = ft_strlen(source);

    // Permutación completa, imprimir
    if (pos == source_len)
    {
        write(1, result, source_len);
        write(1, "\n", 1);
        return;
    }

    // Probar cada carácter de source
    int i = 0;
    while (i < source_len)
    {
        // Si el carácter no está usado
        if (!ft_strchr(result, source[i]))
        {
            result[pos] = source[i];        // Usar carácter
            generate_permutations(source, result, pos + 1);
            result[pos] = '\0';             // Backtrack
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    // Verificar string vacío
    if (ft_strlen(argv[1]) == 0 || (argv[1][0] == ' ' && !argv[1][1]))
        return 0;

    // Validar que solo contiene letras
    int i = 0;
    while (argv[1][i])
    {
        if (!ft_isalpha(argv[1][i]))
            return 0;
        i++;
    }

    int len = ft_strlen(argv[1]);

    // Alocar buffer para el resultado
    char *result = calloc(len + 1, 1);
    if (!result)
        return 1;

    // Ordenar el string alfabéticamente
    char *source = order_string(argv[1]);

    // Generar todas las permutaciones
    generate_permutations(source, result, 0);

    free(result);
    return 0;
}
