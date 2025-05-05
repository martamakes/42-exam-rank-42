/*
 * printf_guide_utils.h - Funciones de utilidad para la guía de ft_printf
 */

#ifndef PRINTF_GUIDE_UTILS_H
#define PRINTF_GUIDE_UTILS_H

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
void show_banner(const char *title);

// Función para mostrar instrucciones y esperar a que el usuario continúe
void show_instruction(const char *instruction);

// Función para mostrar un consejo
void show_tip(const char *tip);

// Función para mostrar un error o advertencia
void show_warning(const char *warning);

// Función para mostrar código de ejemplo
void show_code_example(const char *description, const char *code);

// Función para verificar si un archivo existe
int file_exists(const char *filename);

// Función para verificar si un directorio existe
int dir_exists(const char *dirname);

// Función para compilar y probar el código del estudiante
int compile_and_test(const char *filename, const char *test_program);

// Función para mostrar el contenido del archivo del estudiante
void show_student_code(const char *filename);

// Prototipos de funciones para los pasos
void step1_intro();
void step2_basic_structure();
void step3_handle_format_specifiers();
void step4_handle_strings();
void step5_handle_integers();
void step6_test_full_implementation();

#endif /* PRINTF_GUIDE_UTILS_H */
