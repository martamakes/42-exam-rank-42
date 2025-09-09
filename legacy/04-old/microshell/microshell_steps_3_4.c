/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell_steps_3_4.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** PASOS 3 Y 4: EJECUCIÓN DE COMANDOS Y MANEJO DE PIPES
** En esta parte implementaremos la ejecución de comandos y el manejo de pipes.
*/

#include <unistd.h>     // Para write, chdir, dup2, pipe, close, execve
#include <stdlib.h>     // Para exit
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

// Función para ejecutar un comando simple (sin pipes)
int	execute_command(char **args, char **env)
{
    pid_t pid;
    int status;
    
    // Si es el comando cd, manejarlo directamente
    if (strcmp(args[0], "cd") == 0)
        return execute_cd(args, 2);  // Asumimos que hay 2 argumentos para cd
    
    // Crear un proceso hijo
    pid = fork();
    if (pid < 0)
    {
        ft_putstr_error("error: fatal\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // Proceso hijo ejecuta el comando
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

// Función para manejar comandos con un pipe
int	execute_pipe(char **args_left, char **args_right, char **env)
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
    
    // Crear el primer proceso hijo (lado izquierdo del pipe)
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
        
        // Ejecutar el primer comando
        if (strcmp(args_left[0], "cd") == 0)
        {
            int ret = execute_cd(args_left, 2);
            exit(ret);
        }
        execve(args_left[0], args_left, env);
        ft_putstr_error("error: cannot execute ");
        ft_putstr_error(args_left[0]);
        ft_putstr_error("\n");
        exit(1);
    }
    
    // Crear el segundo proceso hijo (lado derecho del pipe)
    pid2 = fork();
    if (pid2 < 0)
    {
        ft_putstr_error("error: fatal\n");
        close(pipefd[0]);
        close(pipefd[1]);
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
        
        // Ejecutar el segundo comando
        if (strcmp(args_right[0], "cd") == 0)
        {
            int ret = execute_cd(args_right, 2);
            exit(ret);
        }
        execve(args_right[0], args_right, env);
        ft_putstr_error("error: cannot execute ");
        ft_putstr_error(args_right[0]);
        ft_putstr_error("\n");
        exit(1);
    }
    
    // Cerrar los descriptores de archivo del pipe en el proceso padre
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Esperar a que terminen los procesos hijos
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 1;
}

// Main simplificado que maneja un comando o dos comandos conectados por un pipe
int	main(int argc, char **argv, char **env)
{
    int i;
    
    // Si no hay argumentos, simplemente retornar
    if (argc == 1)
        return 0;
    
    // Buscar si hay un pipe en los argumentos
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            // Marcar fin del primer comando
            argv[i] = NULL;
            
            // Ejecutar comandos conectados por pipe
            return execute_pipe(&argv[1], &argv[i + 1], env);
        }
    }
    
    // Si no hay pipe, ejecutar un solo comando
    return execute_command(&argv[1], env);
}

/*
** NOTAS:
** 
** 1. Esta implementación añade:
**    - Ejecución de comandos simples
**    - El comando builtin cd
**    - Manejo de un pipe simple (un solo |)
**
** 2. Aún falta implementar:
**    - Manejo de múltiples pipes
**    - Manejo del separador de comandos (;)
**    - Parsing más completo de los argumentos
**
** 3. En el siguiente paso añadiremos estas funcionalidades.
*/
