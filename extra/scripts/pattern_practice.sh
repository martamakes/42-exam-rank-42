#!/bin/bash

# Script para practicar ejercicios de un patrón específico

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[0;37m'
NC='\033[0m' # No Color

# Verificar argumentos
if [[ $# -ne 1 ]]; then
    echo -e "${RED}❌ Uso: $0 <pattern_number>${NC}"
    echo -e "${YELLOW}Ejemplo: $0 01${NC}"
    exit 1
fi

PATTERN_NUM="$1"
PATTERN_NAME=""

# Mapear números a nombres de patrones
case $PATTERN_NUM in
    01) PATTERN_NAME="manipulacion-caracteres" ;;
    02) PATTERN_NAME="procesamiento-palabras" ;;
    03) PATTERN_NAME="conversion-tipos" ;;
    04) PATTERN_NAME="algoritmos-busqueda" ;;
    05) PATTERN_NAME="operaciones-conjuntos" ;;
    06) PATTERN_NAME="memoria-punteros" ;;
    07) PATTERN_NAME="algoritmos-matematicos" ;;
    08) PATTERN_NAME="listas-enlazadas" ;;
    09) PATTERN_NAME="algoritmos-ordenacion" ;;
    10) PATTERN_NAME="recursividad-flood-fill" ;;
    *) 
        echo -e "${RED}❌ Patrón no válido: $PATTERN_NUM${NC}"
        exit 1
        ;;
esac

PATTERN_DIR="patterns/${PATTERN_NUM}-${PATTERN_NAME}"
PROGRESS_FILE="progress/pattern_${PATTERN_NUM}.txt"

# Función para mostrar el header del patrón
show_pattern_header() {
    clear
    echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}█                           PATRÓN ${PATTERN_NUM}: $(echo $PATTERN_NAME | tr '-' ' ' | tr '[:lower:]' '[:upper:]')                     █${NC}"
    echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
    echo ""
}

# Función para mostrar la explicación del patrón
show_pattern_explanation() {
    explanation_file="explanations/pattern_${PATTERN_NUM}.md"
    if [[ -f "$explanation_file" ]]; then
        echo -e "${BLUE}📚 Explicación del Patrón:${NC}"
        echo ""
        cat "$explanation_file"
        echo ""
    else
        echo -e "${YELLOW}⚠️  Explicación del patrón no disponible aún.${NC}"
        echo ""
    fi
}

# Función para obtener ejercicios del patrón
get_pattern_exercises() {
    local pattern_exercises=()
    
    case $PATTERN_NUM in
        01)
            pattern_exercises=("ft_putstr" "ft_swap" "rot_13" "rotone" "ulstr" "alpha_mirror" "print_bits" "reverse_bits" "swap_bits" "is_power_of_2")
            ;;
        02)
            pattern_exercises=("first_word" "repeat_alpha" "rev_print" "search_and_replace" "last_word" "camel_to_snake" "snake_to_camel" "epur_str" "expand_str" "str_capitalizer" "rstr_capitalizer" "rostring" "rev_wstr")
            ;;
        03)
            pattern_exercises=("ft_strlen" "ft_strcpy" "ft_atoi" "do_op" "print_hex" "ft_strdup" "ft_atoi_base" "ft_range" "ft_rrange" "ft_itoa" "ft_split")
            ;;
        04)
            pattern_exercises=("ft_strcspn" "ft_strspn" "ft_strpbrk" "wdmatch" "max" "hidenp" "paramsum" "tab_mult")
            ;;
        05)
            pattern_exercises=("union" "inter")
            ;;
        06)
            pattern_exercises=("ft_strdup" "ft_strrev" "ft_range" "ft_rrange" "ft_split")
            ;;
        07)
            pattern_exercises=("add_prime_sum" "lcm" "pgcd" "fprime")
            ;;
        08)
            pattern_exercises=("ft_list_size" "ft_list_foreach" "ft_list_remove_if" "sort_list")
            ;;
        09)
            pattern_exercises=("sort_int_tab" "sort_list")
            ;;
        10)
            pattern_exercises=("flood_fill")
            ;;
    esac
    
    echo "${pattern_exercises[@]}"
}

# Función para inicializar el progreso del patrón
init_pattern_progress() {
    if [[ ! -f "$PROGRESS_FILE" ]]; then
        local exercises=($(get_pattern_exercises))
        for exercise in "${exercises[@]}"; do
            echo "${exercise}:pending" >> "$PROGRESS_FILE"
        done
    fi
}

# Función para marcar ejercicio como completado
mark_exercise_completed() {
    local exercise="$1"
    sed -i.bak "s/${exercise}:pending/${exercise}:completed/" "$PROGRESS_FILE" && rm "${PROGRESS_FILE}.bak"
}

# Función para verificar si un ejercicio está completado
is_exercise_completed() {
    local exercise="$1"
    grep -q "${exercise}:completed" "$PROGRESS_FILE" 2>/dev/null
}

# Función para mostrar ejercicios del patrón
show_pattern_exercises() {
    local exercises=($(get_pattern_exercises))
    local completed=0
    local total=${#exercises[@]}
    
    echo -e "${BLUE}📋 Ejercicios en este patrón (${total} total):${NC}"
    echo ""
    
    for i in "${!exercises[@]}"; do
        local exercise="${exercises[$i]}"
        local num=$((i + 1))
        
        if is_exercise_completed "$exercise"; then
            echo -e "  ${GREEN}✓${NC} ${num}. ${exercise} ${GREEN}(Completado)${NC}"
            ((completed++))
        else
            echo -e "  ${YELLOW}○${NC} ${num}. ${exercise}"
        fi
    done
    
    echo ""
    echo -e "${CYAN}Progreso del patrón: ${completed}/${total} ($(( completed * 100 / total ))%)${NC}"
    echo ""
}

# Función para practicar un ejercicio específico
practice_exercise() {
    local exercise="$1"
    local exercise_path=""
    
    # Buscar el ejercicio en los niveles originales
    for level in {1..4}; do
        if [[ -d "../02/Level${level}/${exercise}" ]]; then
            exercise_path="../02/Level${level}/${exercise}"
            break
        fi
    done
    
    if [[ -z "$exercise_path" ]]; then
        echo -e "${RED}❌ No se pudo encontrar el ejercicio: $exercise${NC}"
        return 1
    fi
    
    echo -e "${CYAN}🚀 Practicando: $exercise${NC}"
    echo ""
    
    # Mostrar el enunciado del ejercicio
    if [[ -f "${exercise_path}/subject.en.txt" ]]; then
        echo -e "${BLUE}📜 Enunciado:${NC}"
        cat "${exercise_path}/subject.en.txt"
        echo ""
    fi
    
    # Crear directorio de trabajo temporal
    mkdir -p "temp_practice"
    cd "temp_practice"
    
    # Copiar archivos necesarios
    cp -r "$exercise_path"/* . 2>/dev/null
    
    echo -e "${YELLOW}💡 Archivos disponibles:${NC}"
    ls -la
    echo ""
    
    echo -e "${GREEN}Puedes trabajar aquí. Cuando termines:${NC}"
    echo -e "  ${YELLOW}c${NC}) Compilar y probar"
    echo -e "  ${YELLOW}s${NC}) Marcar como completado y salir"
    echo -e "  ${YELLOW}q${NC}) Salir sin guardar"
    echo ""
    
    while true; do
        echo -n "¿Qué quieres hacer? "
        read -r action
        
        case $action in
            c|C)
                echo -e "${BLUE}🔨 Compilando...${NC}"
                if [[ -f "*.c" ]]; then
                    gcc -Wall -Wextra -Werror *.c -o test_program
                    if [[ $? -eq 0 ]]; then
                        echo -e "${GREEN}✓ Compilación exitosa${NC}"
                        if [[ -f "test_program" ]]; then
                            echo -e "${BLUE}Ejecutando programa...${NC}"
                            ./test_program
                        fi
                    else
                        echo -e "${RED}❌ Error de compilación${NC}"
                    fi
                else
                    echo -e "${YELLOW}⚠️  No se encontraron archivos .c${NC}"
                fi
                ;;
            s|S)
                mark_exercise_completed "$exercise"
                echo -e "${GREEN}✓ Ejercicio marcado como completado${NC}"
                cd ..
                rm -rf "temp_practice"
                return 0
                ;;
            q|Q)
                echo -e "${YELLOW}Saliendo sin guardar...${NC}"
                cd ..
                rm -rf "temp_practice"
                return 0
                ;;
            *)
                echo -e "${RED}Opción no válida${NC}"
                ;;
        esac
    done
}

# Función principal del menú del patrón
pattern_menu() {
    init_pattern_progress
    
    while true; do
        show_pattern_header
        show_pattern_explanation
        show_pattern_exercises
        
        echo -e "${GREEN}¿Qué quieres hacer?${NC}"
        echo ""
        echo -e "  ${YELLOW}1-N${NC})  Practicar ejercicio específico (número)"
        echo -e "  ${YELLOW}n${NC})    Practicar siguiente ejercicio no completado"
        echo -e "  ${YELLOW}a${NC})    Mostrar explicación del patrón completa"
        echo -e "  ${YELLOW}r${NC})    Reiniciar progreso de este patrón"
        echo -e "  ${YELLOW}b${NC})    Volver al menú principal"
        echo ""
        echo -n "Tu elección: "
        
        read -r choice
        
        case $choice in
            [1-9]|[1-9][0-9])
                local exercises=($(get_pattern_exercises))
                local exercise_index=$((choice - 1))
                
                if [[ $exercise_index -ge 0 && $exercise_index -lt ${#exercises[@]} ]]; then
                    practice_exercise "${exercises[$exercise_index]}"
                else
                    echo -e "${RED}❌ Número de ejercicio no válido${NC}"
                    read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                fi
                ;;
            n|N)
                local exercises=($(get_pattern_exercises))
                local found_next=false
                
                for exercise in "${exercises[@]}"; do
                    if ! is_exercise_completed "$exercise"; then
                        practice_exercise "$exercise"
                        found_next=true
                        break
                    fi
                done
                
                if [[ "$found_next" == false ]]; then
                    echo -e "${GREEN}🎉 ¡Felicidades! Has completado todos los ejercicios de este patrón${NC}"
                    read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                fi
                ;;
            a|A)
                show_pattern_explanation
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            r|R)
                echo ""
                echo -e "${RED}¿Estás seguro de que quieres reiniciar el progreso de este patrón? [y/N]${NC}"
                read -r confirm
                if [[ "$confirm" == "y" || "$confirm" == "Y" ]]; then
                    rm -f "$PROGRESS_FILE"
                    init_pattern_progress
                    echo -e "${GREEN}✓ Progreso del patrón reiniciado${NC}"
                else
                    echo -e "${YELLOW}Operación cancelada${NC}"
                fi
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            b|B)
                return 0
                ;;
            *)
                echo ""
                echo -e "${RED}❌ Opción no válida${NC}"
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
        esac
    done
}

# Verificar si estamos en el directorio correcto
if [[ ! -f "exercise_patterns.conf" ]]; then
    echo -e "${RED}❌ Error: No se encuentra el archivo de configuración.${NC}"
    echo -e "${YELLOW}Asegúrate de ejecutar este script desde el directorio extra/${NC}"
    exit 1
fi

# Crear directorio de progreso si no existe
mkdir -p progress

# Iniciar el menú del patrón
pattern_menu
