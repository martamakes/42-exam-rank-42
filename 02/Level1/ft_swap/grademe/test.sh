#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE="ft_swap"
STUDENT_DIR="../../../../rendu/$EXERCISE"

# Verificar que existe el directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    exit 1
fi

# Copiar archivos necesarios
cp "$STUDENT_DIR/$EXERCISE.c" .

# Compilar el programa del estudiante con el test
gcc -Wall -Wextra -Werror test_main.c ft_swap.c -o test_prog
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación${NC}"
    exit 1
fi

# Ejecutar tests
./test_prog
result=$?

# Limpiar
rm -f test_prog *.o ft_swap.c

exit $result