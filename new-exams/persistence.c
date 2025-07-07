/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistence.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42madrid.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by marta             #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** PERSISTENCE.C - Sistema de persistencia y progreso
** 
** Este archivo maneja todo lo relacionado con guardar y cargar
** el progreso del usuario, estadísticas y sesiones anteriores.
*/

#include "examshell.h"

/*
** format_time: Convierte segundos a formato legible (hh:mm:ss)
*/
char *format_time(int seconds)
{
    static char buffer[32];
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, secs);
    return (buffer);
}

/*
** init_progress_data: Inicializa estructura de progreso
*/
void init_progress_data(t_progress_data *progress, int rank)
{
    progress->rank = rank;
    progress->level1_completed = 0;
    progress->level2_completed = 0;
    progress->current_level = 1;
    progress->total_time = 0;
    progress->session_start = time(NULL);
    progress->completed_count = 0;
    
    for (int i = 0; i < MAX_EXERCISES; i++)
        progress->completed_exercises[i][0] = '\0';
}

/*
** is_exercise_completed: Verifica si un ejercicio ya fue completado
*/
int is_exercise_completed(t_progress_data *progress, const char *exercise_name)
{
    for (int i = 0; i < progress->completed_count; i++)
    {
        if (strncmp(progress->completed_exercises[i], exercise_name, MAX_NAME_LEN) == 0)
            return (1);
    }
    return (0);
}

/*
** mark_exercise_completed: Marca un ejercicio como completado
*/
void mark_exercise_completed(t_progress_data *progress, const char *exercise_name)
{
    if (is_exercise_completed(progress, exercise_name))
        return;
    
    if (progress->completed_count < MAX_EXERCISES)
    {
        strncpy(progress->completed_exercises[progress->completed_count], 
                exercise_name, MAX_NAME_LEN - 1);
        progress->completed_exercises[progress->completed_count][MAX_NAME_LEN - 1] = '\0';
        progress->completed_count++;
    }
}

/*
** load_progress: Carga progreso desde archivo
*/
int load_progress(t_progress_data *progress, int rank)
{
    FILE *file = fopen(PROGRESS_FILE, "r");
    char line[256];
    
    if (!file)
        return (0);
    
    init_progress_data(progress, rank);
    
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "RANK=", 5) == 0)
            progress->rank = atoi(line + 5);
        else if (strncmp(line, "LEVEL1_COMPLETED=", 17) == 0)
            progress->level1_completed = atoi(line + 17);
        else if (strncmp(line, "LEVEL2_COMPLETED=", 17) == 0)
            progress->level2_completed = atoi(line + 17);
        else if (strncmp(line, "CURRENT_LEVEL=", 14) == 0)
            progress->current_level = atoi(line + 14);
        else if (strncmp(line, "TOTAL_TIME=", 11) == 0)
            progress->total_time = atoi(line + 11);
        else if (strncmp(line, "COMPLETED_COUNT=", 16) == 0)
            progress->completed_count = atoi(line + 16);
        else if (strncmp(line, "EXERCISE=", 9) == 0)
        {
            char *exercise_name = line + 9;
            char *newline = strchr(exercise_name, '\n');
            if (newline)
                *newline = '\0';
            
            if (progress->completed_count < MAX_EXERCISES)
            {
                strncpy(progress->completed_exercises[progress->completed_count],
                        exercise_name, MAX_NAME_LEN - 1);
                progress->completed_exercises[progress->completed_count][MAX_NAME_LEN - 1] = '\0';
                progress->completed_count++;
            }
        }
    }
    
    fclose(file);
    return (progress->rank == rank);
}

/*
** save_progress: Guarda progreso actual en archivo
*/
int save_progress(t_exam_session *session)
{
    FILE *file = fopen(PROGRESS_FILE, "w");
    if (!file)
    {
        printf(RED "Error: Could not save progress\n" RESET);
        return (0);
    }
    
    time_t current_time = time(NULL);
    session->progress.total_time += (int)(current_time - session->session_start_time);
    
    fprintf(file, "RANK=%d\n", session->progress.rank);
    fprintf(file, "LEVEL1_COMPLETED=%d\n", session->progress.level1_completed);
    fprintf(file, "LEVEL2_COMPLETED=%d\n", session->progress.level2_completed);
    fprintf(file, "CURRENT_LEVEL=%d\n", session->progress.current_level);
    fprintf(file, "TOTAL_TIME=%d\n", session->progress.total_time);
    fprintf(file, "COMPLETED_COUNT=%d\n", session->progress.completed_count);
    
    for (int i = 0; i < session->progress.completed_count; i++)
        fprintf(file, "EXERCISE=%s\n", session->progress.completed_exercises[i]);
    
    fclose(file);
    return (1);
}

/*
** print_statistics: Muestra estadísticas de progreso
*/
void print_statistics(t_progress_data *progress)
{
    time_t current_time = time(NULL);
    int session_time = (int)(current_time - progress->session_start);
    int total_time = progress->total_time + session_time;
    
    printf(CYAN "\n=== EXAM STATISTICS ===\n" RESET);
    printf("Exam Rank: %02d\n", progress->rank);
    printf("Current Level: %d\n\n", progress->current_level);
    
    printf(YELLOW "Progress:\n" RESET);
    printf("  Level 1: %s\n", progress->level1_completed ? 
           GREEN "✅ COMPLETED" RESET : RED "❌ Not completed" RESET);
    printf("  Level 2: %s\n\n", progress->level2_completed ? 
           GREEN "✅ COMPLETED" RESET : RED "❌ Not completed" RESET);
    
    printf(YELLOW "Exercises completed: %d\n" RESET, progress->completed_count);
    if (progress->completed_count > 0)
    {
        printf("  ");
        for (int i = 0; i < progress->completed_count; i++)
        {
            printf("%s", progress->completed_exercises[i]);
            if (i < progress->completed_count - 1)
                printf(", ");
        }
        printf("\n");
    }
    printf("\n");
    
    printf(YELLOW "Time tracking:\n" RESET);
    printf("  Session time: %s\n", format_time(session_time));
    printf("  Total time: %s\n", format_time(total_time));
    
    int completion_percentage = 0;
    if (progress->level1_completed && progress->level2_completed)
        completion_percentage = 100;
    else if (progress->level1_completed)
        completion_percentage = 50;
    
    printf("  Overall progress: %d%%\n", completion_percentage);
    printf(CYAN "======================\n" RESET "\n");
}

/*
** ask_continue_or_new_session: Pregunta si continuar o empezar nueva sesión
** También maneja la limpieza del directorio rendu cuando se cambia de rank
*/
int ask_continue_or_new_session(int rank)
{
    char choice;
    t_progress_data progress;
    
    if (!load_progress(&progress, rank))
    {
        // No hay sesión previa, crear directorio rendu
        create_rendu_directory();
        return (1);
    }
    
    // Verificar si cambiamos de rank
    if (progress.rank != rank)
    {
        printf(YELLOW "📁 Detected rank change from %02d to %02d\n" RESET, progress.rank, rank);
        printf(YELLOW "This will clean the rendu/ directory and start fresh.\n" RESET);
        printf("Continue? [y/N]: ");
        scanf(" %c", &choice);
        
        if (choice == 'y' || choice == 'Y')
        {
            cleanup_rendu_directory();  // Limpia rendu/ del rank anterior
            remove(PROGRESS_FILE);       // Elimina progreso anterior
            create_rendu_directory();    // Crea rendu/ limpio
            printf(GREEN "✅ Started new session for rank %02d\n" RESET, rank);
            return (1);
        }
        else
        {
            printf(YELLOW "Cancelled. Goodbye!\n" RESET);
            return (0);
        }
    }
    
    printf(GREEN "📊 Previous session found for Exam Rank %02d!\n" RESET, rank);
    print_statistics(&progress);
    
    printf("Do you want to:\n");
    printf("  [c] Continue previous session\n");
    printf("  [n] Start new session (will clean rendu/ directory)\n");
    printf("  [q] Quit\n\n");
    printf("Enter your choice: ");
    
    scanf(" %c", &choice);
    
    switch (choice)
    {
        case 'c': case 'C':
            return (1);
        case 'n': case 'N':
            printf(YELLOW "\n⚠️  Are you sure? This will delete all progress and clean rendu/. [y/N]: " RESET);
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y')
            {
                cleanup_rendu_directory();  // Limpia rendu/
                remove(PROGRESS_FILE);       // Elimina progreso
                create_rendu_directory();    // Crea rendu/ limpio
                printf(GREEN "✅ Started new session\n" RESET);
                return (1);
            }
            printf("Continuing previous session...\n");
            return (1);
        case 'q': case 'Q':
            printf(YELLOW "Goodbye!\n" RESET);
            return (0);
        default:
            printf(RED "Invalid option. Continuing previous session...\n" RESET);
            return (1);
    }
}

/*
** clear_progress_file: Elimina el archivo de progreso
*/
void clear_progress_file(int rank)
{
    (void)rank; // Para evitar warning de parámetro no usado
    remove(PROGRESS_FILE);
}
