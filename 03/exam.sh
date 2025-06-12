#!/bin/bash

# VERSION CORREGIDA - Diferencia entre ft_printf (sin .h) y get_next_line (con .h)

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'

# Directorios de trabajo
EXAM_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$EXAM_DIR")"
RENDU_DIR="$EXAM_DIR/rendu"

# Crear directorios si no existen
mkdir -p "$RENDU_DIR"
mkdir -p "$RENDU_DIR/ft_printf"
mkdir -p "$RENDU_DIR/get_next_line"

# Función para validar ejercicio - CORREGIDA
validate_exercise() {
    local exercise=$1
    local student_dir="$RENDU_DIR/$exercise"
    local source_dir="$EXAM_DIR/$exercise"
    
    # Verificar que el usuario ha creado su solución
    if [ ! -d "$student_dir" ]; then
        echo -e "${RED}Error: No se encuentra tu solución en $student_dir${NC}"
        mkdir -p "$student_dir"
        echo -e "${YELLOW}Se ha creado el directorio $student_dir${NC}"
        return 1
    fi
    
    # Verificar existencia de archivos necesarios
    if [ "$exercise" = "ft_printf" ]; then
        if [ ! -f "$student_dir/ft_printf.c" ]; then
            echo -e "${RED}Error: No se encuentra ft_printf.c en $student_dir${NC}"
            return 1
        fi
        echo -e "${BLUE}${BOLD}Ejecutando tests para ft_printf...${NC}"
        
        # COMPILACIÓN ESPECÍFICA PARA FT_PRINTF (SIN .h)
        if [ -f "$source_dir/test_main.c" ]; then
            cd "$student_dir"
            gcc -Wall -Wextra -Werror "$source_dir/test_main.c" "$student_dir/ft_printf.c" -o ft_printf_test
            COMPILE_RESULT=$?
            
            if [ $COMPILE_RESULT -ne 0 ]; then
                echo -e "${RED}Error de compilación. Por favor, corrige los errores.${NC}"
                return 1
            fi
            
            echo -e "${GREEN}✅ Compilación exitosa para ft_printf${NC}"
            ./ft_printf_test
            TEST_RESULT=$?
            rm -f ft_printf_test
        else
            echo -e "${YELLOW}No se encontró test_main.c, compilando solo ft_printf.c${NC}"
            cd "$student_dir"
            gcc -Wall -Wextra -Werror -c ft_printf.c
            TEST_RESULT=$?
            rm -f ft_printf.o
        fi
        
    elif [ "$exercise" = "get_next_line" ]; then
        if [ ! -f "$student_dir/get_next_line.c" ] || [ ! -f "$student_dir/get_next_line.h" ]; then
            echo -e "${RED}Error: Faltan archivos en $student_dir${NC}"
            echo -e "${YELLOW}Asegúrate de tener get_next_line.c y get_next_line.h${NC}"
            return 1
        fi
        echo -e "${BLUE}${BOLD}Ejecutando tests para get_next_line...${NC}"
        
        # COMPILACIÓN ESPECÍFICA PARA GET_NEXT_LINE (CON .h)
        if [ -f "$source_dir/test_main.c" ]; then
            cd "$student_dir"
            gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 "$source_dir/test_main.c" get_next_line.c -o gnl_test
            COMPILE_RESULT=$?
            
            if [ $COMPILE_RESULT -ne 0 ]; then
                echo -e "${RED}Error de compilación. Por favor, corrige los errores.${NC}"
                return 1
            fi
            
            echo -e "${GREEN}✅ Compilación exitosa para get_next_line${NC}"
            ./gnl_test
            TEST_RESULT=$?
            rm -f gnl_test
        else
            echo -e "${YELLOW}No se encontró test_main.c, compilando solo get_next_line.c${NC}"
            cd "$student_dir"
            gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -c get_next_line.c
            TEST_RESULT=$?
            rm -f get_next_line.o
        fi
    fi
    
    if [ $TEST_RESULT -eq 0 ]; then
        echo -e "${GREEN}¡Los tests para $exercise han pasado correctamente!${NC}"
        return 0
    else
        echo -e "${RED}Algunos tests han fallado. Revisa tu implementación.${NC}"
        return 1
    fi
}

# Función para mostrar el subject de un ejercicio
show_subject() {
    local exercise=$1
    local subject_path="$EXAM_DIR/$exercise/README.md"
    
    clear
    echo -e "${BLUE}${BOLD}=== EJERCICIO: $exercise ===${NC}\n"
    echo -e "${CYAN}${BOLD}=== SUBJECT ===${NC}\n"
    
    if [ -f "$subject_path" ]; then
        cat "$subject_path"
        echo -e "\n${YELLOW}Para resolver este ejercicio:${NC}"
        echo -e "1. ${CYAN}Directorio:${NC} rendu/$exercise"
        echo -e "2. ${CYAN}Crea tu solución en:${NC} rendu/$exercise/"
        echo -e "\n${GREEN}${BOLD}IMPORTANTE:${NC} Tu solución debe estar dentro del directorio rendu/$exercise/"
        echo -e "Los archivos que debes crear son:"
        if [ "$exercise" = "ft_printf" ]; then
            echo -e "- ${CYAN}ft_printf.c${NC} ${YELLOW}(NO necesita .h)${NC}"
        elif [ "$exercise" = "get_next_line" ]; then
            echo -e "- ${CYAN}get_next_line.c${NC}"
            echo -e "- ${CYAN}get_next_line.h${NC}"
        fi
    else
        echo -e "${RED}Error: No se encuentra el subject en $subject_path${NC}"
    fi
    
    echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
    read
}

# Función para practicar un ejercicio específico
practice_exercise() {
    local exercise=$1
    
    while true; do
        clear
        show_subject "$exercise"
        
        echo -e "\n${YELLOW}${BOLD}Opciones:${NC}"
        echo "1. Validar mi solución"
        echo "2. Ver ejemplos y guías"
        echo "3. Volver al menú principal"
        
        read -p "Selección: " option
        
        case $option in
            1)
                validate_exercise "$exercise"
                echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
                read
                ;;
            2)
                echo -e "${YELLOW}Ver ejemplos no implementado en versión corregida${NC}"
                sleep 2
                ;;
            3)
                return
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                sleep 1
                ;;
        esac
    done
}

# Menú principal simplificado
main_menu() {
    while true; do
        clear
        echo -e "${BLUE}${BOLD}=== 42 EXAM RANK 03 PRACTICE (VERSIÓN CORREGIDA) ===${NC}"
        echo -e "${GREEN}✅ ft_printf: Solo necesita .c (SIN .h)${NC}"
        echo -e "${GREEN}✅ get_next_line: Necesita .c Y .h${NC}"
        echo ""
        echo -e "${YELLOW}${BOLD}Selecciona una opción:${NC}"
        echo "1. Practicar ft_printf"
        echo "2. Practicar get_next_line"
        echo "3. Ver estructura de directorios"
        echo "4. Limpiar directorio rendu"
        echo "0. Salir"
        
        read -p "Selección: " choice
        
        case $choice in
            1)
                practice_exercise "ft_printf"
                ;;
            2)
                practice_exercise "get_next_line"
                ;;
            3)
                clear
                echo -e "${BLUE}${BOLD}=== ESTRUCTURA DE DIRECTORIOS ===${NC}"
                echo -e "${CYAN}rendu/ft_printf/${NC} - Para ft_printf.c (sin .h)"
                echo -e "${CYAN}rendu/get_next_line/${NC} - Para get_next_line.c y get_next_line.h"
                echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
                read
                ;;
            4)
                echo -e "${RED}¿Estás seguro de que quieres limpiar el directorio rendu? (s/n)${NC}"
                read -r response
                if [[ "$response" =~ ^[Ss]$ ]]; then
                    rm -rf "$RENDU_DIR"/*
                    mkdir -p "$RENDU_DIR/ft_printf"
                    mkdir -p "$RENDU_DIR/get_next_line"
                    echo -e "${GREEN}Directorio rendu limpiado y reconstruido.${NC}"
                    sleep 1
                fi
                ;;
            0)
                exit 0
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                sleep 1
                ;;
        esac
    done
}

# Iniciar menú principal
main_menu