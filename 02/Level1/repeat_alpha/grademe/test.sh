#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Directorios base (relativos a donde está el script)
STUDENT_DIR="../../../../rendu"
REFERENCE_DIR="../../../../02/Level1"
EXERCISE="repeat_alpha"

# Rutas completas a los archivos
STUDENT_FILE="$STUDENT_DIR/$EXERCISE/$EXERCISE.c"
REFERENCE_FILE="$REFERENCE_DIR/$EXERCISE/$EXERCISE.c"

# Verificar directorio del estudiante
if [ ! -f "$STUDENT_FILE" ]; then
    echo -e "${RED}Error: No se encuentra el archivo del estudiante en $STUDENT_FILE${NC}"
    exit 1
fi

# Verificar que existe el código de referencia
if [ ! -f "$REFERENCE_FILE" ]; then
    echo -e "${RED}Error: No se encuentra el código de referencia en $REFERENCE_FILE${NC}"
    exit 1
fi

# Copiar archivos necesarios
cp "$STUDENT_FILE" .
cp "$REFERENCE_FILE" "./reference_$EXERCISE.c"

# Compilar el programa del estudiante
gcc -Wall -Wextra -Werror "$EXERCISE.c" -o "$EXERCISE"
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación del programa del estudiante${NC}"
    exit 1
fi

# Compilar el programa de referencia
gcc -Wall -Wextra -Werror "reference_$EXERCISE.c" -o "reference_$EXERCISE"
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación del programa de referencia${NC}"
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
test_result=$?

# Limpiar
rm -f "$EXERCISE" test_prog "reference_$EXERCISE" "reference_$EXERCISE.c" "$EXERCISE.c"

exit $test_result