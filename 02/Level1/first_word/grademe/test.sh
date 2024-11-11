#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

# Configuración de directorios
EXERCISE="first_word"
GRADEME_DIR="$(cd "$(dirname "$0")" && pwd)"
EXERCISE_DIR="$(dirname "$GRADEME_DIR")"
LEVEL_DIR="$(dirname "$EXERCISE_DIR")"
EXAM_DIR="$(dirname "$LEVEL_DIR")"
PROJECT_ROOT="$(dirname "$EXAM_DIR")"
RENDU_DIR="$PROJECT_ROOT/rendu/$EXERCISE"
REFERENCE_FILE="$EXERCISE_DIR/$EXERCISE.c"

# Verificación inicial de directorios y archivos
echo "Verificando estructura de directorios..."
echo "GRADEME_DIR: $GRADEME_DIR"
echo "EXERCISE_DIR: $EXERCISE_DIR"
echo "LEVEL_DIR: $LEVEL_DIR"
echo "EXAM_DIR: $EXAM_DIR"
echo "PROJECT_ROOT: $PROJECT_ROOT"
echo "RENDU_DIR: $RENDU_DIR"
echo "REFERENCE_FILE: $REFERENCE_FILE"

if [ ! -d "$RENDU_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio de tu implementación: $RENDU_DIR${NC}"
    echo -e "${YELLOW}Creando directorio...${NC}"
    mkdir -p "$RENDU_DIR"
fi

if [ ! -f "$RENDU_DIR/$EXERCISE.c" ]; then
    echo -e "${RED}Error: No se encuentra tu implementación en: $RENDU_DIR/$EXERCISE.c${NC}"
    echo -e "${YELLOW}Por favor, asegúrate de colocar tu código en: $RENDU_DIR/$EXERCISE.c${NC}"
    exit 1
fi

# Crear directorio temporal de trabajo
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

# Copiar archivos necesarios
cp "$RENDU_DIR/$EXERCISE.c" "$TEMP_DIR/"
cp "$GRADEME_DIR/test_main.c" "$TEMP_DIR/"

cd "$TEMP_DIR" || exit 1

# Compilar el programa del estudiante
echo -e "${BLUE}Compilando tu implementación...${NC}"
gcc -Wall -Wextra -Werror "$EXERCISE.c" -o "$EXERCISE" 2>compile_error.txt
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación en tu código:${NC}"
    cat compile_error.txt
    exit 1
fi

# Compilar y ejecutar los tests
echo -e "${BLUE}Compilando tests...${NC}"
gcc -Wall -Wextra -Werror test_main.c -o test_prog 2>test_compile_error.txt
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación de los tests:${NC}"
    cat test_compile_error.txt
    exit 1
fi

# Ejecutar tests
echo -e "${BLUE}Ejecutando tests...${NC}"
./test_prog
result=$?

if [ $result -eq 0 ]; then
    echo -e "${GREEN}¡Todos los tests pasaron! ✓${NC}"
else
    echo -e "${RED}Los tests han fallado ✗${NC}"
fi

exit $result