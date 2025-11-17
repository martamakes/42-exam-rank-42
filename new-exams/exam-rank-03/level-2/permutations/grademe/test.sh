#!/bin/bash

# Test script para permutations - Verificación exhaustiva tipo examen

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

PASSED=0
FAILED=0

echo -e "${YELLOW}=== TESTING PERMUTATIONS ===${NC}\n"

# Limpiar archivos previos
rm -f permutations

# Verificar que existe el archivo fuente
if [ ! -f "permutations.c" ]; then
    echo -e "${RED}❌ Error: permutations.c no encontrado${NC}"
    exit 1
fi

# ===================================================================
# 1. VERIFICAR FUNCIONES PROHIBIDAS
# ===================================================================
echo -e "${YELLOW}[1] Verificando funciones prohibidas...${NC}"
FORBIDDEN_FOUND=0

if grep -q "printf\|fprintf\|sprintf" permutations.c 2>/dev/null; then
    echo -e "${RED}❌ Función prohibida detectada (printf/fprintf/sprintf)${NC}"
    FORBIDDEN_FOUND=1
fi

if grep -q "\bstrlen\s*(" permutations.c 2>/dev/null; then
    if ! grep -q "ft_strlen\|int.*strlen.*{" permutations.c 2>/dev/null; then
        echo -e "${RED}❌ Usando strlen sin implementar${NC}"
        FORBIDDEN_FOUND=1
    fi
fi

if [ $FORBIDDEN_FOUND -eq 0 ]; then
    echo -e "${GREEN}✅ No hay funciones prohibidas${NC}"
    ((PASSED++))
else
    echo -e "${RED}Solo permitidas: puts, malloc, calloc, realloc, free, write${NC}"
    ((FAILED++))
fi

# ===================================================================
# 2. COMPILACIÓN
# ===================================================================
echo -e "\n${YELLOW}[2] Compilando con flags estrictos...${NC}"
if gcc -Wall -Wextra -Werror permutations.c -o permutations 2>compile_errors.txt; then
    echo -e "${GREEN}✅ Compilación exitosa${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ Error de compilación${NC}"
    cat compile_errors.txt
    rm -f compile_errors.txt
    exit 1
fi
rm -f compile_errors.txt

# ===================================================================
# 3. TEST: Un carácter
# ===================================================================
echo -e "\n${YELLOW}[3] Test: ./permutations a${NC}"
OUTPUT=$(./permutations a)
EXPECTED="a"
if [ "$OUTPUT" = "$EXPECTED" ]; then
    echo -e "${GREEN}✅ Correcto${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ Incorrecto${NC}"
    echo "Esperado: $EXPECTED"
    echo "Obtenido: $OUTPUT"
    ((FAILED++))
fi

# ===================================================================
# 4. TEST: Dos caracteres
# ===================================================================
echo -e "\n${YELLOW}[4] Test: ./permutations ab${NC}"
OUTPUT=$(./permutations ab)
EXPECTED="ab
ba"
if [ "$OUTPUT" = "$EXPECTED" ]; then
    echo -e "${GREEN}✅ Correcto${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ Incorrecto${NC}"
    echo -e "Esperado:\n$EXPECTED"
    echo -e "Obtenido:\n$OUTPUT"
    ((FAILED++))
fi

# ===================================================================
# 5. TEST: Tres caracteres (ejemplo del subject)
# ===================================================================
echo -e "\n${YELLOW}[5] Test: ./permutations abc${NC}"
OUTPUT=$(./permutations abc)
EXPECTED="abc
acb
bac
bca
cab
cba"
if [ "$OUTPUT" = "$EXPECTED" ]; then
    echo -e "${GREEN}✅ Correcto (orden alfabético)${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ Incorrecto o no está en orden alfabético${NC}"
    echo -e "Esperado:\n$EXPECTED"
    echo -e "Obtenido:\n$OUTPUT"
    ((FAILED++))
fi

# ===================================================================
# 6. TEST: Orden alfabético con letras no consecutivas
# ===================================================================
echo -e "\n${YELLOW}[6] Test: ./permutations acd${NC}"
OUTPUT=$(./permutations acd)
EXPECTED="acd
adc
cad
cda
dac
dca"
if [ "$OUTPUT" = "$EXPECTED" ]; then
    echo -e "${GREEN}✅ Correcto${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ Incorrecto${NC}"
    echo -e "Esperado:\n$EXPECTED"
    echo -e "Obtenido:\n$OUTPUT"
    ((FAILED++))
fi

# ===================================================================
# 7. TEST: Cuatro caracteres (24 permutaciones)
# ===================================================================
echo -e "\n${YELLOW}[7] Test: ./permutations abcd (4! = 24 permutaciones)${NC}"
OUTPUT=$(./permutations abcd)
LINES=$(echo "$OUTPUT" | wc -l)
if [ "$LINES" -eq 24 ]; then
    FIRST=$(echo "$OUTPUT" | head -n 1)
    LAST=$(echo "$OUTPUT" | tail -n 1)
    if [ "$FIRST" = "abcd" ] && [ "$LAST" = "dcba" ]; then
        echo -e "${GREEN}✅ Correcto (24 permutaciones, primera: abcd, última: dcba)${NC}"
        ((PASSED++))
    else
        echo -e "${RED}❌ Número correcto pero orden incorrecto${NC}"
        echo "Primera: $FIRST (esperado: abcd)"
        echo "Última: $LAST (esperado: dcba)"
        ((FAILED++))
    fi
else
    echo -e "${RED}❌ Número incorrecto de permutaciones${NC}"
    echo "Esperado: 24, Obtenido: $LINES"
    ((FAILED++))
fi

# ===================================================================
# 8. TEST: Mayúsculas (orden ASCII)
# ===================================================================
echo -e "\n${YELLOW}[8] Test: ./permutations Abc (orden ASCII)${NC}"
OUTPUT=$(./permutations Abc)
FIRST=$(echo "$OUTPUT" | head -n 1)
if [ "$FIRST" = "Abc" ]; then
    echo -e "${GREEN}✅ Correcto (respeta ASCII: A < b < c)${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ No respeta orden ASCII${NC}"
    echo "Primera línea: $FIRST (esperado: Abc)"
    ((FAILED++))
fi

# ===================================================================
# 9. TEST: Sin argumentos
# ===================================================================
echo -e "\n${YELLOW}[9] Test: ./permutations (sin argumentos)${NC}"
OUTPUT=$(./permutations)
if [ "$OUTPUT" = "" ]; then
    echo -e "${GREEN}✅ Correcto (no crashea)${NC}"
    ((PASSED++))
else
    echo -e "${YELLOW}⚠️  Imprime algo (aceptable si es solo \\n)${NC}"
    ((PASSED++))
fi

# ===================================================================
# 10. TEST: Múltiples argumentos
# ===================================================================
echo -e "\n${YELLOW}[10] Test: ./permutations abc xyz (múltiples args)${NC}"
OUTPUT=$(./permutations abc xyz)
if [ "$OUTPUT" = "" ]; then
    echo -e "${GREEN}✅ Correcto (ignora args extras)${NC}"
    ((PASSED++))
else
    echo -e "${YELLOW}⚠️  Imprime algo (revisar comportamiento)${NC}"
    ((PASSED++))
fi

# ===================================================================
# 11. TEST: Números
# ===================================================================
echo -e "\n${YELLOW}[11] Test: ./permutations 123${NC}"
OUTPUT=$(./permutations 123)
EXPECTED="123
132
213
231
312
321"
if [ "$OUTPUT" = "$EXPECTED" ]; then
    echo -e "${GREEN}✅ Correcto${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ Incorrecto${NC}"
    echo -e "Esperado:\n$EXPECTED"
    echo -e "Obtenido:\n$OUTPUT"
    ((FAILED++))
fi

# ===================================================================
# 12. TEST: Memory leaks (si valgrind está disponible)
# ===================================================================
echo -e "\n${YELLOW}[12] Test: Memory leaks (valgrind)${NC}"
if command -v valgrind &> /dev/null; then
    valgrind --leak-check=full --error-exitcode=1 --quiet ./permutations abc > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ Sin memory leaks${NC}"
        ((PASSED++))
    else
        echo -e "${RED}❌ Memory leaks detectados${NC}"
        ((FAILED++))
    fi
else
    echo -e "${YELLOW}⚠️  Valgrind no disponible (test omitido)${NC}"
fi

# ===================================================================
# RESUMEN
# ===================================================================
echo -e "\n${YELLOW}================================${NC}"
echo -e "${YELLOW}RESUMEN${NC}"
echo -e "${YELLOW}================================${NC}"
TOTAL=$((PASSED + FAILED))
echo -e "Tests pasados: ${GREEN}$PASSED${NC}/$TOTAL"

# Limpiar
rm -f permutations

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡TODOS LOS TESTS PASARON!${NC}"
    exit 0
else
    echo -e "${RED}❌ $FAILED tests fallaron${NC}"
    exit 1
fi