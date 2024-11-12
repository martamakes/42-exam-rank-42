#include <stdio.h>

// Contador global de errores
int g_tests_failed = 0;

// Prototipo de la función a testear
int ft_strlen(char *str);

// Función auxiliar para los tests
void run_test(char *test_name, char *input, int expected) {
    int result = ft_strlen(input);
    printf("Test %s: ", test_name);
    
    if (result == expected) {
        printf("\033[0;32m[OK]\033[0m");
    } else {
        printf("\033[0;31m[KO]\033[0m");
        g_tests_failed++; // Incrementar contador cuando falla el test
        printf("\n  Expected: %d", expected);
        printf("\n  Got: %d", result);
    }
    printf("\n  Input: \"%s\"\n", input);
}

int main(void) {
    // Inicializar contador de errores
    g_tests_failed = 0;
    
    printf("\n=== Testing ft_strlen ===\n\n");

    // Test 1: String básico
    run_test("String basico", "Hello", 5);

    // Test 2: String vacío (caso límite básico)
    run_test("String vacio", "", 0);

    // Test 3: String con espacios
    run_test("String con espacios", "Hello World", 11);

    // Test 4: String largo pero simple
    run_test("String largo", "abcdefghijklmnopqrstuvwxyz", 26);

    // Test 5: String con caracteres especiales
    run_test("Caracteres especiales", "Hello\tWorld\n", 12);

    // Test 6: String con símbolos
    run_test("Simbolos", "!@#$%^&*()", 10);

    // Resumen final
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed; // Retornar número de tests fallidos
}