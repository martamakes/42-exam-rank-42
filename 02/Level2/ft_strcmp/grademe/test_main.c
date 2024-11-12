// test_main.c

#include <stdio.h>
#include <string.h>

int g_tests_failed = 0;

// Prototipo de la función
int ft_strcmp(char *s1, char *s2);

void run_test(char *test_name, char *s1, char *s2) {
    int result = ft_strcmp(s1, s2);
    int expected = strcmp(s1, s2);
    
    // Para strcmp, solo importa si el resultado es positivo, negativo o cero
    int result_sign = (result > 0) ? 1 : (result < 0) ? -1 : 0;
    int expected_sign = (expected > 0) ? 1 : (expected < 0) ? -1 : 0;
    
    printf("Test %s: ", test_name);
    if (result_sign == expected_sign) {
        printf("\033[0;32m[OK]\033[0m\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n");
        g_tests_failed++;
        printf("  s1: \"%s\"\n", s1);
        printf("  s2: \"%s\"\n", s2);
        printf("  Expected sign: %d\n", expected_sign);
        printf("  Got sign: %d\n", result_sign);
    }
}

int main(void) {
    printf("\n=== Testing ft_strcmp ===\n\n");

    // Test 1: Strings iguales
    run_test("Equal strings", "hello", "hello");

    // Test 2: Primer string mayor
    run_test("First string greater", "hello", "hell");

    // Test 3: Segundo string mayor
    run_test("Second string greater", "hell", "hello");

    // Test 4: Strings completamente diferentes
    run_test("Different strings", "hello", "world");

    // Test 5: String vacío con string no vacío
    run_test("Empty vs non-empty", "", "hello");

    // Test 6: Ambos strings vacíos
    run_test("Both empty", "", "");

    // Test 7: Strings con caracteres especiales
    run_test("Special characters", "hello!", "hello?");

    // Test 8: Strings con números
    run_test("Strings with numbers", "hello123", "hello456");

    // Test 9: Diferencia en el primer carácter
    run_test("First char different", "abc", "bbc");

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}