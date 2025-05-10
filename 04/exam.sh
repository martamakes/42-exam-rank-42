#!/bin/bash

clear

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
RESET='\033[0m'
BOLD='\033[1m'

# Directorios
EXAM_DIR="$(cd "$(dirname "$0")" && pwd)"
MICROSHELL_DIR="$EXAM_DIR/microshell"
RENDU_DIR="$EXAM_DIR/rendu/microshell"

# Verificar argumentos
if [ "$1" = "clean" ]; then
    echo -e "${RED}Limpiando...${RESET}"
    rm -f $MICROSHELL_DIR/microshell
    rm -f $RENDU_DIR/microshell
    make -f $EXAM_DIR/Makefile clean
    echo -e "${GREEN}Limpieza completada.${RESET}"
    exit 0
fi

# Título
echo -e "${BLUE}${BOLD}==========================${RESET}"
echo -e "${BLUE}${BOLD}   EXAM RANK 04 - TESTER${RESET}"
echo -e "${BLUE}${BOLD}==========================${RESET}"
echo ""

# Opciones principales
echo -e "${YELLOW}${BOLD}OPCIONES:${RESET}"
echo -e "1. Ver subject"
echo -e "2. Guía interactiva"
echo -e "3. Ejecutar tests"
echo -e "4. Compilar solución"
echo -e "0. Salir"
echo ""

read -p "Selecciona una opción: " option

case $option in
    1)
        clear
        echo -e "${BLUE}${BOLD}=== SUBJECT MICROSHELL ===${RESET}\n"
        cat "$MICROSHELL_DIR/README.md"
        echo -e "\n${YELLOW}Presiona Enter para volver...${RESET}"
        read
        $EXAM_DIR/exam.sh
        ;;
    2)
        if [ ! -x "$MICROSHELL_DIR/microshell_guide" ]; then
            echo -e "${YELLOW}Compilando la guía interactiva...${RESET}"
            make -f $EXAM_DIR/Makefile guide
        fi
        $MICROSHELL_DIR/microshell_guide
        $EXAM_DIR/exam.sh
        ;;
    3)
        # Verificar si existe una implementación
        if [ ! -f "$RENDU_DIR/microshell.c" ]; then
            echo -e "${RED}Error: No se encuentra el archivo microshell.c en $RENDU_DIR${RESET}"
            echo -e "${YELLOW}Por favor, crea tu implementación en: ${CYAN}$RENDU_DIR/microshell.c${RESET}"
            
            # Preguntar si quiere crear un template
            read -p "¿Quieres crear un template básico para comenzar? (s/n): " create_template
            if [[ "$create_template" =~ ^[Ss]$ ]]; then
                mkdir -p "$RENDU_DIR"
                cat > "$RENDU_DIR/microshell.c" << 'EOL'
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvigara- <mvigara-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:44:49 by mvigara-          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:59 by mvigara-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

// Funciones auxiliares
int ft_strlen(char *str) {
    int i = 0;
    while (str[i])
        i++;
    return i;
}

int ft_strcmp(char *s1, char *s2) {
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return s1[i] - s2[i];
}

void ft_putstr_fd(char *str, int fd) {
    write(fd, str, ft_strlen(str));
}

// Función para el comando cd
int exec_cd(char **argv) {
    if (!argv[1] || argv[2]) {
        ft_putstr_fd("error: cd: bad arguments\n", 2);
        return 1;
    }
    if (chdir(argv[1]) != 0) {
        ft_putstr_fd("error: cd: cannot change directory to ", 2);
        ft_putstr_fd(argv[1], 2);
        ft_putstr_fd("\n", 2);
        return 1;
    }
    return 0;
}

// Función para ejecutar comandos
int exec_cmd(char **argv, char **env) {
    pid_t pid;
    int status;

    if (!argv[0])
        return 0;
    
    if (ft_strcmp(argv[0], "cd") == 0)
        return exec_cd(argv);
    
    pid = fork();
    if (pid < 0) {
        ft_putstr_fd("error: fatal\n", 2);
        exit(1);
    }
    
    if (pid == 0) {
        if (execve(argv[0], argv, env) < 0) {
            ft_putstr_fd("error: cannot execute ", 2);
            ft_putstr_fd(argv[0], 2);
            ft_putstr_fd("\n", 2);
            exit(1);
        }
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        return 1;
    }
    return 0;
}

// TODO: Implementar manejo de pipes
// TODO: Implementar manejo de comandos separados por punto y coma

int main(int argc, char **argv, char **env) {
    int i = 1;
    int ret = 0;
    
    if (argc == 1)
        return 0;
    
    // Ejemplo básico solo para pruebas iniciales
    // Debes implementar el manejo completo de pipes (|) y punto y coma (;)
    ret = exec_cmd(&argv[1], env);
    
    return ret;
}
EOL
                echo -e "${GREEN}Template creado en $RENDU_DIR/microshell.c${RESET}"
            fi
            
            echo -e "\n${YELLOW}Presiona Enter para volver...${RESET}"
            read
            $EXAM_DIR/exam.sh
            exit 1
        fi
        
        # Compilar la solución
        echo -e "${YELLOW}Compilando tu solución...${RESET}"
        gcc -Wall -Wextra -Werror "$RENDU_DIR/microshell.c" -o "$RENDU_DIR/microshell"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Error de compilación.${RESET}"
            echo -e "\n${YELLOW}Presiona Enter para volver...${RESET}"
            read
            $EXAM_DIR/exam.sh
            exit 1
        fi
        echo -e "${GREEN}Compilación exitosa.${RESET}"
        
        # Ejecutar los tests
        echo -e "${YELLOW}Ejecutando tests...${RESET}\n"
        CURRENT_DIR="$(pwd)"
        cd "$MICROSHELL_DIR/grademe"
        cp "$RENDU_DIR/microshell" .
        chmod +x microshell
        ./test.sh
        rm -f microshell
        cd "$CURRENT_DIR"
        
        echo -e "\n${YELLOW}Presiona Enter para volver...${RESET}"
        read
        $EXAM_DIR/exam.sh
        ;;
    4)
        # Verificar si existe una implementación
        if [ ! -f "$RENDU_DIR/microshell.c" ]; then
            echo -e "${RED}Error: No se encuentra el archivo microshell.c en $RENDU_DIR${RESET}"
            echo -e "${YELLOW}Por favor, crea tu implementación en: ${CYAN}$RENDU_DIR/microshell.c${RESET}"
            echo -e "\n${YELLOW}Presiona Enter para volver...${RESET}"
            read
            $EXAM_DIR/exam.sh
            exit 1
        fi
        
        # Compilar la solución
        echo -e "${YELLOW}Compilando tu solución...${RESET}"
        gcc -Wall -Wextra -Werror "$RENDU_DIR/microshell.c" -o "$RENDU_DIR/microshell"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Error de compilación.${RESET}"
        else
            echo -e "${GREEN}Compilación exitosa.${RESET}"
            echo -e "${YELLOW}Puedes encontrar el ejecutable en: ${CYAN}$RENDU_DIR/microshell${RESET}"
        fi
        
        echo -e "\n${YELLOW}Presiona Enter para volver...${RESET}"
        read
        $EXAM_DIR/exam.sh
        ;;
    0)
        echo -e "${GREEN}¡Hasta pronto!${RESET}"
        exit 0
        ;;
    *)
        echo -e "${RED}Opción inválida. Intenta de nuevo.${RESET}"
        sleep 1
        $EXAM_DIR/exam.sh
        ;;
esac

exit 0
