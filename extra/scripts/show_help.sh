#!/bin/bash

# Script de ayuda y documentación del sistema

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
    echo -e "${CYAN}█                                 AYUDA DEL SISTEMA                            █${NC}"
    echo -e "${CYAN}═══════════════════════════════════════════════════════════════════════════════${NC}"
    echo ""
}

show_system_overview() {
    echo -e "${BLUE}🎯 ¿Qué es el Sistema de Práctica por Patrones?${NC}"
    echo ""
    echo -e "Este sistema reorganiza los ejercicios de 42 School Exam Rank 02 por"
    echo -e "patrones conceptuales en lugar de niveles arbitrarios de dificultad."
    echo ""
    echo -e "${GREEN}✅ Ventajas del sistema:${NC}"
    echo -e "  • ${CYAN}Aprendizaje progresivo${NC}: Domina un concepto antes de pasar al siguiente"
    echo -e "  • ${CYAN}Mejor retención${NC}: Agrupa ejercicios similares para reforzar patrones"
    echo -e "  • ${CYAN}Preparación eficiente${NC}: Enfoque sistemático para el examen"
    echo -e "  • ${CYAN}Tracking de progreso${NC}: Ve exactamente qué has dominado"
    echo ""
}

show_pattern_explanation() {
    echo -e "${BLUE}📚 Los 10 Patrones Conceptuales:${NC}"
    echo ""
    
    patterns=(
        "01:🔤:Manipulación básica de caracteres:Transformaciones simples, ROT13, bits"
        "02:📝:Procesamiento palabra por palabra:first_word, camelCase, capitalización"
        "03:🔄:Conversión de tipos:atoi, itoa, ft_split, bases"
        "04:🔍:Algoritmos de búsqueda:wdmatch, hidenp, strcspn"
        "05:📊:Operaciones con conjuntos:union, inter (intersección)"
        "06:💾:Memoria y punteros:malloc, strdup, arrays dinámicos"
        "07:🧮:Algoritmos matemáticos:primos, MCM, MCD, factorización"
        "08:🔗:Listas enlazadas:navegación, modificación, ordenación"
        "09:📈:Algoritmos de ordenación:bubble sort, sort_list"
        "10:🌊:Recursividad avanzada:flood_fill, algoritmos recursivos"
    )
    
    for pattern in "${patterns[@]}"; do
        IFS=':' read -r num emoji name desc <<< "$pattern"
        printf "  ${PURPLE}%s %s${NC} %-35s ${CYAN}%s${NC}\n" "$emoji" "$num." "$name" "$desc"
    done
    echo ""
}

show_usage_guide() {
    echo -e "${BLUE}🚀 Cómo usar el sistema:${NC}"
    echo ""
    echo -e "${GREEN}1. Configuración inicial:${NC}"
    echo -e "   ${YELLOW}make setup${NC}     - Configura enlaces simbólicos"
    echo -e "   ${YELLOW}make start${NC}     - Inicia el sistema"
    echo ""
    echo -e "${GREEN}2. Navegación:${NC}"
    echo -e "   • Usa el menú principal para seleccionar patrones"
    echo -e "   • Cada patrón tiene su propia explicación"
    echo -e "   • Progresa linealmente dentro de cada patrón"
    echo ""
    echo -e "${GREEN}3. Práctica de ejercicios:${NC}"
    echo -e "   • Lee la explicación del patrón primero"
    echo -e "   • Practica ejercicio por ejercicio"
    echo -e "   • Usa 'c' para compilar y probar"
    echo -e "   • Usa 's' para marcar como completado"
    echo ""
    echo -e "${GREEN}4. Seguimiento de progreso:${NC}"
    echo -e "   ${YELLOW}make progress${NC}  - Ver progreso detallado"
    echo -e "   ${YELLOW}make status${NC}    - Estado del sistema"
    echo ""
}

show_exam_strategy() {
    echo -e "${BLUE}🎯 Estrategia Recomendada para el Examen:${NC}"
    echo ""
    echo -e "${GREEN}📋 Plan de Estudio (8-10 semanas):${NC}"
    echo ""
    echo -e "${YELLOW}Semanas 1-2: Fundamentos${NC}"
    echo -e "  • Patrón 01: Manipulación de caracteres"
    echo -e "  • Patrón 02: Procesamiento de palabras"
    echo -e "  • Patrón 03: Conversiones de tipos"
    echo ""
    echo -e "${YELLOW}Semanas 3-4: Algoritmos Básicos${NC}"
    echo -e "  • Patrón 04: Algoritmos de búsqueda"
    echo -e "  • Patrón 05: Operaciones con conjuntos"
    echo ""
    echo -e "${YELLOW}Semanas 5-6: Memoria y Matemáticas${NC}"
    echo -e "  • Patrón 06: Memoria y punteros"
    echo -e "  • Patrón 07: Algoritmos matemáticos"
    echo ""
    echo -e "${YELLOW}Semanas 7-8: Estructuras Avanzadas${NC}"
    echo -e "  • Patrón 08: Listas enlazadas"
    echo -e "  • Patrón 09: Algoritmos de ordenación"
    echo ""
    echo -e "${YELLOW}Semanas 9-10: Recursividad y Repaso${NC}"
    echo -e "  • Patrón 10: Recursividad avanzada"
    echo -e "  • Repaso general con ejercicios aleatorios"
    echo ""
    echo -e "${GREEN}💡 Consejos para el día del examen:${NC}"
    echo -e "  • ${CYAN}Lee todo el enunciado${NC} antes de empezar a codificar"
    echo -e "  • ${CYAN}Identifica el patrón${NC} - ¿a cuál de los 10 pertenece?"
    echo -e "  • ${CYAN}Implementa paso a paso${NC} - no intentes hacerlo todo de una"
    echo -e "  • ${CYAN}Prueba con casos simples${NC} antes de casos complejos"
    echo -e "  • ${CYAN}Gestiona tu tiempo${NC} - 3 horas para 4 ejercicios"
    echo ""
}

show_troubleshooting() {
    echo -e "${BLUE}🔧 Solución de Problemas Comunes:${NC}"
    echo ""
    echo -e "${YELLOW}❓ El sistema no encuentra los ejercicios originales${NC}"
    echo -e "   ${GREEN}→${NC} Asegúrate de estar en el directorio extra/"
    echo -e "   ${GREEN}→${NC} Verifica que ../02/ existe con Level1, Level2, etc."
    echo -e "   ${GREEN}→${NC} Ejecuta 'make setup' para reconfigurar"
    echo ""
    echo -e "${YELLOW}❓ Los scripts no son ejecutables${NC}"
    echo -e "   ${GREEN}→${NC} Ejecuta 'make setup' para corregir permisos"
    echo -e "   ${GREEN}→${NC} O manualmente: chmod +x scripts/*.sh"
    echo ""
    echo -e "${YELLOW}❓ Se perdió el progreso${NC}"
    echo -e "   ${GREEN}→${NC} Los archivos están en progress/pattern_XX.txt"
    echo -e "   ${GREEN}→${NC} Si se borraron, se recrearán automáticamente"
    echo ""
    echo -e "${YELLOW}❓ No compila un ejercicio${NC}"
    echo -e "   ${GREEN}→${NC} Verifica que tienes todos los archivos .c necesarios"
    echo -e "   ${GREEN}→${NC} Lee el subject.en.txt para requirements específicos"
    echo -e "   ${GREEN}→${NC} Algunos ejercicios necesitan archivos adicionales"
    echo ""
    echo -e "${YELLOW}❓ ¿Cómo resetear un patrón específico?${NC}"
    echo -e "   ${GREEN}→${NC} Dentro del menú del patrón, usa la opción 'r'"
    echo -e "   ${GREEN}→${NC} O borra manualmente progress/pattern_XX.txt"
    echo ""
}

show_advanced_features() {
    echo -e "${BLUE}🚀 Características Avanzadas:${NC}"
    echo ""
    echo -e "${GREEN}📁 Estructura de archivos:${NC}"
    echo -e "  patterns/          - Directorios organizados por patrón"
    echo -e "  progress/          - Archivos de seguimiento (.txt)"
    echo -e "  explanations/      - Explicaciones de cada patrón (.md)"
    echo -e "  scripts/           - Scripts del sistema"
    echo -e "  links/             - Enlaces simbólicos a ejercicios originales"
    echo ""
    echo -e "${GREEN}⚙️  Personalización:${NC}"
    echo -e "  • Edita explanations/pattern_XX.md para agregar notas"
    echo -e "  • Modifica exercise_patterns.conf para reorganizar ejercicios"
    echo -e "  • Los archivos de progreso son texto plano y editables"
    echo ""
    echo -e "${GREEN}🔄 Integración:${NC}"
    echo -e "  • El sistema no modifica los ejercicios originales"
    echo -e "  • Usa enlaces simbólicos para acceso directo"
    echo -e "  • Compatible con herramientas externas de testing"
    echo ""
}

# Menú principal de ayuda
help_menu() {
    while true; do
        show_header
        
        echo -e "${GREEN}Selecciona una sección de ayuda:${NC}"
        echo ""
        echo -e "  ${YELLOW}1${NC}) Visión general del sistema"
        echo -e "  ${YELLOW}2${NC}) Explicación de los 10 patrones"
        echo -e "  ${YELLOW}3${NC}) Guía de uso paso a paso"
        echo -e "  ${YELLOW}4${NC}) Estrategia para el examen"
        echo -e "  ${YELLOW}5${NC}) Solución de problemas"
        echo -e "  ${YELLOW}6${NC}) Características avanzadas"
        echo -e "  ${YELLOW}q${NC}) Volver al menú principal"
        echo ""
        echo -n "Tu elección: "
        
        read -r choice
        
        case $choice in
            1)
                show_header
                show_system_overview
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            2)
                show_header
                show_pattern_explanation
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            3)
                show_header
                show_usage_guide
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            4)
                show_header
                show_exam_strategy
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            5)
                show_header
                show_troubleshooting
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            6)
                show_header
                show_advanced_features
                read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
                ;;
            q|Q)
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

# Iniciar menú de ayuda
help_menu
