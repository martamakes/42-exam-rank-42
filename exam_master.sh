#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'

# Directorios de trabajo
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Función para verificar la existencia de exámenes
check_exams() {
    available_exams=()
    
    # Verificar existencia de examen 02
    if [ -f "$SCRIPT_DIR/02/exam.sh" ]; then
        available_exams+=("Exam Rank 02")
    fi
    
    # Verificar existencia de examen 03
    if [ -d "$SCRIPT_DIR/03" ]; then
        available_exams+=("Exam Rank 03")
    fi
    
    # Verificar existencia de examen 04
    if [ -d "$SCRIPT_DIR/04" ]; then
        available_exams+=("Exam Rank 04")
    fi
    
    # Verificar existencia de examen 05
    if [ -d "$SCRIPT_DIR/05" ]; then
        available_exams+=("Exam Rank 05")
    fi
    
    # Verificar existencia de examen 06
    if [ -d "$SCRIPT_DIR/06" ]; then
        available_exams+=("Exam Rank 06")
    fi
}

# Función para iniciar el examen 02
launch_exam_02() {
    clear
    echo -e "${BLUE}Iniciando Exam Rank 02...${NC}"
    cd "$SCRIPT_DIR/02" && bash ./exam.sh
}

# Función para iniciar el examen 03
launch_exam_03() {
    clear
    echo -e "${BLUE}Iniciando Exam Rank 03...${NC}"
    
    # Aquí irá la lógica para el examen 03
    # Por ahora, simplemente mostraremos un mensaje informativo
    echo -e "${YELLOW}El examen 03 contiene:${NC}"
    echo "1. ft_printf"
    echo "2. get_next_line"
    echo
    echo -e "${RED}La interfaz para este examen está en desarrollo.${NC}"
    echo -e "${YELLOW}Por favor, accede directamente a los directorios 03/ft_printf y 03/get_next_line.${NC}"
    echo
    read -p "Presiona Enter para volver al menú principal..."
}

# Función para iniciar el examen 04
launch_exam_04() {
    clear
    echo -e "${BLUE}Iniciando Exam Rank 04...${NC}"
    cd "$SCRIPT_DIR/04" && bash ./exam.sh
}

# Función para iniciar el examen 05
launch_exam_05() {
    clear
    echo -e "${BLUE}Iniciando Exam Rank 05...${NC}"
    cd "$SCRIPT_DIR/05" && bash ./exam.sh
}

# Función para iniciar el examen 06
launch_exam_06() {
    clear
    echo -e "${BLUE}Iniciando Exam Rank 06...${NC}"
    echo -e "${RED}La interfaz para este examen está en desarrollo.${NC}"
    echo
    read -p "Presiona Enter para volver al menú principal..."
}

# Menú principal
main_menu() {
    while true; do
        clear
        echo -e "${BLUE}=== 42 EXAM PRACTICE MASTER ====${NC}"
        echo -e "${YELLOW}Selecciona el examen que quieres practicar:${NC}"
        
        # Verificar exámenes disponibles
        check_exams
        
        # Mostrar opciones disponibles
        for i in "${!available_exams[@]}"; do
            echo "$((i+1)). ${available_exams[$i]}"
        done
        
        echo "0. Salir"
        
        read -p "Selección: " choice
        
        case $choice in
            1)
                if [[ "${available_exams[0]}" == "Exam Rank 02" ]]; then
                    launch_exam_02
                else
                    echo -e "${RED}Opción inválida${NC}"
                    sleep 1
                fi
                ;;
            2)
                if [[ "${available_exams[1]}" == "Exam Rank 03" ]]; then
                    launch_exam_03
                else
                    echo -e "${RED}Opción inválida${NC}"
                    sleep 1
                fi
                ;;
            3)
                if [[ "${available_exams[2]}" == "Exam Rank 04" ]]; then
                    launch_exam_04
                else
                    echo -e "${RED}Opción inválida${NC}"
                    sleep 1
                fi
                ;;
            4)
                if [[ "${available_exams[3]}" == "Exam Rank 05" ]]; then
                    launch_exam_05
                else
                    echo -e "${RED}Opción inválida${NC}"
                    sleep 1
                fi
                ;;
            5)
                if [[ "${available_exams[4]}" == "Exam Rank 06" ]]; then
                    launch_exam_06
                else
                    echo -e "${RED}Opción inválida${NC}"
                    sleep 1
                fi
                ;;
            0)
                echo -e "${GREEN}¡Hasta luego!${NC}"
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
