// test_main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Prototipo de la función a testear
char *ft_strcpy(char *s1, char *s2);

// Función de ayuda para comparar resultados
void test_strcpy(const char *test_name, char *source) {
    // Calcular tamaño necesario y crear buffers
    size_t len = strlen(source) + 1;  // +1 para el null terminator
    char *dest1 = malloc(len);
    char *dest2 = malloc(len);
    
    // Si malloc falla, terminar el test
    if (!dest1 || !dest2) {
        printf("\033[0;31m[KO]\033[0m %s: Error de memoria\n", test_name);
        free(dest1);
        free(dest2);
        return;
    }
    
    // Realizar copias con ambas funciones
    strcpy(dest1, source);  // Guardamos el resultado de strcpy original
    char *ret = ft_strcpy(dest2, source);  // Guardamos el retorno de ft_strcpy para verificarlo
    
    // Verificar el contenido
    int content_ok = (strcmp(dest1, dest2) == 0);
    // Verificar el valor de retorno
    int return_ok = (ret == dest2);
    
    if (content_ok && return_ok) {
        printf("\033[0;32m[OK]\033[0m %s\n", test_name);
    } else {
        printf("\033[0;31m[KO]\033[0m %s\n", test_name);
        if (!content_ok) {
            printf("  Expected: \"%s\"\n", dest1);
            printf("  Got:      \"%s\"\n", dest2);
        }
        if (!return_ok) {
            printf("  Return value incorrect: should return destination pointer\n");
        }
    }
    
    // Liberar memoria
    free(dest1);
    free(dest2);
}

int main(void) {
    printf("=== Tests para ft_strcpy ===\n\n");
    
    // Test 1: String normal
    test_strcpy("Test básico", "Hello, World!");
    
    // Test 2: String vacía
    test_strcpy("String vacía", "");
    
    // Test 3: String con espacios
    test_strcpy("String con espacios", "   Spaces   ");
    
    // Test 4: String con caracteres especiales
    test_strcpy("Caracteres especiales", "Hello\tWorld\n");
    
    // Test 5: String larga
    test_strcpy("String larga", "Esta es una string muy larga para probar que la función maneja correctamente strings de mayor tamaño sin problemas");
    
    // Test 6: String con símbolos
    test_strcpy("Símbolos", "!@#$%^&*()_+");
    
    // Test 7: String alfanumérica
    test_strcpy("Alfanumérica", "abc123DEF456");
    
    return 0;
}