// test_main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Contador global de errores
int g_tests_failed = 0;

// Prototipo de la función
size_t ft_strcspn(const char *s, const char *reject);

// Función auxiliar para ejecutar un test
void run_test(const char *s, const char *reject, const char *test_name)
{
    size_t expected = strcspn(s, reject);
    size_t result = ft_strcspn(s, reject);
    
    printf("Test %s: ", test_name);
    
    if (result == expected)
    {
        printf("\033[0;32m[OK]\033[0m\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  String: \"%s\"\n", s);
        printf("  Reject: \"%s\"\n", reject);
        printf("  Expected: %zu\n", expected);
        printf("  Got: %zu\n", result);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing ft_strcspn ===\n\n");

    // Tests básicos
    run_test("Hello World", " ", "basic space search");
    run_test("Hello World", "aeiou", "basic vowel search");
    run_test("Hello World", "xyz", "no match");
    
    // Tests con strings vacíos
    run_test("", "", "empty strings");
    run_test("Hello", "", "empty reject");
    run_test("", "abc", "empty source");
    
    // Tests con caracteres especiales
    run_test("Hello\tWorld", "\t", "tab character");
    run_test("Hello\nWorld", "\n", "newline character");
    run_test("Hello!@#$%", "!@#$%", "special characters");
    
    // Tests con coincidencias al inicio
    run_test("Hello", "H", "match at start");
    run_test("Hello", "He", "match at start multiple");
    
    // Tests con coincidencias al final
    run_test("Hello", "o", "match at end");
    run_test("Hello", "lo", "match at end multiple");
    
    // Tests con múltiples caracteres de rechazo
    run_test("Hello World", "abcdefghijklmnopqrstuvwxyz", "all lowercase letters");
    run_test("Hello World", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "all uppercase letters");
    run_test("12345", "0123456789", "all numbers");
    
    // Tests con casos especiales
    run_test("   ", " ", "only spaces");
    run_test("\t\t\t", "\t", "only tabs");
    run_test(";;;", ";", "repeated characters");
    run_test("Hello World", "Helo Wrd", "multiple matches");
    run_test("The Quick Brown Fox", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 
             "case sensitive search");

    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
    {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    }
    else
    {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}