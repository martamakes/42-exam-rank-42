/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell_steps_1_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** PASOS 1 Y 2: FUNCIONES AUXILIARES Y COMANDO CD
** En esta parte vamos a implementar las funciones básicas y el comando cd.
*/

#include <unistd.h>     // Para write, chdir
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
    // Verificar número de argumentos (cd requiere exactamente 1 argumento)
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

// Main simplificado que solo maneja comandos simples (sin pipes ni punto y coma)
int	main(int argc, char **argv, char **env)
{
    // Si no hay argumentos, simplemente retornar
    if (argc == 1)
        return 0;
    
    // Ejecutar el comando (pasando todos los argumentos)
    return execute_command(&argv[1], env);
}

/*
** NOTAS:
** 
** 1. Esta implementación básica solo maneja:
**    - Ejecución de comandos simples
**    - El comando builtin cd con validación de errores
**
** 2. Aún falta implementar:
**    - Manejo de pipes (|)
**    - Manejo de separador de comandos (;)
**    - Parsing adecuado de los argumentos
**
** 3. En los siguientes pasos añadiremos estas funcionalidades.
*/
