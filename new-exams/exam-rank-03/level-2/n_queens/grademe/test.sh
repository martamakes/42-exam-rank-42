#!/bin/bash

# Test script para n_queens

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}=== TESTING N_QUEENS ===${NC}"

# Limpiar archivos previos
rm -f n_queens

# Verificar que existe el archivo fuente
if [ ! -f "n_queens.c" ]; then
    echo -e "${RED}❌ Error: n_queens.c no encontrado${NC}"
    exit 1
fi

# Compilar
echo -e "${YELLOW}Compilando n_queens.c...${NC}"
if ! gcc -Wall -Wextra -Werror n_queens.c -o n_queens 2>/dev/null; then
    echo -e "${RED}❌ Error de compilación${NC}"
    gcc -Wall -Wextra -Werror n_queens.c -o n_queens
    exit 1
fi

echo -e "${GREEN}✅ Compilación exitosa${NC}"

# Test básico
echo -e "${YELLOW}Test básico: ./n_queens 4${NC}"
./n_queens 4
result=$?

# Limpiar
rm -f n_queens

if [ $result -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡Test ejecutado correctamente!${NC}"
    echo -e "${YELLOW}Nota: Verificar manualmente que las soluciones son válidas${NC}"
    exit 0
else
    echo -e "${RED}❌ Error durante la ejecución${NC}"
    exit 1
fi