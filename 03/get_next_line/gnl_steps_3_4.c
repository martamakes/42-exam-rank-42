/*
 * gnl_steps_3_4.c - Pasos 3 y 4 de la guía de get_next_line
 */

#include "guide_utils.h"

// Paso 3: Implementación básica de get_next_line
void gnl_step3_basic_implementation() {
    show_banner("PASO 3: Implementación Básica");
    
    show_instruction("Ahora vamos a crear una implementación simple de get_next_line "
                    "que lea carácter por carácter hasta encontrar un salto de línea o EOF.");
    
    const char *code_example = "#include \"get_next_line.h\"\n\n"
                              "char *get_next_line(int fd)\n"
                              "{\n"
                              "    char *line;         // Para almacenar la línea leída\n"
                              "    char buffer;        // Buffer de un solo carácter\n"
                              "    int i = 0;          // Índice para la línea\n"
                              "    int bytes_read;     // Bytes leídos por read()\n"
                              "    \n"
                              "    // Verificar parámetros inválidos\n"
                              "    if (fd < 0 || BUFFER_SIZE <= 0)\n"
                              "        return (NULL);\n"
                              "    \n"
                              "    // Asignar memoria para la línea (tamaño arbitrario grande para simplificar)\n"
                              "    line = (char *)malloc(10000);\n"
                              "    if (!line)\n"
                              "        return (NULL);\n"
                              "    \n"
                              "    // Leer caracteres uno por uno hasta encontrar '\\n' o EOF\n"
                              "    bytes_read = read(fd, &buffer, 1);\n"
                              "    while (bytes_read > 0)\n"
                              "    {\n"
                              "        line[i++] = buffer;\n"
                              "        \n"
                              "        // Si encontramos un salto de línea, terminamos\n"
                              "        if (buffer == '\\n')\n"
                              "            break;\n"
                              "        \n"
                              "        bytes_read = read(fd, &buffer, 1);\n"
                              "    }\n"
                              "    \n"
                              "    // Si no leímos nada o hubo un error, liberar memoria y devolver NULL\n"
                              "    if (bytes_read <= 0 && i == 0)\n"
                              "    {\n"
                              "        free(line);\n"
                              "        return (NULL);\n"
                              "    }\n"
                              "    \n"
                              "    // Añadir el terminador nulo\n"
                              "    line[i] = '\\0';\n"
                              "    \n"
                              "    return (line);\n"
                              "}";
    
    show_code_example("Implementación básica de get_next_line", code_example);
    
    show_instruction("Esta implementación básica hace lo siguiente:");
    show_instruction("1. Verifica que el descriptor de archivo y BUFFER_SIZE sean válidos.");
    show_instruction("2. Reserva memoria para la línea (usamos un tamaño fijo grande por ahora).");
    show_instruction("3. Lee caracteres uno por uno hasta encontrar un salto de línea o EOF.");
    show_instruction("4. Si llega a EOF sin leer nada o hay un error, devuelve NULL.");
    show_instruction("5. Añade el terminador nulo al final de la línea y la devuelve.");
    
    show_tip("Esta implementación es simple pero ineficiente: hacer una llamada a read() por cada carácter es lento.");
    show_tip("En el examen, esta implementación básica puede ser suficiente, pero es importante entender cómo optimizarla.");
    
    show_instruction("Tu tarea ahora es crear un archivo get_next_line.c con esta implementación básica o una similar.");
    
    // Verificar si el usuario ha creado el archivo
    const char *filename = "rendu/get_next_line/get_next_line.c";
    if (file_exists(filename)) {
        show_instruction("¡Bien! Parece que ya has creado el archivo get_next_line.c. "
                        "Vamos a ver su contenido:");
        show_student_code(filename);
    } else {
        show_warning("No he encontrado el archivo get_next_line.c. Por favor, créalo para continuar.");
    }
}

// Paso 4: Probar la implementación básica
void gnl_step4_testing() {
    show_banner("PASO 4: Probando get_next_line");
    
    show_instruction("Ahora vamos a probar nuestra implementación de get_next_line para asegurarnos "
                    "de que funciona correctamente.");
    
    // Crear un archivo de prueba
    const char *test_filename = "test.txt";
    const char *test_content = "Primera línea\nSegunda línea\nTercera línea sin newline";
    create_test_file(test_filename, test_content);
    
    show_instruction("He creado un archivo de prueba llamado 'test.txt' con el siguiente contenido:");
    printf("%s\n", test_content);
    
    // Crear un programa de prueba
    const char *test_program = "/tmp/test_gnl.c";
    FILE *file = fopen(test_program, "w");
    if (file) {
        fprintf(file, "#include <stdio.h>\n");
        fprintf(file, "#include <fcntl.h>\n");
        fprintf(file, "#include \"get_next_line.h\"\n\n");
        fprintf(file, "int main(void)\n");
        fprintf(file, "{\n");
        fprintf(file, "    int fd;\n");
        fprintf(file, "    char *line;\n");
        fprintf(file, "    int line_count = 0;\n\n");
        fprintf(file, "    // Abrir el archivo de prueba\n");
        fprintf(file, "    fd = open(\"%s\", O_RDONLY);\n", test_filename);
        fprintf(file, "    if (fd == -1)\n");
        fprintf(file, "    {\n");
        fprintf(file, "        printf(\"Error al abrir el archivo\\n\");\n");
        fprintf(file, "        return (1);\n");
        fprintf(file, "    }\n\n");
        fprintf(file, "    // Leer y mostrar líneas una por una\n");
        fprintf(file, "    while ((line = get_next_line(fd)) != NULL)\n");
        fprintf(file, "    {\n");
        fprintf(file, "        line_count++;\n");
        fprintf(file, "        printf(\"Línea %%d: %%s\", line_count, line);\n");
        fprintf(file, "        // Si la línea no termina con newline, añadirlo para la salida\n");
        fprintf(file, "        if (line[0] != '\\0' && line[strlen(line) - 1] != '\\n')\n");
        fprintf(file, "            printf(\"\\n\");\n");
        fprintf(file, "        free(line);\n");
        fprintf(file, "    }\n\n");
        fprintf(file, "    close(fd);\n");
        fprintf(file, "    printf(\"Total de líneas leídas: %%d\\n\", line_count);\n");
        fprintf(file, "    return (0);\n");
        fprintf(file, "}\n");
        fclose(file);
        
        show_instruction("He creado un programa de prueba que abre el archivo, lee todas las líneas "
                        "usando get_next_line y las muestra junto con un contador de líneas.");
        
        const char *gnl_filename = "rendu/get_next_line/get_next_line.c";
        if (file_exists(gnl_filename)) {
            show_instruction("Vamos a compilar y ejecutar el programa de prueba:");
            
            // Compilar con el valor predeterminado de BUFFER_SIZE
            if (compile_and_test(gnl_filename, test_program, "-D BUFFER_SIZE=1")) {
                show_instruction("¡Bien hecho! Tu implementación parece estar funcionando correctamente "
                                "con BUFFER_SIZE=1.");
                
                show_instruction("Ahora probemos con un BUFFER_SIZE mayor:");
                
                // Compilar con un BUFFER_SIZE mayor
                if (compile_and_test(gnl_filename, test_program, "-D BUFFER_SIZE=42")) {
                    show_instruction("¡Excelente! Tu implementación funciona con diferentes valores de BUFFER_SIZE.");
                } else {
                    show_warning("Parece que hay problemas con tu implementación cuando BUFFER_SIZE es mayor. "
                                "Revisa tu código y asegúrate de que maneje correctamente el buffer.");
                }
            } else {
                show_warning("Parece que hay problemas con tu implementación básica. "
                            "Revisa tu código y asegúrate de que lee correctamente carácter por carácter.");
            }
            
            // Eliminar el archivo de prueba
            unlink(test_program);
        } else {
            show_warning("No he encontrado el archivo get_next_line.c. Por favor, créalo para continuar.");
        }
    }
}
