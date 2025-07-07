/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42madrid.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by marta             #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** MAIN.C - Función principal del programa
** 
** Punto de entrada que maneja argumentos de línea de comandos
** y coordina la inicialización del sistema de exámenes.
*/

#include "examshell.h"

/*
** validate_rank: Verifica que el rank solicitado esté disponible
*/
static int validate_rank(int selected_rank, int *available_ranks, int rank_count)
{
    for (int i = 0; i < rank_count; i++)
    {
        if (available_ranks[i] == selected_rank)
            return (1);
    }
    return (0);
}

/*
** print_usage: Muestra información de uso del programa
*/
static void print_usage(const char *program_name)
{
    printf("Usage: %s [rank_number]\n", program_name);
    printf("Example: %s 03\n", program_name);
    printf("If no rank is specified, you will be prompted to choose.\n");
}

/*
** print_available_ranks: Muestra los ranks disponibles
*/
static void print_available_ranks(int *available_ranks, int rank_count)
{
    printf("Available ranks: ");
    for (int i = 0; i < rank_count; i++)
        printf("%02d ", available_ranks[i]);
    printf("\n");
}

/*
** main: Función principal del programa
*/
int main(int argc, char **argv)
{
    int *available_ranks;
    int rank_count;
    int selected_rank;

    print_header();
    
    // Detectar ranks disponibles
    rank_count = detect_available_ranks(&available_ranks);
    if (rank_count == 0)
    {
        printf(RED "Error: No exam ranks found in %s\n" RESET, BASE_PATH);
        printf("Make sure you have directories like 'exam-rank-03', 'exam-rank-04', etc.\n");
        return (1);
    }

    // Procesar argumentos de línea de comandos
    if (argc == 2)
    {
        selected_rank = atoi(argv[1]);
        
        if (!validate_rank(selected_rank, available_ranks, rank_count))
        {
            printf(RED "Error: Exam rank %02d not found.\n" RESET, selected_rank);
            print_available_ranks(available_ranks, rank_count);
            free(available_ranks);
            return (1);
        }
    }
    else if (argc == 1)
    {
        selected_rank = get_rank_from_user(available_ranks, rank_count);
    }
    else
    {
        print_usage(argv[0]);
        free(available_ranks);
        return (1);
    }

    // Verificar si continuar sesión anterior o empezar nueva
    if (!ask_continue_or_new_session(selected_rank))
    {
        free(available_ranks);
        return (0);
    }

    // Iniciar sesión de examen
    run_exam_session(selected_rank);
    
    // Limpiar directorios subject al salir
    cleanup_subject_directories();
    
    free(available_ranks);
    return (0);
}
