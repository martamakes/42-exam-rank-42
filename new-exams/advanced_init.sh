#!/bin/bash

# 🎓 ADVANCED EXAM PRACTICE SYSTEM
# Sistema mejorado con tracking de progreso, selección aleatoria y progresión automática

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
PURPLE='\033[0;35m'
BOLD='\033[1m'
GRAY='\033[0;37m'

# Directorio base
NEW_EXAMS_DIR="$(cd "$(dirname "$0")" && pwd)"
PROGRESS_DIR="$NEW_EXAMS_DIR/.progress"

# Crear directorio de progreso si no existe
mkdir -p "$PROGRESS_DIR"

# Archivos de progreso para cada exam rank y nivel
get_progress_file() {
    local rank=$1
    local level=$2
    echo "$PROGRESS_DIR/exam_rank_${rank}_level_${level}_completed.txt"
}

get_current_level_file() {
    local rank=$1
    echo "$PROGRESS_DIR/exam_rank_${rank}_current_level.txt"
}

get_stats_file() {
    local rank=$1
    echo "$PROGRESS_DIR/exam_rank_${rank}_stats.txt"
}

# Función para mostrar el banner avanzado
show_advanced_banner() {
    clear
    echo -e "${PURPLE}${BOLD}"
    echo "╔══════════════════════════════════════════════════════════════════════╗"
    echo "║                    🎓 ADVANCED EXAM PRACTICE SYSTEM 🎓              ║"
    echo "║                     Sistema con Tracking de Progreso                ║"
    echo "║                                                                      ║"
    echo "║  ✨ Selección Aleatoria  📊 Progreso Persistente  🎯 Auto-Progresión ║"
    echo "╚══════════════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
    echo ""
}

# Función para obtener todos los ejercicios de un nivel
get_exercises() {
    local rank=$1
    local level=$2
    local exam_dir="$NEW_EXAMS_DIR/exam-rank-$rank/level-$level"
    
    if [ -d "$exam_dir" ]; then
        find "$exam_dir" -mindepth 1 -maxdepth 1 -type d -exec basename {} \; | sort
    fi
}

# Función para obtener ejercicios completados
get_completed_exercises() {
    local rank=$1
    local level=$2
    local progress_file=$(get_progress_file $rank $level)
    
    if [ -f "$progress_file" ]; then
        cat "$progress_file" | sort
    fi
}

# Función para obtener ejercicios restantes (no completados)
get_remaining_exercises() {
    local rank=$1
    local level=$2
    
    local all_exercises=$(get_exercises $rank $level)
    local completed_exercises=$(get_completed_exercises $rank $level)
    
    # Filtrar ejercicios no completados
    comm -23 <(echo "$all_exercises") <(echo "$completed_exercises")
}

# Función para marcar ejercicio como completado
mark_exercise_completed() {
    local rank=$1
    local level=$2
    local exercise=$3
    local progress_file=$(get_progress_file $rank $level)
    
    # Añadir ejercicio a la lista de completados si no existe
    if ! grep -q "^$exercise$" "$progress_file" 2>/dev/null; then
        echo "$exercise" >> "$progress_file"
        sort -o "$progress_file" "$progress_file"
    fi
    
    # Actualizar estadísticas
    update_stats $rank $level $exercise
}

# Función para actualizar estadísticas
update_stats() {
    local rank=$1
    local level=$2
    local exercise=$3
    local stats_file=$(get_stats_file $rank)
    
    # Crear archivo de stats si no existe
    if [ ! -f "$stats_file" ]; then
        echo "exercises_completed=0" > "$stats_file"
        echo "total_time=0" >> "$stats_file"
        echo "last_session=$(date)" >> "$stats_file"
    fi
    
    # Incrementar contador de ejercicios completados
    local completed_count=$(grep "exercises_completed=" "$stats_file" | cut -d'=' -f2)
    ((completed_count++))
    
    # Actualizar archivo
    sed -i.bak "s/exercises_completed=.*/exercises_completed=$completed_count/" "$stats_file"
    sed -i.bak "s/last_session=.*/last_session=$(date)/" "$stats_file"
    rm -f "$stats_file.bak"
}

# Función para obtener nivel actual
get_current_level() {
    local rank=$1
    local level_file=$(get_current_level_file $rank)
    
    if [ -f "$level_file" ]; then
        cat "$level_file"
    else
        echo "1"  # Nivel por defecto
    fi
}

# Función para establecer nivel actual
set_current_level() {
    local rank=$1
    local level=$2
    local level_file=$(get_current_level_file $rank)
    
    echo "$level" > "$level_file"
}

# Función para mostrar estadísticas detalladas
show_detailed_stats() {
    local rank=$1
    local stats_file=$(get_stats_file $rank)
    
    echo -e "${BLUE}=== ESTADÍSTICAS DETALLADAS - EXAM RANK $rank ===${NC}\n"
    
    if [ ! -f "$stats_file" ]; then
        echo -e "${YELLOW}No hay estadísticas registradas para este exam rank${NC}"
        return
    fi
    
    local exercises_completed=$(grep "exercises_completed=" "$stats_file" | cut -d'=' -f2)
    local last_session=$(grep "last_session=" "$stats_file" | cut -d'=' -f2)
    local current_level=$(get_current_level $rank)
    
    echo -e "${CYAN}📊 Resumen General:${NC}"
    echo -e "   🎯 Ejercicios completados: ${GREEN}$exercises_completed${NC}"
    echo -e "   📋 Nivel actual: ${GREEN}$current_level${NC}"
    echo -e "   🕒 Última sesión: ${GRAY}$last_session${NC}"
    echo ""
    
    echo -e "${CYAN}📈 Progreso por Niveles:${NC}"
    
    # Mostrar progreso de cada nivel
    for level in {1..2}; do
        local all_ex=$(get_exercises $rank $level | wc -l)
        local completed_ex=$(get_completed_exercises $rank $level | wc -l)
        local remaining_ex=$(get_remaining_exercises $rank $level | wc -l)
        
        if [ $all_ex -gt 0 ]; then
            local percentage=$(( (completed_ex * 100) / all_ex ))
            local status_icon="🔴"
            [ $percentage -eq 100 ] && status_icon="✅"
            [ $percentage -gt 0 ] && [ $percentage -lt 100 ] && status_icon="🟡"
            
            echo -e "   Level $level: $status_icon $completed_ex/$all_ex ejercicios (${percentage}%)"
            
            if [ $completed_ex -gt 0 ]; then
                echo -e "      ${GREEN}Completados:${NC} $(get_completed_exercises $rank $level | tr '\n' ', ' | sed 's/,$//')"
            fi
            
            if [ $remaining_ex -gt 0 ]; then
                echo -e "      ${YELLOW}Pendientes:${NC} $(get_remaining_exercises $rank $level | tr '\n' ', ' | sed 's/,$//')"
            fi
            echo ""
        fi
    done
}

# Función para seleccionar ejercicio aleatorio
select_random_exercise() {
    local rank=$1
    local level=$2
    local remaining_exercises=$(get_remaining_exercises $rank $level)
    
    if [ -z "$remaining_exercises" ]; then
        return 1  # No hay ejercicios restantes
    fi
    
    # Seleccionar aleatorio de los restantes
    echo "$remaining_exercises" | shuf | head -n1
}

# Función para avanzar automáticamente de nivel
auto_advance_level() {
    local rank=$1
    local current_level=$(get_current_level $rank)
    local remaining=$(get_remaining_exercises $rank $current_level)
    
    if [ -z "$remaining" ]; then
        # Nivel completado, avanzar al siguiente si existe
        local next_level=$((current_level + 1))
        local next_exercises=$(get_exercises $rank $next_level)
        
        if [ -n "$next_exercises" ]; then
            set_current_level $rank $next_level
            echo -e "${GREEN}🎉 ¡Nivel $current_level completado! Avanzando a nivel $next_level${NC}"
            return 0
        else
            echo -e "${GREEN}🏆 ¡FELICITACIONES! Has completado todos los niveles del Exam Rank $rank${NC}"
            return 2
        fi
    fi
    
    return 1  # No hay avance de nivel
}

# Función para práctica aleatoria
random_practice() {
    local rank=$1
    
    clear
    echo -e "${BLUE}=== PRÁCTICA ALEATORIA - EXAM RANK $rank ===${NC}\n"
    
    while true; do
        local current_level=$(get_current_level $rank)
        local random_exercise=$(select_random_exercise $rank $current_level)
        
        if [ -z "$random_exercise" ]; then
            # Intentar avanzar de nivel automáticamente
            local advance_result=$(auto_advance_level $rank)
            if [ $? -eq 2 ]; then
                # Todos los niveles completados
                break
            elif [ $? -eq 0 ]; then
                # Se avanzó de nivel, intentar de nuevo
                continue
            else
                echo -e "${YELLOW}No hay más ejercicios disponibles en este exam rank${NC}"
                break
            fi
        fi
        
        echo -e "${CYAN}🎲 Ejercicio aleatorio seleccionado:${NC} ${GREEN}$random_exercise${NC}"
        echo -e "${CYAN}📋 Nivel:${NC} $current_level"
        echo -e "${CYAN}📁 Ubicación:${NC} exam-rank-$rank/level-$current_level/$random_exercise/"
        echo ""
        
        show_exercise_progress $rank $current_level
        echo ""
        
        echo -e "${YELLOW}Opciones:${NC}"
        echo "1. ✅ Completar este ejercicio y continuar"
        echo "2. ⏭️  Saltar a otro ejercicio aleatorio"
        echo "3. 📚 Ver subject del ejercicio"
        echo "4. 📁 Crear directorio de trabajo (rendu/$random_exercise)"
        echo "5. 📊 Ver estadísticas detalladas"
        echo "6. 🚪 Volver al menú principal"
        echo ""
        
        read -p "Selecciona una opción: " choice
        
        case $choice in
            1)
                mark_exercise_completed $rank $current_level $random_exercise
                echo -e "${GREEN}✅ Ejercicio $random_exercise marcado como completado${NC}"
                
                # Verificar si se debe avanzar de nivel
                auto_advance_level $rank
                
                echo ""
                read -p "Presiona Enter para continuar con el siguiente ejercicio..."
                clear
                echo -e "${BLUE}=== PRÁCTICA ALEATORIA - EXAM RANK $rank ===${NC}\n"
                ;;
            2)
                echo -e "${CYAN}🎲 Buscando otro ejercicio aleatorio...${NC}"
                clear
                echo -e "${BLUE}=== PRÁCTICA ALEATORIA - EXAM RANK $rank ===${NC}\n"
                ;;
            3)
                echo -e "${CYAN}📚 Subject del ejercicio $random_exercise:${NC}"
                echo ""
                local subject_path="exam-rank-$rank/level-$current_level/$random_exercise/subject.txt"
                if [ -f "$subject_path" ]; then
                    cat "$subject_path"
                else
                    echo -e "${RED}Subject no encontrado en: $subject_path${NC}"
                fi
                echo ""
                read -p "Presiona Enter para continuar..."
                clear
                echo -e "${BLUE}=== PRÁCTICA ALEATORIA - EXAM RANK $rank ===${NC}\n"
                ;;
            4)
                echo -e "${CYAN}📁 Creando directorio de trabajo...${NC}"
                mkdir -p "rendu/$random_exercise"
                if [ -d "rendu/$random_exercise" ]; then
                    echo -e "${GREEN}✅ Directorio creado: rendu/$random_exercise/${NC}"
                    echo -e "${CYAN}💡 Tip: Programa tus archivos .c y .h en este directorio${NC}"
                else
                    echo -e "${RED}❌ Error al crear el directorio${NC}"
                fi
                echo ""
                read -p "Presiona Enter para continuar..."
                clear
                echo -e "${BLUE}=== PRÁCTICA ALEATORIA - EXAM RANK $rank ===${NC}\n"
                ;;
            5)
                show_detailed_stats $rank
                read -p "Presiona Enter para continuar..."
                clear
                echo -e "${BLUE}=== PRÁCTICA ALEATORIA - EXAM RANK $rank ===${NC}\n"
                ;;
            6)
                return
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                ;;
        esac
    done
    
    read -p "Presiona Enter para continuar..."
}

# Función para mostrar progreso de un nivel
show_exercise_progress() {
    local rank=$1
    local level=$2
    
    local all_ex=$(get_exercises $rank $level | wc -l)
    local completed_ex=$(get_completed_exercises $rank $level | wc -l)
    local remaining_ex=$(get_remaining_exercises $rank $level | wc -l)
    
    if [ $all_ex -gt 0 ]; then
        local percentage=$(( (completed_ex * 100) / all_ex ))
        echo -e "${CYAN}📈 Progreso del Nivel $level:${NC} $completed_ex/$all_ex (${percentage}%) - ${remaining_ex} restantes"
    fi
}

# Función para selección manual de ejercicios
manual_exercise_selection() {
    local rank=$1
    
    clear
    echo -e "${BLUE}=== SELECCIÓN MANUAL DE EJERCICIOS - EXAM RANK $rank ===${NC}\n"
    
    # Seleccionar nivel
    echo -e "${CYAN}Selecciona un nivel:${NC}"
    local available_levels=()
    for level in {1..2}; do
        local exercises=$(get_exercises $rank $level)
        if [ -n "$exercises" ]; then
            available_levels+=($level)
            local all_ex=$(echo "$exercises" | wc -l)
            local completed_ex=$(get_completed_exercises $rank $level | wc -l)
            echo "$level. Level $level ($completed_ex/$all_ex completados)"
        fi
    done
    echo "0. Volver al menú principal"
    echo ""
    
    read -p "Selecciona un nivel: " level_choice
    
    if [ "$level_choice" = "0" ]; then
        return
    fi
    
    if [[ ! " ${available_levels[@]} " =~ " ${level_choice} " ]]; then
        echo -e "${RED}Nivel inválido${NC}"
        read -p "Presiona Enter para continuar..."
        return
    fi
    
    while true; do
        clear
        echo -e "${BLUE}=== EJERCICIOS DEL NIVEL $level_choice ===${NC}\n"
        
        show_exercise_progress $rank $level_choice
        echo ""
        
        local exercises=$(get_exercises $rank $level_choice)
        local completed_exercises=$(get_completed_exercises $rank $level_choice)
        
        echo -e "${CYAN}Selecciona un ejercicio:${NC}"
        local counter=1
        local exercise_list=()
        
        for exercise in $exercises; do
            local status="⭕"  # Pendiente
            if echo "$completed_exercises" | grep -q "^$exercise$"; then
                status="✅"  # Completado
            fi
            echo "$counter. $status $exercise"
            exercise_list+=("$exercise")
            ((counter++))
        done
        
        echo "0. Volver al menú anterior"
        echo ""
        
        read -p "Selecciona un ejercicio: " ex_choice
        
        if [ "$ex_choice" = "0" ]; then
            break
        fi
        
        if [[ "$ex_choice" =~ ^[0-9]+$ ]] && [ "$ex_choice" -ge 1 ] && [ "$ex_choice" -le ${#exercise_list[@]} ]; then
            local selected_exercise="${exercise_list[$((ex_choice-1))]}"
            
            echo ""
            echo -e "${CYAN}📚 Ejercicio seleccionado:${NC} ${GREEN}$selected_exercise${NC}"
            echo -e "${CYAN}📁 Ubicación:${NC} exam-rank-$rank/level-$level_choice/$selected_exercise/"
            
            # Verificar si está completado
            if echo "$completed_exercises" | grep -q "^$selected_exercise$"; then
                echo -e "${GREEN}✅ Este ejercicio ya está completado${NC}"
            else
                echo -e "${YELLOW}⭕ Este ejercicio está pendiente${NC}"
            fi
            
            echo ""
            echo -e "${CYAN}💡 Para programar:${NC} Crea tu solución en ${GREEN}rendu/$selected_exercise/${NC}"
            echo ""
            echo -e "${YELLOW}Opciones:${NC}"
            echo "1. ✅ Marcar como completado"
            echo "2. ❌ Marcar como no completado"
            echo "3. 📚 Ver subject del ejercicio"
            echo "4. 📁 Crear directorio de trabajo (rendu/$selected_exercise)"
            echo "5. 🧪 Ejecutar tests (grademe)"
            echo "6. 🚪 Volver a la lista"
            echo ""
            
            read -p "Selecciona una opción: " action
            
            case $action in
                1)
                    mark_exercise_completed $rank $level_choice $selected_exercise
                    echo -e "${GREEN}✅ Ejercicio marcado como completado${NC}"
                    read -p "Presiona Enter para continuar..."
                    ;;
                2)
                    local progress_file=$(get_progress_file $rank $level_choice)
                    if [ -f "$progress_file" ]; then
                        grep -v "^$selected_exercise$" "$progress_file" > "$progress_file.tmp"
                        mv "$progress_file.tmp" "$progress_file"
                        echo -e "${YELLOW}❌ Ejercicio marcado como no completado${NC}"
                    fi
                    read -p "Presiona Enter para continuar..."
                    ;;
                3)
                    echo -e "${CYAN}📚 Subject del ejercicio $selected_exercise:${NC}"
                    echo ""
                    local subject_path="exam-rank-$rank/level-$level_choice/$selected_exercise/subject.txt"
                    if [ -f "$subject_path" ]; then
                        cat "$subject_path"
                    else
                        echo -e "${RED}Subject no encontrado en: $subject_path${NC}"
                    fi
                    echo ""
                    read -p "Presiona Enter para continuar..."
                    ;;
                4)
                    echo -e "${CYAN}📁 Creando directorio de trabajo...${NC}"
                    mkdir -p "rendu/$selected_exercise"
                    if [ -d "rendu/$selected_exercise" ]; then
                        echo -e "${GREEN}✅ Directorio creado: rendu/$selected_exercise/${NC}"
                        echo -e "${CYAN}💡 Tip: Programa tus archivos .c y .h en este directorio${NC}"
                    else
                        echo -e "${RED}❌ Error al crear el directorio${NC}"
                    fi
                    echo ""
                    read -p "Presiona Enter para continuar..."
                    ;;
                5)
                    # Ejecutar tests (grademe)
                    echo -e "${CYAN}🧪 Ejecutando tests para $selected_exercise...${NC}"
                    echo ""
                    
                    local exercise_dir="$NEW_EXAMS_DIR/exam-rank-$rank/level-$level_choice/$selected_exercise"
                    local grademe_dir="$exercise_dir/grademe"
                    local student_dir="$NEW_EXAMS_DIR/rendu/$selected_exercise"
                    local student_file="$student_dir/$selected_exercise.c"
                    local test_script="$grademe_dir/test.sh"
                    
                    # Verificar que existe el directorio de tests
                    if [ ! -d "$grademe_dir" ]; then
                        echo -e "${RED}❌ Error: No se encuentran los tests para $selected_exercise${NC}"
                        echo -e "${YELLOW}📁 Ruta esperada: $grademe_dir${NC}"
                        read -p "Presiona Enter para continuar..."
                        continue
                    fi
                    
                    if [ ! -f "$test_script" ]; then
                        echo -e "${RED}❌ Error: No se encuentra el script de tests${NC}"
                        echo -e "${YELLOW}📁 Archivo esperado: $test_script${NC}"
                        read -p "Presiona Enter para continuar..."
                        continue
                    fi
                    
                    if [ ! -f "$student_file" ]; then
                        echo -e "${RED}❌ Error: No se encuentra tu solución en $student_file${NC}"
                        echo -e "${YELLOW}💡 Tip: Crea el archivo $selected_exercise.c en el directorio rendu/$selected_exercise/${NC}"
                        mkdir -p "$student_dir"
                        read -p "Presiona Enter para continuar..."
                        continue
                    fi
                    
                    # Dar permisos de ejecución al script de test
                    chmod +x "$test_script"
                    
                    # Ejecutar tests
                    echo -e "${BLUE}🚀 Ejecutando tests...${NC}"
                    if ! cd "$grademe_dir"; then
                        echo -e "${RED}❌ Error: No se puede acceder al directorio de tests${NC}"
                        read -p "Presiona Enter para continuar..."
                        continue
                    fi
                    
                    ./test.sh
                    local result=$?
                    cd - > /dev/null
                    
                    echo ""
                    if [ $result -eq 0 ]; then
                        echo -e "${GREEN}✅ ¡Todos los tests han pasado correctamente!${NC}"
                        echo -e "${CYAN}¿Quieres marcar este ejercicio como completado? (s/n):${NC}"
                        read -r mark_complete
                        if [[ "$mark_complete" =~ ^[Ss]$ ]]; then
                            mark_exercise_completed $rank $level_choice $selected_exercise
                            echo -e "${GREEN}🎉 Ejercicio marcado como completado${NC}"
                        fi
                    else
                        echo -e "${RED}❌ Algunos tests han fallado. El ejercicio no está completamente correcto.${NC}"
                        echo -e "${YELLOW}💡 Revisa tu código y vuelve a intentarlo${NC}"
                    fi
                    
                    read -p "Presiona Enter para continuar..."
                    ;;
                6)
                    continue
                    ;;
                *)
                    echo -e "${RED}Opción inválida${NC}"
                    read -p "Presiona Enter para continuar..."
                    ;;
            esac
        else
            echo -e "${RED}Selección inválida${NC}"
            read -p "Presiona Enter para continuar..."
        fi
    done
}

# Función para hacer todos los ejercicios de un nivel
complete_full_level() {
    local rank=$1
    
    clear
    echo -e "${BLUE}=== COMPLETAR NIVEL COMPLETO - EXAM RANK $rank ===${NC}\n"
    
    # Seleccionar nivel
    echo -e "${CYAN}Selecciona un nivel para completar:${NC}"
    local available_levels=()
    for level in {1..2}; do
        local exercises=$(get_exercises $rank $level)
        if [ -n "$exercises" ]; then
            available_levels+=($level)
            local all_ex=$(echo "$exercises" | wc -l)
            local completed_ex=$(get_completed_exercises $rank $level | wc -l)
            local remaining_ex=$((all_ex - completed_ex))
            echo "$level. Level $level ($remaining_ex ejercicios restantes de $all_ex)"
        fi
    done
    echo "0. Volver al menú principal"
    echo ""
    
    read -p "Selecciona un nivel: " level_choice
    
    if [ "$level_choice" = "0" ]; then
        return
    fi
    
    if [[ ! " ${available_levels[@]} " =~ " ${level_choice} " ]]; then
        echo -e "${RED}Nivel inválido${NC}"
        read -p "Presiona Enter para continuar..."
        return
    fi
    
    local remaining_exercises=$(get_remaining_exercises $rank $level_choice)
    
    if [ -z "$remaining_exercises" ]; then
        echo -e "${GREEN}✅ El nivel $level_choice ya está completado${NC}"
        read -p "Presiona Enter para continuar..."
        return
    fi
    
    echo ""
    echo -e "${CYAN}📋 Ejercicios que se marcarán como completados:${NC}"
    echo "$remaining_exercises" | nl
    echo ""
    
    read -p "¿Confirmas que quieres marcar todos estos ejercicios como completados? (s/n): " confirm
    
    if [[ "$confirm" =~ ^[Ss]$ ]]; then
        local count=0
        for exercise in $remaining_exercises; do
            mark_exercise_completed $rank $level_choice $exercise
            ((count++))
            echo -e "${GREEN}✅ $exercise completado${NC}"
        done
        
        echo ""
        echo -e "${GREEN}🎉 ¡Nivel $level_choice completado! $count ejercicios marcados como completados${NC}"
        
        # Actualizar nivel actual si es necesario
        local current_level=$(get_current_level $rank)
        if [ $level_choice -ge $current_level ]; then
            set_current_level $rank $((level_choice + 1))
        fi
    else
        echo -e "${YELLOW}Operación cancelada${NC}"
    fi
    
    read -p "Presiona Enter para continuar..."
}

# Función para resetear progreso
reset_progress() {
    local rank=$1
    
    clear
    echo -e "${RED}=== RESETEAR PROGRESO - EXAM RANK $rank ===${NC}\n"
    
    echo -e "${YELLOW}⚠️  ATENCIÓN: Esta acción eliminará todo el progreso guardado${NC}"
    echo -e "${CYAN}Se eliminará:${NC}"
    echo -e "• Ejercicios completados de todos los niveles"
    echo -e "• Estadísticas acumuladas"
    echo -e "• Progreso de nivel actual"
    echo ""
    
    show_detailed_stats $rank
    echo ""
    
    echo -e "${RED}${BOLD}¿Estás SEGURO de que quieres resetear TODO el progreso?${NC}"
    read -p "Escribe 'RESET' para confirmar: " confirmation
    
    if [ "$confirmation" = "RESET" ]; then
        # Eliminar archivos de progreso para este rank
        rm -f "$PROGRESS_DIR"/exam_rank_${rank}_*.txt
        
        echo ""
        echo -e "${GREEN}✅ Progreso reseteado completamente${NC}"
        echo -e "${CYAN}El Exam Rank $rank ahora está como si fuera la primera vez${NC}"
    else
        echo ""
        echo -e "${YELLOW}Operación cancelada - el progreso se mantiene intacto${NC}"
    fi
    
    read -p "Presiona Enter para continuar..."
}

# Función para mostrar el menú de un exam rank específico
show_exam_rank_menu() {
    local rank=$1
    
    while true; do
        clear
        echo -e "${PURPLE}${BOLD}"
        echo "╔══════════════════════════════════════════════════════════════════════╗"
        echo "║                        EXAM RANK $rank - MENÚ AVANZADO                       ║"
        echo "╚══════════════════════════════════════════════════════════════════════╝"
        echo -e "${NC}"
        echo ""
        
        show_detailed_stats $rank
        echo ""
        
        echo -e "${YELLOW}=== OPCIONES DISPONIBLES ===${NC}"
        echo ""
        echo "1. 🎲 Práctica aleatoria (recomendado)"
        echo "2. 📝 Selección manual de ejercicios"
        echo "3. 🎯 Completar nivel completo"
        echo "4. 📊 Ver estadísticas detalladas"
        echo "5. 🔄 Resetear progreso"
        echo "6. 🚀 Usar EXAMSHELL tradicional"
        echo "7. 🚪 Volver al menú principal"
        echo ""
        
        read -p "Selecciona una opción: " choice
        
        case $choice in
            1)
                random_practice $rank
                ;;
            2)
                manual_exercise_selection $rank
                ;;
            3)
                complete_full_level $rank
                ;;
            4)
                show_detailed_stats $rank
                read -p "Presiona Enter para continuar..."
                ;;
            5)
                reset_progress $rank
                ;;
            6)
                echo -e "${GREEN}🚀 Iniciando EXAMSHELL tradicional...${NC}"
                if [ -f "$NEW_EXAMS_DIR/examshell" ]; then
                    cd "$NEW_EXAMS_DIR"
                    ./examshell $rank
                else
                    echo -e "${RED}Error: examshell no compilado. Ejecuta 'make' primero${NC}"
                    read -p "Presiona Enter para continuar..."
                fi
                ;;
            7)
                return
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                read -p "Presiona Enter para continuar..."
                ;;
        esac
    done
}

# Menú principal mejorado
advanced_main_menu() {
    while true; do
        show_advanced_banner
        
        # Mostrar resumen rápido de progreso
        echo -e "${BLUE}=== RESUMEN RÁPIDO DE PROGRESO ===${NC}"
        
        for rank in 03 04 05; do
            if [ -d "$NEW_EXAMS_DIR/exam-rank-$rank" ]; then
                local stats_file=$(get_stats_file $rank)
                local exercises_completed=0
                local current_level=$(get_current_level $rank)
                
                if [ -f "$stats_file" ]; then
                    exercises_completed=$(grep "exercises_completed=" "$stats_file" | cut -d'=' -f2)
                fi
                
                echo -e "${CYAN}📚 Exam Rank $rank:${NC} $exercises_completed ejercicios completados, nivel actual: $current_level"
            fi
        done
        
        echo ""
        echo -e "${YELLOW}=== MENÚ PRINCIPAL AVANZADO ===${NC}"
        echo ""
        echo "1. 📚 Acceder a Exam Rank 03 (Intermediate)"
        echo "2. 📚 Acceder a Exam Rank 04 (Advanced)"  
        echo "3. 📚 Acceder a Exam Rank 05 (Expert)"
        echo "4. 📊 Ver estadísticas globales"
        echo "5. 🔧 Compilar EXAMSHELL"
        echo "6. ❓ Ayuda del sistema avanzado"
        echo "7. 🚪 Salir"
        echo ""
        
        read -p "Selecciona una opción: " choice
        
        case $choice in
            1)
                if [ -d "$NEW_EXAMS_DIR/exam-rank-03" ]; then
                    show_exam_rank_menu "03"
                else
                    echo -e "${RED}Error: Exam Rank 03 no encontrado${NC}"
                    read -p "Presiona Enter para continuar..."
                fi
                ;;
            2)
                if [ -d "$NEW_EXAMS_DIR/exam-rank-04" ]; then
                    show_exam_rank_menu "04"
                else
                    echo -e "${RED}Error: Exam Rank 04 no encontrado${NC}"
                    read -p "Presiona Enter para continuar..."
                fi
                ;;
            3)
                if [ -d "$NEW_EXAMS_DIR/exam-rank-05" ]; then
                    show_exam_rank_menu "05"
                else
                    echo -e "${RED}Error: Exam Rank 05 no encontrado${NC}"
                    read -p "Presiona Enter para continuar..."
                fi
                ;;
            4)
                clear
                echo -e "${BLUE}=== ESTADÍSTICAS GLOBALES ===${NC}\n"
                for rank in 03 04 05; do
                    if [ -d "$NEW_EXAMS_DIR/exam-rank-$rank" ]; then
                        show_detailed_stats $rank
                        echo ""
                    fi
                done
                read -p "Presiona Enter para continuar..."
                ;;
            5)
                clear
                echo -e "${BLUE}=== COMPILAR EXAMSHELL ===${NC}\n"
                
                if [ -f "$NEW_EXAMS_DIR/examshell" ]; then
                    echo -e "${GREEN}✅ EXAMSHELL ya está compilado${NC}"
                    echo -e "${YELLOW}¿Quieres recompilarlo? (s/n):${NC}"
                    read -r response
                    
                    if [[ ! "$response" =~ ^[Ss]$ ]]; then
                        continue
                    fi
                fi
                
                echo -e "${CYAN}Compilando EXAMSHELL...${NC}"
                
                if make; then
                    echo -e "\n${GREEN}✅ EXAMSHELL compilado exitosamente!${NC}"
                    echo -e "${CYAN}Ahora puedes usar tanto el sistema avanzado como el EXAMSHELL tradicional${NC}"
                else
                    echo -e "\n${RED}❌ Error al compilar EXAMSHELL${NC}"
                    echo -e "${YELLOW}Verifica que tengas GCC instalado y permisos de escritura${NC}"
                fi
                
                read -p "Presiona Enter para continuar..."
                ;;
            6)
                clear
                echo -e "${BLUE}=== AYUDA DEL SISTEMA AVANZADO ===${NC}\n"
                
                echo -e "${CYAN}🎯 Nuevas Funcionalidades:${NC}"
                echo -e "• ${GREEN}Práctica aleatoria:${NC} Ejercicios aleatorios sin repetición"
                echo -e "• ${GREEN}Tracking de progreso:${NC} Se guardan los ejercicios completados"
                echo -e "• ${GREEN}Progresión automática:${NC} Avanza de nivel automáticamente"
                echo -e "• ${GREEN}Selección manual:${NC} Elige ejercicios específicos"
                echo -e "• ${GREEN}Completar nivel:${NC} Marca todo un nivel como completado"
                echo -e "• ${GREEN}Reset de progreso:${NC} Reinicia el progreso guardado"
                echo ""
                
                echo -e "${CYAN}📁 Archivos de Progreso:${NC}"
                echo -e "• Ubicación: ${GRAY}$PROGRESS_DIR/${NC}"
                echo -e "• Se crean automáticamente al usar el sistema"
                echo -e "• Persisten entre sesiones"
                echo ""
                
                echo -e "${CYAN}🎮 Modo de Uso Recomendado:${NC}"
                echo -e "1. Usa ${GREEN}Práctica aleatoria${NC} para experiencia de examen real"
                echo -e "2. Los ejercicios se seleccionan aleatoriamente sin repetir"
                echo -e "3. Progresa automáticamente de nivel al completar todos"
                echo -e "4. Usa ${GREEN}Selección manual${NC} para ejercicios específicos"
                echo -e "5. ${GREEN}EXAMSHELL tradicional${NC} sigue disponible sin cambios"
                echo ""
                
                read -p "Presiona Enter para continuar..."
                ;;
            7)
                echo -e "${GREEN}¡Hasta luego! ¡Que tengas una excelente práctica! 🎓${NC}"
                exit 0
                ;;
            *)
                echo -e "${RED}Opción inválida. Por favor selecciona un número del 1 al 7.${NC}"
                read -p "Presiona Enter para continuar..."
                ;;
        esac
    done
}

# Verificar que estamos en el directorio correcto
if [ ! -d "exam-rank-03" ] && [ ! -d "exam-rank-04" ] && [ ! -d "exam-rank-05" ]; then
    echo -e "${RED}Error: No se encuentran los directorios de exam ranks${NC}"
    echo -e "${YELLOW}Asegúrate de ejecutar este script desde el directorio new-exams${NC}"
    exit 1
fi

# Hacer ejecutable
chmod +x "$0"

# Iniciar el menú principal mejorado
advanced_main_menu