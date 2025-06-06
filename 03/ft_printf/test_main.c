#include <stdio.h>
#include <string.h>

// Incluimos el prototipo de ft_printf
int ft_printf(const char *format, ...);

int main() {
    // Pruebas básicas
    printf("=== Pruebas básicas ===\n");
    
    int len1 = printf("Original - String: %s\n", "Hello World");
    int len2 = ft_printf("Tu ft_printf - String: %s\n", "Hello World");
    printf("Longitud original: %d, tu función: %d\n\n", len1, len2);
    
    len1 = printf("Original - Decimal: %d\n", 42);
    len2 = ft_printf("Tu ft_printf - Decimal: %d\n", 42);
    printf("Longitud original: %d, tu función: %d\n\n", len1, len2);
    
    len1 = printf("Original - Hexadecimal: %x\n", 42);
    len2 = ft_printf("Tu ft_printf - Hexadecimal: %x\n", 42);
    printf("Longitud original: %d, tu función: %d\n\n", len1, len2);
    
    // Pruebas con valores extremos
    printf("=== Pruebas con valores extremos ===\n");
    
    len1 = printf("Original - String NULL: %s\n", (char*)NULL);
    len2 = ft_printf("Tu ft_printf - String NULL: %s\n", (char*)NULL);
    printf("Longitud original: %d, tu función: %d\n\n", len1, len2);
    
    len1 = printf("Original - Decimal extremo: %d\n", 2147483647);
    len2 = ft_printf("Tu ft_printf - Decimal extremo: %d\n", 2147483647);
    printf("Longitud original: %d, tu función: %d\n\n", len1, len2);
    
    len1 = printf("Original - Decimal negativo: %d\n", (int)-2147483648);
    len2 = ft_printf("Tu ft_printf - Decimal negativo: %d\n", (int)-2147483648);
    printf("Longitud original: %d, tu función: %d\n\n", len1, len2);
    
    len1 = printf("Original - Hexadecimal grande: %x\n", 0xABCDEF);
    len2 = ft_printf("Tu ft_printf - Hexadecimal grande: %x\n", 0xABCDEF);
    printf("Longitud original: %d, tu función: %d\n\n", len1, len2);
    
    // Pruebas con múltiples conversiones
    printf("=== Pruebas con múltiples conversiones ===\n");
    
    len1 = printf("Original - Múltiple: %s tiene %d años y su código es %x\n", "Juan", 30, 0xABC);
    len2 = ft_printf("Tu ft_printf - Múltiple: %s tiene %d años y su código es %x\n", "Juan", 30, 0xABC);
    printf("Longitud original: %d, tu función: %d\n\n", len1, len2);
    
    // Pruebas con % solitarios
    printf("=== Pruebas con %% solitarios ===\n");
    
    len1 = printf("Original - Porcentaje solo: %%\n");
    len2 = ft_printf("Tu ft_printf - Porcentaje solo: %%\n");
    printf("Longitud original: %d, tu función: %d\n\n", len1, len2);
    
    printf("=== Fin de las pruebas ===\n");
    
    return 0;
}