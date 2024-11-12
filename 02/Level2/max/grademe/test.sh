#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE="max"
STUDENT_DIR="../../../../rendu"

# Verificar que estamos en el directorio correcto
cd "$TESTDIR"

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
    rm -f "$EXERCISE.c"
    exit 1
fi

# Ejecutar tests
./test_prog
test_result=$?

# Limpiar
rm -f test_prog "$EXERCISE.c"

exit $test_result