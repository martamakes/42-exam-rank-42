/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   examshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42madrid.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by marta             #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXAMSHELL_H
# define EXAMSHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <dirent.h>
# include <sys/stat.h>
# include <time.h>
# include <errno.h>

// Constantes
# define BASE_PATH "."
# define PROGRESS_FILE ".examshell_progress"
# define MAX_SESSION_TIME 7200
# define MAX_EXERCISES 100
# define MAX_NAME_LEN 64

// Códigos de color
# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define WHITE   "\033[37m"

// Estructuras
typedef struct s_exercise
{
    char    *name;
    char    *path;
    int     completed;
    int     attempts;
    int     time_spent;
}   t_exercise;

typedef struct s_progress_data
{
    int     rank;
    int     level1_completed;
    int     level2_completed;
    int     current_level;
    int     total_time;
    time_t  session_start;
    char    completed_exercises[MAX_EXERCISES][MAX_NAME_LEN];
    int     completed_count;
}   t_progress_data;

typedef struct s_exam_session
{
    int             rank;
    t_exercise      *current_exercise;
    t_exercise      *level1_exercises;
    t_exercise      *level2_exercises;
    int             level1_count;
    int             level2_count;
    int             level1_completed;
    int             level2_completed;
    t_progress_data progress;
    time_t          session_start_time;
}   t_exam_session;

// Prototipos por archivo

// ui.c - Interfaz de usuario
void    print_header(void);
void    print_exercise_info(t_exercise *exercise, int level);
void    print_menu(void);
void    read_subject_file(const char *exercise_path);
void    create_subject_directory(const char *exercise_name, const char *exercise_path);
void    cleanup_subject_directories(void);
void    create_rendu_directory(void);
void    cleanup_rendu_directory(void);
void    list_rendu_contents(const char *exercise_name);

// persistence.c - Funciones de persistencia
char    *format_time(int seconds);
void    init_progress_data(t_progress_data *progress, int rank);
int     is_exercise_completed(t_progress_data *progress, const char *exercise_name);
void    mark_exercise_completed(t_progress_data *progress, const char *exercise_name);
int     load_progress(t_progress_data *progress, int rank);
int     save_progress(t_exam_session *session);
void    print_statistics(t_progress_data *progress);
int     ask_continue_or_new_session(int rank);
void    clear_progress_file(int rank);

// exercises.c - Gestión de ejercicios
int     detect_available_ranks(int **ranks);
int     load_exercises_from_level(const char *level_path, t_exercise **exercises);
int     get_rank_from_user(int *available_ranks, int rank_count);
void    free_exercises(t_exercise *exercises, int count);

// exam_session.c - Lógica principal del examen
void    run_exam_session(int rank);

#endif
