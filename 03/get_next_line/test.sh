#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Directorio actual
CURR_DIR=$(pwd)
STUDENT_DIR="../rendu/get_next_line"

echo -e "${BLUE}=== Prueba de get_next_line ===${NC}\n"

# Verificar si existen los archivos necesarios
if [ ! -f "$STUDENT_DIR/get_next_line.c" ] || [ ! -f "$STUDENT_DIR/get_next_line.h" ]; then
    echo -e "${RED}Error: No se encuentran los archivos get_next_line.c y/o get_next_line.h en $STUDENT_DIR${NC}"
    echo -e "${YELLOW}Asegúrate de crear tu solución en ese directorio.${NC}"
    exit 1
fi

# Compilar la solución del estudiante con el main de prueba
echo -e "${BLUE}Compilando tu solución...${NC}"
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 "$STUDENT_DIR/get_next_line.c" "test_main.c" -I"$STUDENT_DIR" -o "gnl_test"

# Verificar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación. Por favor, corrige los errores en tu código.${NC}"
    exit 1
fi

echo -e "${GREEN}Compilación exitosa.${NC}\n"

# Ejecutar las pruebas
echo -e "${BLUE}Ejecutando pruebas...${NC}\n"
./gnl_test

# Limpiar
rm -f gnl_test

echo -e "\n${BLUE}Pruebas completadas.${NC}"
exit 0