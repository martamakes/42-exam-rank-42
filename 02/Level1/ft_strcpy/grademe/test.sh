#!/bin/bash

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

# Configuración de directorios
EXERCISE="ft_strcpy"
GRADEME_DIR="$(cd "$(dirname "$0")" && pwd)"
STUDENT_DIR="../../../../rendu/$EXERCISE"

# Verificar que existe el directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    exit 1
fi

# Crear directorio temporal
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

# Copiar archivos necesarios
cp "$STUDENT_DIR"/* "$TEMP_DIR"
cp test_main.c "$TEMP_DIR"

# Ir al directorio temporal
cd "$TEMP_DIR"

# Compilar
echo -e "${BLUE}Compilando...${NC}"
gcc -Wall -Wextra -Werror test_main.c ft_strcpy.c -o test_program

if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación${NC}"
    exit 1
fi

# Ejecutar tests
echo -e "${BLUE}Ejecutando tests...${NC}"
./test_program

# Verificar memory leaks si estamos en Linux
if [ "$(uname)" = "Linux" ]; then
    echo -e "\n${BLUE}Verificando memory leaks...${NC}"
    valgrind --leak-check=full ./test_program 2>&1 | grep -E "definitely lost|indirectly lost|possibly lost"
fi

exit 0