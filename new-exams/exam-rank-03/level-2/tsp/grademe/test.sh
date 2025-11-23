#!/bin/bash

# Test script para tsp

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}=== TESTING TSP ===${NC}"

# Limpiar archivos previos
rm -f tsp

# Verificar que existe el archivo fuente
if [ ! -f "tsp.c" ]; then
    echo -e "${RED}❌ Error: tsp.c no encontrado${NC}"
    exit 1
fi

# Compilar
echo -e "${YELLOW}Compilando tsp.c...${NC}"
if ! gcc -Wall -Wextra -Werror tsp.c -o tsp -lm 2>/dev/null; then
    echo -e "${RED}❌ Error de compilación${NC}"
    gcc -Wall -Wextra -Werror tsp.c -o tsp -lm
    exit 1
fi

echo -e "${GREEN}✅ Compilación exitosa${NC}"

# Test básico
echo -e "${YELLOW}Test básico: ./tsp 0 0 1 1 2 0${NC}"
./tsp 0 0 1 1 2 0
result=$?

# Limpiar
rm -f tsp

if [ $result -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡Test ejecutado correctamente!${NC}"
    echo -e "${YELLOW}Nota: Verificar manualmente que el resultado es correcto${NC}"
    exit 0
else
    echo -e "${RED}❌ Error durante la ejecución${NC}"
    exit 1
fi