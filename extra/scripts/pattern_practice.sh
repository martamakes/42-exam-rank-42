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
    11) PATTERN_NAME="operaciones-bitwise" ;;
    12) PATTERN_NAME="uso-de-flags" ;;
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
        clear
        echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
        echo -e "${CYAN}█                     EXPLICACIÓN DEL PATRÓN ${PATTERN_NUM}                                █${NC}"
        echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
        echo ""
        cat "$explanation_file"
        echo ""
        echo -e "${BLUE}Presiona cualquier tecla para volver al menú...${NC}"
        read -n 1 -s -r
    else
        echo -e "${YELLOW}⚠️  Explicación del patrón no disponible aún.${NC}"
        echo ""
    fi
}

# Función para mostrar enunciado del ejercicio y dar opciones
show_exercise_subject() {
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
    
    while true; do
        clear
        echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
        echo -e "${CYAN}█                           EJERCICIO: $exercise${NC}"
        echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
        echo ""
        
        # Mostrar el enunciado del ejercicio
        if [[ -f "${exercise_path}/README.md" ]]; then
            echo -e "${BLUE}📜 ENUNCIADO:${NC}"
            echo -e "${CYAN}$(head -1 "${exercise_path}/README.md" | sed 's/# //')${NC}"
            echo ""
            
            # Leer todo el contenido del README
            local readme_content=$(cat "${exercise_path}/README.md")
            
            # Extraer información clave
            local assignment_name=$(echo "$readme_content" | grep "Assignment name" | cut -d: -f2 | xargs)
            local expected_files=$(echo "$readme_content" | grep "Expected files" | cut -d: -f2 | xargs)
            local allowed_functions=$(echo "$readme_content" | grep "Allowed functions" | cut -d: -f2 | xargs)
            
            echo -e "${YELLOW}Assignment:${NC} $assignment_name"
            echo -e "${YELLOW}Files needed:${NC} $expected_files"
            echo -e "${YELLOW}Allowed functions:${NC} $allowed_functions"
            echo -e "${YELLOW}$(printf '%*s' 80 '' | tr ' ' '-')${NC}"
            echo ""
            
            # Mostrar la descripción completa
            echo -e "${WHITE}DESCRIPCIÓN:${NC}"
            local description_start=false
            local in_examples=false
            local empty_line_count=0
            
            while IFS= read -r line; do
                # Detectar inicio de descripción (línea con guiones)
                if [[ "$line" =~ ^-+$ ]]; then
                    description_start=true
                    continue
                fi
                
                # Si encontramos "Examples:" o "Example:", cambiamos de modo
                if [[ "$line" =~ ^Examples?: ]]; then
                    in_examples=true
                    echo ""
                    echo -e "${GREEN}📋 EJEMPLOS:${NC}"
                    continue
                fi
                
                # Mostrar contenido según el modo
                if [[ "$description_start" == true && "$in_examples" == false ]]; then
                    # Estamos en la descripción - mostrar TODAS las líneas incluyendo vacías
                    # pero limitar líneas vacías consecutivas a máximo 2
                    if [[ -z "$line" ]]; then
                        empty_line_count=$((empty_line_count + 1))
                        if [[ $empty_line_count -le 2 ]]; then
                            echo "$line"
                        fi
                    else
                        empty_line_count=0
                        echo "$line"
                    fi
                elif [[ "$in_examples" == true ]]; then
                    # Estamos en los ejemplos
                    echo "$line"
                fi
            done < "${exercise_path}/README.md"
            
            echo ""
        else
            echo -e "${YELLOW}⚠️  No se encontró el archivo README.md${NC}"
            echo ""
        fi
        
        # Verificar estado del directorio rendu
        echo -e "${BLUE}📁 ESTADO DEL EJERCICIO:${NC}"
        local exercise_dir="../rendu/$exercise"
        
        if [[ -d "$exercise_dir" ]]; then
            echo -e "${GREEN}✓ Directorio creado: $exercise_dir/${NC}"
            
            # Mostrar archivos en el directorio
            local files_in_dir=$(ls -la "$exercise_dir" 2>/dev/null | grep -v "^d" | awk '{print $9}' | grep -v "^$" | grep -v "^\.")
            if [[ -n "$files_in_dir" ]]; then
                echo -e "${GREEN}📄 Archivos encontrados:${NC}"
                echo "$files_in_dir" | while read -r file; do
                    echo -e "  • $file"
                done
            else
                echo -e "${YELLOW}⚠️  Directorio vacío - necesitas crear: $expected_files${NC}"
            fi
        else
            echo -e "${YELLOW}⚠️  Directorio no creado: $exercise_dir/${NC}"
            echo -e "${CYAN}💡 Necesitas crear: mkdir -p $exercise_dir${NC}"
        fi
        
        echo ""
        echo -e "${GREEN}¿Qué quieres hacer?${NC}"
        echo -e "  ${YELLOW}c${NC}) Crear directorio y compilar/probar"
        echo -e "  ${YELLOW}t${NC}) Solo compilar y probar (si ya tienes archivos)"
        echo -e "  ${YELLOW}s${NC}) Marcar como completado"
        echo -e "  ${YELLOW}e${NC}) Ver explicación del patrón"
        echo -e "  ${YELLOW}r${NC}) Abrir directorio en terminal"
        echo -e "  ${YELLOW}b${NC}) Volver al menú del patrón"
        echo ""
        echo -n "Tu elección: "
        
        read -r choice
        case $choice in
            c|C)
                create_and_test_exercise "$exercise" "$exercise_path" "$expected_files"
                ;;
            t|T)
                test_exercise "$exercise" "$exercise_path"
                ;;
            s|S)
                mark_exercise_completed "$exercise"
                echo -e "${GREEN}✓ Ejercicio marcado como completado${NC}"
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                return 0
                ;;
            e|E)
                show_pattern_explanation
                ;;
            r|R)
                open_exercise_directory "$exercise"
                ;;
            b|B)
                return 0
                ;;
            *)
                echo -e "${RED}❌ Opción no válida${NC}"
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
        esac
    done
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
        11)
            pattern_exercises=("print_bits" "reverse_bits" "swap_bits" "is_power_of_2")
            ;;
        12)
            pattern_exercises=("wdmatch" "union" "inter" "hidenp" "paramsum" "str_capitalizer")
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

# Función para mostrar descripción breve del patrón
show_pattern_brief() {
    case $PATTERN_NUM in
        01)
            echo -e "${BLUE}🔤 Conceptos clave:${NC} Manipulación ASCII, ROT13, operaciones de bits, potencias de 2"
            ;;
        02)
            echo -e "${BLUE}📝 Conceptos clave:${NC} Navegación por palabras, camelCase/snake_case, capitalización"
            ;;
        03)
            echo -e "${BLUE}🔄 Conceptos clave:${NC} atoi/itoa, conversiones de base, ft_split, gestión de arrays"
            ;;
        04)
            echo -e "${BLUE}🔍 Conceptos clave:${NC} Búsqueda de subcadenas, algoritmos de coincidencia"
            ;;
        05)
            echo -e "${BLUE}📊 Conceptos clave:${NC} Operaciones de conjuntos, unión e intersección"
            ;;
        06)
            echo -e "${BLUE}💾 Conceptos clave:${NC} malloc/free, gestión de memoria, arrays dinámicos"
            ;;
        07)
            echo -e "${BLUE}🧮 Conceptos clave:${NC} Números primos, MCM, MCD, factorización"
            ;;
        08)
            echo -e "${BLUE}🔗 Conceptos clave:${NC} Navegación de listas, modificación, aplicación de funciones"
            ;;
        09)
            echo -e "${BLUE}📈 Conceptos clave:${NC} Bubble sort, ordenación de arrays y listas"
            ;;
        10)
            echo -e "${BLUE}🌊 Conceptos clave:${NC} Recursividad, flood fill, algoritmos de inundación"
            ;;
        11)
            echo -e "${BLUE}🔢 Conceptos clave:${NC} Operaciones AND, OR, XOR, shifts, máscaras de bits"
            ;;
        12)
            echo -e "${BLUE}🚩 Conceptos clave:${NC} Estados booleanos, flags combinados, optimización de memoria"
            ;;
    esac
    echo ""
}

# Función para crear directorio y probar ejercicio
create_and_test_exercise() {
    local exercise="$1"
    local exercise_path="$2"
    local expected_files="$3"
    local exercise_dir="../rendu/$exercise"
    
    echo -e "${BLUE}🏗️  Creando directorio de trabajo...${NC}"
    
    # Crear directorio si no existe
    mkdir -p "$exercise_dir"
    
    echo -e "${GREEN}✓ Directorio creado: $exercise_dir${NC}"
    echo -e "${CYAN}💡 Necesitas crear estos archivos: $expected_files${NC}"
    echo -e "${CYAN}📍 Ubicación: $(pwd)/$exercise_dir${NC}"
    echo ""
    
    # Abrir el directorio en otra terminal si es posible
    if command -v osascript >/dev/null 2>&1; then
        echo -e "${BLUE}🖥️  Abriendo nueva terminal en el directorio...${NC}"
        osascript -e "tell application \"Terminal\" to do script \"cd '$(pwd)/$exercise_dir' && echo 'Directorio para $exercise - Crea: $expected_files' && bash\""
    elif command -v gnome-terminal >/dev/null 2>&1; then
        gnome-terminal --working-directory="$(pwd)/$exercise_dir" &
    fi
    
    echo -e "${YELLOW}Cuando hayas creado tus archivos, presiona cualquier tecla para probar...${NC}"
    read -n 1 -s -r
    
    test_exercise "$exercise" "$exercise_path"
}

# Función para probar ejercicio
test_exercise() {
    local exercise="$1"
    local exercise_path="$2"
    local exercise_dir="../rendu/$exercise"
    
    if [[ ! -d "$exercise_dir" ]]; then
        echo -e "${RED}❌ No existe el directorio $exercise_dir${NC}"
        echo -e "${CYAN}Usa la opción 'c' para crearlo primero${NC}"
        read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
        return 1
    fi
    
    echo -e "${BLUE}🔨 Compilando y probando desde $exercise_dir...${NC}"
    echo ""
    
    # Verificar si existen tests en grademe
    if [[ -d "$exercise_path/grademe" ]]; then
        echo -e "${CYAN}🧪 Tests automáticos disponibles - ejecutando batería completa...${NC}"
        echo ""
        
        # Copiar y ejecutar tests
        cd "$exercise_path/grademe"
        
        if [[ -f "test.sh" ]]; then
            # Modificar script para usar nuestro directorio rendu
            cp test.sh test_exam.sh
            
            # Cambiar las rutas para usar nuestro directorio rendu
            local full_rendu_path=$(cd "$(dirname "$0")" && pwd)
            full_rendu_path="$full_rendu_path/../rendu/$exercise"
            
            sed -i.bak "s|STUDENT_DIR=\"../../../../rendu/\$EXERCISE\"|STUDENT_DIR=\"$full_rendu_path\"|g" test_exam.sh
            
            chmod +x test_exam.sh
            echo -e "${YELLOW}========== RESULTADOS DE TESTS ==========${NC}"
            
            if ./test_exam.sh; then
                echo -e "${GREEN}✅ ¡Todos los tests pasaron correctamente!${NC}"
            else
                echo -e "${RED}❌ Algunos tests fallaron. Revisa tu código.${NC}"
            fi
            echo -e "${YELLOW}=========================================${NC}"
            
            # Limpiar archivos temporales
            rm -f test_exam.sh test_exam.sh.bak
        else
            echo -e "${YELLOW}⚠️  No se encontró test.sh${NC}"
        fi
        
        cd - > /dev/null
    else
        echo -e "${BLUE}🔨 Compilación manual (sin tests automáticos)...${NC}"
        
        cd "$exercise_dir"
        
        c_files=$(find . -name "*.c" -type f -maxdepth 1)
        if [[ -n "$c_files" ]]; then
            echo -e "${BLUE}Compilando: $c_files${NC}"
            
            if gcc -Wall -Wextra -Werror $c_files -o test_program 2>&1; then
                echo -e "${GREEN}✓ Compilación exitosa${NC}"
                
                if [[ -f "test_program" ]]; then
                    echo -e "${BLUE}Ejecutando programa...${NC}"
                    echo -e "${YELLOW}========== OUTPUT ==========${NC}"
                    ./test_program
                    echo -e "${YELLOW}============================${NC}"
                fi
            else
                echo -e "${RED}❌ Error de compilación${NC}"
            fi
        else
            echo -e "${YELLOW}⚠️  No se encontraron archivos .c en $exercise_dir${NC}"
        fi
        
        cd - > /dev/null
    fi
    
    echo ""
    read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
}

# Función para abrir directorio del ejercicio
open_exercise_directory() {
    local exercise="$1"
    local exercise_dir="../rendu/$exercise"
    
    # Crear directorio si no existe
    if [[ ! -d "$exercise_dir" ]]; then
        mkdir -p "$exercise_dir"
        echo -e "${GREEN}✓ Directorio creado: $exercise_dir${NC}"
    fi
    
    echo -e "${BLUE}📁 Abriendo directorio del ejercicio...${NC}"
    echo -e "${CYAN}📍 Ubicación: $(pwd)/$exercise_dir${NC}"
    
    # Abrir el directorio según el sistema
    if command -v osascript >/dev/null 2>&1; then
        # macOS - abrir nueva terminal
        osascript -e "tell application \"Terminal\" to do script \"cd '$(pwd)/$exercise_dir' && echo 'Directorio de trabajo para: $exercise' && ls -la && bash\""
        echo -e "${GREEN}✓ Nueva terminal abierta${NC}"
    elif command -v gnome-terminal >/dev/null 2>&1; then
        # Linux - GNOME
        gnome-terminal --working-directory="$(pwd)/$exercise_dir" &
        echo -e "${GREEN}✓ Nueva terminal abierta${NC}"
    elif command -v xterm >/dev/null 2>&1; then
        # Linux - xterm
        cd "$exercise_dir" && xterm &
        cd - > /dev/null
        echo -e "${GREEN}✓ Nueva terminal abierta${NC}"
    else
        echo -e "${YELLOW}💡 Navega manualmente a: $(pwd)/$exercise_dir${NC}"
    fi
    
    read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
}
# Función principal del menú del patrón
pattern_menu() {
    init_pattern_progress
    
    while true; do
        show_pattern_header
        show_pattern_brief
        show_pattern_exercises
        
        echo -e "${GREEN}¿Qué quieres hacer?${NC}"
        echo ""
        echo -e "  ${YELLOW}1-N${NC})  Ver enunciado y practicar ejercicio específico (número)"
        echo -e "  ${YELLOW}n${NC})    Practicar siguiente ejercicio no completado"
        echo -e "  ${YELLOW}e${NC})    Ver explicación completa del patrón"
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
                    show_exercise_subject "${exercises[$exercise_index]}"
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
                        show_exercise_subject "$exercise"
                        found_next=true
                        break
                    fi
                done
                
                if [[ "$found_next" == false ]]; then
                    echo -e "${GREEN}🎉 ¡Felicidades! Has completado todos los ejercicios de este patrón${NC}"
                    read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                fi
                ;;
            e|E)
                show_pattern_explanation
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
