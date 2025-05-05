/*
 * printf_steps_1_2.c - Pasos 1 y 2 de la guía de ft_printf
 */

#include "printf_guide_utils.h"

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
    const char *filename = "rendu/ft_printf/ft_printf.c";
    if (file_exists(filename)) {
        show_instruction("¡Bien! Parece que ya has creado el archivo ft_printf.c. "
                        "Vamos a ver su contenido:");
        show_student_code(filename);
    } else {
        show_warning("No he encontrado el archivo ft_printf.c. Por favor, créalo para continuar.");
    }
}
