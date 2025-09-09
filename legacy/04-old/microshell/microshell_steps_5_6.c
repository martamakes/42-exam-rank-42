/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell_steps_5_6.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** PASOS 5 Y 6: MANEJO MÚLTIPLES PIPES Y SEPARADORES
** En esta parte implementaremos el manejo de múltiples pipes y separadores de comandos.
*/

#include <unistd.h>     // Para write, chdir, dup2, pipe, close, execve
#include <stdlib.h>     // Para malloc, free, exit
#include <string.h>     // Para strcmp
#include <sys/wait.h>   // Para waitpid

// Función para imprimir mensajes de error
void	ft_putstr_error(char *str)
{
    while (*str)
        write(2, str++, 1);
}

// Implementación del comando cd
int	execute_cd(char **args, int arg_count)
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

// Función para contar argumentos hasta un separador (| o ;)
int	count_args(char **args)
{
    int i = 0;
    
    while (args[i] && strcmp(args[i], "|") != 0 && strcmp(args[i], ";") != 0)
        i++;
    
    return i;
}

// Función para ejecutar un comando simple
int	execute_simple_command(char **args, int arg_count, char **env)
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
        // Marcar el final de los argumentos
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

// Función para ejecutar comandos conectados por un pipe
int	execute_pipe_commands(char **args, int arg_count, char **env)
{
    int pipefd[2];
    pid_t pid;
    int status;
    int next_arg_count;
    int is_pipe;
    
    // Verificar si el siguiente separador es un pipe
    is_pipe = (args[arg_count] && strcmp(args[arg_count], "|") == 0);
    
    // Si no es un pipe, ejecutar como comando simple
    if (!is_pipe)
        return execute_simple_command(args, arg_count, env);
    
    // Crear el pipe
    if (pipe(pipefd) < 0)
    {
        ft_putstr_error("error: fatal\n");
        exit(1);
    }
    
    // Crear proceso hijo para el primer comando
    pid = fork();
    if (pid < 0)
    {
        ft_putstr_error("error: fatal\n");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(1);
    }
    else if (pid == 0)
    {
        // Redirigir stdout al pipe
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) < 0)
        {
            ft_putstr_error("error: fatal\n");
            exit(1);
        }
        close(pipefd[1]);
        
        // Marcar fin de argumentos
        args[arg_count] = NULL;
        
        // Ejecutar el primer comando
        if (strcmp(args[0], "cd") == 0)
        {
            int ret = execute_cd(args, arg_count);
            exit(ret);
        }
        execve(args[0], args, env);
        ft_putstr_error("error: cannot execute ");
        ft_putstr_error(args[0]);
        ft_putstr_error("\n");
        exit(1);
    }
    
    // Proceso padre
    // Cerrar extremo de escritura del pipe
    close(pipefd[1]);
    
    // Contar argumentos del siguiente comando
    next_arg_count = count_args(&args[arg_count + 1]);
    
    // Crear proceso hijo para el segundo comando (o pipeline)
    pid = fork();
    if (pid < 0)
    {
        ft_putstr_error("error: fatal\n");
        close(pipefd[0]);
        exit(1);
    }
    else if (pid == 0)
    {
        // Redirigir stdin desde el pipe
        if (dup2(pipefd[0], STDIN_FILENO) < 0)
        {
            ft_putstr_error("error: fatal\n");
            exit(1);
        }
        close(pipefd[0]);
        
        // Ejecutar el segundo comando (o pipeline)
        int ret = execute_pipe_commands(&args[arg_count + 1], next_arg_count, env);
        exit(ret);
    }
    
    // Cerrar extremo de lectura del pipe
    close(pipefd[0]);
    
    // Esperar a que terminen los procesos hijos
    waitpid(pid, &status, 0);
    
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 1;
}

// Main completo que maneja pipes y separadores
int	main(int argc, char **argv, char **env)
{
    int i = 1;
    int ret = 0;
    int arg_count;
    
    // Si no hay argumentos, simplemente retornar
    if (argc == 1)
        return 0;
    
    // Procesar cada comando o pipeline
    while (i < argc)
    {
        // Contar argumentos hasta el próximo separador
        arg_count = count_args(&argv[i]);
        
        // Si no hay argumentos, avanzar al siguiente
        if (arg_count == 0)
        {
            // Si es un separador de comandos (;), saltar
            if (i < argc && strcmp(argv[i], ";") == 0)
                i++;
            else
                i++;
            continue;
        }
        
        // Ejecutar comando o pipeline
        ret = execute_pipe_commands(&argv[i], arg_count, env);
        
        // Avanzar al siguiente comando
        i += arg_count;
        
        // Si es un separador de comandos (;), saltar
        if (i < argc && strcmp(argv[i], ";") == 0)
            i++;
        // Si es un pipe (|), ya se manejó en execute_pipe_commands
        else if (i < argc && strcmp(argv[i], "|") == 0)
            i++;
    }
    
    return ret;
}

/*
** NOTAS:
** 
** Esta implementación ahora maneja:
** 1. Ejecución de comandos simples
** 2. El comando builtin cd con manejo de errores
** 3. Manejo de múltiples pipes
** 4. Manejo del separador de comandos (;)
** 5. Parsing completo de la línea de comandos
**
** Puntos importantes de esta solución:
** - Usamos recursividad para manejar múltiples pipes
** - Cerramos todos los descriptores de archivo para evitar leaks
** - Manejamos correctamente los errores y códigos de retorno
** - Respetamos las especificaciones del subject
*/
