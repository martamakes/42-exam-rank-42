#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE=fizzbuzz
STUDENT_DIR="../../../rendu/$EXERCISE"

# Verificar que existe el directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    exit 1
fi

# Copiar archivos necesarios
cp $STUDENT_DIR/* .

# Compilar el programa del estudiante
gcc -Wall -Wextra -Werror fizzbuzz.c -o fizzbuzz
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación del programa${NC}"
    exit 1
fi

# Compilar y ejecutar los tests
gcc -Wall -Wextra -Werror test_main.c -o test_prog
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación de los tests${NC}"
    exit 1
fi

# Ejecutar tests
./test_prog
result=$?

# Limpiar
rm -f fizzbuzz test_prog *.o

exit $result