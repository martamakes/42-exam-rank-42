// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_tests_failed = 0;

// Prototipo de la función
char    **ft_split(char *str);

// Función auxiliar para liberar la memoria del split
void    free_split_result(char **split)
{
    int i = 0;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

// Función auxiliar para comparar dos arrays de strings
int compare_splits(char **split1, char **split2)
{
    int i = 0;
    
    while (split1[i] && split2[i])
    {
        if (strcmp(split1[i], split2[i]) != 0)
            return (0);
        i++;
    }
    return (!split1[i] && !split2[i]);
}

void run_test(char *test_name, char *input, char **expected)
{
    printf("Test %s: ", test_name);
    
    char **result = ft_split(input);
    if (!result)
    {
        printf("\033[0;31m[KO] - malloc failed\033[0m\n");
        g_tests_failed++;
        return;
    }

    if (compare_splits(result, expected))
    {
        printf("\033[0;32m[OK]\033[0m\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("Input: \"%s\"\n", input);
        printf("Expected: \n");
        for (int i = 0; expected[i]; i++)
            printf("\"%s\"\n", expected[i]);
        printf("Got: \n");
        for (int i = 0; result[i]; i++)
            printf("\"%s\"\n", result[i]);
        g_tests_failed++;
    }

    free_split_result(result);
}

int main(void)
{
    printf("\n=== Testing ft_split ===\n\n");

    // Test 1: String simple
    {
        char *input = "hello world";
        char *exp[] = {"hello", "world", NULL};
        run_test("Simple string", input, exp);
    }

    // Test 2: Múltiples espacios
    {
        char *input = "   hello   world  ";
        char *exp[] = {"hello", "world", NULL};
        run_test("Multiple spaces", input, exp);
    }

    // Test 3: Tabs y newlines
    {
        char *input = "hello\tworld\n42";
        char *exp[] = {"hello", "world", "42", NULL};
        run_test("Tabs and newlines", input, exp);
    }

    // Test 4: String vacía
    {
        char *input = "";
        char *exp[] = {NULL};
        run_test("Empty string", input, exp);
    }

    // Test 5: Solo separadores
    {
        char *input = "   \t\n  ";
        char *exp[] = {NULL};
        run_test("Only separators", input, exp);
    }

    // Test 6: Una palabra
    {
        char *input = "hello";
        char *exp[] = {"hello", NULL};
        run_test("Single word", input, exp);
    }

    // Test 7: Palabras largas
    {
        char *input = "this is a very long sentence with many words";
        char *exp[] = {"this", "is", "a", "very", "long", "sentence", 
                      "with", "many", "words", NULL};
        run_test("Long sentence", input, exp);
    }

    // Test 8: NULL input
    {
        char *input = NULL;
        char *exp[] = {NULL};
        run_test("NULL input", input, exp);
    }

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}