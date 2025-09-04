#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
PURPLE='\033[0;35m'

# Directorio base
NEW_EXAMS_DIR="$(cd "$(dirname "$0")" && pwd)"

# Función para mostrar el banner
show_banner() {
    clear
    echo -e "${PURPLE}╔══════════════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${PURPLE}║                          42 NEW EXAMS MENU                          ║${NC}"
    echo -e "${PURPLE}║                     Practice System Selector                        ║${NC}"
    echo -e "${PURPLE}╚══════════════════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

# Función para mostrar información de cada exam rank
show_exam_info() {
    echo -e "${BLUE}=== INFORMACIÓN DE EXÁMENES DISPONIBLES ===${NC}\n"
    
    # Verificar si examshell está compilado
    local examshell_status
    if [ -f "$NEW_EXAMS_DIR/examshell" ]; then
        examshell_status="${GREEN}✅ EXAMSHELL compilado y listo${NC}"
    else
        examshell_status="${RED}❌ EXAMSHELL no compilado - ejecuta 'make'${NC}"
    fi
    
    echo -e "${CYAN}🎓 Sistema EXAMSHELL: $examshell_status${NC}\n"
    
    # Exam Rank 03
    if [ -d "$NEW_EXAMS_DIR/exam-rank-03" ]; then
        local ex03_count=$(find "$NEW_EXAMS_DIR/exam-rank-03" -name "level-*" -type d | wc -l)
        local ex03_exercises=$(find "$NEW_EXAMS_DIR/exam-rank-03/level-"* -maxdepth 1 -mindepth 1 -type d 2>/dev/null | wc -l)
        echo -e "${GREEN}📚 EXAM RANK 03 (Intermediate)${NC}"
        echo -e "   └── Niveles: $ex03_count"
        echo -e "   └── Ejercicios: $ex03_exercises"
        echo -e "   └── Ejercicios: filter, scanf, broken_gnl, n_queens, permutations, etc."
        echo ""
    fi
    
    # Exam Rank 04
    if [ -d "$NEW_EXAMS_DIR/exam-rank-04" ]; then
        local ex04_count=$(find "$NEW_EXAMS_DIR/exam-rank-04" -name "level-*" -type d | wc -l)
        local ex04_exercises=$(find "$NEW_EXAMS_DIR/exam-rank-04/level-"* -maxdepth 1 -mindepth 1 -type d 2>/dev/null | wc -l)
        echo -e "${GREEN}📚 EXAM RANK 04 (Advanced)${NC}"
        echo -e "   └── Niveles: $ex04_count"
        echo -e "   └── Ejercicios: $ex04_exercises"
        echo -e "   └── Ejercicios: ft_popen, picoshell, sandbox, argo, vbc"
        echo ""
    fi
    
    # Exam Rank 05
    if [ -d "$NEW_EXAMS_DIR/exam-rank-05" ]; then
        local ex05_count=$(find "$NEW_EXAMS_DIR/exam-rank-05" -name "level-*" -type d | wc -l)
        local ex05_exercises=$(find "$NEW_EXAMS_DIR/exam-rank-05/level-"* -maxdepth 1 -mindepth 1 -type d 2>/dev/null | wc -l)
        echo -e "${GREEN}📚 EXAM RANK 05 (Expert)${NC}"
        echo -e "   └── Niveles: $ex05_count"
        echo -e "   └── Ejercicios: $ex05_exercises"
        echo -e "   └── Ejercicios: string_bigint, vect2, bsq, game_of_life"
        echo ""
    fi
}

# Función para acceder a exam rank 03
access_exam_03() {
    if [ ! -d "$NEW_EXAMS_DIR/exam-rank-03" ]; then
        echo -e "${RED}Error: No se encuentra el directorio exam-rank-03${NC}"
        read -p "Presiona Enter para continuar..."
        return 1
    fi
    
    echo -e "${GREEN}🎓 Iniciando EXAMSHELL - Exam Rank 03...${NC}"
    echo -e "${CYAN}💡 Tip: Usa 3 terminales como en el examen real${NC}"
    echo -e "${YELLOW}Presiona Enter para continuar...${NC}"
    read
    
    if [ -f "$NEW_EXAMS_DIR/examshell" ]; then
        ./examshell 03
    else
        echo -e "${RED}Error: examshell no compilado. Ejecuta 'make' primero${NC}"
        read -p "Presiona Enter para continuar..."
    fi
}

# Función para acceder a exam rank 04
access_exam_04() {
    if [ ! -d "$NEW_EXAMS_DIR/exam-rank-04" ]; then
        echo -e "${RED}Error: No se encuentra el directorio exam-rank-04${NC}"
        read -p "Presiona Enter para continuar..."
        return 1
    fi
    
    echo -e "${GREEN}🎓 Iniciando EXAMSHELL - Exam Rank 04...${NC}"
    echo -e "${CYAN}💡 Tip: Usa 3 terminales como en el examen real${NC}"
    echo -e "${YELLOW}Presiona Enter para continuar...${NC}"
    read
    
    if [ -f "$NEW_EXAMS_DIR/examshell" ]; then
        ./examshell 04
    else
        echo -e "${RED}Error: examshell no compilado. Ejecuta 'make' primero${NC}"
        read -p "Presiona Enter para continuar..."
    fi
}

# Función para acceder a exam rank 05
access_exam_05() {
    if [ ! -d "$NEW_EXAMS_DIR/exam-rank-05" ]; then
        echo -e "${RED}Error: No se encuentra el directorio exam-rank-05${NC}"
        read -p "Presiona Enter para continuar..."
        return 1
    fi
    
    echo -e "${GREEN}🎓 Iniciando EXAMSHELL - Exam Rank 05...${NC}"
    echo -e "${CYAN}💡 Tip: Usa 3 terminales como en el examen real${NC}"
    echo -e "${YELLOW}Presiona Enter para continuar...${NC}"
    read
    
    if [ -f "$NEW_EXAMS_DIR/examshell" ]; then
        ./examshell 05
    else
        echo -e "${RED}Error: examshell no compilado. Ejecuta 'make' primero${NC}"
        read -p "Presiona Enter para continuar..."
    fi
}

# Función para mostrar información del proyecto
show_project_info() {
    clear
    echo -e "${BLUE}=== INFORMACIÓN DEL PROYECTO ===${NC}\n"
    echo -e "${CYAN}📁 Ubicación:${NC} $NEW_EXAMS_DIR"
    echo -e "${CYAN}🎯 Propósito:${NC} Sistema de práctica para exámenes 42 nuevos (Julio 2025+)"
    echo -e "${CYAN}📋 Contenido:${NC} Subjects y soluciones para exam ranks 03, 04 y 05"
    echo ""
    echo -e "${YELLOW}Estructura del proyecto:${NC}"
    echo -e "├── exam-rank-03/     # Ejercicios de rango 03"
    echo -e "├── exam-rank-04/     # Ejercicios de rango 04 (con sistema de tests)"
    echo -e "├── exam-rank-05/     # Ejercicios de rango 05"
    echo -e "└── init.sh           # Este menú principal"
    echo ""
    echo -e "${GREEN}💡 Tip:${NC} Cada exam rank contiene ejercicios organizados por niveles"
    echo -e "${GREEN}💡 Tip:${NC} Los sistemas de tests permiten validar tus soluciones automáticamente"
    echo ""
    read -p "Presiona Enter para volver al menú principal..."
}

# Función para compilar examshell
compile_examshell() {
    clear
    echo -e "${BLUE}=== COMPILAR EXAMSHELL ===${NC}\n"
    
    if [ -f "$NEW_EXAMS_DIR/examshell" ]; then
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
        echo -e "${CYAN}Ahora puedes usar cualquier exam rank desde el menú${NC}"
    else
        echo -e "\n${RED}❌ Error al compilar EXAMSHELL${NC}"
        echo -e "${YELLOW}Verifica que tengas GCC instalado y permisos de escritura${NC}"
    fi
    
    read -p "Presiona Enter para continuar..."
}

# Función para mostrar ayuda y consejos
show_help() {
    clear
    echo -e "${BLUE}=== AYUDA Y CONSEJOS ===${NC}\n"
    
    echo -e "${CYAN}🎯 Cómo usar el EXAMSHELL:${NC}"
    echo -e "1. Compila el sistema con 'Compilar EXAMSHELL'"
    echo -e "2. Selecciona tu exam rank (03, 04, o 05)"
    echo -e "3. El sistema te asignará un ejercicio aleatorio"
    echo -e "4. Lee las instrucciones en subject/[ejercicio]/subject.txt"
    echo -e "5. Crea tu solución en rendu/[ejercicio]/"
    echo ""
    
    echo -e "${CYAN}💡 Consejos para el examen real:${NC}"
    echo -e "• Usa 3 terminales:"
    echo -e "  - Terminal 1: EXAMSHELL (control)"
    echo -e "  - Terminal 2: Leer subject (cat subject/*/subject.txt)"
    echo -e "  - Terminal 3: Programar tu solución"
    echo -e "• Crea directorios manualmente: mkdir rendu/[ejercicio]"
    echo -e "• Compila y testea tu código antes de 'grademe'"
    echo -e "• El tiempo se registra automáticamente"
    echo ""
    
    echo -e "${CYAN}📁 Estructura de directorios:${NC}"
    echo -e "examshell/"
    echo -e "├── rendu/           ← TUS SOLUCIONES (persisten)"
    echo -e "│   └── [ejercicio]/ ← Crear manualmente"
    echo -e "├── subject/         ← INSTRUCCIONES (se limpian al salir)"
    echo -e "│   └── [ejercicio]/"
    echo -e "│       └── subject.txt"
    echo -e "└── .examshell_progress ← PROGRESO (automático)"
    echo ""
    
    echo -e "${CYAN}🎮 Opciones en EXAMSHELL:${NC}"
    echo -e "[g] Grademe - Marcar como aprobado y avanzar"
    echo -e "[n] Next - Nuevo ejercicio del mismo nivel"
    echo -e "[r] Rendu - Ver archivos de tu solución"
    echo -e "[s] Stats - Ver estadísticas y progreso"
    echo -e "[q] Quit - Salir guardando progreso"
    echo ""
    
    read -p "Presiona Enter para continuar..."
}

# Menú principal
main_menu() {
    while true; do
        show_banner
        show_exam_info
        
        echo -e "${YELLOW}=== MENÚ PRINCIPAL ===${NC}"
        echo ""
        echo "1. 📚 Acceder a Exam Rank 03 (Intermediate)"
        echo "2. 📚 Acceder a Exam Rank 04 (Advanced)"
        echo "3. 📚 Acceder a Exam Rank 05 (Expert)"
        echo "4. ℹ️  Información del proyecto"
        echo "5. 🔧 Compilar EXAMSHELL"
        echo "6. ❓ Ayuda y consejos"
        echo "7. 🚪 Salir"
        echo ""
        
        read -p "Selecciona una opción: " choice
        
        case $choice in
            1)
                access_exam_03
                ;;
            2)
                access_exam_04
                ;;
            3)
                access_exam_05
                ;;
            4)
                show_project_info
                ;;
            5)
                compile_examshell
                ;;
            6)
                show_help
                ;;
            7)
                echo -e "${GREEN}¡Hasta luego! 👋${NC}"
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

# Iniciar el menú principal
main_menu