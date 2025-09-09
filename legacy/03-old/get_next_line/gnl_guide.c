/*
 * gnl_guide.c - Programa interactivo para guiar la implementación de get_next_line
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

// Incluir utilidades comunes
#include "guide_utils.h"

// Prototipos de funciones para cada paso
void gnl_step1_intro();
void gnl_step2_header();
void gnl_step3_basic_implementation();
void gnl_step4_testing();
void gnl_step5_optimizing();
void gnl_step6_final_test();

int main() {
    show_banner("GUÍA INTERACTIVA: GET_NEXT_LINE");
    
    printf("%sBienvenido a la guía interactiva para implementar get_next_line.%s\n\n", YELLOW, RESET);
    printf("Esta guía te ayudará a entender e implementar get_next_line paso a paso.\n");
    printf("Para avanzar a través de la guía, simplemente sigue las instrucciones y presiona Enter cuando se te indique.\n\n");
    
    printf("%sPresiona Enter para comenzar...%s", CYAN, RESET);
    getchar();
    
    // Ejecutar cada paso de la guía
    gnl_step1_intro();
    gnl_step2_header();
    gnl_step3_basic_implementation();
    gnl_step4_testing();
    gnl_step5_optimizing();
    gnl_step6_final_test();
    
    show_banner("¡FELICIDADES!");
    printf("%s¡Has completado la guía de get_next_line!%s\n\n", GREEN, RESET);
    printf("Ahora tienes los conocimientos necesarios para implementar esta función en el examen.\n");
    printf("Recuerda practicar y asegurarte de entender completamente el código que has escrito.\n\n");
    
    return 0;
}
