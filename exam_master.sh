#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'

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
    if [ -f "$SCRIPT_DIR/03/exam.sh" ]; then
        available_exams+=("Exam Rank 03")
    fi
    
    # Verificar existencia de examen 04
    if [ -f "$SCRIPT_DIR/04/exam.sh" ]; then
        available_exams+=("Exam Rank 04")
    fi
    
    # Verificar existencia de examen 05
    if [ -f "$SCRIPT_DIR/05/exam.sh" ]; then
        available_exams+=("Exam Rank 05")
    fi
    
    # Verificar existencia de examen 06
    if [ -f "$SCRIPT_DIR/06/exam.sh" ]; then
        available_exams+=("Exam Rank 06")
    fi
}

# Función para iniciar el examen 02
launch_exam_02() {
    clear
    echo -e "${BLUE}${BOLD}Iniciando Exam Rank 02...${NC}"
    cd "$SCRIPT_DIR/02" && bash ./exam.sh
}

# Función para iniciar el examen 03
launch_exam_03() {
    clear
    echo -e "${BLUE}${BOLD}Iniciando Exam Rank 03...${NC}"
    cd "$SCRIPT_DIR/03" && bash ./exam.sh
}

# Función para iniciar el examen 04
launch_exam_04() {
    clear
    echo -e "${BLUE}${BOLD}Iniciando Exam Rank 04...${NC}"
    cd "$SCRIPT_DIR/04" && bash ./exam.sh
}

# Función para iniciar el examen 05
launch_exam_05() {
    clear
    echo -e "${BLUE}${BOLD}Iniciando Exam Rank 05...${NC}"
    cd "$SCRIPT_DIR/05" && bash ./exam.sh
}

# Función para iniciar el examen 06
launch_exam_06() {
    clear
    echo -e "${BLUE}${BOLD}Iniciando Exam Rank 06...${NC}"
    cd "$SCRIPT_DIR/06" && bash ./exam.sh
}

# Verificar y crear directorios de rendu necesarios
setup_directories() {
    # Crear directorio rendu principal si no existe
    mkdir -p "$SCRIPT_DIR/rendu"
    
    # Crear directorios rendu para cada nivel
    if [ -d "$SCRIPT_DIR/02" ]; then
        mkdir -p "$SCRIPT_DIR/02/exam_progress"
    fi
    
    if [ -d "$SCRIPT_DIR/03" ]; then
        mkdir -p "$SCRIPT_DIR/03/rendu"
    fi
    
    if [ -d "$SCRIPT_DIR/04" ]; then
        mkdir -p "$SCRIPT_DIR/04/rendu"
    fi
    
    if [ -d "$SCRIPT_DIR/05" ]; then
        mkdir -p "$SCRIPT_DIR/05/rendu"
    fi
    
    if [ -d "$SCRIPT_DIR/06" ]; then
        mkdir -p "$SCRIPT_DIR/06/rendu"
    fi
    
    # Dar permisos de ejecución a todos los scripts exam.sh
    find "$SCRIPT_DIR" -name "exam.sh" -exec chmod +x {} \;
    
    # Dar permisos de ejecución a otros scripts importantes
    if [ -f "$SCRIPT_DIR/02/init.sh" ]; then
        chmod +x "$SCRIPT_DIR/02/init.sh"
    fi
    
    if [ -f "$SCRIPT_DIR/04/setup.sh" ]; then
        chmod +x "$SCRIPT_DIR/04/setup.sh"
    fi
    
    if [ -f "$SCRIPT_DIR/05/setup.sh" ]; then
        chmod +x "$SCRIPT_DIR/05/setup.sh"
    fi
    
    if [ -f "$SCRIPT_DIR/06/setup.sh" ]; then
        chmod +x "$SCRIPT_DIR/06/setup.sh"
    fi
    
    # Dar permisos a scripts de instalación
    chmod +x "$SCRIPT_DIR/chmod_script.sh" 2>/dev/null
}

# Menú principal
main_menu() {
    # Primero configurar directorios
    setup_directories
    
    while true; do
        clear
        echo -e "${BLUE}${BOLD}=== 42 EXAM PRACTICE MASTER ===${NC}"
        echo -e "${YELLOW}Selecciona el examen que quieres practicar:${NC}"
        
        # Verificar exámenes disponibles
        check_exams
        
        # Mostrar opciones disponibles
        for i in "${!available_exams[@]}"; do
            echo "$((i+1)). ${available_exams[$i]}"
        done
        
        echo "0. Salir"
        
        read -p "Selección: " choice
        
        # Validar la entrada para evitar errores
        if ! [[ "$choice" =~ ^[0-9]+$ ]]; then
            echo -e "${RED}Entrada inválida. Por favor, introduce un número.${NC}"
            sleep 1
            continue
        fi
        
        # No existen exámenes
        if [ ${#available_exams[@]} -eq 0 ]; then
            echo -e "${RED}No se encontraron exámenes disponibles.${NC}"
            sleep 2
            exit 1
        fi
        
        if [ "$choice" -eq 0 ]; then
            echo -e "${GREEN}¡Hasta luego!${NC}"
            exit 0
        elif [ "$choice" -le ${#available_exams[@]} ]; then
            exam="${available_exams[$((choice-1))]}"
            case "$exam" in
                "Exam Rank 02")
                    launch_exam_02
                    ;;
                "Exam Rank 03")
                    launch_exam_03
                    ;;
                "Exam Rank 04")
                    launch_exam_04
                    ;;
                "Exam Rank 05")
                    launch_exam_05
                    ;;
                "Exam Rank 06")
                    launch_exam_06
                    ;;
                *)
                    echo -e "${RED}Opción inválida${NC}"
                    sleep 1
                    ;;
            esac
        else
            echo -e "${RED}Opción inválida${NC}"
            sleep 1
        fi
    done
}

# Iniciar menú principal
main_menu