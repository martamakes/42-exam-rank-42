#!/bin/bash

# Test script para powerset

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}=== TESTING POWERSET ===${NC}"

# Limpiar archivos previos
rm -f powerset

# Verificar que existe el archivo fuente
if [ ! -f "powerset.c" ]; then
    echo -e "${RED}❌ Error: powerset.c no encontrado${NC}"
    exit 1
fi

# Compilar
echo -e "${YELLOW}Compilando powerset.c...${NC}"
if ! gcc -Wall -Wextra -Werror powerset.c -o powerset 2>/dev/null; then
    echo -e "${RED}❌ Error de compilación${NC}"
    gcc -Wall -Wextra -Werror powerset.c -o powerset
    exit 1
fi

echo -e "${GREEN}✅ Compilación exitosa${NC}"

# Test básico
echo -e "${YELLOW}Test básico: ./powerset 1 2 3${NC}"
./powerset 1 2 3
result=$?

# Limpiar
rm -f powerset

if [ $result -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡Test ejecutado correctamente!${NC}"
    echo -e "${YELLOW}Nota: Verificar manualmente que los subconjuntos están ordenados${NC}"
    exit 0
else
    echo -e "${RED}❌ Error durante la ejecución${NC}"
    exit 1
fi