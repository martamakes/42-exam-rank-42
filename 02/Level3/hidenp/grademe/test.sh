#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE="hidenp"
STUDENT_DIR="../../../../rendu/$EXERCISE"

# Verificar directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    exit 1
fi

# Copiar archivos necesarios
cp "$STUDENT_DIR/$EXERCISE.c" .

# Detectar sistema operativo y establecer flags
OS=$(uname -s)
if [ "$OS" = "Linux" ]; then
    CFLAGS="-Wall -Wextra -Werror -D_GNU_SOURCE"
else
    CFLAGS="-Wall -Wextra -Werror"
fi

# Compilar
gcc $CFLAGS "$EXERCISE.c" -o "$EXERCISE"
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación del programa${NC}"
    exit 1
fi

gcc -Wall -Wextra -Werror test_main.c -o test_prog
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación de los tests${NC}"
    exit 1
fi

# Ejecutar tests y guardar resultado
./test_prog
test_result=$?

# Limpiar
rm -f "$EXERCISE" test_prog *.o

# Retornar resultado de los tests
exit $test_result