#!/bin/bash

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[0;37m'
NC='\033[0m' # No Color

# Función para mostrar el header
show_header() {
    clear
    echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}█                    SISTEMA DE PRÁCTICA POR PATRONES - 42 SCHOOL             █${NC}"
    echo -e "${CYAN}█                          Optimizado para Examen Rank 02                     █${NC}"
    echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
    echo ""
    echo -e "${GREEN}🎯 Aprende por conceptos, no por niveles arbitrarios${NC}"
    echo -e "${BLUE}📚 Progresión científica basada en patrones de programación${NC}"
    echo ""
}

# Función para mostrar el progreso general
show_overall_progress() {
    total_exercises=0
    completed_exercises=0
    
    # Contar ejercicios totales y completados
    for pattern in {01..12}; do
        pattern_file="progress/pattern_${pattern}.txt"
        if [[ -f "$pattern_file" ]]; then
            while IFS= read -r line; do
                [[ -n "$line" ]] && ((total_exercises++))
                [[ "$line" == *":completed"* ]] && ((completed_exercises++))
            done < "$pattern_file"
        fi
    done
    
    if [[ $total_exercises -eq 0 ]]; then
        total_exercises=1  # Evitar división por cero
    fi
    
    percentage=$((completed_exercises * 100 / total_exercises))
    
    echo -e "${YELLOW}📊 Progreso General: ${completed_exercises}/${total_exercises} ejercicios (${percentage}%)${NC}"
    
    # Barra de progreso visual
    filled=$((percentage / 5))
    empty=$((20 - filled))
    printf "Progress: ["
    printf "%*s" $filled | tr ' ' '█'
    printf "%*s" $empty | tr ' ' '░'
    printf "] %d%%\n" $percentage
    echo ""
}

# Función para mostrar los patrones disponibles
show_patterns() {
    echo -e "${BLUE}📋 Patrones Conceptuales Disponibles:${NC}"
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
        
        # Verificar progreso del patrón
        pattern_file="progress/pattern_${num}.txt"
        completed=0
        total=0
        
        if [[ -f "$pattern_file" ]]; then
            while IFS= read -r line; do
                [[ -n "$line" ]] && ((total++))
                [[ "$line" == *":completed"* ]] && ((completed++))
            done < "$pattern_file"
        fi
        
        if [[ $total -eq 0 ]]; then
            status_color="${YELLOW}"
            status="No iniciado"
        elif [[ $completed -eq $total ]]; then
            status_color="${GREEN}"
            status="✓ Completado"
        else
            status_color="${CYAN}"
            status="En progreso (${completed}/${total})"
        fi
        
        printf "  ${PURPLE}%s${NC} %s %-40s ${status_color}%s${NC}\n" "$emoji" "$num." "$name" "$status"
    done
    echo ""
}

# Función para el menú principal
main_menu() {
    while true; do
        show_header
        show_overall_progress
        show_patterns
        
        echo -e "${GREEN}¿Qué quieres hacer?${NC}"
        echo ""
        echo -e "  ${YELLOW}1-12${NC}) Practicar un patrón específico"
        echo -e "  ${YELLOW}p${NC})    Ver progreso detallado"
        echo -e "  ${YELLOW}s${NC})    Estado del sistema"
        echo -e "  ${YELLOW}r${NC})    Reiniciar progreso (¡CUIDADO!)"
        echo -e "  ${YELLOW}h${NC})    Ayuda y explicaciones"
        echo -e "  ${YELLOW}q${NC})    Salir"
        echo ""
        echo -n "Tu elección: "
        
        read -r choice
        
        case $choice in
            [1-9]|1[0-2])
                pattern_num=$(printf "%02d" $choice)
                ./scripts/pattern_practice.sh "${pattern_num}"
                ;;
            p|P)
                ./scripts/show_progress.sh
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            s|S)
                ./scripts/system_status.sh
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            r|R)
                echo ""
                echo -e "${RED}¿Estás seguro de que quieres reiniciar TODO el progreso? [y/N]${NC}"
                read -r confirm
                if [[ "$confirm" == "y" || "$confirm" == "Y" ]]; then
                    rm -f progress/*.txt
                    echo -e "${GREEN}✓ Progreso reiniciado${NC}"
                else
                    echo -e "${YELLOW}Operación cancelada${NC}"
                fi
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            h|H)
                ./scripts/show_help.sh
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            q|Q)
                echo ""
                echo -e "${GREEN}¡Hasta luego! Que tengas una buena práctica 🚀${NC}"
                echo ""
                exit 0
                ;;
            *)
                echo ""
                echo -e "${RED}❌ Opción no válida. Por favor elige una opción del menú.${NC}"
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

# Iniciar el menú principal
main_menu
