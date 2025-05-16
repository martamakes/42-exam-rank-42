#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

// Incluir get_next_line
#include "get_next_line.h"

int main() {
    printf("=== Pruebas de get_next_line ===\n\n");
    
    // Crear un archivo temporal para las pruebas
    int fd = open("test_gnl.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Error: No se pudo crear el archivo de prueba\n");
        return 1;
    }
    
    // Escribir líneas en el archivo de prueba
    const char *test_lines[] = {
        "Primera línea de texto.\n",
        "Segunda línea con algunos números 12345.\n",
        "Tercera línea que es un poco más larga y contiene diferentes caracteres !@#$%^&*().\n",
        "Cuarta línea que no termina con un salto de línea"
    };
    
    for (int i = 0; i < 4; i++) {
        if (write(fd, test_lines[i], strlen(test_lines[i])) == -1) {
            printf("Error: No se pudo escribir en el archivo de prueba\n");
            close(fd);
            return 1;
        }
    }
    
    close(fd);
    
    // Abrir el archivo para lectura
    fd = open("test_gnl.txt", O_RDONLY);
    if (fd == -1) {
        printf("Error: No se pudo abrir el archivo de prueba para lectura\n");
        return 1;
    }
    
    // Probar get_next_line
    printf("Leyendo líneas del archivo:\n");
    printf("---------------------------\n");
    
    char *line;
    int line_count = 0;
    
    while ((line = get_next_line(fd)) != NULL) {
        printf("Línea %d: %s", ++line_count, line);
        free(line);
    }
    
    printf("\nTotal de líneas leídas: %d\n", line_count);
    
    // Verificar si se leyeron todas las líneas
    if (line_count == 4) {
        printf("¡Prueba básica pasada! Se leyeron todas las líneas correctamente.\n");
    } else {
        printf("Prueba fallida. No se leyeron todas las líneas (se esperaban 4, se leyeron %d).\n", line_count);
    }
    
    close(fd);
    
    // Eliminar el archivo temporal
    remove("test_gnl.txt");
    
    // Probar con un archivo vacío
    fd = open("empty_test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    close(fd);
    
    fd = open("empty_test.txt", O_RDONLY);
    line = get_next_line(fd);
    
    printf("\nPrueba con archivo vacío: ");
    if (line == NULL) {
        printf("Correcta (devuelve NULL)\n");
    } else {
        printf("Fallida (debería devolver NULL)\n");
        free(line);
    }
    
    close(fd);
    remove("empty_test.txt");
    
    // Probar con un FD inválido
    printf("\nPrueba con FD inválido: ");
    line = get_next_line(-1);
    
    if (line == NULL) {
        printf("Correcta (devuelve NULL)\n");
    } else {
        printf("Fallida (debería devolver NULL)\n");
        free(line);
    }
    
    printf("\n=== Fin de las pruebas ===\n");
    
    return 0;
}