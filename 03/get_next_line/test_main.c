#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

// Incluir get_next_line
#include "get_next_line.h"

// Colores para terminal (compatible Mac/Linux)
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

// Contadores globales
int total_tests = 0;
int passed_tests = 0;

void print_test_result(int passed, const char *test_name, const char *expected, const char *got)
{
    total_tests++;
    if (passed) {
        passed_tests++;
        printf(GREEN "✅ %s: PASS" RESET "\n", test_name);
    } else {
        printf(RED "❌ %s: FAIL" RESET "\n", test_name);
        if (expected && got) {
            printf("   Expected: \"%s\"\n", expected);
            printf("   Got:      \"%s\"\n", got);
        }
    }
}

void print_section(const char *title)
{
    printf("\n" BLUE "=== %s ===" RESET "\n", title);
}

void test_basic_functionality()
{
    print_section("TEST 1: Funcionalidad Básica");
    
    // Crear archivo de prueba
    FILE *file = fopen("test_basic.txt", "w");
    assert(file != NULL);
    
    fprintf(file, "Line 1\n");
    fprintf(file, "Line 2\n");
    fprintf(file, "Line 3\n");
    fprintf(file, "Line 4 no newline");
    fclose(file);
    
    // Abrir con get_next_line
    int fd = open("test_basic.txt", O_RDONLY);
    assert(fd != -1);
    
    char *line;
    
    // Test línea 1
    line = get_next_line(fd);
    print_test_result(line && strcmp(line, "Line 1\n") == 0, 
                     "Línea 1", "Line 1\\n", line);
    free(line);
    
    // Test línea 2
    line = get_next_line(fd);
    print_test_result(line && strcmp(line, "Line 2\n") == 0, 
                     "Línea 2", "Line 2\\n", line);
    free(line);
    
    // Test línea 3
    line = get_next_line(fd);
    print_test_result(line && strcmp(line, "Line 3\n") == 0, 
                     "Línea 3", "Line 3\\n", line);
    free(line);
    
    // Test línea 4 (sin \n)
    line = get_next_line(fd);
    print_test_result(line && strcmp(line, "Line 4 no newline") == 0, 
                     "Línea 4 (sin \\n)", "Line 4 no newline", line);
    free(line);
    
    // Test EOF
    line = get_next_line(fd);
    print_test_result(line == NULL, "EOF", "NULL", line ? "(not NULL)" : "NULL");
    if (line) free(line);
    
    close(fd);
    remove("test_basic.txt");
}

void test_edge_cases()
{
    print_section("TEST 2: Casos Edge");
    
    // Test archivo vacío
    FILE *file = fopen("test_empty.txt", "w");
    fclose(file);
    
    int fd = open("test_empty.txt", O_RDONLY);
    char *line = get_next_line(fd);
    print_test_result(line == NULL, "Archivo vacío", "NULL", line ? "(not NULL)" : "NULL");
    if (line) free(line);
    close(fd);
    remove("test_empty.txt");
    
    // Test FD inválido
    line = get_next_line(-1);
    print_test_result(line == NULL, "FD inválido", "NULL", line ? "(not NULL)" : "NULL");
    if (line) free(line);
    
    // Test solo newlines
    file = fopen("test_newlines.txt", "w");
    fprintf(file, "\n\n\n");
    fclose(file);
    
    fd = open("test_newlines.txt", O_RDONLY);
    
    for (int i = 1; i <= 3; i++) {
        line = get_next_line(fd);
        char test_name[50];
        snprintf(test_name, sizeof(test_name), "Línea vacía %d", i);
        print_test_result(line && strcmp(line, "\n") == 0, test_name, "\\n", line);
        free(line);
    }
    
    line = get_next_line(fd);
    print_test_result(line == NULL, "EOF después de newlines", "NULL", line ? "(not NULL)" : "NULL");
    if (line) free(line);
    
    close(fd);
    remove("test_newlines.txt");
}

void test_long_lines()
{
    print_section("TEST 3: Líneas Largas");
    
    // Crear línea muy larga (mayor que BUFFER_SIZE)
    FILE *file = fopen("test_long.txt", "w");
    
    // Línea de 1000 caracteres
    for (int i = 0; i < 1000; i++) {
        fputc('A' + (i % 26), file);
    }
    fprintf(file, "\n");
    fprintf(file, "Short line\n");
    fclose(file);
    
    int fd = open("test_long.txt", O_RDONLY);
    
    // Test línea larga
    char *line = get_next_line(fd);
    int correct_length = line && strlen(line) == 1001; // 1000 chars + \n
    int correct_content = 1;
    if (line) {
        for (int i = 0; i < 1000; i++) {
            if (line[i] != 'A' + (i % 26)) {
                correct_content = 0;
                break;
            }
        }
        correct_content = correct_content && line[1000] == '\n';
    }
    
    print_test_result(correct_length && correct_content, "Línea larga (1000 chars)", 
                     "1000 chars + \\n", line ? "(verificar longitud)" : "NULL");
    free(line);
    
    // Test línea corta después
    line = get_next_line(fd);
    print_test_result(line && strcmp(line, "Short line\n") == 0, 
                     "Línea corta después de larga", "Short line\\n", line);
    free(line);
    
    close(fd);
    remove("test_long.txt");
}

void test_multiple_calls_same_fd()
{
    print_section("TEST 4: Multiple Llamadas Mismo FD");
    
    FILE *file = fopen("test_multi.txt", "w");
    fprintf(file, "A\nB\nC\n");
    fclose(file);
    
    int fd = open("test_multi.txt", O_RDONLY);
    
    // Leer todas las líneas de una vez
    char *lines[3];
    for (int i = 0; i < 3; i++) {
        lines[i] = get_next_line(fd);
    }
    
    // Verificar
    print_test_result(lines[0] && strcmp(lines[0], "A\n") == 0, "Primera llamada", "A\\n", lines[0]);
    print_test_result(lines[1] && strcmp(lines[1], "B\n") == 0, "Segunda llamada", "B\\n", lines[1]);
    print_test_result(lines[2] && strcmp(lines[2], "C\n") == 0, "Tercera llamada", "C\\n", lines[2]);
    
    // Verificar EOF
    char *eof_line = get_next_line(fd);
    print_test_result(eof_line == NULL, "Cuarta llamada (EOF)", "NULL", eof_line ? "(not NULL)" : "NULL");
    
    // Limpiar
    for (int i = 0; i < 3; i++) {
        free(lines[i]);
    }
    if (eof_line) free(eof_line);
    
    close(fd);
    remove("test_multi.txt");
}

int main() {
    printf(YELLOW "🧪 TESTER COMPLETO GET_NEXT_LINE\n" RESET);
    printf("Compilado con BUFFER_SIZE=%d\n\n", BUFFER_SIZE);
    
    test_basic_functionality();
    test_edge_cases();
    test_long_lines();
    test_multiple_calls_same_fd();
    
    // Resumen final
    print_section("RESUMEN");
    
    double success_rate = (double)passed_tests / total_tests * 100;
    
    printf("Tests ejecutados: %d\n", total_tests);
    printf("Tests pasados: " GREEN "%d" RESET "\n", passed_tests);
    printf("Tests fallados: " RED "%d" RESET "\n", total_tests - passed_tests);
    printf("Tasa de éxito: %.1f%%\n\n", success_rate);
    
    if (passed_tests == total_tests) {
        printf(GREEN "🎉 ¡PERFECTO! Tu get_next_line funciona correctamente." RESET "\n");
        printf("✅ Listo para el examen.\n");
    } else {
        printf(RED "❌ Hay errores en tu implementación." RESET "\n");
        printf("💡 Revisa los tests fallidos y corrige los problemas.\n");
        
        if (success_rate >= 70) {
            printf(YELLOW "⚠️  Estás cerca, solo faltan algunos detalles." RESET "\n");
        } else {
            printf(RED "🔥 Necesitas revisar la lógica fundamental." RESET "\n");
        }
    }
    
    printf("\n" BLUE "Presiona Enter para continuar..." RESET);
    getchar();
    
    return (passed_tests == total_tests) ? 0 : 1;
}