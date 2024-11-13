#include <stdio.h>
#include <stdlib.h>

// Variable global para contar errores
int g_tests_failed = 0;

// Prototipo de la función
int *ft_range(int start, int end);

// Función auxiliar para verificar array
void check_array(char *test_name, int start, int end, int *result)
{
    int size = (end >= start) ? (end - start + 1) : (start - end + 1);
    int expected;
    int i = 0;
    int is_correct = 1;
    
    printf("Test %s: ", test_name);
    
    // Si malloc falló
    if (!result)
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  malloc falló\n");
        g_tests_failed++;
        return;
    }

    // Verificamos cada elemento
    while (i < size)
    {
        if (end >= start)
            expected = start + i;
        else
            expected = start - i;
            
        if (result[i] != expected)
        {
            is_correct = 0;
            break;
        }
        i++;
    }

    if (is_correct)
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Start: %d, End: %d\n", start, end);
        printf("  Expected[%d]: %d\n", i, expected);
        printf("  Got[%d]: %d\n", i, result[i]);
        g_tests_failed++;
    }

    free(result);
}

int main(void)
{
    printf("\n=== Testing ft_range ===\n\n");

    // Test casos básicos
    check_array("Rango positivo simple", 1, 3, ft_range(1, 3));
    check_array("Rango negativo a positivo", -1, 2, ft_range(-1, 2));
    check_array("Mismo número", 0, 0, ft_range(0, 0));
    check_array("Rango descendente", 0, -3, ft_range(0, -3));

    // Test casos adicionales
    check_array("Rango grande positivo", 0, 10, ft_range(0, 10));
    check_array("Rango grande negativo", -10, 0, ft_range(-10, 0));
    check_array("Rango todo negativo", -5, -1, ft_range(-5, -1));
    check_array("Números grandes", 1000, 1005, ft_range(1000, 1005));

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);

    return g_tests_failed;
}