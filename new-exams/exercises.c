/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exercises.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42madrid.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by marta             #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** EXERCISES.C - Gestión de ejercicios y ranks
** 
** Este archivo contiene funciones para detectar exam-ranks disponibles,
** cargar ejercicios de niveles y gestionar la selección de ejercicios.
*/

#include "examshell.h"

/*
** detect_available_ranks: Detecta todos los exam-rank disponibles
*/
int detect_available_ranks(int **ranks)
{
    DIR *dir = opendir(BASE_PATH);
    struct dirent *entry;
    int count = 0;
    int temp_ranks[10]; // Máximo 10 ranks
    
    if (!dir)
    {
        printf("Error: Could not open directory %s\n", BASE_PATH);
        return (0);
    }
    
    while ((entry = readdir(dir)) != NULL)
    {
        if (strncmp(entry->d_name, "exam-rank-", 10) == 0)
        {
            int rank_num = atoi(entry->d_name + 10);
            if (rank_num > 0)
            {
                temp_ranks[count] = rank_num;
                count++;
            }
        }
    }
    closedir(dir);
    
    if (count == 0)
        return (0);
    
    *ranks = malloc(sizeof(int) * count);
    if (!*ranks)
        return (0);
    
    for (int i = 0; i < count; i++)
        (*ranks)[i] = temp_ranks[i];
    
    return (count);
}

/*
** load_exercises_from_level: Carga ejercicios de un nivel específico
*/
int load_exercises_from_level(const char *level_path, t_exercise **exercises)
{
    DIR *dir = opendir(level_path);
    struct dirent *entry;
    int count = 0;
    t_exercise temp_exercises[20]; // Máximo 20 ejercicios por nivel
    
    if (!dir)
        return (0);
    
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.') // Ignora archivos ocultos
        {
            char exercise_path[512];
            char subject_path[600];
            FILE *subject_file;
            
            snprintf(exercise_path, sizeof(exercise_path), "%s/%s", level_path, entry->d_name);
            snprintf(subject_path, sizeof(subject_path), "%s/subject.txt", exercise_path);
            
            // Verifica que existe subject.txt
            subject_file = fopen(subject_path, "r");
            if (subject_file)
            {
                fclose(subject_file);
                temp_exercises[count].name = strdup(entry->d_name);
                temp_exercises[count].path = strdup(exercise_path);
                temp_exercises[count].completed = 0;
                temp_exercises[count].attempts = 0;
                temp_exercises[count].time_spent = 0;
                count++;
            }
        }
    }
    closedir(dir);
    
    if (count == 0)
        return (0);
    
    *exercises = malloc(sizeof(t_exercise) * count);
    if (!*exercises)
        return (0);
    
    for (int i = 0; i < count; i++)
        (*exercises)[i] = temp_exercises[i];
    
    return (count);
}

/*
** get_rank_from_user: Solicita al usuario que seleccione un rank
*/
int get_rank_from_user(int *available_ranks, int rank_count)
{
    int choice;
    
    printf("Available exam ranks:\n");
    for (int i = 0; i < rank_count; i++)
        printf("  %d - Exam Rank %02d\n", i + 1, available_ranks[i]);
    
    printf("\nEnter your choice (1-%d): ", rank_count);
    scanf("%d", &choice);
    
    if (choice < 1 || choice > rank_count)
    {
        printf(RED "Invalid choice. Please select a number between 1 and %d.\n" RESET, rank_count);
        return (get_rank_from_user(available_ranks, rank_count));
    }
    
    return (available_ranks[choice - 1]);
}

/*
** free_exercises: Libera memoria de ejercicios
*/
void free_exercises(t_exercise *exercises, int count)
{
    if (!exercises)
        return;
    
    for (int i = 0; i < count; i++)
    {
        free(exercises[i].name);
        free(exercises[i].path);
    }
    free(exercises);
}
