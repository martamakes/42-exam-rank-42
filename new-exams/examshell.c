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

    // Procesar argumentos
    if (argc == 2)
    {
        selected_rank = ft_atoi(argv[1]);
        
        // Verificar que el rank solicitado está disponible
        int rank_found = 0;
        for (int i = 0; i < rank_count; i++)
        {
            if (available_ranks[i] == selected_rank)
            {
                rank_found = 1;
                break;
            }
        }
        
        if (!rank_found)
        {
            printf(RED "Error: Exam rank %02d not found.\n" RESET, selected_rank);
            printf("Available ranks: ");
            for (int i = 0; i < rank_count; i++)
            {
                printf("%02d ", available_ranks[i]);
            }
            printf("\n");
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
        printf("Usage: %s [rank_number]\n", argv[0]);
        printf("Example: %s 03\n", argv[0]);
        printf("If no rank is specified, you will be prompted to choose.\n");
        free(available_ranks);
        return (1);
    }

    // Verificar si continuar sesión anterior o empezar nueva
    if (!ask_continue_or_new_session(selected_rank))
    {
        free(available_ranks);
        return (0); // Usuario quitó
    }

    // Iniciar sesión de examen
    run_exam_session(selected_rank);
    
    free(available_ranks);
    return (0);
}/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   examshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42madrid.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by marta             #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** EXAMSHELL - 42 School Exam Practice System
** 
** Este programa simula el entorno de examshell de 42 School.
** Permite practicar con ejercicios reales de los exámenes de ranking.
**
** CARACTERÍSTICAS:
** - Detección automática de exam-rank disponibles
** - Selección aleatoria de ejercicios sin repetición
** - Sistema de progreso por niveles
** - Interfaz similar al examshell real
** - Soporte para múltiples rankings (03, 04, 05)
**
** CONCEPTOS CLAVE APLICADOS:
** - Gestión de archivos y directorios (opendir, readdir, closedir)
** - Strings y manipulación de texto
** - Números aleatorios y semillas (srand, rand)
** - Estructuras de datos para organizar información
** - Gestión de memoria dinámica
** - Control de flujo y menús interactivos
*/

#include "examshell.h"

/* 
** ft_strlen: Calcula la longitud de una cadena
** Es una función básica que necesitamos implementar desde cero
*/
int ft_strlen(const char *str)
{
    int len = 0;
    if (!str)
        return (0);
    while (str[len])
        len++;
    return (len);
}

/*
** ft_strcpy: Copia una cadena en otra
** Implementación básica necesaria para el manejo de strings
*/
char *ft_strcpy(char *dest, const char *src)
{
    int i = 0;
    if (!dest || !src)
        return (NULL);
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}

/*
** ft_strdup: Duplica una cadena asignando memoria
** Función esencial para crear copias independientes de strings
*/
char *ft_strdup(const char *src)
{
    char *dup;
    int len;
    int i = 0;

    if (!src)
        return (NULL);
    len = ft_strlen(src);
    dup = malloc(sizeof(char) * (len + 1));
    if (!dup)
        return (NULL);
    while (src[i])
    {
        dup[i] = src[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}

/*
** ft_strncmp: Compara dos cadenas hasta n caracteres
** Necesaria para comparar nombres de directorios y archivos
*/
int ft_strncmp(const char *s1, const char *s2, int n)
{
    int i = 0;
    
    if (!s1 || !s2)
        return (0);
    while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    if (i == n)
        return (0);
    return (s1[i] - s2[i]);
}

/*
** ft_atoi: Convierte string a entero
** Implementación básica para procesar argumentos numéricos
*/
int ft_atoi(const char *str)
{
    int result = 0;
    int sign = 1;
    int i = 0;

    if (!str)
        return (0);
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
        i++;
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
        i++;
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result * sign);
}

/*
** print_header: Muestra el header del examshell
** Simula la interfaz del examshell real de 42
*/
void print_header(void)
{
    printf("\033[2J\033[H"); // Limpia pantalla y mueve cursor al inicio
    printf("=====================================\n");
    printf("        42 SCHOOL - EXAMSHELL        \n");
    printf("         Practice Environment        \n");
    printf("=====================================\n\n");
}

/*
** print_exercise_info: Muestra información del ejercicio actual
** Presenta los detalles del ejercicio de forma clara
*/
void print_exercise_info(t_exercise *exercise, int level)
{
    printf("📚 Current Exercise:\n");
    printf("   Name: %s\n", exercise->name);
    printf("   Level: %d\n", level);
    printf("   Path: %s\n\n", exercise->path);
}

/*
** show_exercise_with_subject: Muestra el ejercicio y su subject automáticamente
** Como en el examshell real, siempre muestra las instrucciones
*/
void show_exercise_with_subject(t_exercise *exercise, int level)
{
    print_exercise_info(exercise, level);
    
    // Mostrar subject automáticamente (como en examshell real)
    printf(YELLOW "=== SUBJECT ===" RESET "\n\n");
    read_subject_file(exercise->path);
    printf("\n" YELLOW "=== END SUBJECT ===" RESET "\n\n");
}

/*
** print_menu: Muestra el menú de opciones disponibles
** Interfaz interactiva para que el usuario elija su acción
*/
void print_menu(void)
{
    printf("Available options:\n");
    printf("  [t] Run tester (coming soon)\n");
    printf("  [g] Grademe - Mark as passed\n");
    printf("  [n] Next exercise (same level)\n");
    printf("  [s] Show statistics\n");
    printf("  [q] Quit (saves progress)\n\n");
    printf("Enter your choice: ");
}

/*
** read_subject_file: Lee y muestra el contenido del subject.txt
** Función esencial para mostrar las instrucciones del ejercicio
*/
void read_subject_file(const char *exercise_path)
{
    FILE *file;
    char subject_path[512];
    char line[1024];

    sprintf(subject_path, "%s/subject.txt", exercise_path);
    file = fopen(subject_path, "r");
    if (!file)
    {
        printf("Error: Could not open subject file at %s\n", subject_path);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    fclose(file);
}
}

/*
** ft_atoi: Convierte string a entero
** Implementación básica para procesar argumentos numéricos
*/
int ft_atoi(const char *str)
{
    int result = 0;
    int sign = 1;
    int i = 0;

    if (!str)
        return (0);
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
        i++;
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
        i++;
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result * sign);
}

/*
** print_header: Muestra el header del examshell
** Simula la interfaz del examshell real de 42
*/
void print_header(void)
{
    printf("\033[2J\033[H"); // Limpia pantalla y mueve cursor al inicio
    printf("=====================================\n");
    printf("        42 SCHOOL - EXAMSHELL        \n");
    printf("         Practice Environment        \n");
    printf("=====================================\n\n");
}

/*
** print_exercise_info: Muestra información del ejercicio actual
** Presenta los detalles del ejercicio de forma clara
*/
void print_exercise_info(t_exercise *exercise, int level)
{
    printf("📚 Current Exercise:\n");
    printf("   Name: %s\n", exercise->name);
    printf("   Level: %d\n", level);
    printf("   Path: %s\n\n", exercise->path);
}

/*
** show_exercise_with_subject: Muestra el ejercicio y su subject automáticamente
** Como en el examshell real, siempre muestra las instrucciones
*/
void show_exercise_with_subject(t_exercise *exercise, int level)
{
    print_exercise_info(exercise, level);
    
    // Mostrar subject automáticamente (como en examshell real)
    printf(YELLOW "=== SUBJECT ===" RESET "\n\n");
    read_subject_file(exercise->path);
    printf("\n" YELLOW "=== END SUBJECT ===" RESET "\n\n");
}

/*
** print_menu: Muestra el menú de opciones disponibles
** Interfaz interactiva para que el usuario elija su acción
*/
void print_menu(void)
{
    printf("Available options:\n");
    printf("  [t] Run tester (coming soon)\n");
    printf("  [g] Grademe - Mark as passed\n");
    printf("  [n] Next exercise (same level)\n");
    printf("  [s] Show statistics\n");
    printf("  [q] Quit (saves progress)\n\n");
    printf("Enter your choice: ");
}

/*
** read_subject_file: Lee y muestra el contenido del subject.txt
** Función esencial para mostrar las instrucciones del ejercicio
*/
void read_subject_file(const char *exercise_path)
{
    FILE *file;
    char subject_path[512];
    char line[1024];

    sprintf(subject_path, "%s/subject.txt", exercise_path);
    file = fopen(subject_path, "r");
    if (!file)
    {
        printf("Error: Could not open subject file at %s\n", subject_path);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    fclose(file);
}

/*
** detect_available_ranks: Detecta todos los exam-rank disponibles
** Explora el directorio y encuentra automáticamente los exámenes
*/
int detect_available_ranks(int **ranks)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    int temp_ranks[10]; // Máximo 10 ranks por simplicidad
    
    dir = opendir(BASE_PATH);
    if (!dir)
    {
        printf("Error: Could not open directory %s\n", BASE_PATH);
        return (0);
    }

    // Primera pasada: contar ranks disponibles
    while ((entry = readdir(dir)) != NULL)
    {
        if (ft_strncmp(entry->d_name, "exam-rank-", 10) == 0)
        {
            int rank_num = ft_atoi(entry->d_name + 10);
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

    // Asignar memoria para los ranks encontrados
    *ranks = malloc(sizeof(int) * count);
    if (!*ranks)
        return (0);
    
    for (int i = 0; i < count; i++)
        (*ranks)[i] = temp_ranks[i];

    return (count);
}

/*
** load_exercises_from_level: Carga ejercicios de un nivel específico
** Explora un directorio de nivel y carga todos los ejercicios disponibles
*/
int load_exercises_from_level(const char *level_path, t_exercise **exercises)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    t_exercise temp_exercises[20]; // Máximo 20 ejercicios por nivel
    
    dir = opendir(level_path);
    if (!dir)
        return (0);

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.') // Ignora archivos ocultos
        {
            char exercise_path[512];
            char subject_path[600];
            FILE *subject_file;
            
            sprintf(exercise_path, "%s/%s", level_path, entry->d_name);
            sprintf(subject_path, "%s/subject.txt", exercise_path);
            
            // Verifica que existe subject.txt
            subject_file = fopen(subject_path, "r");
            if (subject_file)
            {
                fclose(subject_file);
                temp_exercises[count].name = ft_strdup(entry->d_name);
                temp_exercises[count].path = ft_strdup(exercise_path);
                temp_exercises[count].completed = 0;
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
** get_random_exercise: Selecciona un ejercicio aleatorio no completado
** Implementa la lógica para evitar repetir ejercicios ya hechos
*/
t_exercise *get_random_exercise(t_exercise *exercises, int count, int *available_count)
{
    int available[20];
    int avail_count = 0;
    
    // Encuentra ejercicios no completados
    for (int i = 0; i < count; i++)
    {
        if (!exercises[i].completed)
        {
            available[avail_count] = i;
            avail_count++;
        }
    }
    
    *available_count = avail_count;
    if (avail_count == 0)
        return (NULL);
    
    // Selecciona uno aleatoriamente
    int random_index = rand() % avail_count;
    return (&exercises[available[random_index]]);
}

/*
** run_exam_session: Función principal que ejecuta una sesión de examen
** Implementa la lógica real del examen: 1 ejercicio aleatorio por nivel
*/
void run_exam_session(int rank)
{
    t_exam_session session;
    char level_path[512];
    char choice;
    int current_level = 1;
    
    // Inicializar sesión
    session.rank = rank;
    session.current_exercise = NULL;
    session.level1_exercises = NULL;
    session.level2_exercises = NULL;
    session.level1_count = 0;
    session.level2_count = 0;
    session.level1_completed = 0;
    session.level2_completed = 0;

    printf(GREEN "Starting Exam Rank %02d session...\n" RESET, rank);
    printf(YELLOW "EXAM MODE: 1 exercise per level (like real exam)\n" RESET);

    // Cargar ejercicios de nivel 1
    sprintf(level_path, "%s/exam-rank-%02d/level-1", BASE_PATH, rank);
    session.level1_count = load_exercises_from_level(level_path, &session.level1_exercises);
    
    if (session.level1_count == 0)
    {
        printf(RED "Error: No exercises found in level 1 for rank %02d\n" RESET, rank);
        return;
    }

    // Cargar ejercicios de nivel 2
    sprintf(level_path, "%s/exam-rank-%02d/level-2", BASE_PATH, rank);
    session.level2_count = load_exercises_from_level(level_path, &session.level2_exercises);

    printf("Available: %d exercises in level 1, %d exercises in level 2\n\n", 
           session.level1_count, session.level2_count);

    // Inicializar generador de números aleatorios
    srand(time(NULL));

    // Comenzar con nivel 1
    while (current_level <= 2)
    {
        t_exercise *exercises;
        int exercise_count;
        int level_completed;
        
        if (current_level == 1)
        {
            exercises = session.level1_exercises;
            exercise_count = session.level1_count;
            level_completed = session.level1_completed;
        }
        else
        {
            exercises = session.level2_exercises;
            exercise_count = session.level2_count;
            level_completed = session.level2_completed;
        }

        if (level_completed)
        {
            printf(GREEN "✅ Level %d already completed!\n" RESET, current_level);
            if (current_level == 1)
            {
                printf("Moving to level 2...\n\n");
                current_level = 2;
                continue;
            }
            else
            {
                printf(GREEN "🎊 EXAM COMPLETED! All levels finished!\n" RESET);
                break;
            }
        }

        // Seleccionar ejercicio aleatorio si no hay uno asignado
        // Evitar ejercicios ya completados en sesiones anteriores
        if (!session.current_exercise)
        {
            int attempts = 0;
            int random_index;
            
            do {
                random_index = rand() % exercise_count;
                attempts++;
            } while (is_exercise_completed(&session.progress, exercises[random_index].name) && 
                     attempts < exercise_count * 2);
            
            session.current_exercise = &exercises[random_index];
            
            // Mostrar ejercicio con subject automáticamente
            print_header();
            printf(BLUE "EXAM RANK %02d - LEVEL %d\n" RESET, rank, current_level);
            if (current_level == 1 && !session.level1_completed)
                printf("Status: Working on level 1 exercise\n\n");
            else if (current_level == 2 && !session.level2_completed)
                printf("Status: Working on level 2 exercise\n\n");
            
            show_exercise_with_subject(session.current_exercise, current_level);
            printf("Press Enter to continue to menu...");
            getchar();
        }

        // Bucle del ejercicio actual
        while (1)
        {
            print_header();
            printf(BLUE "EXAM RANK %02d - LEVEL %d\n" RESET, rank, current_level);
            if (current_level == 1 && !session.level1_completed)
                printf("Status: Working on level 1 exercise\n\n");
            else if (current_level == 2 && !session.level2_completed)
                printf("Status: Working on level 2 exercise\n\n");
            
            print_exercise_info(session.current_exercise, current_level);
            print_menu();
            
            scanf(" %c", &choice);
            
            switch (choice)
            {
                case 't':
                case 'T':
                    printf(YELLOW "\n🔧 Tester functionality coming soon!\n" RESET);
                    printf("Press Enter to continue...");
                    getchar();
                    getchar();
                    break;
                    
                case 'g':
                case 'G':
                    // Marcar ejercicio como completado
                    mark_exercise_completed(&session.progress, session.current_exercise->name);
                    
                    // Marcar nivel como completado
                    if (current_level == 1)
                    {
                        session.level1_completed = 1;
                        session.progress.level1_completed = 1;
                        session.progress.current_level = 2;
                    }
                    else
                    {
                        session.level2_completed = 1;
                        session.progress.level2_completed = 1;
                    }
                    
                    session.current_exercise = NULL; // Reset para próximo nivel
                    
                    // Guardar progreso
                    save_progress(&session);
                    
                    printf(GREEN "\n✅ Level %d completed! Well done!\n" RESET, current_level);
                    printf("Exercise '%s' marked as completed.\n", session.current_exercise ? session.current_exercise->name : "unknown");
                    
                    if (current_level == 1)
                    {
                        printf("Moving to level 2...\n");
                        current_level = 2;
                    }
                    else
                    {
                        printf(GREEN "🎊 EXAM COMPLETED! You passed both levels!\n" RESET);
                        print_statistics(&session.progress);
                        current_level = 3; // Para salir del bucle
                    }
                    
                    printf("Press Enter to continue...");
                    getchar();
                    getchar();
                    goto next_level;
                    
                case 'n':
                case 'N':
                    // Cambiar a otro ejercicio del mismo nivel
                    if (exercise_count > 1)
                    {
                        int new_index;
                        int attempts = 0;
                        
                        do {
                            new_index = rand() % exercise_count;
                            attempts++;
                        } while ((&exercises[new_index] == session.current_exercise || 
                                 is_exercise_completed(&session.progress, exercises[new_index].name)) && 
                                 attempts < exercise_count * 2);
                        
                        session.current_exercise = &exercises[new_index];
                        printf(YELLOW "\n⏭️  Changed to different exercise in level %d\n" RESET, current_level);
                        printf("Press Enter to see the new exercise...");
                        getchar();
                        getchar();
                        
                        // Mostrar el nuevo ejercicio con su subject
                        print_header();
                        printf(BLUE "EXAM RANK %02d - LEVEL %d\n" RESET, rank, current_level);
                        if (current_level == 1 && !session.level1_completed)
                            printf("Status: Working on level 1 exercise\n\n");
                        else if (current_level == 2 && !session.level2_completed)
                            printf("Status: Working on level 2 exercise\n\n");
                        
                        show_exercise_with_subject(session.current_exercise, current_level);
                        printf("Press Enter to continue to menu...");
                        getchar();
                    }
                    else
                    {
                        printf(YELLOW "\n⚠️  Only one exercise available in this level\n" RESET);
                        printf("Press Enter to continue...");
                        getchar();
                        getchar();
                    }
                    break;
                    
                case 's':
                case 'S':
                    // Mostrar estadísticas
                    print_statistics(&session.progress);
                    printf("Press Enter to continue...");
                    getchar();
                    getchar();
                    break;
                    
                case 'q':
                case 'Q':
                    // Guardar progreso antes de salir
                    save_progress(&session);
                    printf(YELLOW "\n👋 Progress saved. Exiting exam session...\n" RESET);
                    printf(GREEN "Your progress has been saved. Run the same command to continue.\n" RESET);
                    goto cleanup;
                    
                default:
                    printf(RED "\n❌ Invalid option. Please try again.\n" RESET);
                    printf("Press Enter to continue...");
                    getchar();
                    getchar();
                    break;
            }
        }
        
        next_level:
        continue;
    }

    cleanup:
    // Liberar memoria
    if (session.level1_exercises)
    {
        for (int i = 0; i < session.level1_count; i++)
        {
            free(session.level1_exercises[i].name);
            free(session.level1_exercises[i].path);
        }
        free(session.level1_exercises);
    }
    
    if (session.level2_exercises)
    {
        for (int i = 0; i < session.level2_count; i++)
        {
            free(session.level2_exercises[i].name);
            free(session.level2_exercises[i].path);
        }
        free(session.level2_exercises);
    }
}

/*
** get_rank_from_user: Solicita al usuario que seleccione un rank
** Interfaz para elegir el exam-rank cuando no se proporciona como argumento
*/
int get_rank_from_user(int *available_ranks, int rank_count)
{
    int choice;
    
    printf("Available exam ranks:\n");
    for (int i = 0; i < rank_count; i++)
    {
        printf("  %d - Exam Rank %02d\n", i + 1, available_ranks[i]);
    }
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
** format_time: Convierte segundos a formato legible (hh:mm:ss)
** Función auxiliar para mostrar tiempos de forma clara
*/
char *format_time(int seconds)
{
    static char buffer[32];
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, secs);
    return (buffer);
}

/*
** init_progress_data: Inicializa estructura de progreso
** Configura los valores por defecto para una nueva sesión
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
    
    // Limpiar array de ejercicios completados
    for (int i = 0; i < 100; i++)
    {
        progress->completed_exercises[i][0] = '\0';
    }
}

/*
** is_exercise_completed: Verifica si un ejercicio ya fue completado
** Busca el nombre del ejercicio en la lista de completados
*/
int is_exercise_completed(t_progress_data *progress, const char *exercise_name)
{
    for (int i = 0; i < progress->completed_count; i++)
    {
        if (ft_strncmp(progress->completed_exercises[i], exercise_name, 64) == 0)
            return (1);
    }
    return (0);
}

/*
** mark_exercise_completed: Marca un ejercicio como completado
** Añade el ejercicio a la lista de completados si no está ya
*/
void mark_exercise_completed(t_progress_data *progress, const char *exercise_name)
{
    if (is_exercise_completed(progress, exercise_name))
        return;
    
    if (progress->completed_count < 100)
    {
        ft_strcpy(progress->completed_exercises[progress->completed_count], exercise_name);
        progress->completed_count++;
    }
}

/*
** load_progress: Carga progreso desde archivo
** Lee el archivo .examshell_progress y carga los datos de sesión anterior
*/
int load_progress(t_progress_data *progress, int rank)
{
    FILE *file;
    char line[256];
    
    file = fopen(PROGRESS_FILE, "r");
    if (!file)
        return (0); // No hay archivo previo
    
    // Leer datos línea por línea
    while (fgets(line, sizeof(line), file))
    {
        if (ft_strncmp(line, "RANK=", 5) == 0)
            progress->rank = ft_atoi(line + 5);
        else if (ft_strncmp(line, "LEVEL1_COMPLETED=", 17) == 0)
            progress->level1_completed = ft_atoi(line + 17);
        else if (ft_strncmp(line, "LEVEL2_COMPLETED=", 17) == 0)
            progress->level2_completed = ft_atoi(line + 17);
        else if (ft_strncmp(line, "CURRENT_LEVEL=", 14) == 0)
            progress->current_level = ft_atoi(line + 14);
        else if (ft_strncmp(line, "TOTAL_TIME=", 11) == 0)
            progress->total_time = ft_atoi(line + 11);
        else if (ft_strncmp(line, "SESSION_START=", 14) == 0)
            progress->session_start = (time_t)ft_atoi(line + 14);
        else if (ft_strncmp(line, "COMPLETED_COUNT=", 16) == 0)
            progress->completed_count = ft_atoi(line + 16);
        else if (ft_strncmp(line, "EXERCISE=", 9) == 0)
        {
            int len = ft_strlen(line + 9);
            if (len > 0 && line[9 + len - 1] == '\n')
                line[9 + len - 1] = '\0'; // Eliminar newline
            
            if (progress->completed_count < 100)
            {
                ft_strcpy(progress->completed_exercises[progress->completed_count], line + 9);
                progress->completed_count++;
            }
        }
    }
    
    fclose(file);
    
    // Verificar que es el mismo rank
    if (progress->rank != rank)
        return (0);
    
    return (1);
}

/*
** save_progress: Guarda progreso actual en archivo
** Escribe todos los datos de la sesión en .examshell_progress
*/
int save_progress(t_exam_session *session)
{
    FILE *file;
    time_t current_time = time(NULL);
    
    // Actualizar tiempo total
    session->progress.total_time += (int)(current_time - session->session_start_time);
    
    file = fopen(PROGRESS_FILE, "w");
    if (!file)
    {
        printf(RED "Error: Could not save progress to %s\n" RESET, PROGRESS_FILE);
        return (0);
    }
    
    // Escribir datos principales
    fprintf(file, "RANK=%d\n", session->progress.rank);
    fprintf(file, "LEVEL1_COMPLETED=%d\n", session->progress.level1_completed);
    fprintf(file, "LEVEL2_COMPLETED=%d\n", session->progress.level2_completed);
    fprintf(file, "CURRENT_LEVEL=%d\n", session->progress.current_level);
    fprintf(file, "TOTAL_TIME=%d\n", session->progress.total_time);
    fprintf(file, "SESSION_START=%ld\n", (long)session->progress.session_start);
    fprintf(file, "COMPLETED_COUNT=%d\n", session->progress.completed_count);
    
    // Escribir ejercicios completados
    for (int i = 0; i < session->progress.completed_count; i++)
    {
        fprintf(file, "EXERCISE=%s\n", session->progress.completed_exercises[i]);
    }
    
    fclose(file);
    return (1);
}

/*
** print_statistics: Muestra estadísticas de progreso
** Presenta información detallada sobre el progreso del usuario
*/
void print_statistics(t_progress_data *progress)
{
    time_t current_time = time(NULL);
    int session_time = (int)(current_time - progress->session_start);
    int total_time = progress->total_time + session_time;
    
    printf(CYAN "\n=== EXAM STATISTICS ===\n" RESET);
    printf("Exam Rank: %02d\n", progress->rank);
    printf("Current Level: %d\n", progress->current_level);
    printf("\n");
    
    printf(YELLOW "Progress:\n" RESET);
    printf("  Level 1: %s\n", progress->level1_completed ? GREEN "✅ COMPLETED" RESET : RED "❌ Not completed" RESET);
    printf("  Level 2: %s\n", progress->level2_completed ? GREEN "✅ COMPLETED" RESET : RED "❌ Not completed" RESET);
    printf("\n");
    
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
    
    // Mostrar progreso general
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
** Interfaz para que el usuario elija entre continuar o resetear
*/
int ask_continue_or_new_session(int rank)
{
    char choice;
    t_progress_data progress;
    
    if (!load_progress(&progress, rank))
        return (1); // No hay progreso previo, nueva sesión
    
    printf(GREEN "📊 Previous session found for Exam Rank %02d!\n" RESET, rank);
    print_statistics(&progress);
    
    printf("Do you want to:\n");
    printf("  [c] Continue previous session\n");
    printf("  [n] Start new session (will lose current progress)\n");
    printf("  [q] Quit\n\n");
    printf("Enter your choice: ");
    
    scanf(" %c", &choice);
    
    switch (choice)
    {
        case 'c':
        case 'C':
            return (1); // Continuar
            
        case 'n':
        case 'N':
            printf(YELLOW "\n⚠️  Are you sure you want to start a new session?\n" RESET);
            printf("This will delete all current progress. [y/N]: ");
            scanf(" %c", &choice);
            
            if (choice == 'y' || choice == 'Y')
            {
                clear_progress_file(rank);
                printf(GREEN "✅ Started new session\n" RESET);
                return (1);
            }
            else
            {
                printf("Continuing previous session...\n");
                return (1);
            }
            
        case 'q':
        case 'Q':
            printf(YELLOW "Goodbye!\n" RESET);
            return (0); // Salir del programa
            
        default:
            printf(RED "Invalid option. Continuing previous session...\n" RESET);
            return (1);
    }
}

/*
** clear_progress_file: Elimina el archivo de progreso
** Resetea completamente el progreso de un rank específico
*/
void clear_progress_file(int rank)
{
    // Simplemente eliminamos el archivo
    // En un sistema más complejo podríamos solo limpiar el rank específico
    remove(PROGRESS_FILE);
}

/*
** main: Función principal del programa
** Punto de entrada que maneja argumentos y inicia el sistema
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

    // Procesar argumentos
    if (argc == 2)
    {
        selected_rank = ft_atoi(argv[1]);
        
        // Verificar que el rank solicitado está disponible
        int rank_found = 0;
        for (int i = 0; i < rank_count; i++)
        {
            if (available_ranks[i] == selected_rank)
            {
                rank_found = 1;
                break;
            }
        }
        
        if (!rank_found)
        {
            printf(RED "Error: Exam rank %02d not found.\n" RESET, selected_rank);
            printf("Available ranks: ");
            for (int i = 0; i < rank_count; i++)
            {
                printf("%02d ", available_ranks[i]);
            }
            printf("\n");
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
        printf("Usage: %s [rank_number]\n", argv[0]);
        printf("Example: %s 03\n", argv[0]);
        printf("If no rank is specified, you will be prompted to choose.\n");
        free(available_ranks);
        return (1);
    }

    // Verificar si continuar sesión anterior o empezar nueva
    if (!ask_continue_or_new_session(selected_rank))
    {
        free(available_ranks);
        return (0); // Usuario quitó
    }

    // Iniciar sesión de examen
    run_exam_session(selected_rank);
    
    free(available_ranks);
    return (0);
}
