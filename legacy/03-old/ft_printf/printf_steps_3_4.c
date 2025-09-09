/*
 * printf_steps_3_4.c - Pasos 3 y 4 de la guía de ft_printf
 */

#include "printf_guide_utils.h"

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
    
    show_instruction("En este código, estamos detectando el carácter '%' e implementando un "
                    "manejo básico de especificadores de formato.");
    
    show_instruction("Hemos añadido va_list para manejar los argumentos variables, y estamos "
                    "usando va_start y va_end para inicializar y limpiar la lista de argumentos.");
    
    show_tip("va_start inicializa la lista de argumentos variables, tomando va_list y el último parámetro fijo.");
    show_tip("va_end limpia la lista de argumentos variables cuando ya no la necesitamos.");
    
    // Verificar si el usuario ha actualizado el archivo
    const char *filename = "rendu/ft_printf/ft_printf.c";
    if (file_exists(filename)) {
        show_instruction("Parece que ya has creado ft_printf.c. Ahora actualízalo para "
                        "detectar especificadores de formato como se muestra en el ejemplo.");
    } else {
        show_warning("No he encontrado el archivo ft_printf.c. Por favor, créalo para continuar.");
    }
}

// Paso 4: Manejo de strings
void step4_handle_strings() {
    show_banner("PASO 4: Manejo de Strings (%s)");
    
    show_instruction("Ahora vamos a implementar la impresión de strings (%s). Para ello, "
                    "necesitamos crear una función auxiliar que imprima una cadena de caracteres.");
    
    const char *code_example_putstr = "// Función para imprimir una cadena\n"
                                     "void ft_putstr(char *str, int *len)\n"
                                     "{\n"
                                     "    // Si el puntero es NULL, imprimir \"(null)\"\n"
                                     "    if (!str)\n"
                                     "        str = \"(null)\";\n\n"
                                     "    // Imprimir la cadena carácter por carácter\n"
                                     "    while (*str)\n"
                                     "    {\n"
                                     "        *len += write(1, str, 1);\n"
                                     "        str++;\n"
                                     "    }\n"
                                     "}";
    
    show_code_example("Función auxiliar para imprimir strings", code_example_putstr);
    
    const char *code_example_printf = "int ft_printf(const char *format, ...)\n"
                                     "{\n"
                                     "    int count = 0;\n"
                                     "    va_list args;\n\n"
                                     "    va_start(args, format);\n\n"
                                     "    while (*format)\n"
                                     "    {\n"
                                     "        if (*format == '%' && *(format + 1))\n"
                                     "        {\n"
                                     "            format++;\n\n"
                                     "            if (*format == 's')  // Si es %s, imprimir una cadena\n"
                                     "            {\n"
                                     "                ft_putstr(va_arg(args, char *), &count);\n"
                                     "            }\n"
                                     "            else  // Si es otro especificador que aún no manejamos\n"
                                     "            {\n"
                                     "                count += write(1, format, 1);\n"
                                     "            }\n"
                                     "        }\n"
                                     "        else\n"
                                     "        {\n"
                                     "            count += write(1, format, 1);\n"
                                     "        }\n"
                                     "        format++;\n"
                                     "    }\n\n"
                                     "    va_end(args);\n"
                                     "    return count;\n"
                                     "}";
    
    show_code_example("ft_printf con manejo de strings", code_example_printf);
    
    show_instruction("En este código hemos añadido una función ft_putstr para imprimir cadenas "
                    "de caracteres, y hemos modificado ft_printf para manejar el especificador %s.");
    
    show_instruction("Nota cómo usamos va_arg para obtener el siguiente argumento de la lista, "
                    "especificando que esperamos un char *.");
    
    show_tip("va_arg(args, tipo) obtiene el siguiente argumento de la lista, donde 'tipo' es el tipo de dato esperado.");
    show_tip("No olvides manejar el caso de punteros NULL, como se hace en ft_putstr.");
    
    // Verificar si el usuario ha actualizado el archivo
    const char *filename = "rendu/ft_printf/ft_printf.c";
    if (file_exists(filename)) {
        show_instruction("Ahora actualiza tu ft_printf.c para manejar el especificador %s, "
                        "como se muestra en el ejemplo.");
        
        // Crear un programa de prueba para strings
        const char *test_program = "/tmp/test_printf_strings.c";
        FILE *file = fopen(test_program, "w");
        if (file) {
            fprintf(file, "#include <stdio.h>\n\n");
            fprintf(file, "int ft_printf(const char *format, ...);\n\n");
            fprintf(file, "int main(void)\n");
            fprintf(file, "{\n");
            fprintf(file, "    int ret1, ret2;\n\n");
            fprintf(file, "    // Prueba 1: String simple\n");
            fprintf(file, "    ret1 = ft_printf(\"%%s\\n\", \"toto\");\n");
            fprintf(file, "    ret2 = printf(\"%%s\\n\", \"toto\");\n");
            fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
            fprintf(file, "    // Prueba 2: String NULL\n");
            fprintf(file, "    ret1 = ft_printf(\"%%s\\n\", NULL);\n");
            fprintf(file, "    ret2 = printf(\"%%s\\n\", NULL);\n");
            fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
            fprintf(file, "    return 0;\n");
            fprintf(file, "}\n");
            fclose(file);
            
            show_instruction("He creado un programa de prueba para verificar el manejo de strings. "
                            "Una vez que hayas implementado el soporte para %s, vamos a probarlo.");
            
            if (compile_and_test(filename, test_program)) {
                show_instruction("¡Bien hecho! Tu implementación parece estar funcionando correctamente "
                                "para strings. Vamos al siguiente paso.");
            } else {
                show_warning("Parece que hay problemas con tu implementación. "
                            "Revisa tu código y asegúrate de que maneje correctamente el especificador %s.");
            }
            
            // Eliminar el archivo de prueba
            unlink(test_program);
        }
    } else {
        show_warning("No he encontrado el archivo ft_printf.c. Por favor, créalo para continuar.");
    }
}
