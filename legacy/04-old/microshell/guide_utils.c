/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guide_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** UTILIDADES PARA LA GUÍA DE MICROSHELL
** Estas funciones pueden ayudarte a construir tu solución.
*/

#include "guide_utils.h"

// Imprime un mensaje con color para mejor visualización
void	print_message(char *color, char *message)
{
	printf("%s%s%s\n", color, message, COLOR_RESET);
}

// Imprime un título de sección
void	print_section_title(char *title)
{
	printf("\n%s=== %s ===%s\n", COLOR_BLUE, title, COLOR_RESET);
}

// Imprime un paso de la guía
void	print_step(int step_num, char *step_description)
{
	printf("%sPaso %d: %s%s\n", COLOR_YELLOW, step_num, step_description, COLOR_RESET);
}

// Función para contar argumentos hasta un separador (| o ;)
int	count_args_until_separator(char **args)
{
	int i = 0;
	
	while (args[i] && strcmp(args[i], "|") != 0 && strcmp(args[i], ";") != 0)
		i++;
	
	return i;
}

// Función para imprimir una lista de argumentos
void	print_args(char **args, int count)
{
	int i;
	
	printf("%sArgumentos:%s\n", COLOR_CYAN, COLOR_RESET);
	for (i = 0; i < count; i++)
	{
		printf("  args[%d] = '%s'\n", i, args[i]);
	}
}

// Función para imprimir un comando con sus argumentos
void	print_command(char **args, int count)
{
	int i;
	
	printf("%sComando:%s %s\n", COLOR_GREEN, COLOR_RESET, args[0]);
	printf("%sArgumentos:%s ", COLOR_CYAN, COLOR_RESET);
	for (i = 1; i < count; i++)
	{
		printf("%s ", args[i]);
	}
	printf("\n");
}

// Función para simular la ejecución de un comando
void	simulate_command_execution(char **args, int count)
{
	print_command(args, count);
	printf("%sSimulando ejecución...%s\n", COLOR_MAGENTA, COLOR_RESET);
	
	if (strcmp(args[0], "cd") == 0)
	{
		printf("  Ejecutando comando interno cd\n");
		if (count != 2)
			printf("  %sError: cd: bad arguments%s\n", COLOR_RED, COLOR_RESET);
		else
			printf("  Cambiando directorio a: %s\n", args[1]);
	}
	else
	{
		printf("  Ejecutando comando externo con execve\n");
		printf("  Comando: %s\n", args[0]);
	}
}

// Función para simular la ejecución de un pipe
void	simulate_pipe_execution(char **args_left, int count_left, char **args_right, int count_right)
{
	printf("%sSimulando ejecución de pipe:%s\n", COLOR_MAGENTA, COLOR_RESET);
	printf("%sComando izquierdo:%s\n", COLOR_CYAN, COLOR_RESET);
	print_command(args_left, count_left);
	printf("%sComando derecho:%s\n", COLOR_CYAN, COLOR_RESET);
	print_command(args_right, count_right);
	printf("%sProceso:%s\n", COLOR_MAGENTA, COLOR_RESET);
	printf("  1. Crear pipe con pipe()\n");
	printf("  2. Fork para el primer comando\n");
	printf("  3. Redirigir stdout al pipe con dup2()\n");
	printf("  4. Ejecutar primer comando\n");
	printf("  5. Fork para el segundo comando\n");
	printf("  6. Redirigir stdin desde el pipe con dup2()\n");
	printf("  7. Ejecutar segundo comando\n");
	printf("  8. Cerrar descriptores y esperar a hijos\n");
}
