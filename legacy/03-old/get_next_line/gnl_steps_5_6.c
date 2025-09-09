/*
 * gnl_steps_5_6.c - Pasos 5 y 6 de la guía de get_next_line
 */

#include "guide_utils.h"

// Paso 5: Optimización - Usar BUFFER_SIZE para la lectura
void gnl_step5_optimizing() {
    show_banner("PASO 5: Optimizando get_next_line");
    
    show_instruction("Nuestra implementación básica funciona, pero es ineficiente porque hace una llamada "
                    "a read() por cada carácter. Vamos a optimizarla usando BUFFER_SIZE para leer "
                    "varios caracteres a la vez.");
    
    const char *code_example = "#include \"get_next_line.h\"\n\n"
                              "char *get_next_line(int fd)\n"
                              "{\n"
                              "    char *line;                // Para almacenar la línea leída\n"
                              "    static char buffer[BUFFER_SIZE + 1]; // Buffer estático para mantener datos entre llamadas\n"
                              "    int i = 0;                 // Índice para la línea\n"
                              "    static int buffer_pos = 0; // Posición actual en el buffer\n"
                              "    static int bytes_in_buffer = 0; // Bytes disponibles en el buffer\n"
                              "    \n"
                              "    // Verificar parámetros inválidos\n"
                              "    if (fd < 0 || BUFFER_SIZE <= 0)\n"
                              "        return (NULL);\n"
                              "    \n"
                              "    // Asignar memoria para la línea\n"
                              "    line = (char *)malloc(10000);\n"
                              "    if (!line)\n"
                              "        return (NULL);\n"
                              "    \n"
                              "    // Mientras no encontremos newline o EOF\n"
                              "    while (1)\n"
                              "    {\n"
                              "        // Si no hay más datos en el buffer, leer más\n"
                              "        if (buffer_pos >= bytes_in_buffer)\n"
                              "        {\n"
                              "            buffer_pos = 0;\n"
                              "            bytes_in_buffer = read(fd, buffer, BUFFER_SIZE);\n"
                              "            buffer[bytes_in_buffer] = '\\0';\n"
                              "            \n"
                              "            // Si no hay más datos para leer\n"
                              "            if (bytes_in_buffer <= 0)\n"
                              "                break;\n"
                              "        }\n"
                              "        \n"
                              "        // Añadir el carácter actual a la línea\n"
                              "        line[i++] = buffer[buffer_pos++];\n"
                              "        \n"
                              "        // Si encontramos newline, terminar\n"
                              "        if (line[i - 1] == '\\n')\n"
                              "            break;\n"
                              "    }\n"
                              "    \n"
                              "    // Si no leímos nada o hubo un error\n"
                              "    if (i == 0 || bytes_in_buffer < 0)\n"
                              "    {\n"
                              "        free(line);\n"
                              "        return (NULL);\n"
                              "    }\n"
                              "    \n"
                              "    // Añadir terminador nulo\n"
                              "    line[i] = '\\0';\n"
                              "    \n"
                              "    return (line);\n"
                              "}";
    
    show_code_example("Implementación optimizada de get_next_line", code_example);
    
    show_instruction("Esta implementación optimizada hace lo siguiente:");
    show_instruction("1. Usa un buffer estático para mantener datos entre llamadas a get_next_line.");
    show_instruction("2. Llena el buffer con BUFFER_SIZE bytes cuando está vacío.");
    show_instruction("3. Procesa caracteres desde el buffer hasta encontrar un salto de línea o EOF.");
    show_instruction("4. Conserva la posición actual en el buffer entre llamadas para no perder datos.");
    
    show_tip("El uso de variables estáticas permite que get_next_line recuerde dónde se quedó entre llamadas.");
    show_tip("Esta implementación es más eficiente porque hace menos llamadas a read().");
    
    show_warning("Ten en cuenta que este enfoque tiene limitaciones. En particular, no maneja bien "
                "múltiples descriptores de archivo. Para eso, necesitarías un array de buffers o usar "
                "memoria dinámica.");
    
    show_instruction("Tu tarea ahora es actualizar tu implementación para usar BUFFER_SIZE de manera eficiente.");
    
    // Verificar si el usuario ha actualizado el archivo
    const char *filename = "rendu/get_next_line/get_next_line.c";
    if (file_exists(filename)) {
        show_instruction("Vamos a ver tu implementación actual:");
        show_student_code(filename);
        
        show_instruction("Ahora actualiza tu código para usar BUFFER_SIZE de manera más eficiente.");
    } else {
        show_warning("No he encontrado el archivo get_next_line.c. Por favor, créalo para continuar.");
    }
}

// Paso 6: Prueba final
void gnl_step6_final_test() {
    show_banner("PASO 6: Prueba Final de get_next_line");
    
    show_instruction("¡Felicitaciones! Has implementado get_next_line de manera eficiente. "
                    "Vamos a hacer una prueba final con diferentes BUFFER_SIZE y archivos más complejos.");
    
    // Crear un archivo de prueba más complejo
    const char *test_filename1 = "test_complex.txt";
    const char *test_content1 = "Esta es una línea larga que podría exceder algunos valores de BUFFER_SIZE para probar cómo maneja tu implementación los buffers parciales.\n"
                               "Esta es una línea muy corta.\n\n"
                               "Aquí hay una línea después de una línea vacía.\n"
                               "Y finalmente una línea sin salto de línea al final";
    create_test_file(test_filename1, test_content1);
    
    // Crear un archivo vacío
    const char *test_filename2 = "test_empty.txt";
    create_test_file(test_filename2, "");
    
    show_instruction("He creado dos archivos de prueba:");
    show_instruction("1. 'test_complex.txt': Un archivo con líneas de diferentes longitudes, líneas vacías y sin newline al final.");
    show_instruction("2. 'test_empty.txt': Un archivo vacío para probar el manejo de archivos sin contenido.");
    
    // Crear un programa de prueba más completo
    const char *test_program = "/tmp/test_gnl_final.c";
    FILE *file = fopen(test_program, "w");
    if (file) {
        fprintf(file, "#include <stdio.h>\n");
        fprintf(file, "#include <fcntl.h>\n");
        fprintf(file, "#include <string.h>\n");
        fprintf(file, "#include \"get_next_line.h\"\n\n");
        fprintf(file, "void test_file(const char *filename)\n");
        fprintf(file, "{\n");
        fprintf(file, "    int fd;\n");
        fprintf(file, "    char *line;\n");
        fprintf(file, "    int line_count = 0;\n\n");
        fprintf(file, "    printf(\"\\nProbando archivo: %%s\\n\", filename);\n");
        fprintf(file, "    fd = open(filename, O_RDONLY);\n");
        fprintf(file, "    if (fd == -1)\n");
        fprintf(file, "    {\n");
        fprintf(file, "        printf(\"Error al abrir el archivo\\n\");\n");
        fprintf(file, "        return;\n");
        fprintf(file, "    }\n\n");
        fprintf(file, "    while ((line = get_next_line(fd)) != NULL)\n");
        fprintf(file, "    {\n");
        fprintf(file, "        line_count++;\n");
        fprintf(file, "        printf(\"Línea %%d (%%zu chars): \", line_count, strlen(line));\n");
        fprintf(file, "        printf(\"%%s\", line);\n");
        fprintf(file, "        if (line[0] != '\\0' && line[strlen(line) - 1] != '\\n')\n");
        fprintf(file, "            printf(\"\\n\");\n");
        fprintf(file, "        free(line);\n");
        fprintf(file, "    }\n\n");
        fprintf(file, "    close(fd);\n");
        fprintf(file, "    printf(\"Total de líneas leídas: %%d\\n\", line_count);\n");
        fprintf(file, "}\n\n");
        fprintf(file, "int main(void)\n");
        fprintf(file, "{\n");
        fprintf(file, "    printf(\"Testing with BUFFER_SIZE = %%d\\n\", BUFFER_SIZE);\n\n");
        fprintf(file, "    test_file(\"%s\");\n", test_filename1);
        fprintf(file, "    test_file(\"%s\");\n", test_filename2);
        fprintf(file, "    \n");
        fprintf(file, "    // Probar con descriptor de archivo inválido\n");
        fprintf(file, "    char *line = get_next_line(-1);\n");
        fprintf(file, "    if (line == NULL)\n");
        fprintf(file, "        printf(\"\\nCorrecto: get_next_line(-1) devolvió NULL\\n\");\n");
        fprintf(file, "    else\n");
        fprintf(file, "    {\n");
        fprintf(file, "        printf(\"\\nError: get_next_line(-1) no devolvió NULL\\n\");\n");
        fprintf(file, "        free(line);\n");
        fprintf(file, "    }\n");
        fprintf(file, "    \n");
        fprintf(file, "    return (0);\n");
        fprintf(file, "}\n");
        fclose(file);
        
        const char *gnl_filename = "rendu/get_next_line/get_next_line.c";
        if (file_exists(gnl_filename)) {
            show_instruction("Vamos a probar tu implementación con diferentes valores de BUFFER_SIZE:");
            
            // Probar con BUFFER_SIZE pequeño
            show_instruction("Prueba con BUFFER_SIZE = 1:");
            compile_and_test(gnl_filename, test_program, "-D BUFFER_SIZE=1");
            
            // Probar con BUFFER_SIZE mediano
            show_instruction("Prueba con BUFFER_SIZE = 42:");
            compile_and_test(gnl_filename, test_program, "-D BUFFER_SIZE=42");
            
            // Probar con BUFFER_SIZE grande
            show_instruction("Prueba con BUFFER_SIZE = 10000:");
            compile_and_test(gnl_filename, test_program, "-D BUFFER_SIZE=10000");
            
            show_instruction("¡Felicitaciones! Has completado todas las pruebas de get_next_line.");
            
            // Consejo final
            show_instruction("Recuerda estos puntos importantes para tu implementación final:");
            show_instruction("1. Tu función debe manejar correctamente EOF, errores y líneas sin newline.");
            show_instruction("2. Debe funcionar con cualquier BUFFER_SIZE positivo.");
            show_instruction("3. Cuida las fugas de memoria: asegúrate de hacer free() cuando sea necesario.");
            show_instruction("4. Para el examen, una implementación más simple (incluso leyendo carácter por carácter) "
                            "puede ser suficiente si funciona correctamente y sin errores.");
            
            // Eliminar archivos de prueba
            unlink(test_program);
            unlink(test_filename1);
            unlink(test_filename2);
        } else {
            show_warning("No he encontrado el archivo get_next_line.c. Por favor, créalo para continuar.");
        }
    }
}
