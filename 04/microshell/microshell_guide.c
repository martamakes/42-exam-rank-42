/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell_guide.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>     // Para write, chdir, dup, dup2, pipe, close, execve
#include <stdlib.h>     // Para malloc, free, exit
#include <string.h>     // Para strcmp, strncmp
#include <sys/wait.h>   // Para waitpid
#include "guide_utils.h"

/*
** MICROSHELL - GUÍA COMPLETA
** 
** Este archivo es una guía completa para la implementación del ejercicio microshell.
** El código está completamente comentado para ayudarte a entender cada paso.
**
** Autor: mvigara- <mvigara-@student.42madrid.com>
*/

int main(int argc, char **argv, char **env)
{
    // Si se ejecuta este archivo directamente, mostrar instrucciones
    print_section_title("GUÍA MICROSHELL");
    print_message(COLOR_GREEN, "Este archivo es una guía para implementar el ejercicio microshell.");
    print_message(COLOR_YELLOW, "Para ver la guía interactiva, ejecuta el script 'microshell_guide'.");
    print_message(COLOR_CYAN, "Para ver cada paso de la implementación, revisa los archivos:");
    printf("  - microshell_steps_1_2.c\n");
    printf("  - microshell_steps_3_4.c\n");
    printf("  - microshell_steps_5_6.c\n\n");
    
    return 0;
}

/*
** ESTRUCTURA DE LA SOLUCIÓN
**
** Para implementar microshell de forma efectiva, seguimos estos pasos:
**
** 1. FUNCIONES AUXILIARES
**    - ft_putstr_error: Para imprimir mensajes de error en stderr
**    - count_args: Contar argumentos hasta un separador
**
** 2. COMANDO CD
**    - execute_cd: Implementa el comando builtin cd
**    - Maneja errores específicos de cd según el subject
**
** 3. EJECUCIÓN DE COMANDOS SIMPLES
**    - execute_command: Ejecuta un comando externo con sus argumentos
**    - Maneja la creación de procesos y errores de execve
**
** 4. MANEJO DE PIPES SIMPLES
**    - execute_pipe: Conecta la salida de un comando con la entrada de otro
**    - Maneja la creación de pipes y redirección de stdin/stdout
**
** 5. MÚLTIPLES PIPES Y SEPARADORES
**    - Implementa el manejo de múltiples pipes en cascada
**    - Maneja el separador de comandos (;)
**
** 6. FUNCIÓN PRINCIPAL
**    - Procesa los argumentos de línea de comandos
**    - Identifica comandos, pipes y separadores
**    - Maneja la ejecución del flujo completo
*/

/*
** IMPLEMENTACIÓN DETALLADA
**
** A continuación se describe la implementación completa de cada parte:
*/

/*
** 1. FUNCIONES AUXILIARES
*/

// Imprime un mensaje de error en stderr
void ft_putstr_error(char *str)
{
    while (*str)
        write(2, str++, 1);
}

// Cuenta el número de argumentos hasta un separador (| o ;)
int count_args(char **args)
{
    int i = 0;
    
    while (args[i] && strcmp(args[i], "|") != 0 && strcmp(args[i], ";") != 0)
        i++;
    
    return i;
}

/*
** 2. COMANDO CD
*/

// Implementa el comando builtin cd
int execute_cd(char **args, int arg_count)
{
    // Verificar número de argumentos
    if (arg_count != 2)
    {
        ft_putstr_error("error: cd: bad arguments\n");
        return 1;
    }
    
    // Intentar cambiar de directorio
    if (chdir(args[1]) != 0)
    {
        ft_putstr_error("error: cd: cannot change directory to ");
        ft_putstr_error(args[1]);
        ft_putstr_error("\n");
        return 1;
    }
    
    return 0;
}

/*
** 3. EJECUCIÓN DE COMANDOS SIMPLES
*/

// Ejecuta un comando simple
int execute_simple_command(char **args, int arg_count, char **env)
{
    pid_t pid;
    int status;
    
    // Si es el comando cd, manejarlo directamente
    if (strcmp(args[0], "cd") == 0)
        return execute_cd(args, arg_count);
    
    // Crear un proceso hijo
    pid = fork();
    if (pid < 0)
    {
        ft_putstr_error("error: fatal\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // Marcar final de argumentos
        args[arg_count] = NULL;
        
        // Ejecutar el comando
        execve(args[0], args, env);
        ft_putstr_error("error: cannot execute ");
        ft_putstr_error(args[0]);
        ft_putstr_error("\n");
        exit(1);
    }
    else
    {
        // Proceso padre espera a que el hijo termine
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        return 1;
    }
}

/*
** 4. MANEJO DE PIPES SIMPLES
*/

// Función para ejecutar comandos conectados por un pipe
int execute_pipe(char **args_left, int count_left, char **args_right, int count_right, char **env)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;
    
    // Crear el pipe
    if (pipe(pipefd) < 0)
    {
        ft_putstr_error("error: fatal\n");
        exit(1);
    }
    
    // Primer proceso hijo (lado izquierdo del pipe)
    pid1 = fork();
    if (pid1 < 0)
    {
        ft_putstr_error("error: fatal\n");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(1);
    }
    else if (pid1 == 0)
    {
        // Redirigir stdout al pipe
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) < 0)
        {
            ft_putstr_error("error: fatal\n");
            exit(1);
        }
        close(pipefd[1]);
        
        // Marcar final de argumentos
        args_left[count_left] = NULL;
        
        // Ejecutar el primer comando
        if (strcmp(args_left[0], "cd") == 0)
        {
            int ret = execute_cd(args_left, count_left);
            exit(ret);
        }
        execve(args_left[0], args_left, env);
        ft_putstr_error("error: cannot execute ");
        ft_putstr_error(args_left[0]);
        ft_putstr_error("\n");
        exit(1);
    }
    
    // Segundo proceso hijo (lado derecho del pipe)
    pid2 = fork();
    if (pid2 < 0)
    {
        ft_putstr_error("error: fatal\n");
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, NULL, 0);  // Esperar al primer hijo antes de salir
        exit(1);
    }
    else if (pid2 == 0)
    {
        // Redirigir stdin desde el pipe
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) < 0)
        {
            ft_putstr_error("error: fatal\n");
            exit(1);
        }
        close(pipefd[0]);
        
        // Marcar final de argumentos
        args_right[count_right] = NULL;
        
        // Ejecutar el segundo comando
        if (strcmp(args_right[0], "cd") == 0)
        {
            int ret = execute_cd(args_right, count_right);
            exit(ret);
        }
        execve(args_right[0], args_right, env);
        ft_putstr_error("error: cannot execute ");
        ft_putstr_error(args_right[0]);
        ft_putstr_error("\n");
        exit(1);
    }
    
    // Cerrar descriptores de archivo en el proceso padre
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Esperar a que terminen los procesos hijos
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 1;
}

/*
** 5. MÚLTIPLES PIPES Y SEPARADORES (implementación recursiva)
*/

// Función para ejecutar comandos conectados por múltiples pipes
int execute_command_line(char **args, char **env)
{
    int i = 0;
    int pipe_pos = -1;
    int semicolon_pos = -1;
    int ret = 0;
    
    // Buscar el primer pipe o punto y coma
    while (args[i])
    {
        if (strcmp(args[i], "|") == 0 && pipe_pos == -1)
            pipe_pos = i;
        if (strcmp(args[i], ";") == 0)
        {
            semicolon_pos = i;
            break;
        }
        i++;
    }
    
    // Si encontramos un punto y coma primero
    if (semicolon_pos != -1)
    {
        // Terminar la lista de argumentos en el punto y coma
        args[semicolon_pos] = NULL;
        
        // Ejecutar la primera parte de la línea
        ret = execute_command_line(args, env);
        
        // Ejecutar el resto de la línea después del punto y coma
        if (args[semicolon_pos + 1])
            ret = execute_command_line(&args[semicolon_pos + 1], env);
            
        return ret;
    }
    
    // Si encontramos un pipe
    if (pipe_pos != -1)
    {
        // Terminar la lista de argumentos en el pipe
        args[pipe_pos] = NULL;
        
        // Contar argumentos para el segundo comando
        int next_pipe_pos = -1;
        int next_semicolon_pos = -1;
        i = pipe_pos + 1;
        
        while (args[i])
        {
            if (strcmp(args[i], "|") == 0 && next_pipe_pos == -1)
                next_pipe_pos = i;
            if (strcmp(args[i], ";") == 0)
            {
                next_semicolon_pos = i;
                break;
            }
            i++;
        }
        
        // Si hay otro pipe después
        if (next_pipe_pos != -1)
        {
            // Ejecutar el primer comando y su pipe
            char *temp = args[next_pipe_pos];
            args[next_pipe_pos] = NULL;
            
            ret = execute_pipe(args, pipe_pos, &args[pipe_pos + 1], next_pipe_pos - pipe_pos - 1, env);
            
            // Restaurar el pipe y continuar con el resto
            args[next_pipe_pos] = temp;
            ret = execute_command_line(&args[pipe_pos + 1], env);
        }
        // Si hay un punto y coma después
        else if (next_semicolon_pos != -1)
        {
            // Ejecutar el comando con pipe
            ret = execute_pipe(args, pipe_pos, &args[pipe_pos + 1], next_semicolon_pos - pipe_pos - 1, env);
            
            // Continuar con lo que hay después del punto y coma
            if (args[next_semicolon_pos + 1])
                ret = execute_command_line(&args[next_semicolon_pos + 1], env);
        }
        // Si no hay más separadores
        else
        {
            // Calcular cuántos argumentos hay después del pipe
            int count_right = 0;
            while (args[pipe_pos + 1 + count_right])
                count_right++;
            
            // Ejecutar el comando con pipe
            ret = execute_pipe(args, pipe_pos, &args[pipe_pos + 1], count_right, env);
        }
        
        return ret;
    }
    
    // Si no hay separadores, ejecutar como comando simple
    int count = 0;
    while (args[count])
        count++;
    
    return execute_simple_command(args, count, env);
}

/*
** 6. FUNCIÓN PRINCIPAL
*/

// Función principal que procesa la línea de comandos
int main_implementation(int argc, char **argv, char **env)
{
    // Si no hay argumentos, simplemente retornar
    if (argc == 1)
        return 0;
    
    // Ejecutar la línea de comandos completa
    return execute_command_line(&argv[1], env);
}

/*
** NOTAS FINALES:
**
** 1. La implementación mostrada aquí está diseñada para ser clara y didáctica.
**    En un examen real, podrías simplificar algunas partes o usar una aproximación
**    diferente según tu estilo de programación.
**
** 2. Recuerda que la gestión de descriptores de archivo es crítica:
**    - Cierra siempre todos los descriptores que abras
**    - Ten cuidado con los leaks de file descriptors en pipes anidados
**
** 3. Para manejar muchos pipes con un límite de archivos abiertos bajo,
**    puedes optimizar cerrando descriptores lo antes posible.
**
** 4. Verifica siempre los valores de retorno de las funciones del sistema
**    como fork(), pipe(), dup2(), etc.
**
** 5. La solución recursiva para múltiples pipes es elegante, pero también
**    podrías implementar una solución iterativa con un bucle.
*/
