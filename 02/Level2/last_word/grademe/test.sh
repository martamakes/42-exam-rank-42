#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE="last_word"
STUDENT_DIR="../../../../rendu"
REFERENCE_DIR="../../../../02/Level1"

# Verificar directorio del estudiante
if [ ! -d "$STUDENT_DIR/$EXERCISE" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR/$EXERCISE${NC}"
    exit 1
fi

# Copiar archivos necesarios
cp "$STUDENT_DIR/$EXERCISE/$EXERCISE.c" .

# Compilar tests y programa
gcc -Wall -Wextra -Werror test_main.c -o test_prog
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación de los tests${NC}"
    exit 1
fi

gcc -Wall -Wextra -Werror "$EXERCISE.c" -o $EXERCISE
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación del programa${NC}"
    rm -f test_prog
    exit 1
fi

# Ejecutar tests
./test_prog
test_result=$?

# Limpiar
rm -f test_prog $EXERCISE *.o

exit $test_result