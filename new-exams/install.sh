#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    install.sh                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marta <marta@student.42madrid.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/06 00:00:00 by marta             #+#    #+#              #
#    Updated: 2025/07/06 00:00:00 by marta            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# EXAMSHELL Installation Script
# Automatiza la instalación y configuración del sistema de práctica

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para imprimir headers
print_header() {
    echo -e "${BLUE}"
    echo "=========================================="
    echo "    EXAMSHELL - Installation Script"
    echo "    42 School Exam Practice System"
    echo "=========================================="
    echo -e "${NC}"
}

# Función para verificar dependencias
check_dependencies() {
    echo -e "${YELLOW}Checking dependencies...${NC}"
    
    # Verificar gcc
    if ! command -v gcc &> /dev/null; then
        echo -e "${RED}❌ Error: gcc not found. Please install gcc first.${NC}"
        exit 1
    fi
    echo -e "${GREEN}✅ gcc found${NC}"
    
    # Verificar make
    if ! command -v make &> /dev/null; then
        echo -e "${RED}❌ Error: make not found. Please install make first.${NC}"
        exit 1
    fi
    echo -e "${GREEN}✅ make found${NC}"
    
    echo -e "${GREEN}✅ All dependencies satisfied${NC}\n"
}

# Función para compilar el proyecto
compile_project() {
    echo -e "${YELLOW}Compiling examshell...${NC}"
    
    if make; then
        echo -e "${GREEN}✅ Compilation successful${NC}\n"
    else
        echo -e "${RED}❌ Compilation failed${NC}"
        exit 1
    fi
}

# Función para verificar estructura de directorios
check_structure() {
    echo -e "${YELLOW}Checking exam structure...${NC}"
    
    exam_count=0
    
    for rank in 03 04 05 06; do
        if [ -d "exam-rank-$rank" ]; then
            echo -e "${GREEN}✅ Found exam-rank-$rank${NC}"
            exam_count=$((exam_count + 1))
            
            # Verificar niveles
            for level in 1 2; do
                if [ -d "exam-rank-$rank/level-$level" ]; then
                    exercise_count=$(find "exam-rank-$rank/level-$level" -name "subject.txt" | wc -l)
                    echo -e "${GREEN}  └── Level $level: $exercise_count exercises${NC}"
                else
                    echo -e "${YELLOW}  └── Level $level: not found${NC}"
                fi
            done
        fi
    done
    
    if [ $exam_count -eq 0 ]; then
        echo -e "${RED}❌ No exam ranks found!${NC}"
        echo -e "${YELLOW}Make sure you have directories like 'exam-rank-03', 'exam-rank-04', etc.${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}✅ Found $exam_count exam ranks${NC}\n"
}

# Función para hacer test básico
run_basic_test() {
    echo -e "${YELLOW}Running basic test...${NC}"
    
    if [ -f "./examshell" ]; then
        echo -e "${GREEN}✅ Executable found${NC}"
        
        # Test con argumentos inválidos
        if ./examshell --help > /dev/null 2>&1; then
            echo -e "${GREEN}✅ Program runs without crashing${NC}"
        fi
    else
        echo -e "${RED}❌ Executable not found${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}✅ Basic test passed${NC}\n"
}

# Función para instalación global
install_global() {
    echo -e "${YELLOW}Do you want to install examshell globally? (y/n): ${NC}"
    read -r response
    
    if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
        echo -e "${YELLOW}Installing globally (requires sudo)...${NC}"
        
        if sudo make install; then
            echo -e "${GREEN}✅ Global installation successful${NC}"
            echo -e "${GREEN}You can now use 'examshell' from any directory${NC}"
        else
            echo -e "${RED}❌ Global installation failed${NC}"
            echo -e "${YELLOW}You can still use './examshell' from this directory${NC}"
        fi
    else
        echo -e "${YELLOW}Skipping global installation${NC}"
        echo -e "${YELLOW}Use './examshell' to run the program${NC}"
    fi
    echo ""
}

# Función para mostrar información de uso
show_usage_info() {
    echo -e "${BLUE}=========================================="
    echo "           Installation Complete!"
    echo "==========================================${NC}"
    echo ""
    echo -e "${GREEN}Usage:${NC}"
    echo "  ./examshell [rank_number]"
    echo "  ./examshell 03"
    echo "  ./examshell 04"
    echo "  ./examshell 05"
    echo ""
    echo -e "${GREEN}Examples:${NC}"
    echo "  ./examshell 03    # Start exam rank 03"
    echo "  ./examshell       # Interactive rank selection"
    echo ""
    echo -e "${GREEN}Available commands in exam:${NC}"
    echo "  [s] - Show exercise subject"
    echo "  [g] - Mark exercise as passed (grademe)"
    echo "  [n] - Move to next exercise"
    echo "  [t] - Run tester (coming soon)"
    echo "  [q] - Quit exam session"
    echo ""
    echo -e "${YELLOW}Pro tip: Start with './examshell 03' for easier exercises${NC}"
    echo ""
}

# Función principal
main() {
    print_header
    
    # Verificar que estamos en el directorio correcto
    if [ ! -f "examshell.c" ]; then
        echo -e "${RED}❌ Error: examshell.c not found in current directory${NC}"
        echo -e "${YELLOW}Please run this script from the examshell project directory${NC}"
        exit 1
    fi
    
    check_dependencies
    check_structure
    compile_project
    run_basic_test
    install_global
    show_usage_info
    
    echo -e "${GREEN}🎉 Setup complete! Happy coding!${NC}"
}

# Ejecutar función principal
main "$@"
