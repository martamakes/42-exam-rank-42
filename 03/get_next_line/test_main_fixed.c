#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Incluir get_next_line
#include "get_next_line.h"

// Función auxiliar para comparar strings
int ft_strcmp(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return (s1 == s2 ? 0 : (s1 ? 1 : -1));
    
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

int main() {
    printf("=== TESTER CORREGIDO para get_next_line ===\n\n");
    
    int total_tests = 0;
    int passed_tests = 0;
    
    // ========= TEST 1: Funcionalidad básica =========
    printf("TEST 1: Funcionalidad básica\n");
    printf("-----------------------------\n");
    
    // Crear archivo de prueba con contenido conocido
    int fd = open("test_gnl.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("❌ Error: No se pudo crear el archivo de prueba\n");
        return 1;
    }
    
    // Escribir líneas específicas para verificar
    write(fd, "Linea 1\n", 8);
    write(fd, "Linea 2\n", 8);
    write(fd, "Linea 3\n", 8);
    write(fd, "Linea 4 sin newline", 19);  // Sin \n al final
    close(fd);
    
    // Abrir para lectura
    fd = open("test_gnl.txt", O_RDONLY);
    if (fd == -1) {
        printf("❌ Error: No se pudo abrir archivo para lectura\n");
        return 1;
    }
    
    char *line;
    
    // Verificar línea 1
    total_tests++;
    line = get_next_line(fd);
    if (line && ft_strcmp(line, "Linea 1\n") == 0) {
        printf("✅ Línea 1: Correcta\n");
        passed_tests++;
    } else {
        printf("❌ Línea 1: Esperado 'Linea 1\\n', Recibido: '%s'\n", 
               line ? line : "NULL");
    }
    free(line);
    
    // Verificar línea 2
    total_tests++;
    line = get_next_line(fd);
    if (line && ft_strcmp(line, "Linea 2\n") == 0) {
        printf("✅ Línea 2: Correcta\n");
        passed_tests++;
    } else {
        printf("❌ Línea 2: Esperado 'Linea 2\\n', Recibido: '%s'\n", 
               line ? line : "NULL");
    }
    free(line);
    
    // Verificar línea 3
    total_tests++;
    line = get_next_line(fd);
    if (line && ft_strcmp(line, "Linea 3\n") == 0) {
        printf("✅ Línea 3: Correcta\n");
        passed_tests++;
    } else {
        printf("❌ Línea 3: Esperado 'Linea 3\\n', Recibido: '%s'\n", 
               line ? line : "NULL");
    }
    free(line);
    
    // Verificar línea 4 (sin \n)
    total_tests++;
    line = get_next_line(fd);
    if (line && ft_strcmp(line, "Linea 4 sin newline") == 0) {
        printf("✅ Línea 4 (sin \\n): Correcta\n");
        passed_tests++;
    } else {
        printf("❌ Línea 4: Esperado 'Linea 4 sin newline', Recibido: '%s'\n", 
               line ? line : "NULL");
    }
    free(line);
    
    // Verificar que no hay más líneas
    total_tests++;
    line = get_next_line(fd);
    if (line == NULL) {
        printf("✅ Final de archivo: Correctamente devuelve NULL\n");
        passed_tests++;
    } else {
        printf("❌ Final de archivo: Debería devolver NULL, pero devolvió: '%s'\n", line);
        free(line);
    }
    
    close(fd);
    remove("test_gnl.txt");
    
    // ========= TEST 2: Archivo vacío =========
    printf("\nTEST 2: Archivo vacío\n");
    printf("---------------------\n");
    
    total_tests++;
    fd = open("empty_test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    close(fd);
    
    fd = open("empty_test.txt", O_RDONLY);
    line = get_next_line(fd);
    if (line == NULL) {
        printf("✅ Archivo vacío: Correctamente devuelve NULL\n");
        passed_tests++;
    } else {
        printf("❌ Archivo vacío: Debería devolver NULL, pero devolvió: '%s'\n", line);
        free(line);
    }
    close(fd);
    remove("empty_test.txt");
    
    // ========= TEST 3: FD inválido =========
    printf("\nTEST 3: FD inválido\n");
    printf("-------------------\n");
    
    total_tests++;
    line = get_next_line(-1);
    if (line == NULL) {
        printf("✅ FD inválido: Correctamente devuelve NULL\n");
        passed_tests++;
    } else {
        printf("❌ FD inválido: Debería devolver NULL, pero devolvió: '%s'\n", line);
        free(line);
    }
    
    // ========= TEST 4: Solo newlines =========
    printf("\nTEST 4: Solo newlines\n");
    printf("---------------------\n");
    
    fd = open("newlines_test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, "\n\n\n", 3);  // Tres líneas vacías
    close(fd);
    
    fd = open("newlines_test.txt", O_RDONLY);
    
    // Verificar 3 líneas vacías
    for (int i = 1; i <= 3; i++) {
        total_tests++;
        line = get_next_line(fd);
        if (line && ft_strcmp(line, "\n") == 0) {
            printf("✅ Línea vacía %d: Correcta\n", i);
            passed_tests++;
        } else {
            printf("❌ Línea vacía %d: Esperado '\\n', Recibido: '%s'\n", 
                   i, line ? line : "NULL");
        }
        free(line);
    }
    
    // Verificar final
    total_tests++;
    line = get_next_line(fd);
    if (line == NULL) {
        printf("✅ Final después de líneas vacías: Correcta\n");
        passed_tests++;
    } else {
        printf("❌ Final: Debería devolver NULL, Recibido: '%s'\n", line);
        free(line);
    }
    
    close(fd);
    remove("newlines_test.txt");
    
    // ========= RESUMEN =========
    printf("\n" "=" * 50 "\n");
    printf("RESUMEN DE TESTS\n");
    printf("================\n");
    printf("Tests ejecutados: %d\n", total_tests);
    printf("Tests pasados: %d\n", passed_tests);
    printf("Tests fallados: %d\n", total_tests - passed_tests);
    
    if (passed_tests == total_tests) {
        printf("\n🎉 ¡TODOS LOS TESTS PASARON! Tu implementación es correcta.\n");
    } else {
        printf("\n❌ HAY ERRORES EN TU IMPLEMENTACIÓN. Revisa los fallos específicos.\n");
        printf("El tester anterior era deficiente y no detectaba estos errores.\n");
    }
    
    printf("\nPresiona Enter para continuar...");
    getchar();
    
    return 0;
}