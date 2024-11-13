// test_main.c
#include <stdio.h>
#include <stdlib.h>

int g_tests_failed = 0;

// Prototipo de la función a testear
unsigned char reverse_bits(unsigned char octet);

// Función auxiliar para imprimir un byte en formato binario
void print_byte(unsigned char byte) {
    for (int i = 7; i >= 0; i--)
        printf("%d", (byte >> i) & 1);
}

// Función para ejecutar un test individual
void run_test(char *test_name, unsigned char input, unsigned char expected) {
    unsigned char result = reverse_bits(input);
    
    printf("Test %s: ", test_name);
    if (result == expected) {
        printf("\033[0;32m[OK]\033[0m\n");
        printf("  Input:    "); print_byte(input); printf("\n");
        printf("  Expected: "); print_byte(expected); printf("\n");
        printf("  Got:      "); print_byte(result); printf("\n\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input:    "); print_byte(input); printf("\n");
        printf("  Expected: "); print_byte(expected); printf("\n");
        printf("  Got:      "); print_byte(result); printf("\n\n");
        g_tests_failed++;
    }
}

int main(void) {
    printf("\n=== Testing reverse_bits ===\n\n");

    // Test del ejemplo del subject
    run_test("Subject Example", 0x41, 0x82);  // 01000001 -> 10000010

    // Tests adicionales
    run_test("All zeros", 0x00, 0x00);        // 00000000 -> 00000000
    run_test("All ones", 0xFF, 0xFF);         // 11111111 -> 11111111
    run_test("Alternating", 0xAA, 0x55);      // 10101010 -> 01010101
    run_test("Single bit", 0x01, 0x80);       // 00000001 -> 10000000

    // Resumen
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);

    return g_tests_failed;
}