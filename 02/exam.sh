#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'

# Directorios de trabajo
PROGRESS_DIR="./exam_progress"
mkdir -p "$PROGRESS_DIR"

# Archivos para almacenar ejercicios completados por nivel
LEVEL1_DONE="$PROGRESS_DIR/level1_done.txt"
LEVEL2_DONE="$PROGRESS_DIR/level2_done.txt"
LEVEL3_DONE="$PROGRESS_DIR/level3_done.txt"
LEVEL4_DONE="$PROGRESS_DIR/level4_done.txt"

# Crear archivos si no existen
touch "$LEVEL1_DONE" "$LEVEL2_DONE" "$LEVEL3_DONE" "$LEVEL4_DONE"

# Función para validar ejercicio
validate_exercise() {
    local level=$1
    local exercise=$2
    local level_dir="Level${level}"  # Cambiado aquí
    local grademe_dir="${level_dir}/${exercise}/grademe"
    local student_dir="rendu/${exercise}"
    local student_file="${student_dir}/${exercise}.c"
    
    if [ ! -d "$grademe_dir" ]; then
        echo -e "${YELLOW}Inicializando tests para $exercise...${NC}"
        if [ -f "./init.sh" ]; then
            ./init.sh "${level_dir}" "$exercise"  # Cambiado aquí: pasamos "Level1" en lugar de "1"
            if [ $? -ne 0 ]; then
                echo -e "${RED}Error al inicializar los tests${NC}"
                return 1
            fi
        else
            echo -e "${RED}Error: No se encuentra init.sh${NC}"
            return 1
        fi
    fi

    if [ ! -f "$student_file" ]; then
        echo -e "${RED}Error: No se encuentra tu solución en $student_file${NC}"
        echo -e "${YELLOW}Debes crear el archivo $exercise.c en el directorio $student_dir${NC}"
        return 1
    fi
    
    echo -e "${BLUE}Ejecutando tests para $exercise...${NC}"
    cd "$grademe_dir"
    ./test.sh
    local result=$?
    cd - > /dev/null
    
    return $result
}

# Función para obtener ejercicios disponibles de un nivel
get_available_exercises() {
    local level=$1
    local done_file="$PROGRESS_DIR/level${level}_done.txt"
    
    # Obtener lista de directorios en el nivel
    for dir in Level${level}/*/; do
        if [ -d "$dir" ]; then
            # Obtener solo el nombre del directorio
            dirname=$(basename "$dir")
            # Verificar si el ejercicio ya está completado
            if ! grep -q "^$dirname$" "$done_file"; then
                echo "$dirname"
            fi
        fi
    done
}

# Función para contar ejercicios totales en un nivel
count_total_exercises() {
    local level=$1
    find "Level${level}" -maxdepth 1 -mindepth 1 -type d | wc -l
}

# Función para mostrar el subject de un ejercicio
show_subject() {
    local level=$1
    local exercise=$2
    local subject_path="Level${level}/${exercise}/README.md"
    local student_dir="rendu/${exercise}"
    
    clear
    echo -e "${BLUE}=== EJERCICIO NIVEL $level: $exercise ===${NC}\n"
    echo -e "${CYAN}=== SUBJECT ===${NC}\n"
    
    if [ -f "$subject_path" ]; then
        cat "$subject_path"
        echo -e "\n${YELLOW}Para resolver este ejercicio:${NC}"
        # Usar echo -e para interpretar los colores y escapar las comillas correctamente
        echo -e "1. ${CYAN}Crea el directorio:${NC} mkdir -p 02/$student_dir"
        echo -e "2. ${CYAN}Crea tu solución en:${NC} 02/$student_dir/${exercise}.c"
        mkdir -p "$student_dir"
    else
        echo -e "${RED}Error: No se encuentra el subject en $subject_path${NC}"
        return 1
    fi
}

# Función para marcar un ejercicio como completado
mark_as_completed() {
    local level=$1
    local exercise=$2
    local done_file="$PROGRESS_DIR/level${level}_done.txt"
    echo "$exercise" >> "$done_file"
}

# Función para mostrar progreso
show_progress() {
    echo -e "\n${BLUE}=== PROGRESO ACTUAL ===${NC}"
    for i in {1..4}; do
        local done_file="$PROGRESS_DIR/level${i}_done.txt"
        local total=$(count_total_exercises $i)
        local completed=$(wc -l < "$done_file")
        echo -e "${GREEN}Nivel $i: $completed/$total ejercicios completados${NC}"
    done
    echo
}

# Función para seleccionar un ejercicio aleatorio de un nivel
select_random_exercise() {
    local level=$1
    local exercises=($(get_available_exercises $level))
    local count=${#exercises[@]}
    
    if [ $count -eq 0 ]; then
        return 1
    fi
    
    local random_index=$((RANDOM % count))
    echo "${exercises[$random_index]}"
}

# Función para practicar ejercicios
practice_exercises() {
    while true; do
        clear
        show_progress
        
        # Seleccionar ejercicios para cada nivel
        ex1=$(select_random_exercise 1)
        ex2=$(select_random_exercise 2)
        ex3=$(select_random_exercise 3)
        ex4=$(select_random_exercise 4)
        
        # Mostrar ejercicios seleccionados
        [ -n "$ex1" ] && echo -e "${GREEN}Nivel 1:${NC} $ex1" || echo -e "${RED}Nivel 1: Completado${NC}"
        [ -n "$ex2" ] && echo -e "${GREEN}Nivel 2:${NC} $ex2" || echo -e "${RED}Nivel 2: Completado${NC}"
        [ -n "$ex3" ] && echo -e "${GREEN}Nivel 3:${NC} $ex3" || echo -e "${RED}Nivel 3: Completado${NC}"
        [ -n "$ex4" ] && echo -e "${GREEN}Nivel 4:${NC} $ex4" || echo -e "${RED}Nivel 4: Completado${NC}"
        
        echo -e "\n${YELLOW}Presiona Enter para comenzar con los ejercicios o 'q' para salir${NC}"
        read -r response
        [ "$response" = "q" ] && break
        
        # Practicar cada ejercicio seleccionado
        for level in {1..4}; do
            local current_ex
            case $level in
                1) current_ex=$ex1 ;;
                2) current_ex=$ex2 ;;
                3) current_ex=$ex3 ;;
                4) current_ex=$ex4 ;;
            esac
            
            if [ -n "$current_ex" ]; then
                show_subject $level "$current_ex"
                while true; do
                    echo -e "\n${YELLOW}Opciones:${NC}"
                    echo "1. Validar ejercicio"
                    echo "2. Marcar como completado sin validar"
                    echo "3. Dejar pendiente"
                    echo "4. Salir"
                    read -r option
                    
                    case $option in
                        1)
                            if validate_exercise $level "$current_ex"; then
                                echo -e "${GREEN}¡Tests pasados correctamente!${NC}"
                                mark_as_completed $level "$current_ex"
                                break
                            else
                                echo -e "${RED}Los tests han fallado. Sigue intentándolo.${NC}"
                                echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
                                read
                            fi
                            ;;
                        2)
                            mark_as_completed $level "$current_ex"
                            echo -e "${GREEN}¡Ejercicio marcado como completado!${NC}"
                            break
                            ;;
                        3)
                            echo -e "${YELLOW}Ejercicio pendiente para la próxima vez${NC}"
                            break
                            ;;
                        4)
                            return
                            ;;
                        *)
                            echo "Opción inválida"
                            ;;
                    esac
                done
                
                echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
                read
            fi
        done
    done
}

# Verificar y preparar el entorno
init_environment() {
    # Verificar que existe init.sh
    if [ ! -f "./init.sh" ]; then
        echo -e "${RED}Error: No se encuentra init.sh${NC}"
        exit 1
    fi
    
    # Dar permisos de ejecución si no los tiene
    chmod +x ./init.sh
    
    # Crear directorio rendu si no existe
    mkdir -p rendu
    
    # Verificar estructura de niveles
    for i in {1..4}; do
        if [ ! -d "Level$i" ]; then
            echo -e "${RED}Error: No se encuentra el directorio Level$i${NC}"
            exit 1
        fi
    done
}

# Inicializar el entorno
init_environment

# Menú principal
while true; do
    clear
    echo -e "${BLUE}=== 42 EXAM PRACTICE ===${NC}"
    show_progress
    
    echo -e "\n${YELLOW}Opciones:${NC}"
    echo "1. Comenzar práctica"
    echo "2. Resetear todo el progreso"
    echo "3. Salir"
    
    read -p "Selecciona una opción: " option

    case $option in
        1)
            practice_exercises
            ;;
        2)
            echo -e "${RED}¿Estás seguro de que quieres resetear todo el progreso? (s/n)${NC}"
            read -r response
            if [[ "$response" =~ ^[Ss]$ ]]; then
                > "$LEVEL1_DONE"
                > "$LEVEL2_DONE"
                > "$LEVEL3_DONE"
                > "$LEVEL4_DONE"
                echo "Progreso reseteado"
            fi
            ;;
        3)
            echo "¡Hasta luego!"
            exit 0
            ;;
        *)
            echo "Opción inválida"
            ;;
    esac

    [ "$option" != "1" ] && read -p "Presiona Enter para continuar..."
done