#include <stdio.h>
#include <string.h>
#include <stddef.h>

int g_tests_failed = 0;

size_t ft_strspn(const char *s, const char *accept);

void run_test(char *test_name, const char *s, const char *accept)
{
    printf("Test %s: ", test_name);
    
    size_t expected = strspn(s, accept);
    size_t result = ft_strspn(s, accept);
    
    if (result == expected)
        printf("\033[0;32m[OK]\033[0m\n");
    else {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  String  : \"%s\"\n", s);
        printf("  Accept  : \"%s\"\n", accept);
        printf("  Expected: %zu\n", expected);
        printf("  Got     : %zu\n", result);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing ft_strspn ===\n\n");
    
    // Test 1: Cadena básica
    run_test("Basic string", "Hello, World!", "abcdefghijklmnopqrstuvwxyz");
    
    // Test 2: String vacío
    run_test("Empty string", "", "abc");
    
    // Test 3: Accept vacío
    run_test("Empty accept", "Hello", "");
    
    // Test 4: Todos los caracteres en accept
    run_test("All chars in accept", "hello", "abcdefghijklmnopqrstuvwxyz");
    
    // Test 5: Ningún carácter en accept
    run_test("No chars in accept", "hello", "123");
    
    // Test 6: Caracteres repetidos en accept
    run_test("Repeated chars", "hello", "lllleeeehhhhoooo");
    
    // Test 7: Caso mixto
    run_test("Mixed case", "HelloWorld", "abcdefghijklmnopqrstuvwxyz");
    
    // Test 8: Caracteres especiales
    run_test("Special chars", "123!@#abc", "123!@#");
    
    // Test 9: Coincidencia parcial
    run_test("Partial match", "12345abc", "0123456789");
    
    // Test 10: String largo
    run_test("Long string", 
             "This is a very long string with many characters!",
             "abcdefghijklmnopqrstuvwxyzThi s");
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}