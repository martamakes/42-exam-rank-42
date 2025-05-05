#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE="ft_printf"
STUDENT_DIR="../../../../rendu/$EXERCISE"

# Mostrar ayuda si se solicita
if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    echo "Uso: ./test.sh [opciones]"
    echo ""
    echo "Opciones:"
    echo "  --help, -h     Muestra esta ayuda"
    echo "  --valgrind     Ejecuta tests con Valgrind para detectar fugas de memoria"
    echo "  --memcheck     Alias para --valgrind"
    echo ""
    echo "Ejemplos:"
    echo "  ./test.sh              # Ejecuta tests normales"
    echo "  ./test.sh --valgrind   # Ejecuta con detección de fugas de memoria"
    exit 0
fi

# Verificar si se debe usar valgrind
USE_VALGRIND=0
if [ "$1" == "--valgrind" ] || [ "$1" == "--memcheck" ]; then
    if command -v valgrind &> /dev/null; then
        USE_VALGRIND=1
        echo -e "${BLUE}Modo Valgrind activado${NC}"
    else
        echo -e "${RED}Valgrind no está instalado. Ejecutando tests normales.${NC}"
    fi
fi

# Información
echo -e "${BLUE}Testing $EXERCISE${NC}"
echo -e "${BLUE}=======================${NC}"

# Verificar directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    echo -e "${BLUE}Asegúrate de que la estructura de directorios es correcta:${NC}"
    echo -e "${BLUE}  - Tu solución debe estar en: $STUDENT_DIR/${NC}"
    
    # Crear el directorio para el estudiante
    echo -e "${BLUE}¿Quieres crear el directorio $STUDENT_DIR? (s/n)${NC}"
    read -n 1 answer
    echo
    
    if [ "$answer" = "s" ] || [ "$answer" = "S" ]; then
        mkdir -p "$STUDENT_DIR"
        echo -e "${GREEN}Directorio creado.${NC}"
        echo -e "${BLUE}Por favor, copia tu ft_printf.c en: $STUDENT_DIR/${NC}"
        exit 0
    else
        exit 1
    fi
fi

# Copiar archivos necesarios
cp "$STUDENT_DIR/$EXERCISE.c" .

# Verificar si el archivo existe
if [ ! -f "$EXERCISE.c" ]; then
    echo -e "${RED}Error: No se encuentra el archivo $EXERCISE.c en $STUDENT_DIR${NC}"
    exit 1
fi

# Compilar los tests
echo -e "${BLUE}Compilando tests...${NC}"
gcc -Wall -Wextra -Werror test_main.c "$EXERCISE.c" -o test_prog

if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación${NC}"
    # Limpiar
    rm -f "$EXERCISE.c"
    exit 1
fi

# Ejecutar tests con o sin valgrind
if [ $USE_VALGRIND -eq 1 ]; then
    echo -e "${BLUE}Ejecutando tests con valgrind...${NC}"
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test_prog
    test_result=$?
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}No se detectaron fugas de memoria${NC}"
    else
        echo -e "${RED}Se detectaron fugas de memoria${NC}"
    fi
else
    echo -e "${BLUE}Ejecutando tests...${NC}"
    ./test_prog
    test_result=$?
fi

# Limpiar
rm -f test_prog "$EXERCISE.c"

# Salir con el código de resultado de los tests
exit $test_result