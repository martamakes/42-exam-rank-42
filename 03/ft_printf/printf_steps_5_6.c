/*
 * printf_steps_5_6.c - Pasos 5 y 6 de la guía de ft_printf
 */

#include "printf_guide_utils.h"

// Paso 5: Manejo de enteros
void step5_handle_integers() {
    show_banner("PASO 5: Manejo de Enteros (%d y %x)");
    
    show_instruction("Ahora vamos a implementar la impresión de enteros en formato decimal (%d) "
                    "y hexadecimal (%x). Para ello, crearemos una función auxiliar que imprima "
                    "un número en cualquier base.");
    
    const char *code_example_putnbr = "// Función para imprimir un número en cualquier base\n"
                                     "void ft_putnbr_base(long long n, int base, int *len)\n"
                                     "{\n"
                                     "    char *digits = \"0123456789abcdef\";\n\n"
                                     "    // Manejar signo negativo para base decimal\n"
                                     "    if (n < 0 && base == 10)\n"
                                     "    {\n"
                                     "        *len += write(1, \"-\", 1);\n"
                                     "        n = -n;\n"
                                     "    }\n\n"
                                     "    // Conversión recursiva\n"
                                     "    if (n >= base)\n"
                                     "        ft_putnbr_base(n / base, base, len);\n\n"
                                     "    *len += write(1, &digits[n % base], 1);\n"
                                     "}";
    
    show_code_example("Función auxiliar para imprimir números", code_example_putnbr);
    
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
                                     "            if (*format == 's')\n"
                                     "                ft_putstr(va_arg(args, char *), &count);\n"
                                     "            else if (*format == 'd')  // Decimal\n"
                                     "                ft_putnbr_base(va_arg(args, int), 10, &count);\n"
                                     "            else if (*format == 'x')  // Hexadecimal\n"
                                     "                ft_putnbr_base(va_arg(args, unsigned int), 16, &count);\n"
                                     "            else\n"
                                     "                count += write(1, format, 1);\n"
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
    
    show_code_example("ft_printf con manejo de enteros", code_example_printf);
    
    show_instruction("Hemos añadido una función ft_putnbr_base que puede imprimir números en cualquier base, "
                    "y hemos modificado ft_printf para manejar los especificadores %d y %x.");
    
    show_instruction("La función ft_putnbr_base convierte el número a la base especificada de forma recursiva "
                    "y luego imprime los dígitos correspondientes.");
    
    show_tip("Usamos long long para manejar valores negativos correctamente.");
    show_tip("Para %x (hexadecimal), usamos unsigned int porque queremos representación sin signo.");
    show_tip("La recursión es una forma elegante de imprimir los dígitos en el orden correcto.");
    
    // Verificar si el usuario ha actualizado el archivo
    const char *filename = "rendu/ft_printf/ft_printf.c";
    if (file_exists(filename)) {
        show_instruction("Ahora actualiza tu ft_printf.c para manejar los especificadores %d y %x, "
                        "como se muestra en el ejemplo.");
        
        // Crear un programa de prueba para enteros
        const char *test_program = "/tmp/test_printf_integers.c";
        FILE *file = fopen(test_program, "w");
        if (file) {
            fprintf(file, "#include <stdio.h>\n\n");
            fprintf(file, "int ft_printf(const char *format, ...);\n\n");
            fprintf(file, "int main(void)\n");
            fprintf(file, "{\n");
            fprintf(file, "    int ret1, ret2;\n\n");
            fprintf(file, "    // Prueba 1: Número decimal positivo\n");
            fprintf(file, "    ret1 = ft_printf(\"%%d\\n\", 42);\n");
            fprintf(file, "    ret2 = printf(\"%%d\\n\", 42);\n");
            fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
            fprintf(file, "    // Prueba 2: Número decimal negativo\n");
            fprintf(file, "    ret1 = ft_printf(\"%%d\\n\", -42);\n");
            fprintf(file, "    ret2 = printf(\"%%d\\n\", -42);\n");
            fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
            fprintf(file, "    // Prueba 3: Número hexadecimal\n");
            fprintf(file, "    ret1 = ft_printf(\"%%x\\n\", 255);\n");
            fprintf(file, "    ret2 = printf(\"%%x\\n\", 255);\n");
            fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
            fprintf(file, "    return 0;\n");
            fprintf(file, "}\n");
            fclose(file);
            
            show_instruction("He creado un programa de prueba para verificar el manejo de enteros. "
                            "Una vez que hayas implementado el soporte para %d y %x, vamos a probarlo.");
            
            if (compile_and_test(filename, test_program)) {
                show_instruction("¡Bien hecho! Tu implementación parece estar funcionando correctamente "
                                "para enteros. Vamos al siguiente paso.");
            } else {
                show_warning("Parece que hay problemas con tu implementación. "
                            "Revisa tu código y asegúrate de que maneje correctamente los especificadores %d y %x.");
            }
            
            // Eliminar el archivo de prueba
            unlink(test_program);
        }
    } else {
        show_warning("No he encontrado el archivo ft_printf.c. Por favor, créalo para continuar.");
    }
}

// Paso 6: Prueba final
void step6_test_full_implementation() {
    show_banner("PASO 6: Prueba Final de ft_printf");
    
    show_instruction("¡Felicitaciones! Ahora tienes una implementación completa de ft_printf "
                    "que maneja los especificadores %s, %d y %x. Vamos a hacer una prueba final "
                    "para asegurarnos de que todo funciona correctamente.");
    
    // Crear un programa de prueba completo
    const char *test_program = "/tmp/test_printf_full.c";
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
        fprintf(file, "    // Prueba 2: String y decimal\n");
        fprintf(file, "    ret1 = ft_printf(\"Magic %%s is %%d\\n\", \"number\", 42);\n");
        fprintf(file, "    ret2 = printf(\"Magic %%s is %%d\\n\", \"number\", 42);\n");
        fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
        fprintf(file, "    // Prueba 3: Decimal y hexadecimal\n");
        fprintf(file, "    ret1 = ft_printf(\"Hexadecimal for %%d is %%x\\n\", 42, 42);\n");
        fprintf(file, "    ret2 = printf(\"Hexadecimal for %%d is %%x\\n\", 42, 42);\n");
        fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
        fprintf(file, "    // Prueba 4: Caso extremo - NULL\n");
        fprintf(file, "    ret1 = ft_printf(\"String NULL: %%s\\n\", NULL);\n");
        fprintf(file, "    ret2 = printf(\"String NULL: %%s\\n\", NULL);\n");
        fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
        fprintf(file, "    // Prueba 5: Caso extremo - Números grandes\n");
        fprintf(file, "    ret1 = ft_printf(\"Números grandes: %%d, %%x\\n\", 2147483647, 2147483647);\n");
        fprintf(file, "    ret2 = printf(\"Números grandes: %%d, %%x\\n\", 2147483647, 2147483647);\n");
        fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
        fprintf(file, "    // Prueba 6: Caso extremo - Números negativos\n");
        fprintf(file, "    ret1 = ft_printf(\"Negativos: %%d, %%x\\n\", -42, -42);\n");
        fprintf(file, "    ret2 = printf(\"Negativos: %%d, %%x\\n\", -42, -42);\n");
        fprintf(file, "    printf(\"ft_printf: %%d, printf: %%d\\n\", ret1, ret2);\n\n");
        fprintf(file, "    return 0;\n");
        fprintf(file, "}\n");
        fclose(file);
        
        const char *filename = "rendu/ft_printf/ft_printf.c";
        if (file_exists(filename)) {
            show_instruction("Vamos a ejecutar una prueba completa de tu implementación de ft_printf:");
            
            if (compile_and_test(filename, test_program)) {
                show_instruction("¡Excelente trabajo! Tu implementación de ft_printf está completa "
                                "y funciona correctamente para todos los casos de prueba.");
                
                show_instruction("Resumen de lo que has aprendido:\n"
                               "1. Manejo de funciones variádicas (va_list, va_start, va_arg, va_end)\n"
                               "2. Análisis de cadenas de formato\n"
                               "3. Impresión de diferentes tipos de datos\n"
                               "4. Recursión");
                
                show_warning("Recuerda que en el examen tendrás que implementar esta función desde cero, "
                           "así que asegúrate de entender bien cada parte del código.");
                
                show_instruction("Puedes volver a ejecutar esta guía en cualquier momento para repasar "
                                "la implementación de ft_printf. ¡Buena suerte en el examen!");
            } else {
                show_warning("Parece que hay problemas con tu implementación final. "
                            "Revisa tu código y asegúrate de que maneje correctamente todos los casos de prueba.");
            }
            
            // Eliminar el archivo de prueba
            unlink(test_program);
        } else {
            show_warning("No he encontrado el archivo ft_printf.c. Por favor, créalo para continuar.");
        }
    }
}
