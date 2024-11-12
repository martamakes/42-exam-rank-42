// test_main.c

#include <stdio.h>

int g_tests_failed = 0;

// Prototipo de la función
unsigned char swap_bits(unsigned char octet);

// Función auxiliar para imprimir bits
void print_bits(unsigned char octet) {
    for (int i = 7; i >= 0; i--)
        printf("%d", (octet >> i) & 1);
}

void run_test(char *test_name, unsigned char input, unsigned char expected) {
    unsigned char result = swap_bits(input);
    printf("Test %s: ", test_name);
    
    if (result == expected) {
        printf("\033[0;32m[OK]\033[0m\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n");
        g_tests_failed++;
        printf("  Input:    "); print_bits(input); printf("\n");
        printf("  Expected: "); print_bits(expected); printf("\n");
        printf("  Got:      "); print_bits(result); printf("\n");
    }
}

int main(void) {
    printf("\n=== Testing swap_bits ===\n\n");

    // Test del ejemplo del subject
    run_test("Example", 0x41, 0x14);  // 0100 0001 -> 0001 0100
    
    // Tests adicionales basados en el subject
    run_test("All zeros", 0x00, 0x00);  // 0000 0000 -> 0000 0000
    run_test("All ones", 0xFF, 0xFF);   // 1111 1111 -> 1111 1111

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}