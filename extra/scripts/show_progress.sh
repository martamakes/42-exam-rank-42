#!/bin/bash

# Script para mostrar progreso detallado

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

show_header() {
    clear
    echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}█                            PROGRESO DETALLADO                               █${NC}"
    echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
    echo ""
}

show_overall_stats() {
    local total_exercises=0
    local completed_exercises=0
    local patterns_started=0
    local patterns_completed=0
    
    # Contabilizar estadísticas generales
    for pattern in {01..12}; do
        pattern_file="progress/pattern_${pattern}.txt"
        if [[ -f "$pattern_file" ]]; then
            ((patterns_started++))
            local pattern_total=0
            local pattern_completed=0
            
            while IFS= read -r line; do
                [[ -n "$line" ]] && ((total_exercises++)) && ((pattern_total++))
                [[ "$line" == *":completed"* ]] && ((completed_exercises++)) && ((pattern_completed++))
            done < "$pattern_file"
            
            [[ $pattern_completed -eq $pattern_total && $pattern_total -gt 0 ]] && ((patterns_completed++))
        fi
    done
    
    if [[ $total_exercises -eq 0 ]]; then
        total_exercises=1  # Evitar división por cero
    fi
    
    local percentage=$((completed_exercises * 100 / total_exercises))
    
    echo -e "${BLUE}📊 Estadísticas Generales:${NC}"
    echo -e "  🎯 Ejercicios completados: ${GREEN}$completed_exercises${NC}/${total_exercises} (${percentage}%)"
    echo -e "  📚 Patrones iniciados: ${CYAN}$patterns_started${NC}/12"
    echo -e "  ✅ Patrones completados: ${GREEN}$patterns_completed${NC}/12"
    echo ""
    
    # Barra de progreso general
    local filled=$((percentage / 5))
    local empty=$((20 - filled))
    printf "Progreso General: ["
    printf "%*s" $filled | tr ' ' '█'
    printf "%*s" $empty | tr ' ' '░'
    printf "] %d%%\n" $percentage
    echo ""
}

show_pattern_progress() {
    echo -e "${BLUE}📋 Progreso por Patrón:${NC}"
    echo ""
    
    patterns=(
        "01:Manipulación básica de caracteres:🔤"
        "02:Procesamiento de cadenas palabra por palabra:📝"
        "03:Funciones de conversión de tipos:🔄"
        "04:Algoritmos de búsqueda:🔍"
        "05:Operaciones con conjuntos:📊"
        "06:Manipulación de memoria y punteros:💾"
        "07:Algoritmos matemáticos:🧮"
        "08:Manipulación de listas enlazadas:🔗"
        "09:Algoritmos de ordenación:📈"
        "10:Recursividad y algoritmos de inundación:🌊"
        "11:Operaciones Bitwise (Manipulación de bits):🔢"
        "12:Uso de Flags (Banderas de estado):🚩"
    )
    
    for pattern in "${patterns[@]}"; do
        IFS=':' read -r num name emoji <<< "$pattern"
        
        pattern_file="progress/pattern_${num}.txt"
        completed=0
        total=0
        exercises_list=()
        
        if [[ -f "$pattern_file" ]]; then
            while IFS=':' read -r exercise status; do
                [[ -n "$exercise" ]] && ((total++))
                exercises_list+=("$exercise:$status")
                [[ "$status" == "completed" ]] && ((completed++))
            done < "$pattern_file"
        fi
        
        # Determinar estado y color
        if [[ $total -eq 0 ]]; then
            status_color="${YELLOW}"
            status="No iniciado"
            progress_bar=""
        elif [[ $completed -eq $total ]]; then
            status_color="${GREEN}"
            status="✓ Completado"
            progress_bar="████████████████████ 100%"
        else
            status_color="${CYAN}"
            status="En progreso"
            local percentage=$((completed * 100 / total))
            local filled=$((percentage / 5))
            local empty=$((20 - filled))
            progress_bar=""
            for ((i=0; i<filled; i++)); do progress_bar+="█"; done
            for ((i=0; i<empty; i++)); do progress_bar+="░"; done
            progress_bar+=" ${percentage}%"
        fi
        
        printf "  ${PURPLE}%s${NC} %s %-40s ${status_color}%s${NC}\n" "$emoji" "$num." "$name" "$status"
        
        if [[ $total -gt 0 ]]; then
            printf "     Progreso: [%s] (%d/%d ejercicios)\n" "$progress_bar" "$completed" "$total"
            
            # Mostrar ejercicios si el patrón está en progreso
            if [[ $completed -gt 0 && $completed -lt $total ]]; then
                printf "     Completados: "
                local first=true
                for exercise_info in "${exercises_list[@]}"; do
                    IFS=':' read -r exercise status <<< "$exercise_info"
                    if [[ "$status" == "completed" ]]; then
                        [[ "$first" == false ]] && printf ", "
                        printf "${GREEN}%s${NC}" "$exercise"
                        first=false
                    fi
                done
                echo ""
            fi
        fi
        echo ""
    done
}

show_recommendations() {
    echo -e "${BLUE}💡 Recomendaciones:${NC}"
    echo ""
    
    # Encontrar próximo patrón recomendado
    local next_pattern=""
    local next_exercise=""
    
    for pattern in {01..12}; do
        pattern_file="progress/pattern_${pattern}.txt"
        if [[ -f "$pattern_file" ]]; then
            while IFS=':' read -r exercise status; do
                if [[ "$status" == "pending" ]]; then
                    next_pattern="$pattern"
                    next_exercise="$exercise"
                    break 2
                fi
            done < "$pattern_file"
        else
            next_pattern="01"
            break
        fi
    done
    
    if [[ -n "$next_pattern" ]]; then
        if [[ -n "$next_exercise" ]]; then
            echo -e "  🎯 ${GREEN}Continúa con:${NC} Patrón $next_pattern, ejercicio ${YELLOW}$next_exercise${NC}"
        else
            echo -e "  🎯 ${GREEN}Empieza con:${NC} Patrón $next_pattern"
        fi
        
        # Consejos basados en el patrón
        case $next_pattern in
            01)
                echo -e "  💡 ${CYAN}Consejo:${NC} Domina la manipulación básica de caracteres antes de avanzar"
                ;;
            02)
                echo -e "  💡 ${CYAN}Consejo:${NC} Aprende a procesar strings palabra por palabra - muy común en exámenes"
                ;;
            03)
                echo -e "  💡 ${CYAN}Consejo:${NC} Las conversiones de tipos son fundamentales para muchos ejercicios"
                ;;
            04)
                echo -e "  💡 ${CYAN}Consejo:${NC} Los algoritmos de búsqueda aparecen frecuentemente en exámenes"
                ;;
            05)
                echo -e "  💡 ${CYAN}Consejo:${NC} Operaciones con conjuntos - practica union e inter muy bien"
                ;;
            06)
                echo -e "  💡 ${CYAN}Consejo:${NC} Gestión de memoria - cuidado con malloc/free"
                ;;
            07)
                echo -e "  💡 ${CYAN}Consejo:${NC} Algoritmos matemáticos - repasa números primos y MCM/MCD"
                ;;
            08)
                echo -e "  💡 ${CYAN}Consejo:${NC} Listas enlazadas - domina la manipulación de punteros"
                ;;
            09)
                echo -e "  💡 ${CYAN}Consejo:${NC} Algoritmos de ordenación - bubble sort es tu amigo"
                ;;
            10)
                echo -e "  💡 ${CYAN}Consejo:${NC} Recursividad avanzada - piensa en casos base y recursivos"
                ;;
        esac
    else
        echo -e "  🎉 ${GREEN}¡Felicidades! Has completado todos los patrones${NC}"
        echo -e "  🚀 ${CYAN}Estás listo para el examen - practica con ejercicios aleatorios${NC}"
    fi
    
    echo ""
}

show_time_estimate() {
    local total_exercises=0
    local completed_exercises=0
    
    # Contar ejercicios
    for pattern in {01..10}; do
        pattern_file="progress/pattern_${pattern}.txt"
        if [[ -f "$pattern_file" ]]; then
            while IFS= read -r line; do
                [[ -n "$line" ]] && ((total_exercises++))
                [[ "$line" == *":completed"* ]] && ((completed_exercises++))
            done < "$pattern_file"
        fi
    done
    
    local remaining=$((total_exercises - completed_exercises))
    
    if [[ $remaining -gt 0 ]]; then
        # Estimación: 30 minutos por ejercicio promedio
        local hours=$((remaining * 30 / 60))
        local minutes=$((remaining * 30 % 60))
        
        echo -e "${BLUE}⏱️  Estimación de tiempo restante:${NC}"
        echo -e "  📚 Ejercicios restantes: ${YELLOW}$remaining${NC}"
        echo -e "  ⏰ Tiempo estimado: ${CYAN}~${hours}h ${minutes}m${NC} (30 min/ejercicio promedio)"
        echo ""
    fi
}

# Función principal
show_progress() {
    show_header
    show_overall_stats
    show_pattern_progress
    show_recommendations
    show_time_estimate
}

# Verificar si estamos en el directorio correcto
if [[ ! -f "exercise_patterns.conf" ]]; then
    echo -e "${RED}❌ Error: No se encuentra el archivo de configuración.${NC}"
    echo -e "${YELLOW}Asegúrate de ejecutar este script desde el directorio extra/${NC}"
    exit 1
fi

# Mostrar progreso
show_progress
