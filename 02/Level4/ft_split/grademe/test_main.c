// test_main_improved.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_tests_failed = 0;

// Prototipo de la función
char    **ft_split(char *str);

// Función auxiliar para liberar la memoria del split
void    free_split_result(char **split)
{
    if (!split)
        return;
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
    
    // CASO CRÍTICO: Si input es NULL, debe manejar graciosamente
    if (!input)
    {
        if (!result || (result && !result[0]))
        {
            printf("\033[0;32m[OK] - NULL input handled correctly\033[0m\n");
            if (result)
                free_split_result(result);
            return;
        }
        else
        {
            printf("\033[0;31m[KO] - NULL input not handled correctly\033[0m\n");
            g_tests_failed++;
            free_split_result(result);
            return;
        }
    }
    
    if (!result)
    {
        printf("\033[0;31m[KO] - malloc failed or returned NULL unexpectedly\033[0m\n");
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
        printf("Input: \"%s\"\n", input ? input : "NULL");
        printf("Expected: \n");
        for (int i = 0; expected[i]; i++)
            printf("  [%d]: \"%s\"\n", i, expected[i]);
        printf("Got: \n");
        for (int i = 0; result[i]; i++)
            printf("  [%d]: \"%s\"\n", i, result[i]);
        g_tests_failed++;
    }

    free_split_result(result);
}

int main(void)
{
    printf("\n=== Testing ft_split - EXAM VERSION ===\n\n");

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

    // Test 3: Tabs y newlines mezclados
    {
        char *input = "hello\tworld\n42\t\nschool";
        char *exp[] = {"hello", "world", "42", "school", NULL};
        run_test("Mixed separators", input, exp);
    }

    // Test 4: String vacía
    {
        char *input = "";
        char *exp[] = {NULL};
        run_test("Empty string", input, exp);
    }

    // Test 5: Solo separadores
    {
        char *input = "   \t\n  \t ";
        char *exp[] = {NULL};
        run_test("Only separators", input, exp);
    }

    // Test 6: Una palabra
    {
        char *input = "hello";
        char *exp[] = {"hello", NULL};
        run_test("Single word", input, exp);
    }

    // Test 7: Una palabra con espacios alrededor
    {
        char *input = "   hello   ";
        char *exp[] = {"hello", NULL};
        run_test("Single word with spaces", input, exp);
    }

    // Test 8: Palabras largas
    {
        char *input = "this is a very long sentence with many words";
        char *exp[] = {"this", "is", "a", "very", "long", "sentence", 
                      "with", "many", "words", NULL};
        run_test("Long sentence", input, exp);
    }

    // *** CASOS CRÍTICOS QUE FALTABAN EN TU TESTER ***

    // Test 9: NULL input (MUY IMPORTANTE EN EXAMEN)
    {
        char *input = NULL;
        char *exp[] = {NULL};
        run_test("NULL input", input, exp);
    }

    // Test 10: Solo un carácter
    {
        char *input = "a";
        char *exp[] = {"a", NULL};
        run_test("Single character", input, exp);
    }

    // Test 11: Solo un separador
    {
        char *input = " ";
        char *exp[] = {NULL};
        run_test("Single space", input, exp);
    }

    // Test 12: Tabs únicamente
    {
        char *input = "\t\t\t";
        char *exp[] = {NULL};
        run_test("Only tabs", input, exp);
    }

    // Test 13: Newlines únicamente
    {
        char *input = "\n\n\n";
        char *exp[] = {NULL};
        run_test("Only newlines", input, exp);
    }

    // Test 14: Palabras de un carácter separadas
    {
        char *input = "a b c d e";
        char *exp[] = {"a", "b", "c", "d", "e", NULL};
        run_test("Single char words", input, exp);
    }

    // Test 15: Combinación extrema de separadores
    {
        char *input = "\t\n  hello  \t\n  world \n\t  42   \t\n";
        char *exp[] = {"hello", "world", "42", NULL};
        run_test("Extreme separators", input, exp);
    }

    // Test 16: String muy largo (test de rendimiento)
    {
        char *input = "word1 word2 word3 word4 word5 word6 word7 word8 word9 word10 word11 word12 word13 word14 word15";
        char *exp[] = {"word1", "word2", "word3", "word4", "word5", "word6", "word7", 
                      "word8", "word9", "word10", "word11", "word12", "word13", "word14", "word15", NULL};
        run_test("Many words", input, exp);
    }

    // Test 17: Palabras con caracteres especiales (válidos)
    {
        char *input = "hello! world? 42school@";
        char *exp[] = {"hello!", "world?", "42school@", NULL};
        run_test("Special characters", input, exp);
    }

    // Test 18: Solo newlines mezclado con espacios
    {
        char *input = " \n \n \n ";
        char *exp[] = {NULL};
        run_test("Mixed spaces and newlines", input, exp);
    }

    // Test 19: Palabra muy larga
    {
        char *input = "supercalifragilisticexpialidocious hello";
        char *exp[] = {"supercalifragilisticexpialidocious", "hello", NULL};
        run_test("Very long word", input, exp);
    }

    // Test 20: Secuencia de un solo tipo de separador
    {
        char *input = "hello\n\n\n\nworld";
        char *exp[] = {"hello", "world", NULL};
        run_test("Multiple newlines", input, exp);
    }

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32m✅ Todos los tests pasaron correctamente! Tu ft_split está lista para el examen.\033[0m\n");
    else
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);

    
    return g_tests_failed;
}

