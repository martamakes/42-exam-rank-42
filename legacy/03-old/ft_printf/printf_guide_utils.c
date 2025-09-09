/*
 * printf_guide_utils.c - Implementación de funciones de utilidad para la guía de ft_printf
 */

#include "printf_guide_utils.h"

// Función para mostrar un banner
void show_banner(const char *title) {
    printf("\n%s", PURPLE);
    for (int i = 0; i < 80; i++) printf("*");
    printf("\n*%*s%s%*s*\n", 40 - (int)(strlen(title) / 2), "", title, 40 - (int)((strlen(title) + 1) / 2), "");
    for (int i = 0; i < 80; i++) printf("*");
    printf("%s\n\n", RESET);
}

// Función para mostrar instrucciones y esperar a que el usuario continúe
void show_instruction(const char *instruction) {
    printf("%s%s%s\n", YELLOW, instruction, RESET);
    printf("%sPresiona Enter para continuar...%s", CYAN, RESET);
    getchar();
}

// Función para mostrar un consejo
void show_tip(const char *tip) {
    printf("%s💡 TIP: %s%s\n\n", GREEN, tip, RESET);
}

// Función para mostrar un error o advertencia
void show_warning(const char *warning) {
    printf("%s⚠️ ATENCIÓN: %s%s\n\n", RED, warning, RESET);
}

// Función para mostrar código de ejemplo
void show_code_example(const char *description, const char *code) {
    printf("%s%s:%s\n", BLUE, description, RESET);
    printf("```c\n%s\n```\n\n", code);
}

// Función para verificar si un archivo existe
int file_exists(const char *filename) {
    return access(filename, F_OK) == 0;
}

// Función para verificar si un directorio existe
int dir_exists(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (dir) {
        closedir(dir);
        return 1;
    }
    return 0;
}

// Función para compilar y probar el código del estudiante
int compile_and_test(const char *filename, const char *test_program) {
    char cmd[256];
    
    // Compilar el código
    sprintf(cmd, "gcc -Wall -Wextra -Werror -o test %s %s", filename, test_program);
    if (system(cmd) != 0) {
        printf("%sError al compilar. Revisa tu código.%s\n", RED, RESET);
        return 0;
    }
    
    // Ejecutar el test
    printf("%sEjecutando las pruebas...%s\n", BLUE, RESET);
    system("./test");
    
    // Eliminar el ejecutable
    system("rm -f test");
    
    return 1;
}

// Función para mostrar el contenido del archivo del estudiante
void show_student_code(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("%sNo se pudo abrir el archivo %s%s\n", RED, filename, RESET);
        return;
    }
    
    char line[256];
    printf("%sContenido de %s:%s\n", BLUE, filename, RESET);
    printf("```c\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("```\n\n");
    
    fclose(file);
}
