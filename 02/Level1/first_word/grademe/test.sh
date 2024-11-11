#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE=first_word
STUDENT_DIR="../../../rendu/$EXERCISE"

# Verificar que existe el directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    exit 1
fi

# Copiar archivos necesarios
cp $STUDENT_DIR/* .

# Compilar programa
gcc -Wall -Wextra -Werror *.c -o $EXERCISE
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación${NC}"
    exit 1
fi

# Ejecutar tests
./test_main
result=$?

# Limpiar
rm -f $EXERCISE *.o

exit $result
