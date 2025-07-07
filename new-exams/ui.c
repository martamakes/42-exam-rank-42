/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42madrid.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by marta             #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** UI.C - Funciones de interfaz de usuario
** 
** Este archivo contiene todas las funciones relacionadas con la
** presentación visual del examshell: headers, menús, mostrar ejercicios, etc.
*/

#include "examshell.h"

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
** print_menu: Muestra el menú de opciones disponibles
** Interfaz interactiva para que el usuario elija su acción
*/
void print_menu(void)
{
    printf("Available options:\n");
    printf("  [t] Run tester (coming soon)\n");
    printf("  [g] Grademe - Mark as passed\n");
    printf("  [n] Next exercise (same level)\n");
    printf("  [r] Show rendu files\n");
    printf("  [s] Show statistics\n");
    printf("  [q] Quit (saves progress)\n\n");
    printf("Enter your choice: ");
}

/*
** print_exercise_info: Muestra información básica del ejercicio
** Como en examshell real, NO muestra el subject automáticamente
*/
void print_exercise_info(t_exercise *exercise, int level)
{
    printf("📚 Current Exercise:\n");
    printf("   Name: %s\n", exercise->name);
    printf("   Level: %d\n", level);
    printf("\n");
    
    printf(CYAN "💻 EXAMSHELL WORKFLOW (like real exam):\n" RESET);
    printf("  Terminal 1: This examshell (control)\n");
    printf("  Terminal 2: " YELLOW "cat subject/%s/subject.txt" RESET " (read instructions)\n", exercise->name);
    printf("  Terminal 3: Code your solution\n");
    printf("\n");
    
    printf(GREEN "✅ Subject: " RESET "subject/%s/subject.txt\n", exercise->name);
    printf(GREEN "✅ Submit to: " RESET "rendu/%s/\n", exercise->name);
    printf(CYAN "    (Create this directory and put your .c and .h files here)\n" RESET);
    printf("\n");
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

    snprintf(subject_path, sizeof(subject_path), "%s/subject.txt", exercise_path);
    file = fopen(subject_path, "r");
    if (!file)
    {
        printf("Error: Could not open subject file at %s\n", subject_path);
        return;
    }

    while (fgets(line, sizeof(line), file))
        printf("%s", line);
    
    fclose(file);
}

/*
** create_subject_directory: Crea directorio /subject/[exercise-name]/ y copia el subject
** Simula exactamente lo que hace el examshell real
*/
void create_subject_directory(const char *exercise_name, const char *exercise_path)
{
    char subject_dir[512];
    char subject_file_path[512];
    char original_subject[512];
    FILE *src_file, *dst_file;
    char line[1024];
    
    // Crear directorio /subject si no existe
    if (mkdir("subject", 0755) != 0 && errno != EEXIST)
    {
        printf(RED "Error: Could not create subject directory\n" RESET);
        return;
    }
    
    // Crear directorio /subject/[exercise-name]
    snprintf(subject_dir, sizeof(subject_dir), "subject/%s", exercise_name);
    if (mkdir(subject_dir, 0755) != 0 && errno != EEXIST)
    {
        printf(RED "Error: Could not create exercise subject directory\n" RESET);
        return;
    }
    
    // Rutas de archivos
    snprintf(original_subject, sizeof(original_subject), "%s/subject.txt", exercise_path);
    snprintf(subject_file_path, sizeof(subject_file_path), "%s/subject.txt", subject_dir);
    
    // Copiar subject.txt original al nuevo directorio
    src_file = fopen(original_subject, "r");
    if (!src_file)
    {
        printf(RED "Error: Could not open original subject file\n" RESET);
        return;
    }
    
    dst_file = fopen(subject_file_path, "w");
    if (!dst_file)
    {
        printf(RED "Error: Could not create subject file copy\n" RESET);
        fclose(src_file);
        return;
    }
    
    // Copiar contenido
    while (fgets(line, sizeof(line), src_file))
        fputs(line, dst_file);
    
    fclose(src_file);
    fclose(dst_file);
    
    // printf(GREEN "📁 Subject directory created: %s\n" RESET, subject_dir);
    // Mensaje silenciado - como en examshell real
}

/*
** cleanup_subject_directories: Limpia todos los directorios /subject creados
** Se ejecuta al salir del programa para limpiar como el examshell real
*/
void cleanup_subject_directories(void)
{
    char command[256];
    
    // Verificar si existe el directorio subject
    DIR *dir = opendir("subject");
    if (!dir)
        return; // No hay nada que limpiar
    
    closedir(dir);
    
    // Eliminar recursivamente el directorio subject y todo su contenido
    snprintf(command, sizeof(command), "rm -rf subject");
    
    if (system(command) == 0)
        printf(GREEN "🧹 Cleaned up subject directories\n" RESET);
    else
        printf(YELLOW "⚠️  Could not clean up subject directories\n" RESET);
}

/*
** create_rendu_directory: Crea el directorio rendu/ principal
** Este directorio persiste entre sesiones y solo se borra al cambiar de rank
*/
void create_rendu_directory(void)
{
    if (mkdir("rendu", 0755) != 0 && errno != EEXIST)
    {
        printf(RED "Error: Could not create rendu directory\n" RESET);
        return;
    }
    // Mensaje silenciado - como en examshell real
}

/*
** cleanup_rendu_directory: Limpia el directorio rendu
** Solo se ejecuta cuando se cambia de rank o se resetea el examen
*/
void cleanup_rendu_directory(void)
{
    char command[256];
    
    // Verificar si existe el directorio rendu
    DIR *dir = opendir("rendu");
    if (!dir)
        return; // No hay nada que limpiar
    
    closedir(dir);
    
    // Eliminar recursivamente el directorio rendu y todo su contenido
    snprintf(command, sizeof(command), "rm -rf rendu");
    
    if (system(command) == 0)
        printf(GREEN "🧹 Cleaned up rendu directory (rank reset)\n" RESET);
    else
        printf(YELLOW "⚠️  Could not clean up rendu directory\n" RESET);
}

/*
** list_rendu_contents: Muestra el contenido del directorio rendu/[exercise]
** Útil para que el usuario vea qué archivos tiene en su solución
*/
void list_rendu_contents(const char *exercise_name)
{
    char rendu_path[512];
    DIR *dir;
    struct dirent *entry;
    int file_count = 0;
    
    snprintf(rendu_path, sizeof(rendu_path), "rendu/%s", exercise_name);
    
    dir = opendir(rendu_path);
    if (!dir)
    {
        printf(YELLOW "📁 Directory rendu/%s/ doesn't exist yet\n" RESET, exercise_name);
        printf(CYAN "   Create it manually: " YELLOW "mkdir rendu/%s\n" RESET, exercise_name);
        printf(CYAN "   Then put your .c and .h files there\n" RESET);
        return;
    }
    
    printf(CYAN "📁 Files in rendu/%s/:\n" RESET, exercise_name);
    
    while ((entry = readdir(dir)) != NULL)
    {
        // Filtrar . y ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
            
        printf("  - %s", entry->d_name);
        
        // Destacar archivos .c y .h
        if (strstr(entry->d_name, ".c") || strstr(entry->d_name, ".h"))
            printf(GREEN " ✓" RESET);
            
        printf("\n");
        file_count++;
    }
    
    if (file_count == 0)
    {
        printf(YELLOW "  (directory exists but is empty)\n" RESET);
        printf(CYAN "  Put your .c and .h files here for the tester\n" RESET);
    }
    else
        printf(CYAN "  Total: %d files\n" RESET, file_count);
        
    closedir(dir);
}
