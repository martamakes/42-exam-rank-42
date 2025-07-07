/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exam_session.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42madrid.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by marta             #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** EXAM_SESSION.C - Lógica principal del examen
*/

#include "examshell.h"

static int handle_completed_exam(t_exam_session *session)
{
    char choice;
    printf(GREEN "🎊 EXAM ALREADY COMPLETED!\n" RESET);
    print_statistics(&session->progress);
    printf("\nDo you want to start a new exam session? [y/N]: ");
    scanf(" %c", &choice);
    
    if (choice == 'y' || choice == 'Y') {
        clear_progress_file(session->rank);
        init_progress_data(&session->progress, session->rank);
        session->level1_completed = 0;
        session->level2_completed = 0;
        printf(GREEN "Started new exam session\n" RESET);
        return (1);
    }
    printf("Goodbye!\n");
    return (0);
}

static t_exercise *select_random_exercise(t_exercise *exercises, int count, 
                                         t_progress_data *progress)
{
    int attempts = 0, random_index;
    do {
        random_index = rand() % count;
        attempts++;
    } while (is_exercise_completed(progress, exercises[random_index].name) && 
             attempts < count * 2);
    return (&exercises[random_index]);
}

static int handle_grademe(t_exam_session *session, int current_level)
{
    mark_exercise_completed(&session->progress, session->current_exercise->name);
    
    if (current_level == 1) {
        session->level1_completed = 1;
        session->progress.level1_completed = 1;
        session->progress.current_level = 2;
    } else {
        session->level2_completed = 1;
        session->progress.level2_completed = 1;
    }
    
    session->current_exercise = NULL;
    save_progress(session);
    
    printf(GREEN "\n✅ Level %d completed! Well done!\n" RESET, current_level);
    
    if (current_level == 1) {
        printf("Moving to level 2...\n");
        return (2);
    } else {
        printf(GREEN "🎊 EXAM COMPLETED! You passed both levels!\n" RESET);
        print_statistics(&session->progress);
        return (3);
    }
}

static void handle_next_exercise(t_exam_session *session, t_exercise *exercises, 
                                int exercise_count, int current_level)
{
    if (exercise_count > 1) {
        int new_index, attempts = 0;
        do {
            new_index = rand() % exercise_count;
            attempts++;
        } while ((&exercises[new_index] == session->current_exercise || 
                 is_exercise_completed(&session->progress, exercises[new_index].name)) && 
                 attempts < exercise_count * 2);
        
        session->current_exercise = &exercises[new_index];
        
        // Crear directorio subject para el nuevo ejercicio
        create_subject_directory(session->current_exercise->name, session->current_exercise->path);
        
        printf(YELLOW "\n⏭️  Changed to different exercise in level %d\n" RESET, current_level);
        printf("Press Enter to see the new exercise...");
        getchar(); getchar();
        
        print_header();
        printf(BLUE "EXAM RANK %02d - LEVEL %d\n" RESET, session->rank, current_level);
        printf("Status: Working on level %d exercise\n\n", current_level);
        print_exercise_info(session->current_exercise, current_level);
        printf("Press Enter to continue to menu...");
        getchar();
    } else {
        printf(YELLOW "\n⚠️  Only one exercise available in this level\n" RESET);
        printf("Press Enter to continue...");
        getchar(); getchar();
    }
}

static int exercise_menu_loop(t_exam_session *session, t_exercise *exercises, 
                             int exercise_count, int current_level)
{
    char choice;
    
    while (1) {
        print_header();
        printf(BLUE "EXAM RANK %02d - LEVEL %d\n" RESET, session->rank, current_level);
        printf("Status: Working on level %d exercise\n\n", current_level);
        print_exercise_info(session->current_exercise, current_level);
        print_menu();
        
        scanf(" %c", &choice);
        
        switch (choice) {
            case 't': case 'T':
                printf(YELLOW "\n🔧 Tester functionality coming soon!\n" RESET);
                printf("Press Enter to continue...");
                getchar(); getchar();
                break;
                
            case 'g': case 'G':
                return (handle_grademe(session, current_level));
                
            case 'n': case 'N':
                handle_next_exercise(session, exercises, exercise_count, current_level);
                break;
                
            case 'r': case 'R':
                printf("\n");
                list_rendu_contents(session->current_exercise->name);
                printf(CYAN "\nReminder: You must manually create rendu/%s/ and put your files there\n" RESET, session->current_exercise->name);
                printf("Press Enter to continue...");
                getchar(); getchar();
                break;
                
            case 's': case 'S':
                print_statistics(&session->progress);
                printf("Press Enter to continue...");
                getchar(); getchar();
                break;
                
            case 'q': case 'Q':
                save_progress(session);
                cleanup_subject_directories();
                printf(YELLOW "\n👋 Progress saved. Exiting...\n" RESET);
                printf(GREEN "Run the same command to continue.\n" RESET);
                return (-1);
                
            default:
                printf(RED "\n❌ Invalid option.\n" RESET);
                printf("Press Enter to continue...");
                getchar(); getchar();
                break;
        }
    }
}

void run_exam_session(int rank)
{
    t_exam_session session;
    char level_path[512];
    int current_level = 1;
    
    memset(&session, 0, sizeof(session));
    session.rank = rank;
    session.session_start_time = time(NULL);

    if (load_progress(&session.progress, rank)) {
        printf(GREEN "Continuing previous session...\n" RESET);
        session.level1_completed = session.progress.level1_completed;
        session.level2_completed = session.progress.level2_completed;
        current_level = session.progress.current_level;
        
        if (session.level1_completed && session.level2_completed) {
            if (!handle_completed_exam(&session))
                return;
            current_level = 1;
        }
    } else {
        printf(GREEN "Starting new session...\n" RESET);
        init_progress_data(&session.progress, rank);
    }

    printf(YELLOW "EXAM MODE: 1 exercise per level (like real exam)\n" RESET);

    snprintf(level_path, sizeof(level_path), "%s/exam-rank-%02d/level-1", BASE_PATH, rank);
    session.level1_count = load_exercises_from_level(level_path, &session.level1_exercises);
    
    if (session.level1_count == 0) {
        printf(RED "Error: No exercises found in level 1 for rank %02d\n" RESET, rank);
        return;
    }

    snprintf(level_path, sizeof(level_path), "%s/exam-rank-%02d/level-2", BASE_PATH, rank);
    session.level2_count = load_exercises_from_level(level_path, &session.level2_exercises);

    printf("Available: %d exercises in level 1, %d exercises in level 2\n\n", 
           session.level1_count, session.level2_count);

    srand(time(NULL));

    while (current_level <= 2) {
        t_exercise *exercises;
        int exercise_count, level_completed;
        
        if (current_level == 1) {
            exercises = session.level1_exercises;
            exercise_count = session.level1_count;
            level_completed = session.level1_completed;
        } else {
            exercises = session.level2_exercises;
            exercise_count = session.level2_count;
            level_completed = session.level2_completed;
        }

        if (exercise_count == 0) {
            current_level++;
            continue;
        }

        if (level_completed) {
            if (current_level == 1) {
                printf("Moving to level 2...\n\n");
                current_level = 2;
                continue;
            } else {
                printf(GREEN "🎊 EXAM COMPLETED!\n" RESET);
                break;
            }
        }

        if (!session.current_exercise) {
            session.current_exercise = select_random_exercise(exercises, exercise_count, &session.progress);
            
            // Crear directorio subject como en examshell real
            create_subject_directory(session.current_exercise->name, session.current_exercise->path);
            
            print_header();
            printf(BLUE "EXAM RANK %02d - LEVEL %d\n" RESET, rank, current_level);
            printf("Status: Working on level %d exercise\n\n", current_level);
            print_exercise_info(session.current_exercise, current_level);
            printf("Press Enter to continue to menu...");
            getchar();
        }

        int result = exercise_menu_loop(&session, exercises, exercise_count, current_level);
        
        if (result == -1)
            break;
        else if (result > current_level) {
            current_level = result;
            if (current_level > 2)
                break;
        }
    }

    free_exercises(session.level1_exercises, session.level1_count);
    free_exercises(session.level2_exercises, session.level2_count);
    
    // Limpiar directorios subject al finalizar
    cleanup_subject_directories();
}
