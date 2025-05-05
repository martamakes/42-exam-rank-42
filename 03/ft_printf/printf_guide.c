/*
 * printf_guide.c - Programa principal para la guía interactiva de ft_printf
 */

#include "printf_guide_utils.h"

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
