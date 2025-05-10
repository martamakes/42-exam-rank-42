#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'

# Directorios de trabajo
EXAM_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$EXAM_DIR")"
RENDU_DIR="$EXAM_DIR/rendu"

# Crear directorio rendu si no existe
mkdir -p "$RENDU_DIR"

# Función para mostrar el subject de un ejercicio
show_subject() {
    local exercise=$1
    local subject_path="$EXAM_DIR/$exercise/README.md"
    
    clear
    echo -e "${BLUE}=== EJERCICIO: $exercise ===${NC}\n"
    echo -e "${CYAN}=== SUBJECT ===${NC}\n"
    
    if [ -f "$subject_path" ]; then
        cat "$subject_path"
        echo -e "\n${YELLOW}Para resolver este ejercicio:${NC}"
        echo -e "1. ${CYAN}Crea el directorio:${NC} mkdir -p $RENDU_DIR/$exercise"
        echo -e "2. ${CYAN}Crea tu solución en:${NC} $RENDU_DIR/$exercise/"
        echo -e "\n${GREEN}IMPORTANTE:${NC} Tu solución debe estar dentro del directorio $RENDU_DIR/$exercise/"
        echo -e "Los archivos que debes crear son:"
        if [ "$exercise" = "ft_printf" ]; then
            echo -e "- ${CYAN}$RENDU_DIR/$exercise/ft_printf.c${NC}"
        elif [ "$exercise" = "get_next_line" ]; then
            echo -e "- ${CYAN}$RENDU_DIR/$exercise/get_next_line.c${NC}"
            echo -e "- ${CYAN}$RENDU_DIR/$exercise/get_next_line.h${NC}"
        fi
        mkdir -p "$RENDU_DIR/$exercise"
    else
        echo -e "${RED}Error: No se encuentra el subject en $subject_path${NC}"
        return 1
    fi
}

# Función para validar ejercicio
validate_exercise() {
    local exercise=$1
    local grademe_dir="$EXAM_DIR/$exercise/grademe"
    local student_dir="$RENDU_DIR/$exercise"
    local test_script="$grademe_dir/test.sh"
    
    # Verificar que existe el directorio de tests
    if [ ! -d "$grademe_dir" ]; then
        echo -e "${RED}Error: No se encuentran los tests para $exercise${NC}"
        return 1
    }

    if [ ! -f "$test_script" ]; then
        echo -e "${RED}Error: No se encuentra el script de test${NC}"
        return 1
    }

    # Verificar que el usuario ha creado su solución
    if [ ! -d "$student_dir" ]; then
        echo -e "${RED}Error: No se encuentra tu solución en $student_dir${NC}"
        echo -e "${YELLOW}Debes crear el directorio $exercise en el directorio rendu${NC}"
        mkdir -p "$student_dir"
        return 1
    }
    
    # Dar permisos de ejecución al script de test
    chmod +x "$test_script"
    
    echo -e "${BLUE}Ejecutando tests para $exercise...${NC}"
    if ! cd "$grademe_dir"; then
        echo -e "${RED}Error: No se puede acceder al directorio de tests${NC}"
        return 1
    fi
    
    ./test.sh
    local result=$?
    cd - > /dev/null
    
    if [ $result -eq 0 ]; then
        echo -e "${GREEN}¡Todos los tests han pasado correctamente!${NC}"
        return 0
    else
        echo -e "${RED}Algunos tests han fallado. El ejercicio no está completamente correcto.${NC}"
        return 1
    fi
}

# Función para practicar un ejercicio específico
practice_exercise() {
    local exercise=$1
    
    while true; do
        show_subject "$exercise"
        
        echo -e "\n${YELLOW}Opciones:${NC}"
        echo "1. Validar ejercicio"
        echo "2. Ver guías y ejemplos"
        echo "3. Volver al menú principal"
        read -r option
        
        case $option in
            1)
                if validate_exercise "$exercise"; then
                    echo -e "${GREEN}¡Tests pasados correctamente!${NC}"
                else
                    echo -e "${RED}Los tests han fallado. Sigue intentándolo.${NC}"
                fi
                echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
                read
                ;;
            2)
                view_guides "$exercise"
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

# Función para ver guías y ejemplos
view_guides() {
    local exercise=$1
    local guide_dir="$EXAM_DIR/$exercise"
    
    clear
    echo -e "${BLUE}=== GUÍAS Y EJEMPLOS: $exercise ===${NC}\n"
    
    # Mostrar archivos que contienen "guide" o "steps" en el nombre
    echo -e "${CYAN}Archivos de guía disponibles:${NC}"
    
    local guides=($(find "$guide_dir" -type f -name "*guide*" -o -name "*steps*" | sort))
    
    if [ ${#guides[@]} -eq 0 ]; then
        echo -e "${RED}No se encontraron guías o ejemplos para este ejercicio.${NC}"
    else
        for i in "${!guides[@]}"; do
            echo "$((i+1)). $(basename "${guides[$i]}")"
        done
        
        echo -e "\n${YELLOW}Selecciona un archivo para ver (0 para volver):${NC}"
        read -r guide_choice
        
        if [[ "$guide_choice" =~ ^[0-9]+$ ]] && [ "$guide_choice" -gt 0 ] && [ "$guide_choice" -le "${#guides[@]}" ]; then
            clear
            echo -e "${CYAN}=== $(basename "${guides[$((guide_choice-1))]}" ) ===${NC}\n"
            cat "${guides[$((guide_choice-1))]}"
            echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
            read
        fi
    fi
}

# Menú principal
main_menu() {
    while true; do
        clear
        echo -e "${BLUE}=== 42 EXAM RANK 03 PRACTICE ===${NC}"
        echo -e "${YELLOW}Selecciona un ejercicio:${NC}"
        echo "1. ft_printf"
        echo "2. get_next_line"
        echo "3. Ver estructura de directorios"
        echo "4. Limpiar directorio rendu"
        echo "0. Volver al menú principal"
        
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
                echo -e "${BLUE}=== ESTRUCTURA DE DIRECTORIOS ===${NC}"
                find "$EXAM_DIR" -type d -maxdepth 2 | sort
                echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
                read
                ;;
            4)
                echo -e "${RED}¿Estás seguro de que quieres limpiar el directorio rendu? (s/n)${NC}"
                read -r response
                if [[ "$response" =~ ^[Ss]$ ]]; then
                    rm -rf "$RENDU_DIR"/*
                    mkdir -p "$RENDU_DIR"
                    echo -e "${GREEN}Directorio rendu limpiado${NC}"
                    sleep 1
                fi
                ;;
            0)
                return
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
