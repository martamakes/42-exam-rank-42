#!/bin/bash

# Test script para rip

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}=== TESTING RIP ===${NC}"

# Limpiar archivos previos
rm -f rip

# Verificar que existe el archivo fuente
if [ ! -f "rip.c" ]; then
    echo -e "${RED}❌ Error: rip.c no encontrado${NC}"
    exit 1
fi

# Compilar
echo -e "${YELLOW}Compilando rip.c...${NC}"
if ! gcc -Wall -Wextra -Werror rip.c -o rip 2>/dev/null; then
    echo -e "${RED}❌ Error de compilación${NC}"
    gcc -Wall -Wextra -Werror rip.c -o rip
    exit 1
fi

echo -e "${GREEN}✅ Compilación exitosa${NC}"

# Test básico
echo -e "${YELLOW}Test básico: ./rip (()((()${NC}"
./rip "(()((())"
result=$?

# Limpiar
rm -f rip

if [ $result -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡Test ejecutado correctamente!${NC}"
    echo -e "${YELLOW}Nota: Verificar manualmente que las soluciones son correctas${NC}"
    exit 0
else
    echo -e "${RED}❌ Error durante la ejecución${NC}"
    exit 1
fi