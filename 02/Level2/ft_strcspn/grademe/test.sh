#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE="ft_strcspn"
STUDENT_DIR="../../../../rendu"
REFERENCE_DIR="../../../../02/Level2"

# Verificar directorio del estudiante
if [ ! -d "$STUDENT_DIR/$EXERCISE" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR/$EXERCISE${NC}"
    exit 1
fi

# Copiar archivos necesarios
cp "$STUDENT_DIR/$EXERCISE/$EXERCISE.c" .

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
rm -f test_prog "$EXERCISE.c"

# Retornar resultado de los tests
exit $test_result