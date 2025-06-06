// test_main.c para flood_fill - VERSIÓN SIMPLIFICADA Y CORREGIDA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Si no tienes flood_fill.h, incluir la definición aquí
#ifndef FLOOD_FILL_H
# define FLOOD_FILL_H

typedef struct s_point
{
    int x;
    int y;
} t_point;

void flood_fill(char **tab, t_point size, t_point begin);

#endif

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

// Función para imprimir área
void print_area(char** area, t_point size, const char* title)
{
    printf("%s:\n", title);
    for (int i = 0; i < size.y; i++)
        printf("%s\n", area[i]);
    printf("\n");
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
    printf("Test %s:\n", test_name);
    printf("Begin point: (%d, %d)\n", begin.x, begin.y);
    
    char **area = make_area(input, size);
    if (!area) {
        printf("\033[0;31m[ERROR] - No se pudo crear área\033[0m\n\n");
        g_tests_failed++;
        return;
    }
    
    print_area(input, size, "Input");
    
    flood_fill(area, size, begin);
    
    print_area(area, size, "Output");
    print_area(expected, size, "Expected");
    
    if (compare_areas(area, expected, size))
        printf("\033[0;32m[OK]\033[0m\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        g_tests_failed++;
    }
    
    free_area(area, size.y);
    printf("\n");
}

int main(void)
{
    printf("\n=== Testing flood_fill - VERSIÓN SIMPLIFICADA ===\n\n");

    // Test 1: Ejemplo del subject (CRÍTICO)
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

    // Test 2: Área pequeña (CRÍTICO)
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

    // Test 3: Línea única (CRÍTICO)
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
        if (!area) {
            printf("\033[0;31m[ERROR] - No se pudo crear área\033[0m\n\n");
            g_tests_failed++;
            return 1;
        }
        
        t_point begin = {10, 10};
        printf("Begin point: (%d, %d) - FUERA DE LÍMITES\n", begin.x, begin.y);
        
        print_area(input, size, "Input");
        
        flood_fill(area, size, begin);
        
        print_area(area, size, "Output (no debe cambiar)");
        
        // Verificar que no cambió
        if (compare_areas(area, input, size))
            printf("\033[0;32m[OK] - No modificó área con coordenadas inválidas\033[0m\n");
        else
        {
            printf("\033[0;31m[KO] - Modificó área con coordenadas inválidas\033[0m\n");
            g_tests_failed++;
        }
        
        free_area(area, size.y);
        printf("\n");
    }

    // Test 5: Coordenadas negativas (CRÍTICO)
    {
        printf("Test 5: Coordenadas negativas\n");
        t_point size = {3, 3};
        char *input[] = {
            "111",
            "101",
            "111"
        };
        
        char **area = make_area(input, size);
        if (!area) {
            printf("\033[0;31m[ERROR] - No se pudo crear área\033[0m\n\n");
            g_tests_failed++;
            return 1;
        }
        
        t_point begin = {-1, -1};
        printf("Begin point: (%d, %d) - COORDENADAS NEGATIVAS\n", begin.x, begin.y);
        
        print_area(input, size, "Input");
        
        flood_fill(area, size, begin);
        
        print_area(area, size, "Output (no debe cambiar)");
        
        // Verificar que no cambió
        if (compare_areas(area, input, size))
            printf("\033[0;32m[OK] - No modificó área con coordenadas negativas\033[0m\n");
        else
        {
            printf("\033[0;31m[KO] - Modificó área con coordenadas negativas\033[0m\n");
            g_tests_failed++;
        }
        
        free_area(area, size.y);
        printf("\n");
    }

    // Test 6: Área de 1x1 (CRÍTICO)
    {
        t_point size = {1, 1};
        char *input[] = {"1"};
        char *expected[] = {"F"};
        t_point begin = {0, 0};
        run_test("Area 1x1", input, expected, size, begin);
    }

    // Test 7: Carácter diferente - CORREGIDO
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
            "FFF"  // Solo los '2' deben cambiar a 'F'
        };
        t_point begin = {0, 2}; // Empezar en '2' de la esquina inferior izquierda
        run_test("Carácter diferente", input, expected, size, begin);
    }

    // Test 8: Área ya con F (CRÍTICO - evitar recursión infinita)
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
            "FFF"  // No debe cambiar nada
        };
        t_point begin = {0, 0}; // Empezar en 'F' - no debe causar segfault
        run_test("Area ya con F", input, expected, size, begin);
    }

    // Test 9: Rellenar zona interna (CRÍTICO)
    {
        t_point size = {5, 5};
        char *input[] = {
            "11111",
            "10001",
            "10001",
            "10001",
            "11111"
        };
        char *expected[] = {
            "11111",
            "1FFF1",
            "1FFF1",
            "1FFF1",
            "11111"
        };
        t_point begin = {2, 2}; // Centro (zona de '0')
        run_test("Zona interna", input, expected, size, begin);
    }

    // Test 10: Esquina (CRÍTICO)
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
        t_point begin = {2, 0}; // Esquina superior derecha
        run_test("Esquina superior derecha", input, expected, size, begin);
    }

    // Resumen final
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32m✅ Todos los tests pasaron correctamente! Tu flood_fill está lista para el examen.\033[0m\n");
    } else {
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);
    }

    printf("\n\033[0;33m📝 CASOS CRÍTICOS CUBIERTOS:\033[0m\n");
    printf("✅ Ejemplo del subject\n");
    printf("✅ Área pequeña (3x3)\n");
    printf("✅ Línea única\n");
    printf("✅ Coordenadas fuera de límites\n");
    printf("✅ Coordenadas negativas\n");
    printf("✅ Área de 1x1\n");
    printf("✅ Carácter diferente\n");
    printf("✅ Área ya con F (evitar segfault)\n");
    printf("✅ Zona interna\n");
    printf("✅ Esquinas\n");
    
    printf("\n\033[0;31m⚠️  PUNTOS CRÍTICOS PARA EL EXAMEN:\033[0m\n");
    printf("• Verificar límites ANTES de acceder al array\n");
    printf("• No llenar diagonalmente (solo 4 direcciones)\n");
    printf("• Solo llenar caracteres iguales al inicial\n");
    printf("• Manejar coordenadas negativas y fuera de límites\n");
    printf("• Evitar recursión infinita con caracteres 'F'\n");
    printf("• Recursión correcta: arriba, abajo, izquierda, derecha\n");

    return g_tests_failed;
}

/*
CASOS SIMPLIFICADOS PERO CRÍTICOS:

✅ CRÍTICOS PARA EXAMEN:
1. Ejemplo del subject - Siempre aparece
2. Coordenadas fuera de límites - Muy común
3. Coordenadas negativas - Muy común  
4. Área ya con F - Evitar segfault
5. Área 1x1 - Caso mínimo
6. Zona interna - Rellenar dentro de bordes

✅ IMPORTANTES:
7. Línea única - Casos 1D
8. Carácter diferente - Verificar que solo cambia el correcto
9. Esquinas - Verificar límites
10. Área pequeña - Caso típico

ERRORES COMUNES DETECTADOS:
- Segfault por coordenadas inválidas
- Recursión infinita con 'F'
- No verificar límites correctamente
- Llenar diagonalmente (incorrecto)
- No parar en caracteres diferentes

TIP EXAMEN:
El algoritmo es recursivo simple, pero HAY QUE:
1. Verificar límites SIEMPRE
2. Solo llenar caracteres iguales al original
3. Evitar recursión infinita con 'F'
4. Solo 4 direcciones (no diagonales)
*/