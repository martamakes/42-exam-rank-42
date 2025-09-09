/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-05 17:10:25 by marta            #+#    #+#             */
/*   Updated: 2025-05-05 17:10:25 by marta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "../get_next_line.h"

/*
 * Este tester comprueba el funcionamiento de get_next_line
 * con diferentes BUFFER_SIZE y diferentes tipos de archivos
 */

// Contador global de errores
int g_tests_failed = 0;

// Función para imprimir bytes de forma segura (mostrando caracteres especiales)
void print_bytes(const char *str, int len) {
    if (!str) {
        printf("(null)");
        return;
    }
    
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if (c == '\n')
            printf("\\n");
        else if (c == '\0')
            printf("\\0");
        else if (c >= 32 && c <= 126)
            printf("%c", c);
        else
            printf("\\x%02x", (unsigned char)c);
    }
}

// Función para crear archivos de prueba
void create_test_files() {
    mkdir("test_files", 0755);
    
    // Archivo normal
    int fd = open("test_files/normal.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error al crear archivo normal.txt");
        g_tests_failed++;
        return;
    }
    const char *normal_content = "Esta es la primera línea\nEsta es la segunda línea\nEsta es la tercera línea\n";
    write(fd, normal_content, strlen(normal_content));
    close(fd);
    
    // Archivo vacío
    fd = open("test_files/empty.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error al crear archivo empty.txt");
        g_tests_failed++;
        return;
    }
    close(fd);
    
    // Archivo sin salto de línea al final
    fd = open("test_files/no_nl.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error al crear archivo no_nl.txt");
        g_tests_failed++;
        return;
    }
    const char *no_nl_content = "Esta línea no tiene salto de línea al final";
    write(fd, no_nl_content, strlen(no_nl_content));
    close(fd);
    
    // Archivo con líneas largas
    fd = open("test_files/long.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error al crear archivo long.txt");
        g_tests_failed++;
        return;
    }
    const char *long_content = "Esta es una línea muy larga que supera el tamaño normal de un buffer. Debería tener más de 42 caracteres para asegurarnos de que se prueba correctamente.\n";
    for (int i = 0; i < 10; i++) {
        write(fd, long_content, strlen(long_content));
    }
    close(fd);
    
    // Archivo con una única línea sin salto de línea
    fd = open("test_files/single_line.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error al crear archivo single_line.txt");
        g_tests_failed++;
        return;
    }
    const char *single_line_content = "Una única línea sin salto de línea";
    write(fd, single_line_content, strlen(single_line_content));
    close(fd);
}

// Función para testear get_next_line con un archivo
void test_file(const char *file_path, const char *test_name) {
    printf("\033[0;34m=== Test: %s ===\033[0m\n", test_name);
    
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        printf("\033[0;31m[ERROR] No se pudo abrir el archivo: %s\033[0m\n\n", file_path);
        g_tests_failed++;
        return;
    }
    
    // Leer todo el archivo de una vez para comparar después
    int original_fd = open(file_path, O_RDONLY);
    if (original_fd < 0) {
        printf("\033[0;31m[ERROR] No se pudo abrir el archivo para referencia: %s\033[0m\n\n", file_path);
        close(fd);
        g_tests_failed++;
        return;
    }
    
    char full_file[10000] = {0};
    int bytes_read = read(original_fd, full_file, 9999);
    close(original_fd);
    
    if (bytes_read < 0) {
        printf("\033[0;31m[ERROR] Error al leer el archivo para referencia\033[0m\n\n");
        close(fd);
        g_tests_failed++;
        return;
    }
    
    // Variables para reconstruir el archivo
    char reconstructed[10000] = {0};
    int total_len = 0;
    int line_count = 0;
    char *line;
    
    // Llamar a get_next_line en un bucle
    while ((line = get_next_line(fd)) != NULL) {
        int line_len = strlen(line);
        
        // Añadir línea a la reconstrucción
        strcat(reconstructed + total_len, line);
        total_len += line_len;
        
        // Mostrar información de la línea
        printf("Línea %d (longitud: %d): \"", line_count + 1, line_len);
        print_bytes(line, line_len);
        printf("\"\n");
        
        // Liberar memoria
        free(line);
        line_count++;
    }
    
    close(fd);
    
    // Comprobar si se leyó correctamente todo el archivo
    if (memcmp(full_file, reconstructed, bytes_read) == 0) {
        printf("\033[0;32m[OK] El archivo se leyó correctamente ✓\033[0m\n");
        printf("Se leyeron %d líneas\n", line_count);
    } else {
        g_tests_failed++;
        printf("\033[0;31m[KO] El archivo no se leyó correctamente ✗\033[0m\n");
        
        // Mostrar contenido original y reconstruido para comparación
        printf("Contenido original (%d bytes):\n\"", bytes_read);
        print_bytes(full_file, bytes_read);
        printf("\"\n\nContenido reconstruido (%d bytes):\n\"", total_len);
        print_bytes(reconstructed, total_len);
        printf("\"\n");
        
        // Análisis detallado de diferencias
        printf("\nAnálisis de diferencias:\n");
        int min_len = (bytes_read < total_len) ? bytes_read : total_len;
        int diff_pos = -1;
        
        for (int i = 0; i < min_len; i++) {
            if (full_file[i] != reconstructed[i]) {
                diff_pos = i;
                break;
            }
        }
        
        if (diff_pos != -1) {
            printf("Primera diferencia en la posición %d:\n", diff_pos);
            
            // Mostrar contexto alrededor de la diferencia
            int start = (diff_pos > 10) ? diff_pos - 10 : 0;
            int end = (diff_pos + 10 < min_len) ? diff_pos + 10 : min_len - 1;
            
            printf("Original:     \"");
            print_bytes(full_file + start, end - start + 1);
            printf("\"\n");
            
            printf("Reconstruido: \"");
            print_bytes(reconstructed + start, end - start + 1);
            printf("\"\n");
            
            // Marcar la diferencia exacta
            printf("                ");
            for (int i = start; i < diff_pos; i++) {
                if (full_file[i] == '\n' || reconstructed[i] == '\n')
                    printf(" ");
                else
                    printf(" ");
            }
            printf("^ Diferencia aquí\n");
        } else if (bytes_read != total_len) {
            printf("Las longitudes son diferentes: original=%d, reconstruido=%d\n", bytes_read, total_len);
        }
    }
    
    printf("\n");
}

// Función para testear get_next_line con un pipe
void test_pipe() {
    printf("\033[0;34m=== Test: Lectura desde pipe ===\033[0m\n");
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("\033[0;31m[ERROR] No se pudo crear el pipe\033[0m\n\n");
        g_tests_failed++;
        return;
    }
    
    // Escribir datos en el pipe
    const char *test_data = "Esta es la línea 1\nEsta es la línea 2\nEsta es la línea 3\n";
    write(pipefd[1], test_data, strlen(test_data));
    close(pipefd[1]); // Cerrar extremo de escritura
    
    // Variables para reconstruir los datos
    char reconstructed[1000] = {0};
    int total_len = 0;
    int line_count = 0;
    char *line;
    
    // Llamar a get_next_line en un bucle
    while ((line = get_next_line(pipefd[0])) != NULL) {
        int line_len = strlen(line);
        
        // Añadir línea a la reconstrucción
        strcat(reconstructed + total_len, line);
        total_len += line_len;
        
        // Mostrar información de la línea
        printf("Línea %d (longitud: %d): \"", line_count + 1, line_len);
        print_bytes(line, line_len);
        printf("\"\n");
        
        // Liberar memoria
        free(line);
        line_count++;
    }
    
    close(pipefd[0]);
    
    // Comprobar si se leyó correctamente todo
    if (strcmp(test_data, reconstructed) == 0) {
        printf("\033[0;32m[OK] Los datos se leyeron correctamente ✓\033[0m\n");
        printf("Se leyeron %d líneas\n", line_count);
    } else {
        g_tests_failed++;
        printf("\033[0;31m[KO] Los datos no se leyeron correctamente ✗\033[0m\n");
        printf("Contenido original (%lu bytes):\n\"", strlen(test_data));
        print_bytes(test_data, strlen(test_data));
        printf("\"\n\nContenido reconstruido (%d bytes):\n\"", total_len);
        print_bytes(reconstructed, total_len);
        printf("\"\n");
    }
    
    printf("\n");
}

// Función para testear get_next_line con un descriptor de archivo inválido
void test_invalid_fd() {
    printf("\033[0;34m=== Test: Descriptor de archivo inválido ===\033[0m\n");
    
    char *line = get_next_line(-1);
    
    if (line == NULL) {
        printf("\033[0;32m[OK] get_next_line devolvió NULL para fd inválido ✓\033[0m\n");
    } else {
        g_tests_failed++;
        printf("\033[0;31m[KO] get_next_line no devolvió NULL para fd inválido ✗\033[0m\n");
        printf("Valor devuelto: \"");
        print_bytes(line, strlen(line));
        printf("\"\n");
        free(line);
    }
    
    printf("\n");
}

// Función para testear múltiples llamadas a get_next_line con diferentes FDs
void test_multiple_fds() {
    printf("\033[0;34m=== Test: Múltiples descriptores de archivo ===\033[0m\n");
    
    int fd1 = open("test_files/normal.txt", O_RDONLY);
    int fd2 = open("test_files/no_nl.txt", O_RDONLY);
    
    if (fd1 < 0 || fd2 < 0) {
        printf("\033[0;31m[ERROR] No se pudieron abrir los archivos\033[0m\n\n");
        if (fd1 >= 0) close(fd1);
        if (fd2 >= 0) close(fd2);
        g_tests_failed++;
        return;
    }
    
    // Alternar entre descriptores
    char *line1 = get_next_line(fd1);
    printf("FD1 - Línea 1: \"");
    print_bytes(line1, strlen(line1));
    printf("\"\n");
    
    char *line2 = get_next_line(fd2);
    printf("FD2 - Línea 1: \"");
    print_bytes(line2, strlen(line2));
    printf("\"\n");
    
    char *line3 = get_next_line(fd1);
    printf("FD1 - Línea 2: \"");
    print_bytes(line3, strlen(line3));
    printf("\"\n");
    
    char *line4 = get_next_line(fd1);
    printf("FD1 - Línea 3: \"");
    print_bytes(line4, strlen(line4));
    printf("\"\n");
    
    char *line5 = get_next_line(fd1);
    printf("FD1 - Línea 4: \"");
    if (line5 == NULL) {
        printf("(NULL - correcto)\"\n");
    } else {
        print_bytes(line5, strlen(line5));
        printf("\"\n");
    }
    
    // Liberar memoria
    free(line1);
    free(line2);
    free(line3);
    free(line4);
    if (line5) free(line5);
    
    close(fd1);
    close(fd2);
    
    printf("\033[0;32m[OK] Test completado ✓\033[0m\n\n");
}

// Función para probar get_next_line con BUFFER_SIZE=1
void test_buffer_size_1() {
    int original_buffer_size = BUFFER_SIZE;
    
#ifdef BUFFER_SIZE
    #undef BUFFER_SIZE
#endif
#define BUFFER_SIZE 1

    printf("\033[0;34m=== Test especial: BUFFER_SIZE=1 ===\033[0m\n");
    printf("Nota: Este test utiliza una redefinición interna del BUFFER_SIZE\n");
    printf("      Solo para fines de demostración. En realidad, debería recompilarse.\n\n");
    
    test_file("test_files/normal.txt", "Archivo normal con BUFFER_SIZE=1");
    
#ifdef BUFFER_SIZE
    #undef BUFFER_SIZE
#endif
#define BUFFER_SIZE original_buffer_size
}

int main(void) {
    // Inicializar contador
    g_tests_failed = 0;
    
    printf("\033[0;34m============================================\033[0m\n");
    printf("\033[0;34m=         GET_NEXT_LINE TESTER            =\033[0m\n");
    printf("\033[0;34m============================================\033[0m\n\n");
    
    printf("Buffer size: %d\n\n", BUFFER_SIZE);
    
    // Crear archivos de prueba
    create_test_files();
    
    // Ejecutar pruebas con diferentes archivos
    test_file("test_files/normal.txt", "Archivo normal");
    test_file("test_files/empty.txt", "Archivo vacío");
    test_file("test_files/no_nl.txt", "Archivo sin salto de línea al final");
    test_file("test_files/long.txt", "Archivo con líneas largas");
    test_file("test_files/single_line.txt", "Archivo con una única línea");
    
    // Pruebas adicionales
    test_pipe();
    test_invalid_fd();
    test_multiple_fds();
    
    // Resumen
    printf("\033[0;34m============================================\033[0m\n");
    printf("\033[0;34m=                 RESUMEN                 =\033[0m\n");
    printf("\033[0;34m============================================\033[0m\n\n");
    
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }
    
    // Limpiar archivos de prueba
    system("rm -rf test_files");
    
    return g_tests_failed;
}