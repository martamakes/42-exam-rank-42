#!/bin/bash

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Configuración de directorios
EXERCISE="repeat_alpha"
TESTDIR=$(dirname $0)
STUDENT_DIR="../../../../rendu/$EXERCISE"

# Verificar que existe el directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    exit 1
fi

# Crear directorio temporal
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

# Copiar archivos necesarios al directorio temporal
cp "$STUDENT_DIR"/* "$TEMP_DIR"
cp "test_main.c" "$TEMP_DIR"
cd "$TEMP_DIR"

# Compilar el programa del estudiante
echo "Compilando repeat_alpha..."
gcc -Wall -Wextra -Werror "$TEMP_DIR/repeat_alpha.c" -o repeat_alpha
if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Fallo en la compilación del programa${NC}"
    exit 1
fi

# Compilar el programa de test
echo "Compilando tests..."
gcc -Wall -Wextra -Werror test_main.c -o test_prog
if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Fallo en la compilación de los tests${NC}"
    exit 1
fi

# Ejecutar los tests
echo "Ejecutando tests..."
./test_prog
result=$?

# Salir con el código de resultado de los tests
exit $result