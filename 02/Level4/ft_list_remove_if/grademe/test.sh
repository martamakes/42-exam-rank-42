#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE="ft_list_remove_if"
STUDENT_DIR="../../../../rendu/$EXERCISE"

# Verificar directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    exit 1
fi

# Copiar archivos necesarios
cp "$STUDENT_DIR/$EXERCISE.c" .
cp "$STUDENT_DIR/ft_list.h" .

# Compilar
echo -e "${BLUE}Compilando...${NC}"
gcc test_main.c "$EXERCISE.c" -o test_prog 2>compile_error.txt

if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación:${NC}"
    cat compile_error.txt
    exit 1
fi

# Ejecutar tests
echo -e "${BLUE}Ejecutando tests...${NC}"
./test_prog
test_result=$?

# Limpiar
rm -f test_prog *.o

exit $test_result