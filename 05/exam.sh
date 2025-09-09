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
PROGRESS_DIR="$EXAM_DIR/exam_progress"
RENDU_DIR="$PROJECT_ROOT/rendu"

# Crear directorios necesarios
mkdir -p "$PROGRESS_DIR"
mkdir -p "$RENDU_DIR"

# Archivos para almacenar ejercicios completados por nivel
LEVEL1_DONE="$PROGRESS_DIR/level1_done.txt"
LEVEL2_DONE="$PROGRESS_DIR/level2_done.txt"

# Crear archivos si no existen
touch "$LEVEL1_DONE" "$LEVEL2_DONE"

# Función para validar ejercicio
validate_exercise() {
    local level=$1
    local exercise=$2
    local exercise_dir="$EXAM_DIR/level-${level}/${exercise}"
    local grademe_dir="$exercise_dir/grademe"
    local student_dir="$RENDU_DIR/${exercise}"
    local student_file="${student_dir}/${exercise}.c"
    local test_script="$grademe_dir/test.sh"
    
    # Verificar que existe el directorio de tests
    if [ ! -d "$grademe_dir" ]; then
        echo -e "${RED}Error: No se encuentran los tests para $exercise${NC}"
        echo -e "${YELLOW}Creando directorio: $grademe_dir${NC}"
        mkdir -p "$grademe_dir"
        if [ $? -ne 0 ]; then
            echo -e "${RED}Error: No se pudo crear el directorio de tests${NC}"
            return 1
        fi
        
        # Crear archivos vacíos de test
        touch "$grademe_dir/test.sh"
        chmod +x "$grademe_dir/test.sh"
        echo "echo 'Los tests aún no se han creado'" > "$grademe_dir/test.sh"
        
        touch "$grademe_dir/test.sh"
        chmod +x "$grademe_dir/test.sh"
        echo "echo 'Los tests aún no se han creado'" > "$grademe_dir/test.sh"
    fi
    
    # Verificar que el estudiante ha creado su directorio
    if [ ! -d "$student_dir" ]; then
        echo -e "${RED}Error: No se encuentra el directorio $student_dir${NC}"
        echo -e "${YELLOW}Crea el directorio: mkdir $student_dir${NC}"
        return 1
    fi
    
    # Para ejercicios de C++, buscar archivos .cpp en lugar de .c
    local student_cpp_file="${student_dir}/${exercise}.cpp"
    if [ ! -f "$student_file" ] && [ ! -f "$student_cpp_file" ]; then
        echo -e "${RED}Error: No se encuentra el archivo $student_file o $student_cpp_file${NC}"
        return 1
    fi
    
    echo -e "${BLUE}Ejecutando tests para $exercise...${NC}"
    cd "$grademe_dir"
    
    # Ejecutar el script de tests
    ./test.sh
    local test_result=$?
    
    cd "$EXAM_DIR"
    
    if [ $test_result -eq 0 ]; then
        echo -e "${GREEN}✅ Tests pasados para $exercise${NC}"
        return 0
    else
        echo -e "${RED}❌ Tests fallaron para $exercise${NC}"
        return 1
    fi
}

# Función para contar ejercicios totales por nivel
count_total_exercises() {
    local level=$1
    find "$EXAM_DIR/level-${level}" -maxdepth 1 -mindepth 1 -type d | wc -l
}

# Función para obtener ejercicios disponibles (no completados)
get_available_exercises() {
    local level=$1
    local done_file="$PROGRESS_DIR/level${level}_done.txt"
    local exercises=()
    
    # Obtener todos los ejercicios del nivel
    for dir in level-${level}/*/; do
        if [ -d "$dir" ]; then
            dirname=$(basename "$dir")
            # Verificar si no está completado
            if ! grep -q "^$dirname$" "$done_file" 2>/dev/null; then
                exercises+=("$dirname")
            fi
        fi
    done
    
    echo "${exercises[@]}"
}

# Función para mostrar progreso
show_progress() {
    echo -e "\n${BLUE}=== PROGRESO ACTUAL ===${NC}"
    for i in {1..2}; do
        local done_file="$PROGRESS_DIR/level${i}_done.txt"
        local total=$(count_total_exercises $i)
        # Usar sort y uniq para contar solo ejercicios únicos
        local completed=$(sort "$done_file" 2>/dev/null | uniq | wc -l)
        echo -e "${GREEN}Nivel $i: $completed/$total ejercicios completados${NC}"
    done
    echo
}

# Función para limpiar duplicados de los archivos de progreso
clean_progress_files() {
    for i in {1..2}; do
        local done_file="$PROGRESS_DIR/level${i}_done.txt"
        if [ -f "$done_file" ]; then
            # Crear archivo temporal con entradas únicas
            sort "$done_file" | uniq > "${done_file}.tmp"
            # Reemplazar archivo original
            mv "${done_file}.tmp" "$done_file"
        fi
    done
}

# Función para seleccionar un ejercicio aleatorio de un nivel
select_random_exercise() {
    local level=$1
    local exercises=($(get_available_exercises $level))
    local count=${#exercises[@]}
    
    if [ $count -eq 0 ]; then
        echo -e "${YELLOW}¡Todos los ejercicios del nivel $level están completados!${NC}"
        return 1
    fi
    
    local random_index=$((RANDOM % count))
    echo "${exercises[$random_index]}"
}

# Función para listar ejercicios por nivel
list_level_exercises() {
    local level=$1
    local exercises=()
    local index=1
    
    echo -e "\n${BLUE}=== EJERCICIOS NIVEL $level ===${NC}"
    
    # Obtener y mostrar todos los ejercicios del nivel
    for dir in level-${level}/*/; do
        if [ -d "$dir" ]; then
            dirname=$(basename "$dir")
            exercises+=("$dirname")
            echo "$index) $dirname"
            ((index++))
        fi
    done
    
    echo -e "\n${YELLOW}Selecciona un ejercicio (1-$((index-1))) o 0 para volver:${NC}"
    read -r selection
    
    if [[ "$selection" =~ ^[0-9]+$ ]] && [ "$selection" -gt 0 ] && [ "$selection" -lt "$index" ]; then
        selected_exercise=${exercises[$((selection-1))]}
        practice_single_exercise "$level" "$selected_exercise"
    elif [ "$selection" != "0" ]; then
        echo -e "${RED}Selección inválida${NC}"
        read -p "Presiona Enter para continuar..."
    fi
}

# Función para seleccionar nivel
select_level() {
    while true; do
        clear
        echo -e "${BLUE}=== SELECCIONAR NIVEL ===${NC}"
        echo "1. Level 1"
        echo "2. Level 2"
        echo "0. Volver"
        
        read -r level_choice
        
        case $level_choice in
            [1-2])
                list_level_exercises "$level_choice"
                ;;
            0)
                return
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                read -p "Presiona Enter para continuar..."
                ;;
        esac
    done
}

# Función para practicar un ejercicio específico
practice_single_exercise() {
    local level=$1
    local exercise=$2
    
    show_subject $level "$exercise"
    while true; do
        echo -e "\n${YELLOW}Opciones:${NC}"
        echo "1. Validar ejercicio"
        echo "2. Marcar como completado sin validar"
        echo "3. Dejar pendiente"
        echo "4. Salir"
        read -r option
        
        case $option in
            1)
                if validate_exercise $level "$exercise"; then
                    mark_as_completed $level "$exercise"
                    echo -e "${GREEN}Ejercicio $exercise marcado como completado${NC}"
                    read -p "Presiona Enter para continuar..."
                    return
                else
                    echo -e "${RED}El ejercicio falló la validación${NC}"
                    read -p "Presiona Enter para continuar..."
                fi
                ;;
            2)
                mark_as_completed $level "$exercise"
                echo -e "${GREEN}Ejercicio $exercise marcado como completado${NC}"
                read -p "Presiona Enter para continuar..."
                return
                ;;
            3)
                echo -e "${YELLOW}Ejercicio dejado pendiente${NC}"
                read -p "Presiona Enter para continuar..."
                return
                ;;
            4)
                return
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                ;;
        esac
    done
}

# Función para mostrar el subject
show_subject() {
    local level=$1
    local exercise=$2
    local subject_file="$EXAM_DIR/level-${level}/${exercise}/subject.txt"
    
    clear
    echo -e "${BLUE}=== EXAM RANK 05 - NIVEL $level ===${NC}"
    echo -e "${CYAN}Ejercicio: $exercise${NC}"
    echo ""
    
    if [ -f "$subject_file" ]; then
        cat "$subject_file"
    else
        echo -e "${RED}Error: No se encuentra el archivo de subject${NC}"
        echo "Buscando en: $subject_file"
    fi
    
    echo ""
    echo -e "${YELLOW}Directorio de trabajo: $PROJECT_ROOT/rendu/${exercise}/${NC}"
    echo -e "${YELLOW}Archivo esperado: ${exercise}.cpp (o ${exercise}.c)${NC}"
    echo ""
}

# Función para marcar ejercicio como completado
mark_as_completed() {
    local level=$1
    local exercise=$2
    local done_file="$PROGRESS_DIR/level${level}_done.txt"
    
    echo "$exercise" >> "$done_file"
    clean_progress_files
}

# Función para practicar ejercicios aleatorios
practice_random() {
    while true; do
        clear
        echo -e "${BLUE}=== PRACTICE MODE - EXAM RANK 05 ===${NC}"
        show_progress
        
        echo -e "${YELLOW}Selecciona nivel para ejercicio aleatorio:${NC}"
        echo "1. Level 1"
        echo "2. Level 2"
        echo "0. Volver al menú principal"
        
        read -r level_choice
        
        case $level_choice in
            [1-2])
                exercise=$(select_random_exercise $level_choice)
                if [ $? -eq 0 ] && [ -n "$exercise" ]; then
                    echo -e "${GREEN}Ejercicio aleatorio seleccionado: $exercise${NC}"
                    read -p "Presiona Enter para continuar..."
                    practice_single_exercise $level_choice "$exercise"
                else
                    read -p "Presiona Enter para continuar..."
                fi
                ;;
            0)
                return
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                read -p "Presiona Enter para continuar..."
                ;;
        esac
    done
}

# Función para mostrar estadísticas
show_stats() {
    clear
    echo -e "${BLUE}=== ESTADÍSTICAS - EXAM RANK 05 ===${NC}"
    show_progress
    
    echo -e "${CYAN}=== EJERCICIOS COMPLETADOS ===${NC}"
    for i in {1..2}; do
        local done_file="$PROGRESS_DIR/level${i}_done.txt"
        echo -e "\n${GREEN}Nivel $i:${NC}"
        if [ -f "$done_file" ] && [ -s "$done_file" ]; then
            sort "$done_file" | uniq | nl
        else
            echo "  Ningún ejercicio completado"
        fi
    done
    
    echo ""
    read -p "Presiona Enter para continuar..."
}

# Función para resetear progreso
reset_progress() {
    clear
    echo -e "${BLUE}=== RESETEAR PROGRESO - EXAM RANK 05 ===${NC}"
    echo -e "${RED}¿Estás seguro de que quieres resetear todo el progreso? (s/n)${NC}"
    read -r confirm
    
    if [[ "$confirm" =~ ^[Ss]$ ]]; then
        # Limpiar archivos de progreso
        > "$LEVEL1_DONE"
        > "$LEVEL2_DONE"
        
        # Limpiar directorio rendu
        for dir in level-{1,2}/*/; do
            if [ -d "$dir" ]; then
                exercise=$(basename "$dir")
                rm -rf "$RENDU_DIR/$exercise"
            fi
        done
        
        echo -e "${GREEN}Progreso reseteado y directorio rendu limpiado${NC}"
    else
        echo -e "${YELLOW}Operación cancelada${NC}"
    fi
    
    read -p "Presiona Enter para continuar..."
}

# Menú principal
main_menu() {
    # Limpiar archivos de progreso al inicio
    clean_progress_files
    
    while true; do
        clear
        echo -e "${BLUE}╔══════════════════════════════════════════════════════╗${NC}"
        echo -e "${BLUE}║                 EXAM RANK 05 - PRACTICE              ║${NC}"
        echo -e "${BLUE}╚══════════════════════════════════════════════════════╝${NC}"
        echo ""
        
        show_progress
        
        echo -e "${YELLOW}=== MENÚ PRINCIPAL ===${NC}"
        echo "1. 🎲 Ejercicio aleatorio por nivel"
        echo "2. 📋 Elegir ejercicio manualmente"
        echo "3. 📊 Ver estadísticas y progreso"
        echo "4. 🔄 Resetear progreso"
        echo "5. 🚪 Salir"
        echo ""
        
        read -p "Selecciona una opción: " choice
        
        case $choice in
            1)
                practice_random
                ;;
            2)
                select_level
                ;;
            3)
                show_stats
                ;;
            4)
                reset_progress
                ;;
            5)
                echo -e "${GREEN}¡Hasta luego!${NC}"
                exit 0
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                read -p "Presiona Enter para continuar..."
                ;;
        esac
    done
}

# Verificar que estamos en el directorio correcto
if [ ! -d "level-1" ] && [ ! -d "level-2" ]; then
    echo -e "${RED}Error: No se encuentran los directorios de niveles${NC}"
    echo -e "${YELLOW}Asegúrate de ejecutar este script desde el directorio 05${NC}"
    exit 1
fi

# Iniciar el menú principal
main_menu