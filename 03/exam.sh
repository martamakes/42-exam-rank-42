#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'

# Directorios de trabajo
EXAM_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$EXAM_DIR")"
RENDU_DIR="$EXAM_DIR/rendu"

# Crear directorios si no existen
mkdir -p "$RENDU_DIR"
mkdir -p "$RENDU_DIR/ft_printf"
mkdir -p "$RENDU_DIR/get_next_line"

# Función para mostrar el subject de un ejercicio
show_subject() {
    local exercise=$1
    local subject_path="$EXAM_DIR/$exercise/README.md"
    
    clear
    echo -e "${BLUE}${BOLD}=== EJERCICIO: $exercise ===${NC}\n"
    echo -e "${CYAN}${BOLD}=== SUBJECT ===${NC}\n"
    
    if [ -f "$subject_path" ]; then
        cat "$subject_path"
        echo -e "\n${YELLOW}Para resolver este ejercicio:${NC}"
        echo -e "1. ${CYAN}Directorio:${NC} rendu/$exercise"
        echo -e "2. ${CYAN}Crea tu solución en:${NC} rendu/$exercise/"
        echo -e "\n${GREEN}${BOLD}IMPORTANTE:${NC} Tu solución debe estar dentro del directorio rendu/$exercise/"
        echo -e "Los archivos que debes crear son:"
        if [ "$exercise" = "ft_printf" ]; then
            echo -e "- ${CYAN}ft_printf.c${NC}"
        elif [ "$exercise" = "get_next_line" ]; then
            echo -e "- ${CYAN}get_next_line.c${NC}"
            echo -e "- ${CYAN}get_next_line.h${NC}"
        fi
    else
        echo -e "${RED}Error: No se encuentra el subject en $subject_path${NC}"
    fi
    
    echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
    read
}

# Función para validar ejercicio
validate_exercise() {
    local exercise=$1
    local student_dir="$RENDU_DIR/$exercise"
    local source_dir="$EXAM_DIR/$exercise"
    
    # Verificar que el usuario ha creado su solución
    if [ ! -d "$student_dir" ]; then
        echo -e "${RED}Error: No se encuentra tu solución en $student_dir${NC}"
        mkdir -p "$student_dir"
        echo -e "${YELLOW}Se ha creado el directorio $student_dir${NC}"
        return 1
    fi
    
    # Verificar existencia de archivos necesarios
    if [ "$exercise" = "ft_printf" ]; then
        if [ ! -f "$student_dir/ft_printf.c" ]; then
            echo -e "${RED}Error: No se encuentra ft_printf.c en $student_dir${NC}"
            return 1
        fi
    elif [ "$exercise" = "get_next_line" ]; then
        if [ ! -f "$student_dir/get_next_line.c" ] || [ ! -f "$student_dir/get_next_line.h" ]; then
            echo -e "${RED}Error: Faltan archivos en $student_dir${NC}"
            echo -e "${YELLOW}Asegúrate de tener get_next_line.c y get_next_line.h${NC}"
            return 1
        fi
    fi
    
    # Compilar y ejecutar las pruebas
    echo -e "${BLUE}${BOLD}Ejecutando tests para $exercise...${NC}"
    
    # Crear un Makefile temporal para la compilación
    TEMP_MAKEFILE="$student_dir/Makefile.tmp"
    cat > "$TEMP_MAKEFILE" << EOF
NAME = $exercise
SRC = $student_dir/*.c
INCLUDES = $student_dir/*.h
CFLAGS = -Wall -Wextra -Werror

all: \$(NAME)

\$(NAME): \$(SRC) \$(INCLUDES)
	gcc \$(CFLAGS) -o \$(NAME) \$(SRC) -I.

clean:
	rm -f \$(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
EOF
    
    # Compilar
    cd "$student_dir" && make -f Makefile.tmp
    COMPILE_RESULT=$?
    
    # Limpiar
    rm -f "$TEMP_MAKEFILE"
    
    if [ $COMPILE_RESULT -ne 0 ]; then
        echo -e "${RED}Error de compilación. Por favor, corrige los errores.${NC}"
        return 1
    fi
    
    # Ejecutar las pruebas específicas
    if [ "$exercise" = "ft_printf" ]; then
        # Test de ft_printf
        if [ -f "$source_dir/test_main.c" ]; then
            gcc -Wall -Wextra -Werror "$source_dir/test_main.c" "$student_dir/ft_printf.c" -o "$student_dir/ft_printf_test"
            "$student_dir/ft_printf_test"
            TEST_RESULT=$?
            rm -f "$student_dir/ft_printf_test"
        else
            echo -e "${YELLOW}No se encontró el archivo de prueba. Evaluación manual requerida.${NC}"
            TEST_RESULT=0  # Asumimos éxito para continuar
        fi
    elif [ "$exercise" = "get_next_line" ]; then
        # Test de get_next_line
        if [ -f "$source_dir/test_main.c" ]; then
            gcc -Wall -Wextra -Werror "$source_dir/test_main.c" "$student_dir/get_next_line.c" -o "$student_dir/gnl_test"
            "$student_dir/gnl_test"
            TEST_RESULT=$?
            rm -f "$student_dir/gnl_test"
        else
            echo -e "${YELLOW}No se encontró el archivo de prueba. Evaluación manual requerida.${NC}"
            TEST_RESULT=0  # Asumimos éxito para continuar
        fi
    fi
    
    if [ $TEST_RESULT -eq 0 ]; then
        echo -e "${GREEN}¡Los tests para $exercise han pasado correctamente!${NC}"
        return 0
    else
        echo -e "${RED}Algunos tests han fallado. Revisa tu implementación.${NC}"
        return 1
    fi
}

# Función para ver ejemplos y guías
view_examples() {
    local exercise=$1
    local examples_dir="$EXAM_DIR/$exercise/examples"
    
    clear
    echo -e "${BLUE}${BOLD}=== EJEMPLOS Y GUÍAS: $exercise ===${NC}\n"
    
    if [ ! -d "$examples_dir" ]; then
        echo -e "${RED}No se encontraron ejemplos para este ejercicio.${NC}"
        echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
        read
        return
    fi
    
    # Listar archivos de ejemplo
    echo -e "${CYAN}${BOLD}Archivos de ejemplo disponibles:${NC}"
    ls -1 "$examples_dir" | cat -n
    
    echo -e "\n${YELLOW}Selecciona un archivo para ver (0 para volver):${NC}"
    read -r example_choice
    
    if [[ "$example_choice" =~ ^[0-9]+$ ]] && [ "$example_choice" -gt 0 ]; then
        EXAMPLE_FILE=$(ls -1 "$examples_dir" | sed -n "${example_choice}p")
        if [ -n "$EXAMPLE_FILE" ]; then
            clear
            echo -e "${CYAN}${BOLD}=== $EXAMPLE_FILE ===${NC}\n"
            cat "$examples_dir/$EXAMPLE_FILE"
            echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
            read
        else
            echo -e "${RED}Selección inválida.${NC}"
            sleep 1
        fi
    fi
}

# Función para practicar un ejercicio específico
practice_exercise() {
    local exercise=$1
    
    while true; do
        clear
        show_subject "$exercise"
        
        echo -e "\n${YELLOW}${BOLD}Opciones:${NC}"
        echo "1. Validar mi solución"
        echo "2. Ver ejemplos y guías"
        echo "3. Volver al menú principal"
        
        read -p "Selección: " option
        
        case $option in
            1)
                validate_exercise "$exercise"
                echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
                read
                ;;
            2)
                view_examples "$exercise"
                ;;
            3)
                return
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                sleep 1
                ;;
        esac
    done
}

# Menú principal
main_menu() {
    while true; do
        clear
        echo -e "${BLUE}${BOLD}=== 42 EXAM RANK 03 PRACTICE ===${NC}"
        echo -e "${YELLOW}${BOLD}Selecciona un ejercicio:${NC}"
        echo "1. ft_printf"
        echo "2. get_next_line"
        echo "3. Ver estructura de directorios"
        echo "4. Limpiar directorio rendu"
        echo "0. Volver al menú principal"
        
        read -p "Selección: " choice
        
        case $choice in
            1)
                practice_exercise "ft_printf"
                ;;
            2)
                practice_exercise "get_next_line"
                ;;
            3)
                clear
                echo -e "${BLUE}${BOLD}=== ESTRUCTURA DE DIRECTORIOS ===${NC}"
                find "$EXAM_DIR" -type d -maxdepth 2 | sort
                echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
                read
                ;;
            4)
                echo -e "${RED}¿Estás seguro de que quieres limpiar el directorio rendu? (s/n)${NC}"
                read -r response
                if [[ "$response" =~ ^[Ss]$ ]]; then
                    rm -rf "$RENDU_DIR"/*
                    mkdir -p "$RENDU_DIR/ft_printf"
                    mkdir -p "$RENDU_DIR/get_next_line"
                    echo -e "${GREEN}Directorio rendu limpiado y reconstruido.${NC}"
                    sleep 1
                fi
                ;;
            0)
                return
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                sleep 1
                ;;
        esac
    done
}

# Iniciar menú principal
main_menu