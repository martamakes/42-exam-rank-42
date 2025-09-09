/*
 * ft_printf_guide.c - Programa interactivo para guiar la implementación de ft_printf
 * Creado para ayudar a los estudiantes de 42 a preparar el Exam Rank 03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

// Definición de colores para la salida
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

// Función para mostrar un banner
void show_banner(const char *title) {
    printf("\n%s", PURPLE);
    for (int i = 0; i < 80; i++) printf("*");
    printf("\n*%*s%s%*s*\n", 40 - (strlen(title) / 2), "", title, 40 - ((strlen(title) + 1) / 2), "");
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

// Funciones para cada paso de la implementación de ft_printf
void step1_intro();
void step2_basic_structure();
void step3_handle_format_specifiers();
void step4_handle_strings();
void step5_handle_integers();
void step6_test_full_implementation();

int main() {
    show_banner("GUÍA INTERACTIVA: FT_PRINTF");
    
    printf("%sBienvenido a la guía interactiva para implementar ft_printf.%s\n\n", YELLOW, RESET);
    printf("Esta guía te ayudará a entender e implementar ft_printf paso a paso.\n");
    printf("Para avanzar a través de la guía, simplemente sigue las instrucciones y presiona Enter cuando se te indique.\n\n");
    
    printf("%sPresiona Enter para comenzar...%s", CYAN, RESET);
    getchar();
    
    // Ejecutar cada paso de la guía
    step1_intro();
    step2_basic_structure();
    step3_handle_format_specifiers();
    step4_handle_strings();
    step5_handle_integers();
    step6_test_full_implementation();
    
    show_banner("¡FELICIDADES!");
    printf("%s¡Has completado la guía de ft_printf!%s\n\n", GREEN, RESET);
    printf("Ahora tienes los conocimientos necesarios para implementar esta función en el examen.\n");
    printf("Recuerda practicar y asegurarte de entender completamente el código que has escrito.\n\n");
    
    return 0;
}

// Paso 1: Introducción a ft_printf
void step1_intro() {
    show_banner("PASO 1: Entendiendo ft_printf");
    
    show_instruction("Bienvenido a la guía interactiva para implementar ft_printf. "
                    "En este ejercicio, deberás crear una versión simplificada de la función printf "
                    "que maneje solo las conversiones %s (strings), %d (enteros decimales) y %x "
                    "(enteros hexadecimales).");
    
    show_instruction("Primero, echemos un vistazo al enunciado del ejercicio:");
    
    const char *subject = "Debes crear una función llamada ft_printf que imite al printf original\n"
                         "pero solo debe manejar las conversiones: %s, %d y %x.\n\n"
                         "La función debe ser declarada así:\n"
                         "int ft_printf(const char *, ... );\n\n"
                         "Esta función devuelve el número de caracteres impresos.\n\n"
                         "Funciones permitidas: malloc, free, write, va_start, va_arg, va_copy, va_end\n\n"
                         "Ejemplos:\n"
                         "- ft_printf(\"%s\\n\", \"toto\") -> imprime \"toto\" y un salto de línea\n"
                         "- ft_printf(\"Magic %s is %d\", \"number\", 42) -> imprime \"Magic number is 42\"\n"
                         "- ft_printf(\"Hexadecimal for %d is %x\\n\", 42, 42) -> imprime \"Hexadecimal for 42 is 2a\\n\"";
    
    printf("%s%s%s\n\n", CYAN, subject, RESET);
    
    show_instruction("Para implementar ft_printf, necesitarás entender los siguientes conceptos clave:");
    
    show_instruction("1. Funciones variádicas: Son funciones que aceptan un número variable de argumentos. "
                    "En C, se manejan con macros de <stdarg.h>: va_list, va_start, va_arg, va_end.");
    
    show_instruction("2. Formato de printf: Debes analizar la cadena de formato para identificar "
                    "especificadores de formato (%s, %d, %x) y manejarlos adecuadamente.");
    
    show_instruction("3. Manejo de diferentes tipos de datos: Debes saber cómo convertir diferentes "
                    "tipos de datos (strings, enteros) a formato texto para imprimirlos.");
    
    show_tip("Consulta el man de printf para entender mejor su comportamiento: man 3 printf");
    show_tip("También deberías leer sobre argumentos variádicos: man va_arg");
    
    show_instruction("¡Vamos a empezar a programar!");
}

// Paso 2: Estructura básica de ft_printf
void step2_basic_structure() {
    show_banner("PASO 2: Estructura Básica de ft_printf");
    
    show_instruction("Primero, vamos a crear una estructura básica para ft_printf que simplemente "
                    "recorra la cadena de formato e imprima los caracteres normales (sin formatear nada aún).");
    
    const char *code_example = "#include <stdarg.h>\n"
                              "#include <unistd.h>\n\n"
                              "int ft_printf(const char *format, ...)\n"
                              "{\n"
                              "    int count = 0;  // Para contar caracteres impresos\n\n"
                              "    // Recorrer la cadena de formato\n"
                              "    while (*format)\n"
                              "    {\n"
                              "        // Por ahora, simplemente escribir cada carácter\n"
                              "        count += write(1, format, 1);\n"
                              "        format++;\n"
                              "    }\n\n"
                              "    return count;\n"
                              "}";
    
    show_code_example("Estructura básica de ft_printf", code_example);
    
    show_instruction("Este código simplemente recorre la cadena de formato e imprime cada carácter, "
                    "llevando la cuenta de cuántos caracteres se han impreso.");
    
    show_instruction("Tu tarea ahora es crear un archivo ft_printf.c con esta estructura básica.");
    
    show_tip("No olvides incluir las cabeceras necesarias: <stdarg.h> para va_list y <unistd.h> para write.");
    
    // Verificar si el usuario ha creado el archivo
    const char *filename = "ft_printf.c";
    if (file_exists(filename)) {
        show_instruction("¡Bien! Parece que ya has creado el archivo ft_printf.c. "
                        "Vamos a ver su contenido:");
        show_student_code(filename);
    } else {
        show_warning("No he encontrado el archivo ft_printf.c. Por favor, créalo para continuar.");
    }
}

// Paso 3: Manejo de especificadores de formato
void step3_handle_format_specifiers() {
    show_banner("PASO 3: Manejo de Especificadores de Formato");
    
    show_instruction("Ahora vamos a modificar nuestra función para que detecte y maneje "
                    "los especificadores de formato (%s, %d, %x).");
    
    const char *code_example = "int ft_printf(const char *format, ...)\n"
                              "{\n"
                              "    int count = 0;\n"
                              "    va_list args;  // Para manejar los argumentos variables\n\n"
                              "    // Inicializar la lista de argumentos\n"
                              "    va_start(args, format);\n\n"
                              "    // Recorrer la cadena de formato\n"
                              "    while (*format)\n"
                              "    {\n"
                              "        if (*format == '%' && *(format + 1))\n"
                              "        {\n"
                              "            format++;  // Avanzar al carácter después de %\n\n"
                              "            // Aquí manejaremos los especificadores de formato\n"
                              "            // Por ahora, simplemente imprimimos el carácter tal cual\n"
                              "            count += write(1, format, 1);\n"
                              "        }\n"
                              "        else  // Carácter normal, no es parte de un formato\n"
                              "        {\n"
                              "            count += write(1, format, 1);\n"
                              "        }\n"
                              "        format++;\n"
                              "    }\n\n"
                              "    // Limpiar recursos de lista de argumentos\n"
                              "    va_end(args);\n\n"
                              "    return count;\n"
                              "}";
    
    show_code_example("ft_printf con detección de especificadores", code_example);
    
    show_instruction("En este código, est