#include <stdio.h>

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
    }
    printf(" Input: \"%s\" | Result: %d\n", input, result);
}

int main(void) {
    printf("\n=== Testing ft_strlen ===\n\n");

    // Test 1: String básico
    run_test("String basico", "Hello", 5);

    // Test 2: String vacío (caso límite básico)
    run_test("String vacio", "", 0);

    // Test 3: String con espacios
    run_test("String con espacios", "Hello World", 11);

    // Test 4: String largo pero simple
    run_test("String largo", "abcdefghijklmnopqrstuvwxyz", 26);

    printf("\n=== Tests completed ===\n\n");
    return 0;
}