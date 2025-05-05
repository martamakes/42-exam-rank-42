/*
 * gnl_steps_1_2.c - Pasos 1 y 2 de la guía de get_next_line
 */

#include "guide_utils.h"

// Paso 1: Introducción a get_next_line
void gnl_step1_intro() {
    show_banner("PASO 1: Entendiendo get_next_line");
    
    show_instruction("Bienvenido a la guía interactiva para implementar get_next_line. "
                    "En este ejercicio, deberás crear una función que lea una línea "
                    "desde un descriptor de archivo.");
    
    show_instruction("Primero, echemos un vistazo al enunciado del ejercicio:");
    
    const char *subject = "Escribe una función llamada get_next_line que devuelva una línea\n"
                         "leída desde un descriptor de archivo. La línea es una sucesión\n"
                         "de caracteres que termina con '\\n' (código ASCII 0x0a) o con\n"
                         "EOF (End Of File).\n\n"
                         "La función debe ser declarada así:\n"
                         "char *get_next_line(int fd);\n\n"
                         "La línea retornada debe incluir el '\\n' si existe. Cuando se\n"
                         "llega a EOF, se debe almacenar el buffer actual en un char * y\n"
                         "devolverlo. Si el buffer está vacío, se debe devolver NULL.\n\n"
                         "En caso de error, también se devuelve NULL. Tu programa se compilará\n"
                         "con el flag -D BUFFER_SIZE=xx, que se debe usar como tamaño del\n"
                         "buffer para las llamadas a read en tu función.\n\n"
                         "Funciones permitidas: read, free, malloc";
    
    printf("%s%s%s\n\n", CYAN, subject, RESET);
    
    show_instruction("Para implementar get_next_line, necesitarás entender los siguientes conceptos clave:");
    
    show_instruction("1. Descriptores de archivo (file descriptors): Son números enteros "
                    "que representan recursos abiertos en el sistema operativo. Por ejemplo, "
                    "0 es stdin, 1 es stdout, 2 es stderr. Cualquier archivo abierto con "
                    "open() tendrá un descriptor de archivo ≥ 3.");
    
    show_instruction("2. Lectura de archivos: La función read() permite leer datos de un "
                    "descriptor de archivo. Recibe tres parámetros: el descriptor, un buffer "
                    "donde se almacenarán los datos leídos y el número de bytes a leer.");
    
    show_instruction("3. Memoria dinámica: Necesitarás usar malloc() para reservar memoria "
                    "para la línea que vas a devolver, y free() para liberar memoria que ya "
                    "no necesites.");
    
    show_instruction("4. Manejo de buffer: El valor de BUFFER_SIZE determina cuántos bytes "
                    "leerás en cada llamada a read(). Este valor puede variar, por lo que "
                    "tu solución debe funcionar con cualquier BUFFER_SIZE positivo.");
    
    show_tip("La función read() devuelve el número de bytes leídos, 0 al llegar a EOF, o -1 en caso de error.");
    show_tip("Recuerda que un texto puede no terminar en '\\n'. En este caso, debes devolver la línea leída hasta EOF.");
    
    show_instruction("¡Vamos a empezar a programar!");
}

// Paso 2: Crear el archivo de cabecera
void gnl_step2_header() {
    show_banner("PASO 2: Creando el Header");
    
    show_instruction("Primero, vamos a crear un archivo de cabecera (get_next_line.h) con "
                    "la declaración de la función y algunas definiciones útiles.");
    
    const char *code_example = "#ifndef GET_NEXT_LINE_H\n"
                              "# define GET_NEXT_LINE_H\n\n"
                              "# include <unistd.h>\n"
                              "# include <stdlib.h>\n\n"
                              "// Si BUFFER_SIZE no está definido, usar un valor predeterminado\n"
                              "# ifndef BUFFER_SIZE\n"
                              "#  define BUFFER_SIZE 42\n"
                              "# endif\n\n"
                              "// Prototipo de la función principal\n"
                              "char *get_next_line(int fd);\n\n"
                              "#endif";
    
    show_code_example("get_next_line.h básico", code_example);
    
    show_instruction("Este archivo de cabecera incluye las bibliotecas necesarias (unistd.h para read "
                    "y stdlib.h para malloc/free), define BUFFER_SIZE si no está definido, y declara "
                    "el prototipo de nuestra función get_next_line.");
    
    show_instruction("Tu tarea ahora es crear un archivo get_next_line.h con este contenido.");
    
    show_tip("Las guardas de inclusión (#ifndef GET_NEXT_LINE_H, #define GET_NEXT_LINE_H, #endif) "
             "evitan que el archivo se incluya más de una vez, lo que podría causar errores de compilación.");
    
    // Verificar si el usuario ha creado el archivo
    const char *filename = "get_next_line.h";
    if (file_exists(filename)) {
        show_instruction("¡Bien! Parece que ya has creado el archivo get_next_line.h. "
                        "Vamos a ver su contenido:");
        show_student_code(filename);
    } else {
        show_warning("No he encontrado el archivo get_next_line.h. Por favor, créalo para continuar.");
    }
    
    show_instruction("Ahora estamos listos para implementar la función get_next_line.");
}
