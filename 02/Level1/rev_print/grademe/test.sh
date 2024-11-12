#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Directorios
EXERCISE="rev_print"
STUDENT_DIR="../../../../rendu/$EXERCISE"

# Verificar directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    exit 1
fi

# Copiar archivos
cp "$STUDENT_DIR/$EXERCISE.c" .

# Compilar
gcc -Wall -Wextra -Werror test_main.c "$EXERCISE.c" -o test_prog

if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación${NC}"
    rm -f "$EXERCISE.c"
    exit 1
fi

# Ejecutar tests
./test_prog
result=$?

# Limpiar
rm -f test_prog "$EXERCISE.c"

exit $result