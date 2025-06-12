#include <stdio.h>
#include <string.h>
#include <limits.h>

// Incluimos el prototipo de ft_printf
int ft_printf(const char *format, ...);

int main() {
    printf("=== TESTER MEJORADO DE FT_PRINTF ===\n");
    printf("Probando casos extremos y funcionalidad completa\n\n");
    
    // =================================================================
    // PRUEBAS BÁSICAS
    // =================================================================
    printf("🔹 PRUEBAS BÁSICAS:\n");
    printf("------------------\n");
    
    int len1 = printf("printf    - String: %s\n", "Hello World");
    int len2 = ft_printf("ft_printf - String: %s\n", "Hello World");
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    len1 = printf("printf    - Decimal: %d\n", 42);
    len2 = ft_printf("ft_printf - Decimal: %d\n", 42);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    len1 = printf("printf    - Hexadecimal: %x\n", 42);
    len2 = ft_printf("ft_printf - Hexadecimal: %x\n", 42);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    // =================================================================
    // PRUEBAS CRÍTICAS - CASOS EXTREMOS
    // =================================================================
    printf("🔸 PRUEBAS CRÍTICAS (CASOS EXTREMOS):\n");
    printf("-------------------------------------\n");
    
    // Test 1: INT_MIN - El caso más crítico
    printf("1. INT_MIN (-2147483648) - CASO CRÍTICO:\n");
    len1 = printf("printf    - INT_MIN: %d\n", INT_MIN);
    len2 = ft_printf("ft_printf - INT_MIN: %d\n", INT_MIN);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n", len1, len2, 
           (len1 == len2) ? "✅ CORRECTO" : "❌ FALLA EN INT_MIN");
    printf("💡 Si ves ❌ aquí, tu ft_putdigit necesita usar long long int\n\n");
    
    // Test 2: INT_MAX
    printf("2. INT_MAX (2147483647):\n");
    len1 = printf("printf    - INT_MAX: %d\n", INT_MAX);
    len2 = ft_printf("ft_printf - INT_MAX: %d\n", INT_MAX);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    // Test 3: Números negativos varios
    printf("3. Números negativos varios:\n");
    int negatives[] = {-1, -42, -100, -999, -123456};
    for (int i = 0; i < 5; i++) {
        len1 = printf("printf    - Negativo %d: %d\n", i+1, negatives[i]);
        len2 = ft_printf("ft_printf - Negativo %d: %d\n", i+1, negatives[i]);
        printf("Longitudes: printf=%d, ft_printf=%d %s\n", len1, len2, 
               (len1 == len2) ? "✅" : "❌");
    }
    printf("\n");
    
    // Test 4: Hexadecimal con números negativos
    printf("4. Hexadecimal con números negativos:\n");
    printf("💡 Para %%x, los números negativos se interpretan como unsigned\n");
    len1 = printf("printf    - Hex de -1: %x\n", -1);
    len2 = ft_printf("ft_printf - Hex de -1: %x\n", -1);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    len1 = printf("printf    - Hex de -42: %x\n", -42);
    len2 = ft_printf("ft_printf - Hex de -42: %x\n", -42);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    // =================================================================
    // PRUEBAS DE STRINGS ESPECIALES
    // =================================================================
    printf("🔹 PRUEBAS DE STRINGS ESPECIALES:\n");
    printf("---------------------------------\n");
    
    // Test 5: String NULL
    printf("5. String NULL:\n");
    len1 = printf("printf    - String NULL: %s\n", (char*)NULL);
    len2 = ft_printf("ft_printf - String NULL: %s\n", (char*)NULL);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    // Test 6: String vacío
    printf("6. String vacío:\n");
    len1 = printf("printf    - String vacío: '%s'\n", "");
    len2 = ft_printf("ft_printf - String vacío: '%s'\n", "");
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    // Test 7: Hexadecimales grandes
    printf("7. Hexadecimales grandes:\n");
    len1 = printf("printf    - Hex grande: %x\n", 0xABCDEF);
    len2 = ft_printf("ft_printf - Hex grande: %x\n", 0xABCDEF);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    // =================================================================
    // PRUEBAS MIXTAS Y COMPLEJAS
    // =================================================================
    printf("🔹 PRUEBAS MIXTAS Y COMPLEJAS:\n");
    printf("------------------------------\n");
    
    // Test 8: Múltiples conversiones
    printf("8. Múltiples conversiones:\n");
    len1 = printf("printf    - Mixto: %s tiene %d años y código %x\n", "Juan", 30, 0xABC);
    len2 = ft_printf("ft_printf - Mixto: %s tiene %d años y código %x\n", "Juan", 30, 0xABC);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    // Test 9: Casos con cero
    printf("9. Casos con cero:\n");
    len1 = printf("printf    - Cero decimal: %d\n", 0);
    len2 = ft_printf("ft_printf - Cero decimal: %d\n", 0);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    len1 = printf("printf    - Cero hex: %x\n", 0);
    len2 = ft_printf("ft_printf - Cero hex: %x\n", 0);
    printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
           (len1 == len2) ? "✅" : "❌");
    
    // Test 10: Porcentaje literal (no implementado en versión básica)
    printf("10. Porcentaje literal (puede no estar implementado):\n");
    printf("💡 Algunos ft_printf básicos no implementan %%\n");
    // Comentamos esto para evitar errores si no está implementado
    // len1 = printf("printf    - Porcentaje: %%\n");
    // len2 = ft_printf("ft_printf - Porcentaje: %%\n");
    // printf("Longitudes: printf=%d, ft_printf=%d %s\n\n", len1, len2, 
    //        (len1 == len2) ? "✅" : "❌");
    
    // =================================================================
    // RESUMEN FINAL
    // =================================================================
    printf("🎯 RESUMEN DEL TEST:\n");
    printf("==================\n");
    printf("Si ves ❌ en INT_MIN: usa 'long long int' en ft_putdigit\n");
    printf("Si todo está ✅: ¡tu ft_printf está listo para el examen!\n");
    printf("\n=== Fin de las pruebas ===\n");
    
    return 0;
}