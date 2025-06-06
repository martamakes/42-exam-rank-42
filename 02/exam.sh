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
LEVEL3_DONE="$PROGRESS_DIR/level3_done.txt"
LEVEL4_DONE="$PROGRESS_DIR/level4_done.txt"

# Crear archivos si no existen
touch "$LEVEL1_DONE" "$LEVEL2_DONE" "$LEVEL3_DONE" "$LEVEL4_DONE"

# Función para validar ejercicio
validate_exercise() {
    local level=$1
    local exercise=$2
    local exercise_dir="$EXAM_DIR/Level${level}/${exercise}"
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
        
        touch "$grademe_dir/test_main.c"
        echo "// Tests pendientes de crear" > "$grademe_dir/test_main.c"
        
        echo -e "${YELLOW}Tests pendientes de crear. Se han creado archivos vacíos.${NC}"
        return 1
    fi

    if [ ! -f "$test_script" ]; then
        echo -e "${YELLOW}Creando archivos de test vacíos...${NC}"
        touch "$grademe_dir/test.sh"
        chmod +x "$grademe_dir/test.sh"
        echo "echo 'Los tests aún no se han creado'" > "$grademe_dir/test.sh"
        
        touch "$grademe_dir/test_main.c"
        echo "// Tests pendientes de crear" > "$grademe_dir/test_main.c"
        
        echo -e "${YELLOW}Tests pendientes de crear. Se han creado archivos vacíos.${NC}"
        return 1
    fi

    if [ ! -f "$student_file" ]; then
        echo -e "${RED}Error: No se encuentra tu solución en $student_file${NC}"
        echo -e "${YELLOW}Debes crear el archivo $exercise.c en el directorio $student_dir${NC}"
        mkdir -p "$student_dir"
        return 1
    fi
    
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
    local student_dir="$RENDU_DIR/${exercise}"
    
    clear
    echo -e "${BLUE}=== EJERCICIO NIVEL $level: $exercise ===${NC}\n"
    echo -e "${CYAN}=== SUBJECT ===${NC}\n"
    
    if [ -f "$subject_path" ]; then
        cat "$subject_path"
        echo -e "\n${YELLOW}Para resolver este ejercicio:${NC}"
        echo -e "1. ${CYAN}Crea el directorio:${NC} mkdir -p rendu/$exercise"
        echo -e "2. ${CYAN}Crea tu solución en:${NC} rendu/$exercise/${exercise}.c"
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
    
    # Verificar si el ejercicio ya está marcado como completado
    if ! grep -q "^${exercise}$" "$done_file"; then
        echo "$exercise" >> "$done_file"
        echo -e "${GREEN}¡Ejercicio $exercise marcado como completado!${NC}"
    else
        echo -e "${YELLOW}El ejercicio $exercise ya estaba marcado como completado${NC}"
    fi
}

# Función para mostrar progreso
show_progress() {
    echo -e "\n${BLUE}=== PROGRESO ACTUAL ===${NC}"
    for i in {1..4}; do
        local done_file="$PROGRESS_DIR/level${i}_done.txt"
        local total=$(count_total_exercises $i)
        # Usar sort y uniq para contar solo ejercicios únicos
        local completed=$(sort "$done_file" | uniq | wc -l)
        echo -e "${GREEN}Nivel $i: $completed/$total ejercicios completados${NC}"
    done
    echo
}

# Función para limpiar duplicados de los archivos de progreso
clean_progress_files() {
    for i in {1..4}; do
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
    for dir in Level${level}/*/; do
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
        echo "3. Level 3"
        echo "4. Level 4"
        echo "0. Volver"
        
        read -r level_choice
        
        case $level_choice in
            [1-4])
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
                    echo -e "${GREEN}¡Tests pasados correctamente!${NC}"
                    mark_as_completed $level "$exercise"
                    break
                else
                    echo -e "${RED}Los tests han fallado. Sigue intentándolo.${NC}"
                    echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
                    read
                fi
                ;;
            2)
                mark_as_completed $level "$exercise"
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

# Función para practicar todos los ejercicios de un nivel aleatoriamente
practice_level_randomly() {
    local level=$1
    local exercises=()
    local done_file="$PROGRESS_DIR/level${level}_done.txt"
    local total=$(count_total_exercises $level)
    local completed=$(sort "$done_file" | uniq | wc -l)
    
    # Obtener lista de todos los ejercicios del nivel
    for dir in Level${level}/*/; do
        if [ -d "$dir" ]; then
            dirname=$(basename "$dir")
            # Verificar si el ejercicio ya está completado
            if ! grep -q "^$dirname$" "$done_file"; then
                exercises+=("$dirname")
            fi
        fi
    done
    
    # Verificar si hay ejercicios disponibles
    local available=${#exercises[@]}
    if [ $available -eq 0 ]; then
        echo -e "${YELLOW}No hay ejercicios disponibles en el nivel $level. ¡Todos están completados!${NC}"
        read -p "Presiona Enter para continuar..."
        return
    fi
    
    clear
    echo -e "${BLUE}=== PRACTICANDO NIVEL $level ALEATORIAMENTE ===${NC}"
    echo -e "${GREEN}Progreso: $completed/$total ejercicios completados${NC}"
    echo -e "${YELLOW}Ejercicios disponibles: $available${NC}"
    echo -e "\n${YELLOW}Presiona Enter para comenzar o 'q' para salir${NC}"
    read -r response
    [ "$response" = "q" ] && return
    
    # Aleatorizar el orden de los ejercicios
    # Usamos el algoritmo Fisher-Yates para mezclar el array
    local shuffled_exercises=()
    local size=${#exercises[@]}
    local indices=()

    # Generar un array de índices
    for i in $(seq 0 $((size-1))); do
        indices[$i]=$i
    done

    # Mezclar índices usando el algoritmo Fisher-Yates
    for ((i = size - 1; i > 0; i--)); do
        j=$((RANDOM % (i + 1)))
        # Intercambiar índices
        temp=${indices[$i]}
        indices[$i]=${indices[$j]}
        indices[$j]=$temp
    done

    # Usar los índices mezclados para crear el array de ejercicios
    for i in "${indices[@]}"; do
        shuffled_exercises+=("${exercises[$i]}")
    done
    
    # Practicar cada ejercicio en orden aleatorio
    for exercise in "${shuffled_exercises[@]}"; do
        clear
        echo -e "${BLUE}=== PRACTICANDO NIVEL $level ALEATORIAMENTE ===${NC}"
        echo -e "${GREEN}Progreso: $completed/$total ejercicios completados${NC}"
        echo -e "${YELLOW}Ejercicio actual: $exercise${NC}"
        
        show_subject $level "$exercise"
        while true; do
            echo -e "\n${YELLOW}Opciones:${NC}"
            echo "1. Validar ejercicio"
            echo "2. Marcar como completado sin validar"
            echo "3. Dejar pendiente (pasar al siguiente)"
            echo "4. Salir al menú principal"
            read -r option
            
            case $option in
                1)
                    if validate_exercise $level "$exercise"; then
                        echo -e "${GREEN}¡Tests pasados correctamente!${NC}"
                        mark_as_completed $level "$exercise"
                        break
                    else
                        echo -e "${RED}Los tests han fallado. Sigue intentándolo.${NC}"
                        echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
                        read
                    fi
                    ;;
                2)
                    mark_as_completed $level "$exercise"
                    echo -e "${GREEN}¡Ejercicio marcado como completado!${NC}"
                    break
                    ;;
                3)
                    echo -e "${YELLOW}Pasando al siguiente ejercicio...${NC}"
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
        
        # Actualizar contador de completados
        completed=$(sort "$done_file" | uniq | wc -l)
        
        # Si no quedan más ejercicios, salir
        if [ $completed -eq $total ]; then
            echo -e "${GREEN}¡Felicidades! Has completado todos los ejercicios del nivel $level${NC}"
            read -p "Presiona Enter para continuar..."
            return
        fi
        
        echo -e "\n${YELLOW}Presiona Enter para continuar con el siguiente ejercicio o 'q' para salir${NC}"
        read -r response
        [ "$response" = "q" ] && break
    done
}

# Función para seleccionar un nivel y practicar todos sus ejercicios aleatoriamente
select_level_for_random_practice() {
    while true; do
        clear
        echo -e "${BLUE}=== SELECCIONAR NIVEL PARA PRÁCTICA ALEATORIA ===${NC}"
        echo "1. Level 1"
        echo "2. Level 2"
        echo "3. Level 3"
        echo "4. Level 4"
        echo "0. Volver"
        
        read -r level_choice
        
        case $level_choice in
            [1-4])
                practice_level_randomly "$level_choice"
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

# Función para inicializar el entorno
init_environment() {
    # Verificar que existe init.sh
    if [ ! -f "./init.sh" ]; then
        echo -e "${RED}Error: No se encuentra init.sh${NC}"
        exit 1
    fi
    
    # Dar permisos de ejecución si no los tiene
    chmod +x ./init.sh
    
    # Verificar estructura de niveles
    for i in {1..4}; do
        if [ ! -d "Level$i" ]; then
            echo -e "${RED}Error: No se encuentra el directorio Level$i${NC}"
            exit 1
        fi
    done
    
    # Limpiar archivos de progreso de duplicados
    clean_progress_files
}

# Inicializar el entorno
init_environment

# Menú principal
while true; do
    clear
    echo -e "${BLUE}=== 42 EXAM PRACTICE ===${NC}"
    show_progress
    
    echo -e "\n${YELLOW}Opciones:${NC}"
    echo "1. Comenzar práctica aleatoria (todos los niveles)"
    echo "2. Practicar todos los ejercicios de un nivel aleatoriamente"
    echo "3. Seleccionar ejercicio específico"
    echo "4. Resetear todo el progreso"
    echo "5. Salir"
    
    read -p "Selecciona una opción: " option

    case $option in
        1)
            practice_exercises
            ;;
        2)
            select_level_for_random_practice
            ;;
        3)
            select_level
            ;;
        4)
            echo -e "${RED}¿Estás seguro de que quieres resetear todo el progreso? (s/n)${NC}"
            read -r response
            if [[ "$response" =~ ^[Ss]$ ]]; then
                # Vaciar archivos de progreso (manteniendo los archivos)
                > "$LEVEL1_DONE"
                > "$LEVEL2_DONE"
                > "$LEVEL3_DONE"
                > "$LEVEL4_DONE"
                
                # Eliminar todos los archivos y carpetas dentro de rendu
                # pero recrear el directorio para asegurar que existe
                rm -rf "$RENDU_DIR"/*
                mkdir -p "$RENDU_DIR"
                
                echo -e "${GREEN}Progreso reseteado y directorio rendu limpiado${NC}"
            fi
            ;;
        5)
            echo "¡Hasta luego!"
            exit 0
            ;;
        *)
            echo "Opción inválida"
            ;;
    esac

    [ "$option" != "1" ] && [ "$option" != "2" ] && [ "$option" != "3" ] && read -p "Presiona Enter para continuar..."
done