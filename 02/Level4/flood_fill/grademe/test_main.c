// test_main_improved.c para flood_fill - VERSIÓN CORREGIDA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flood_fill.h"

int g_tests_failed = 0;

// Función para crear una copia del área
char** make_area(char** zone, t_point size)
{
    if (size.x <= 0 || size.y <= 0)
        return NULL;
        
    char** new = malloc(sizeof(char*) * size.y);
    if (!new)
        return NULL;
        
    for (int i = 0; i < size.y; ++i)
    {
        new[i] = malloc(size.x + 1);
        if (!new[i])
        {
            // Liberar memoria previamente asignada en caso de error
            for (int j = 0; j < i; j++)
                free(new[j]);
            free(new);
            return NULL;
        }
        strcpy(new[i], zone[i]);
    }
    return new;
}

// Función para liberar el área
void free_area(char** area, int height)
{
    if (!area)
        return;
    for (int i = 0; i < height; i++)
        free(area[i]);
    free(area);
}

// Función para comparar dos áreas
int compare_areas(char** area1, char** area2, t_point size)
{
    for (int i = 0; i < size.y; i++)
    {
        if (strcmp(area1[i], area2[i]) != 0)
            return 0;
    }
    return 1;
}

void run_test(char *test_name, char **input, char **expected, t_point size, t_point begin)
{
    char **area = make_area(input, size);
    
    printf("Test %s:\n", test_name);
    printf("Input:\n");
    for (int i = 0; i < size.y; i++)
        printf("%s\n", input[i]);
    
    printf("Begin point: (%d, %d)\n", begin.x, begin.y);
    
    flood_fill(area, size, begin);
    
    printf("\nOutput:\n");
    for (int i = 0; i < size.y; i++)
        printf("%s\n", area[i]);
    
    printf("\nExpected:\n");
    for (int i = 0; i < size.y; i++)
        printf("%s\n", expected[i]);
    
    if (compare_areas(area, expected, size))
        printf("\n\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\n\033[0;31m[KO]\033[0m\n");
        g_tests_failed++;
    }
    
    free_area(area, size.y);
    printf("\n");
}

int main(void)
{
    printf("\n=== Testing flood_fill - VERSIÓN CORREGIDA ===\n\n");

    // Test 1: Ejemplo del subject
    {
        t_point size = {8, 5};
        char *input[] = {
            "11111111",
            "10001001",
            "10010001",
            "10110001",
            "11100001"
        };
        char *expected[] = {
            "FFFFFFFF",
            "F000F00F",
            "F00F000F",
            "F0FF000F",
            "FFF0000F"
        };
        t_point begin = {7, 4};
        run_test("Subject example", input, expected, size, begin);
    }

    // Test 2: Área pequeña
    {
        t_point size = {3, 3};
        char *input[] = {
            "111",
            "101",
            "111"
        };
        char *expected[] = {
            "FFF",
            "F0F",
            "FFF"
        };
        t_point begin = {0, 0};
        run_test("Small area", input, expected, size, begin);
    }

    // Test 3: Línea única
    {
        t_point size = {5, 1};
        char *input[] = {
            "11111"
        };
        char *expected[] = {
            "FFFFF"
        };
        t_point begin = {2, 0};
        run_test("Single line", input, expected, size, begin);
    }

    // *** CASOS CRÍTICOS CORREGIDOS ***

    // Test 4: Coordenadas fuera de límites (CRÍTICO)
    {
        printf("Test 4: Coordenadas fuera de límites\n");
        t_point size = {3, 3};
        char *input[] = {
            "111",
            "101", 
            "111"
        };
        char **area = make_area(input, size);
        
        printf("Input:\n");
        for (int i = 0; i < size.y; i++)
            printf("%s\n", input[i]);
        
        // Punto fuera de límites
        t_point begin = {10, 10};
        printf("Begin point: (%d, %d) - FUERA DE LÍMITES\n", begin.x, begin.y);
        
        flood_fill(area, size, begin);
        
        printf("\nOutput (no debe cambiar):\n");
        for (int i = 0; i < size.y; i++)
            printf("%s\n", area[i]);
        
        // Verificar que no cambió
        if (strcmp(area[0], "111") == 0 && strcmp(area[1], "101") == 0 && strcmp(area[2], "111") == 0)
            printf("\n\033[0;32m[OK] - No modificó área con coordenadas inválidas\033[0m\n");
        else
        {
            printf("\n\033[0;31m[KO] - Modificó área con coordenadas inválidas\033[0m\n");
            g_tests_failed++;
        }
        
        free_area(area, size.y);
        printf("\n");
    }

    // Test 5: Coordenadas negativas (CORREGIDO)
    {
        printf("Test 5: Coordenadas negativas\n");
        t_point size = {3, 3};
        char *input[] = {
            "111",
            "101",
            "111"
        };
        char **area = make_area(input, size);
        
        printf("Input:\n");
        for (int i = 0; i < size.y; i++)
            printf("%s\n", input[i]);
        
        t_point begin = {-1, -1};
        printf("Begin point: (%d, %d) - COORDENADAS NEGATIVAS\n", begin.x, begin.y);
        
        flood_fill(area, size, begin);
        
        printf("\nOutput (no debe cambiar):\n");
        for (int i = 0; i < size.y; i++)
            printf("%s\n", area[i]);
        
        // Verificar que no cambió
        if (strcmp(area[0], "111") == 0 && strcmp(area[1], "101") == 0 && strcmp(area[2], "111") == 0)
            printf("\n\033[0;32m[OK] - No modificó área con coordenadas negativas\033[0m\n");
        else
        {
            printf("\n\033[0;31m[KO] - Modificó área con coordenadas negativas\033[0m\n");
            g_tests_failed++;
        }
        
        free_area(area, size.y);
        printf("\n");
    }

    // Test 6: Área de 1x1
    {
        t_point size = {1, 1};
        char *input[] = {"1"};
        char *expected[] = {"F"};
        t_point begin = {0, 0};
        run_test("Area 1x1", input, expected, size, begin);
    }

    // Test 7: Sin conexión (carácter diferente)
    {
        t_point size = {3, 3};
        char *input[] = {
            "111",
            "101",
            "222"
        };
        char *expected[] = {
            "111",
            "101",
            "FFF"
        };
        t_point begin = {0, 2}; // Empezar en '2'
        run_test("Carácter diferente", input, expected, size, begin);
    }

    // Test 8: Área ya llena de 'F'
    {
        t_point size = {3, 3};
        char *input[] = {
            "FFF",
            "F0F",
            "FFF"
        };
        char *expected[] = {
            "FFF",
            "F0F",
            "FFF"
        };
        t_point begin = {0, 0}; // Empezar en 'F' (no debe cambiar)
        run_test("Area ya con F", input, expected, size, begin);
    }

    // Test 9: Isla aislada
    {
        t_point size = {5, 5};
        char *input[] = {
            "11111",
            "10001",
            "10101",
            "10001",
            "11111"
        };
        char *expected[] = {
            "FFFFF",
            "F000F",
            "F0F0F",
            "F000F",
            "FFFFF"
        };
        t_point begin = {0, 0};
        run_test("Isla aislada", input, expected, size, begin);
    }

    // Test 10: Forma compleja (L)
    {
        t_point size = {4, 4};
        char *input[] = {
            "1000",
            "1000",
            "1000",
            "1111"
        };
        char *expected[] = {
            "F000",
            "F000",
            "F000",
            "FFFF"
        };
        t_point begin = {0, 0};
        run_test("Forma L", input, expected, size, begin);
    }

    // Test 11: Múltiples caracteres diferentes
    {
        t_point size = {4, 4};
        char *input[] = {
            "1234",
            "5678",
            "9ABC",
            "DEFG"
        };
        char *expected[] = {
            "F234",
            "5678",
            "9ABC",
            "DEFG"
        };
        t_point begin = {0, 0}; // Solo cambia el '1'
        run_test("Múltiples caracteres", input, expected, size, begin);
    }

    // Test 12: Corredor estrecho
    {
        t_point size = {7, 3};
        char *input[] = {
            "1010101",
            "0101010",
            "1010101"
        };
        char *expected[] = {
            "F010101",
            "0101010",
            "1010101"
        };
        t_point begin = {0, 0};
        run_test("Corredor estrecho", input, expected, size, begin);
    }

    // Test 13: Área grande para rendimiento
    {
        printf("Test 13: Área grande (10x10)\n");
        t_point size = {10, 10};
        char *input[] = {
            "1111111111",
            "1000000001",
            "1011111101",
            "1010000101",
            "1010110101",
            "1010110101",
            "1010000101",
            "1011111101",
            "1000000001",
            "1111111111"
        };
        
        char **area = make_area(input, size);
        printf("Input: 10x10 grid\n");
        
        t_point begin = {0, 0};
        printf("Begin point: (%d, %d)\n", begin.x, begin.y);
        
        flood_fill(area, size, begin);
        
        printf("Output: Debería llenar el borde exterior\n");
        
        // Verificar que se llenó correctamente (el borde debe ser F)
        if (area[0][0] == 'F' && area[0][9] == 'F' && area[9][0] == 'F' && area[9][9] == 'F')
            printf("\033[0;32m[OK] - Área grande procesada correctamente\033[0m\n");
        else
        {
            printf("\033[0;31m[KO] - Error en área grande\033[0m\n");
            g_tests_failed++;
        }
        
        free_area(area, size.y);
        printf("\n");
    }

    // Test 14: Empezar en el centro
    {
        t_point size = {5, 5};
        char *input[] = {
            "00000",
            "01110",
            "01110",
            "01110",
            "00000"
        };
        char *expected[] = {
            "00000",
            "0FFF0",
            "0FFF0",
            "0FFF0",
            "00000"
        };
        t_point begin = {2, 2}; // Centro
        run_test("Empezar en centro", input, expected, size, begin);
    }

    // Test 15: Línea vertical
    {
        t_point size = {1, 5};
        char *input[] = {"1", "1", "1", "1", "1"};
        char *expected[] = {"F", "F", "F", "F", "F"};
        t_point begin = {0, 2};
        run_test("Línea vertical", input, expected, size, begin);
    }

    // Test 16: Esquinas del área
    {
        t_point size = {3, 3};
        char *input[] = {
            "111",
            "101",
            "111"
        };
        
        // Test esquina superior derecha
        char *expected[] = {
            "FFF",
            "F0F",
            "FFF"
        };
        t_point begin = {2, 0};
        run_test("Esquina superior derecha", input, expected, size, begin);
    }

    // Test 17: Área vacía (size 0x0) - CRÍTICO
    {
        printf("Test 17: Área vacía (0x0)\n");
        t_point size = {0, 0};
        t_point begin = {0, 0};
        
        printf("Size: (%d, %d)\n", size.x, size.y);
        printf("Begin point: (%d, %d)\n", begin.x, begin.y);
        
        flood_fill(NULL, size, begin); // No debe crashear
        printf("\033[0;32m[OK] - No crasheó con área vacía\033[0m\n\n");
    }

    // Test 18: Caracteres especiales
    {
        t_point size = {3, 3};
        char *input[] = {
            "@@@",
            "@#@",
            "@@@"
        };
        char *expected[] = {
            "FFF",
            "F#F",
            "FFF"
        };
        t_point begin = {0, 0};
        run_test("Caracteres especiales", input, expected, size, begin);
    }

    // Test 19: Solo espacios
    {
        t_point size = {3, 3};
        char *input[] = {
            "   ",
            " X ",
            "   "
        };
        char *expected[] = {
            "FFF",
            "FXF",
            "FFF"
        };
        t_point begin = {0, 0};
        run_test("Solo espacios", input, expected, size, begin);
    }

    // Test 20: Patrón de tablero de ajedrez
    {
        t_point size = {4, 4};
        char *input[] = {
            "1010",
            "0101",
            "1010",
            "0101"
        };
        char *expected[] = {
            "F010",
            "0101",
            "1010",
            "0101"
        };
        t_point begin = {0, 0};
        run_test("Tablero ajedrez", input, expected, size, begin);
    }

    // Resumen
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32m✅ Todos los tests pasaron correctamente! Tu flood_fill está lista para el examen.\033[0m\n");
    } else {
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);
    }

    printf("\n\033[0;33m📝 CASOS CRÍTICOS CUBIERTOS:\033[0m\n");
    printf("✅ Coordenadas fuera de límites\n");
    printf("✅ Coordenadas negativas\n");
    printf("✅ Área vacía (0x0)\n");
    printf("✅ Área de 1x1\n");
    printf("✅ Carácter diferente al inicio\n");
    printf("✅ Área ya llena de 'F'\n");
    printf("✅ Formas complejas (L, islas, corredores)\n");
    printf("✅ Área grande (rendimiento)\n");
    printf("✅ Esquinas y bordes\n");
    printf("✅ Caracteres especiales\n");
    
    printf("\n\033[0;31m⚠️  PUNTOS CRÍTICOS PARA EL EXAMEN:\033[0m\n");
    printf("• Verificar límites ANTES de acceder al array\n");
    printf("• No llenar diagonalmente\n");
    printf("• Solo llenar caracteres iguales al inicial\n");
    printf("• Manejar coordenadas negativas\n");
    printf("• No crashear con áreas vacías o inválidas\n");
    printf("• Recursión correcta en 4 direcciones\n");

    printf("\n\033[0;36m💡 ALGORITMO RECOMENDADO:\033[0m\n");
    printf("```c\n");
    printf("void flood_fill(char **tab, t_point size, t_point begin)\n");
    printf("{\n");
    printf("    // Verificar límites\n");
    printf("    if (begin.x < 0 || begin.x >= size.x ||\n");
    printf("        begin.y < 0 || begin.y >= size.y)\n");
    printf("        return;\n");
    printf("    \n");
    printf("    char original = tab[begin.y][begin.x];\n");
    printf("    fill_recursive(tab, size, begin, original);\n");
    printf("}\n");
    printf("\n");
    printf("void fill_recursive(char **tab, t_point size, t_point pos, char target)\n");
    printf("{\n");
    printf("    // Verificar límites y carácter\n");
    printf("    if (pos.x < 0 || pos.x >= size.x ||\n");
    printf("        pos.y < 0 || pos.y >= size.y ||\n");
    printf("        tab[pos.y][pos.x] != target)\n");
    printf("        return;\n");
    printf("    \n");
    printf("    // Llenar actual\n");
    printf("    tab[pos.y][pos.x] = 'F';\n");
    printf("    \n");
    printf("    // Llenar 4 direcciones\n");
    printf("    fill_recursive(tab, size, (t_point){pos.x-1, pos.y}, target);\n");
    printf("    fill_recursive(tab, size, (t_point){pos.x+1, pos.y}, target);\n");
    printf("    fill_recursive(tab, size, (t_point){pos.x, pos.y-1}, target);\n");
    printf("    fill_recursive(tab, size, (t_point){pos.x, pos.y+1}, target);\n");
    printf("}\n");
    printf("```\n");

    return g_tests_failed;
}