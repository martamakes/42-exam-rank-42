/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guide_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUIDE_UTILS_H
# define GUIDE_UTILS_H

#include <stdio.h>      // Para printf
#include <unistd.h>     // Para write, chdir
#include <stdlib.h>     // Para exit
#include <string.h>     // Para strcmp
#include <sys/wait.h>   // Para waitpid

// Colores para la salida
# define COLOR_RED     "\033[0;31m"
# define COLOR_GREEN   "\033[0;32m"
# define COLOR_YELLOW  "\033[0;33m"
# define COLOR_BLUE    "\033[0;34m"
# define COLOR_MAGENTA "\033[0;35m"
# define COLOR_CYAN    "\033[0;36m"
# define COLOR_RESET   "\033[0m"

// Funciones de utilidad para la guía
void	print_message(char *color, char *message);
void	print_section_title(char *title);
void	print_step(int step_num, char *step_description);
int	count_args_until_separator(char **args);
void	print_args(char **args, int count);
void	print_command(char **args, int count);
void	simulate_command_execution(char **args, int count);
void	simulate_pipe_execution(char **args_left, int count_left, char **args_right, int count_right);

#endif /* GUIDE_UTILS_H */
