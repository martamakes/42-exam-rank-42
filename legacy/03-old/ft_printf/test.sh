#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Directorio actual
CURR_DIR=$(pwd)
STUDENT_DIR="../rendu/ft_printf"

echo -e "${BLUE}=== Prueba de ft_printf ===${NC}\n"

# Verificar si existe el archivo de ft_printf.c
if [ ! -f "$STUDENT_DIR/ft_printf.c" ]; then
    echo -e "${RED}Error: No se encuentra ft_printf.c en $STUDENT_DIR${NC}"
    echo -e "${YELLOW}Asegúrate de crear tu solución en ese directorio.${NC}"
    exit 1
fi

# Compilar la solución del estudiante con el main de prueba
echo -e "${BLUE}Compilando tu solución...${NC}"
gcc -Wall -Wextra -Werror "$STUDENT_DIR/ft_printf.c" "test_main.c" -o "ft_printf_test"

# Verificar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación. Por favor, corrige los errores en tu código.${NC}"
    exit 1
fi

echo -e "${GREEN}Compilación exitosa.${NC}\n"

# Ejecutar las pruebas
echo -e "${BLUE}Ejecutando pruebas...${NC}\n"
./ft_printf_test

# Limpiar
rm -f ft_printf_test

echo -e "\n${BLUE}Pruebas completadas.${NC}"
exit 0