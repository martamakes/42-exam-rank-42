#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE="flood_fill"
STUDENT_DIR="../../../../rendu/$EXERCISE"
REFERENCE_DIR="../../../../02/Level1"

# Verificar directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    exit 1
fi

# Copiar archivos necesarios
cp "$STUDENT_DIR/$EXERCISE.c" .
cp "$STUDENT_DIR/$EXERCISE.h" .

# Compilar
gcc -Wall -Wextra -Werror test_main.c "$EXERCISE.c" -o test_prog

if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación${NC}"
    exit 1
fi

# Ejecutar tests y guardar resultado
./test_prog
test_result=$?

# Limpiar
rm -f test_prog "$EXERCISE.c" "$EXERCISE.h"

# Retornar resultado de los tests
exit $test_result