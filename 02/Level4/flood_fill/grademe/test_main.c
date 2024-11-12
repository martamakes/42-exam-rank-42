// test_main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flood_fill.h"

int g_tests_failed = 0;

// Función para crear una copia del área
char** make_area(char** zone, t_point size)
{
    char** new = malloc(sizeof(char*) * size.y);
    for (int i = 0; i < size.y; ++i)
    {
        new[i] = malloc(size.x + 1);
        strcpy(new[i], zone[i]);
    }
    return new;
}

// Función para liberar el área
void free_area(char** area, int height)
{
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
    printf("\n=== Testing flood_fill ===\n\n");

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

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}