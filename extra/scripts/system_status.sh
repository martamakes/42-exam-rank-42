#!/bin/bash

# Script para mostrar estado del sistema

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
    echo -e "${CYAN}█                            ESTADO DEL SISTEMA                               █${NC}"
    echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
    echo ""
}

check_system_integrity() {
    echo -e "${BLUE}🔍 Verificación de Integridad del Sistema:${NC}"
    echo ""
    
    local issues=0
    
    # Verificar estructura de directorios
    echo -e "📁 Estructura de directorios:"
    
    required_dirs=("patterns" "progress" "explanations" "scripts")
    for dir in "${required_dirs[@]}"; do
        if [[ -d "$dir" ]]; then
            echo -e "  ${GREEN}✓${NC} $dir/"
        else
            echo -e "  ${RED}❌${NC} $dir/ (¡Falta!)"
            ((issues++))
        fi
    done
    
    # Verificar archivos de configuración
    echo ""
    echo -e "📄 Archivos de configuración:"
    
    required_files=("exercise_patterns.conf" "Makefile")
    for file in "${required_files[@]}"; do
        if [[ -f "$file" ]]; then
            echo -e "  ${GREEN}✓${NC} $file"
        else
            echo -e "  ${RED}❌${NC} $file (¡Falta!)"
            ((issues++))
        fi
    done
    
    # Verificar scripts
    echo ""
    echo -e "🔧 Scripts del sistema:"
    
    required_scripts=("main_menu.sh" "pattern_practice.sh" "setup_links.sh" "show_progress.sh" "system_status.sh")
    for script in "${required_scripts[@]}"; do
        script_path="scripts/$script"
        if [[ -f "$script_path" ]]; then
            if [[ -x "$script_path" ]]; then
                echo -e "  ${GREEN}✓${NC} $script (ejecutable)"
            else
                echo -e "  ${YELLOW}⚠️${NC}  $script (no ejecutable)"
                chmod +x "$script_path" 2>/dev/null && echo -e "     ${GREEN}→ Permisos corregidos${NC}"
            fi
        else
            echo -e "  ${RED}❌${NC} $script (¡Falta!)"
            ((issues++))
        fi
    done
    
    # Verificar enlaces simbólicos
    echo ""
    echo -e "🔗 Enlaces simbólicos:"
    
    if [[ -d "links" ]]; then
        local broken_links=0
        local total_links=0
        
        for link in links/*; do
            if [[ -L "$link" ]]; then
                ((total_links++))
                if [[ ! -e "$link" ]]; then
                    ((broken_links++))
                fi
            fi
        done
        
        if [[ $total_links -eq 0 ]]; then
            echo -e "  ${YELLOW}⚠️${NC}  Sin enlaces configurados (ejecuta 'make setup')"
        elif [[ $broken_links -eq 0 ]]; then
            echo -e "  ${GREEN}✓${NC} $total_links enlaces válidos"
        else
            echo -e "  ${RED}❌${NC} $broken_links/$total_links enlaces rotos"
            ((issues++))
        fi
    else
        echo -e "  ${YELLOW}⚠️${NC}  Directorio 'links' no existe"
    fi
    
    echo ""
    if [[ $issues -eq 0 ]]; then
        echo -e "${GREEN}✅ Sistema en perfecto estado${NC}"
    else
        echo -e "${RED}⚠️  Se encontraron $issues problemas${NC}"
        echo -e "${YELLOW}💡 Ejecuta 'make setup' para corregir problemas comunes${NC}"
    fi
    echo ""
}

show_file_statistics() {
    echo -e "${BLUE}📊 Estadísticas de Archivos:${NC}"
    echo ""
    
    # Contar archivos por tipo
    local progress_files=$(find progress -name "*.txt" 2>/dev/null | wc -l)
    local explanation_files=$(find explanations -name "*.md" 2>/dev/null | wc -l)
    local pattern_dirs=$(find patterns -maxdepth 1 -type d 2>/dev/null | grep -c "^patterns/[0-9]")
    
    echo -e "  📁 Directorios de patrones: ${CYAN}$pattern_dirs${NC}/10"
    echo -e "  📈 Archivos de progreso: ${CYAN}$progress_files${NC}/10"
    echo -e "  📚 Archivos de explicaciones: ${CYAN}$explanation_files${NC}/10"
    
    # Tamaño total del directorio
    if command -v du >/dev/null 2>&1; then
        local total_size=$(du -sh . 2>/dev/null | cut -f1)
        echo -e "  💾 Tamaño total: ${CYAN}$total_size${NC}"
    fi
    
    echo ""
}

show_exercise_coverage() {
    echo -e "${BLUE}📋 Cobertura de Ejercicios:${NC}"
    echo ""
    
    # Lista de todos los ejercicios posibles
    local all_exercises=(
        "ft_putstr" "ft_swap" "ft_strlen" "ft_strcpy" "first_word" "fizzbuzz" 
        "repeat_alpha" "rev_print" "rot_13" "rotone" "search_and_replace" "ulstr"
        "alpha_mirror" "camel_to_snake" "do_op" "ft_atoi" "ft_strcmp" "ft_strcspn"
        "ft_strdup" "ft_strpbrk" "ft_strrev" "ft_strspn" "inter" "is_power_of_2"
        "last_word" "max" "print_bits" "reverse_bits" "snake_to_camel" "swap_bits"
        "union" "wdmatch" "add_prime_sum" "epur_str" "expand_str" "ft_atoi_base"
        "ft_list_size" "ft_range" "ft_rrange" "hidenp" "lcm" "paramsum" "pgcd"
        "print_hex" "rstr_capitalizer" "str_capitalizer" "tab_mult" "flood_fill"
        "fprime" "ft_itoa" "ft_list_foreach" "ft_list_remove_if" "ft_split"
        "rev_wstr" "rostring" "sort_int_tab" "sort_list"
    )
    
    local covered_exercises=0
    local available_exercises=0
    
    # Verificar cuántos ejercicios están cubiertos en el sistema por patrones
    for exercise in "${all_exercises[@]}"; do
        ((available_exercises++))
        
        # Buscar en archivos de progreso
        if grep -q "$exercise" progress/*.txt 2>/dev/null; then
            ((covered_exercises++))
        fi
    done
    
    local coverage_percentage=$((covered_exercises * 100 / available_exercises))
    
    echo -e "  🎯 Ejercicios cubiertos: ${GREEN}$covered_exercises${NC}/$available_exercises (${coverage_percentage}%)"
    
    # Barra de progreso
    local filled=$((coverage_percentage / 5))
    local empty=$((20 - filled))
    printf "  Cobertura: ["
    printf "%*s" $filled | tr ' ' '█'
    printf "%*s" $empty | tr ' ' '░'
    printf "] %d%%\n" $coverage_percentage
    
    echo ""
}

show_system_recommendations() {
    echo -e "${BLUE}💡 Recomendaciones del Sistema:${NC}"
    echo ""
    
    # Verificar si hay progreso
    local has_progress=false
    for pattern in {01..10}; do
        if [[ -f "progress/pattern_${pattern}.txt" ]]; then
            has_progress=true
            break
        fi
    done
    
    if [[ "$has_progress" == false ]]; then
        echo -e "  🚀 ${GREEN}Primera vez:${NC} Ejecuta 'make start' para comenzar"
        echo -e "  📚 ${CYAN}Consejo:${NC} Empieza por el Patrón 01 (Manipulación de caracteres)"
    else
        echo -e "  📈 ${GREEN}En progreso:${NC} Continúa con tu patrón actual"
        echo -e "  ⏰ ${CYAN}Rutina:${NC} Dedica 30-60 minutos diarios a la práctica"
    fi
    
    # Verificar si faltan explicaciones
    local missing_explanations=0
    for pattern in {01..10}; do
        if [[ ! -f "explanations/pattern_${pattern}.md" ]]; then
            ((missing_explanations++))
        fi
    done
    
    if [[ $missing_explanations -gt 0 ]]; then
        echo -e "  📝 ${YELLOW}Pendiente:${NC} $missing_explanations explicaciones de patrones por crear"
    fi
    
    # Sugerencias de mejora
    echo ""
    echo -e "  ${PURPLE}🎯 Optimizaciones sugeridas:${NC}"
    echo -e "     • Practica 3-5 ejercicios por sesión"
    echo -e "     • Revisa patrones completados semanalmente"
    echo -e "     • Usa 'make progress' para tracking regular"
    
    echo ""
}

show_version_info() {
    echo -e "${BLUE}ℹ️  Información del Sistema:${NC}"
    echo ""
    echo -e "  📌 Versión: ${CYAN}Sistema de Patrones v1.0${NC}"
    echo -e "  🎯 Objetivo: ${CYAN}42 School Exam Rank 02${NC}"
    echo -e "  📅 Creado: $(date +"%Y-%m-%d")"
    echo -e "  🌍 Ubicación: $(pwd)"
    echo ""
}

# Función principal
main() {
    show_header
    check_system_integrity
    show_file_statistics
    show_exercise_coverage
    show_system_recommendations
    show_version_info
}

# Verificar si estamos en el directorio correcto
if [[ ! -f "exercise_patterns.conf" ]]; then
    echo -e "${RED}❌ Error: No se encuentra el archivo de configuración.${NC}"
    echo -e "${YELLOW}Asegúrate de ejecutar este script desde el directorio extra/${NC}"
    exit 1
fi

# Ejecutar función principal
main
