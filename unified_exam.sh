#!/bin/bash

# 🎓 UNIFIED 42 EXAM PRACTICE SYSTEM
# Integrates legacy (02, 06) and new exams (03, 04, 05) into one system

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
PURPLE='\033[0;35m'
BOLD='\033[1m'

# Directorio base
BASE_DIR="$(cd "$(dirname "$0")" && pwd)"

# Función para mostrar el banner principal
show_main_banner() {
    clear
    echo -e "${PURPLE}${BOLD}"
    echo "╔══════════════════════════════════════════════════════════════════════╗"
    echo "║                    🎓 42 EXAM PRACTICE SYSTEM 🎓                    ║"
    echo "║                         UNIFIED INTERFACE                           ║"
    echo "║                                                                      ║"
    echo "║  Legacy System (02, 06) + New EXAMSHELL System (03, 04, 05)        ║"
    echo "╚══════════════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
    echo ""
}

# Función para detectar sistemas disponibles
detect_available_systems() {
    local legacy_count=0
    local new_count=0
    
    echo -e "${BLUE}=== SISTEMAS DISPONIBLES ===${NC}\n"
    
    # Detectar sistema legacy
    echo -e "${CYAN}🔧 SISTEMA LEGACY (Shell-based):${NC}"
    [ -f "$BASE_DIR/02/exam.sh" ] && echo -e "   ✅ Exam Rank 02 (Básico - 4 niveles)" && ((legacy_count++))
    [ -f "$BASE_DIR/06/exam.sh" ] && echo -e "   ✅ Exam Rank 06 (Avanzado)" && ((legacy_count++))
    
    if [ $legacy_count -eq 0 ]; then
        echo -e "   ${RED}❌ No disponible${NC}"
    fi
    
    echo ""
    
    # Detectar nuevo sistema EXAMSHELL
    echo -e "${CYAN}🚀 NUEVO SISTEMA EXAMSHELL (C-based):${NC}"
    if [ -f "$BASE_DIR/new-exams/examshell" ]; then
        echo -e "   ✅ Sistema EXAMSHELL compilado"
        [ -d "$BASE_DIR/new-exams/exam-rank-03" ] && echo -e "   ✅ Exam Rank 03 (Intermediate)" && ((new_count++))
        [ -d "$BASE_DIR/new-exams/exam-rank-04" ] && echo -e "   ✅ Exam Rank 04 (Advanced)" && ((new_count++))
        [ -d "$BASE_DIR/new-exams/exam-rank-05" ] && echo -e "   ✅ Exam Rank 05 (Expert)" && ((new_count++))
    else
        echo -e "   ${YELLOW}⚠️ EXAMSHELL no compilado${NC}"
        echo -e "   ${CYAN}💡 Usa opción 'Compilar EXAMSHELL' para activarlo${NC}"
    fi
    
    if [ $new_count -eq 0 ]; then
        echo -e "   ${RED}❌ No disponible${NC}"
    fi
    
    echo ""
    
    # Resumen
    local total=$((legacy_count + new_count))
    echo -e "${YELLOW}📊 RESUMEN: $total exam ranks disponibles${NC}"
    echo -e "   • Sistema Legacy: $legacy_count exámenes"
    echo -e "   • Sistema EXAMSHELL: $new_count exámenes"
    echo ""
}

# Función para mostrar información detallada
show_detailed_info() {
    clear
    echo -e "${BLUE}=== INFORMACIÓN DETALLADA DE SISTEMAS ===${NC}\n"
    
    echo -e "${CYAN}🔧 SISTEMA LEGACY (Shell-based):${NC}"
    echo -e "   • Características:"
    echo -e "     - Sistema de menús interactivos"
    echo -e "     - Ejercicios organizados por niveles"
    echo -e "     - Tests automáticos con grademe/"
    echo -e "     - Progreso persistente"
    echo -e "   • Estructura: Level1/, Level2/, Level3/, Level4/"
    echo -e "   • Soluciones en: rendu/[ejercicio]/"
    echo ""
    
    echo -e "${CYAN}🚀 SISTEMA EXAMSHELL (C-based):${NC}"
    echo -e "   • Características:"
    echo -e "     - Réplica exacta del examshell real de 42"
    echo -e "     - Workflow idéntico al examen oficial"
    echo -e "     - Tracking de tiempo y estadísticas"
    echo -e "     - Ejercicios aleatorios por sesión"
    echo -e "   • Estructura: level-1/, level-2/"
    echo -e "   • Soluciones en: rendu/[ejercicio]/"
    echo ""
    
    echo -e "${YELLOW}💡 RECOMENDACIONES DE USO:${NC}"
    echo -e "   • Exam Rank 02: Ideal para principiantes (sistema legacy)"
    echo -e "   • Exam Ranks 03-05: Usa EXAMSHELL para experiencia real"
    echo -e "   • Exam Rank 06: Sistema legacy especializado"
    echo ""
    
    read -p "Presiona Enter para continuar..."
}

# Función para acceder al sistema legacy
access_legacy_system() {
    echo -e "${GREEN}🔧 Accediendo al Sistema Legacy...${NC}"
    echo -e "${CYAN}Usando: exam_master.sh${NC}"
    echo -e "${YELLOW}Presiona Enter para continuar...${NC}"
    read
    
    if [ -f "$BASE_DIR/exam_master.sh" ]; then
        cd "$BASE_DIR"
        ./exam_master.sh
    else
        echo -e "${RED}Error: exam_master.sh no encontrado${NC}"
        read -p "Presiona Enter para continuar..."
    fi
}

# Función para acceder al sistema EXAMSHELL
access_examshell_system() {
    echo -e "${GREEN}🚀 Accediendo al Sistema EXAMSHELL...${NC}"
    echo -e "${CYAN}Usando: new-exams/init.sh${NC}"
    echo -e "${YELLOW}Presiona Enter para continuar...${NC}"
    read
    
    if [ -f "$BASE_DIR/new-exams/init.sh" ]; then
        cd "$BASE_DIR/new-exams"
        ./init.sh
    else
        echo -e "${RED}Error: new-exams/init.sh no encontrado${NC}"
        read -p "Presiona Enter para continuar..."
    fi
}

# Función para acceso directo a un exam rank específico
direct_exam_access() {
    clear
    echo -e "${BLUE}=== ACCESO DIRECTO POR EXAM RANK ===${NC}\n"
    
    local available_ranks=()
    local rank_systems=()
    local rank_paths=()
    
    # Detectar ranks disponibles
    if [ -f "$BASE_DIR/02/exam.sh" ]; then
        available_ranks+=("02 - Básico (4 niveles)")
        rank_systems+=("legacy")
        rank_paths+=("$BASE_DIR/02/exam.sh")
    fi
    
    if [ -f "$BASE_DIR/new-exams/examshell" ] && [ -d "$BASE_DIR/new-exams/exam-rank-03" ]; then
        available_ranks+=("03 - Intermediate")
        rank_systems+=("examshell")
        rank_paths+=("03")
    fi
    
    if [ -f "$BASE_DIR/new-exams/examshell" ] && [ -d "$BASE_DIR/new-exams/exam-rank-04" ]; then
        available_ranks+=("04 - Advanced")
        rank_systems+=("examshell")
        rank_paths+=("04")
    fi
    
    if [ -f "$BASE_DIR/new-exams/examshell" ] && [ -d "$BASE_DIR/new-exams/exam-rank-05" ]; then
        available_ranks+=("05 - Expert")
        rank_systems+=("examshell")
        rank_paths+=("05")
    fi
    
    if [ -f "$BASE_DIR/06/exam.sh" ]; then
        available_ranks+=("06 - Avanzado")
        rank_systems+=("legacy")
        rank_paths+=("$BASE_DIR/06/exam.sh")
    fi
    
    # Mostrar opciones
    if [ ${#available_ranks[@]} -eq 0 ]; then
        echo -e "${RED}No hay exam ranks disponibles${NC}"
        read -p "Presiona Enter para continuar..."
        return
    fi
    
    echo -e "${YELLOW}Selecciona un exam rank:${NC}"
    for i in "${!available_ranks[@]}"; do
        local system_type="${rank_systems[$i]}"
        local icon="🔧"
        [ "$system_type" = "examshell" ] && icon="🚀"
        echo "$((i+1)). $icon Exam Rank ${available_ranks[$i]}"
    done
    echo "0. Volver al menú principal"
    echo ""
    
    read -p "Selecciona una opción: " choice
    
    if [[ "$choice" =~ ^[0-9]+$ ]] && [ "$choice" -ge 1 ] && [ "$choice" -le ${#available_ranks[@]} ]; then
        local selected_index=$((choice-1))
        local system_type="${rank_systems[$selected_index]}"
        local rank_path="${rank_paths[$selected_index]}"
        
        echo -e "${GREEN}Iniciando Exam Rank ${available_ranks[$selected_index]}...${NC}"
        
        if [ "$system_type" = "legacy" ]; then
            cd "$(dirname "$rank_path")"
            ./exam.sh
        else
            cd "$BASE_DIR/new-exams"
            ./examshell "$rank_path"
        fi
    elif [ "$choice" != "0" ]; then
        echo -e "${RED}Selección inválida${NC}"
        read -p "Presiona Enter para continuar..."
    fi
}

# Función para compilar EXAMSHELL
compile_examshell() {
    clear
    echo -e "${BLUE}=== COMPILAR SISTEMA EXAMSHELL ===${NC}\n"
    
    if [ ! -d "$BASE_DIR/new-exams" ]; then
        echo -e "${RED}Error: Directorio new-exams no encontrado${NC}"
        read -p "Presiona Enter para continuar..."
        return
    fi
    
    cd "$BASE_DIR/new-exams"
    
    if [ -f "examshell" ]; then
        echo -e "${GREEN}✅ EXAMSHELL ya está compilado${NC}"
        echo -e "${YELLOW}¿Quieres recompilarlo? (s/n):${NC}"
        read -r response
        
        if [[ ! "$response" =~ ^[Ss]$ ]]; then
            return
        fi
    fi
    
    echo -e "${CYAN}Compilando EXAMSHELL...${NC}"
    
    if make; then
        echo -e "\n${GREEN}✅ EXAMSHELL compilado exitosamente!${NC}"
        echo -e "${CYAN}Ahora puedes usar los exam ranks 03, 04 y 05${NC}"
    else
        echo -e "\n${RED}❌ Error al compilar EXAMSHELL${NC}"
        echo -e "${YELLOW}Verifica que tengas GCC instalado y permisos de escritura${NC}"
    fi
    
    read -p "Presiona Enter para continuar..."
}

# Función para mostrar ayuda completa
show_complete_help() {
    clear
    echo -e "${BLUE}=== AYUDA COMPLETA DEL SISTEMA ===${NC}\n"
    
    echo -e "${CYAN}🎯 CÓMO USAR EL SISTEMA UNIFICADO:${NC}"
    echo -e "1. Usa 'Información detallada' para entender los sistemas"
    echo -e "2. Compila EXAMSHELL si planeas usar ranks 03-05"
    echo -e "3. Selecciona 'Acceso por sistema' o 'Acceso directo'"
    echo -e "4. ¡Practica como si fuera el examen real!"
    echo ""
    
    echo -e "${CYAN}🔧 SISTEMA LEGACY (02, 06):${NC}"
    echo -e "• Ideal para: Principiantes y práctica guiada"
    echo -e "• Workflow: Menús → Ejercicio → Validar → Siguiente nivel"
    echo -e "• Características: Tests automáticos, progreso por niveles"
    echo ""
    
    echo -e "${CYAN}🚀 SISTEMA EXAMSHELL (03, 04, 05):${NC}"
    echo -e "• Ideal para: Experiencia real de examen"
    echo -e "• Workflow: Ejercicio aleatorio → subject/ → rendu/ → grademe"
    echo -e "• Características: Tiempo real, workflow exacto de 42"
    echo ""
    
    echo -e "${CYAN}💡 CONSEJOS GENERALES:${NC}"
    echo -e "• Practica regularmente en ambos sistemas"
    echo -e "• Usa 3 terminales en EXAMSHELL (como examen real)"
    echo -e "• Lee siempre el subject completo antes de programar"
    echo -e "• Testea tu código antes de hacer grademe"
    echo ""
    
    read -p "Presiona Enter para continuar..."
}

# Menú principal unificado
main_unified_menu() {
    while true; do
        show_main_banner
        detect_available_systems
        
        echo -e "${YELLOW}=== MENÚ PRINCIPAL UNIFICADO ===${NC}"
        echo ""
        echo "1. 🔧 Acceder al Sistema Legacy (02, 06)"
        echo "2. 🚀 Acceder al Sistema EXAMSHELL (03, 04, 05)"
        echo "3. 🎯 Acceso directo por Exam Rank"
        echo "4. ℹ️  Información detallada de sistemas"
        echo "5. 🔧 Compilar EXAMSHELL"
        echo "6. ❓ Ayuda completa"
        echo "7. 🚪 Salir"
        echo ""
        
        read -p "Selecciona una opción: " choice
        
        case $choice in
            1)
                access_legacy_system
                ;;
            2)
                access_examshell_system
                ;;
            3)
                direct_exam_access
                ;;
            4)
                show_detailed_info
                ;;
            5)
                compile_examshell
                ;;
            6)
                show_complete_help
                ;;
            7)
                echo -e "${GREEN}¡Hasta luego! Que tengas buena práctica! 🎓${NC}"
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
if [ ! -f "$BASE_DIR/exam_master.sh" ] && [ ! -d "$BASE_DIR/new-exams" ]; then
    echo -e "${RED}Error: No se encuentran los sistemas de examen${NC}"
    echo -e "${YELLOW}Asegúrate de ejecutar este script desde el directorio raíz del proyecto${NC}"
    exit 1
fi

# Iniciar el sistema unificado
main_unified_menu