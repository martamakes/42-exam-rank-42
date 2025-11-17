#!/bin/bash

# Test script para filter

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${YELLOW}=== TESTING FILTER ===${NC}"

# Limpiar archivos previos
rm -f filter test_output expected_output

# Verificar que existe el archivo fuente
if [ ! -f "filter.c" ]; then
    echo -e "${RED}❌ Error: filter.c no encontrado${NC}"
    exit 1
fi

# VALIDACIÓN DE FUNCIONES PROHIBIDAS
echo -e "${CYAN}Verificando funciones prohibidas...${NC}"
ALLOWED_FUNCS="read|write|strlen|memmem|memmove|malloc|calloc|realloc|free|printf|fprintf|perror"
FORBIDDEN=$(grep -oE '\b[a-z_][a-z0-9_]*\s*\(' filter.c | \
            grep -v "^main\|^ft_\|^get_\|^process_\|^str_\|^buffer_\|^search_\|^replace_\|^filter_" | \
            grep -vE "^(if|for|while|switch|return|sizeof)\s*\(" | \
            grep -vE "^($ALLOWED_FUNCS)\s*\(" | \
            sort -u)

if [ ! -z "$FORBIDDEN" ]; then
    echo -e "${RED}❌ Funciones prohibidas detectadas:${NC}"
    echo "$FORBIDDEN"
    echo -e "${YELLOW}Funciones permitidas: read, write, strlen, memmem, memmove, malloc, calloc, realloc, free, printf, fprintf, perror${NC}"
    exit 1
fi
echo -e "${GREEN}✅ No se detectaron funciones prohibidas${NC}"

# Compilar
echo -e "${YELLOW}Compilando filter.c...${NC}"
if ! gcc -Wall -Wextra -Werror filter.c -o filter 2>/dev/null; then
    echo -e "${RED}❌ Error de compilación${NC}"
    gcc -Wall -Wextra -Werror filter.c -o filter
    exit 1
fi

echo -e "${GREEN}✅ Compilación exitosa${NC}"

# Test 1: Caso básico
echo -e "${YELLOW}Test 1: Reemplazo básico${NC}"
echo "hello world hello" | ./filter "hello" > test_output
echo "***** world *****" > expected_output
if diff test_output expected_output >/dev/null; then
    echo -e "${GREEN}✅ Test 1 pasado${NC}"
else
    echo -e "${RED}❌ Test 1 fallido${NC}"
    echo "Esperado:"
    cat expected_output
    echo "Obtenido:"
    cat test_output
    exit 1
fi

# Test 2: Sin coincidencias
echo -e "${YELLOW}Test 2: Sin coincidencias${NC}"
echo "hello world" | ./filter "xyz" > test_output
echo "hello world" > expected_output
if diff test_output expected_output >/dev/null; then
    echo -e "${GREEN}✅ Test 2 pasado${NC}"
else
    echo -e "${RED}❌ Test 2 fallido${NC}"
    exit 1
fi

# Test 3: Sin argumentos
echo -e "${YELLOW}Test 3: Sin argumentos${NC}"
echo "test" | ./filter 2>/dev/null
if [ $? -eq 1 ]; then
    echo -e "${GREEN}✅ Test 3 pasado${NC}"
else
    echo -e "${RED}❌ Test 3 fallido (debe retornar 1)${NC}"
    exit 1
fi

# Test 4: Múltiples argumentos
echo -e "${YELLOW}Test 4: Múltiples argumentos${NC}"
echo "test" | ./filter "arg1" "arg2" 2>/dev/null
if [ $? -eq 1 ]; then
    echo -e "${GREEN}✅ Test 4 pasado${NC}"
else
    echo -e "${RED}❌ Test 4 fallido (debe retornar 1)${NC}"
    exit 1
fi

# Test 5: Argumento vacío
echo -e "${YELLOW}Test 5: Argumento vacío${NC}"
echo "test" | ./filter "" 2>/dev/null
if [ $? -eq 1 ]; then
    echo -e "${GREEN}✅ Test 5 pasado${NC}"
else
    echo -e "${RED}❌ Test 5 fallido (debe retornar 1)${NC}"
    exit 1
fi

# Test 6: Cadena completa reemplazada
echo -e "${YELLOW}Test 6: Cadena completa${NC}"
echo "test" | ./filter "test" > test_output
echo "****" > expected_output
if diff test_output expected_output >/dev/null; then
    echo -e "${GREEN}✅ Test 6 pasado${NC}"
else
    echo -e "${RED}❌ Test 6 fallido${NC}"
    exit 1
fi

# Test 7: Ejemplo del subject - abc
echo -e "${YELLOW}Test 7: Ejemplo del subject (abc)${NC}"
echo 'abcdefaaaabcdeabcabcdabc' | ./filter abc > test_output
echo '***defaaa***de******d***' > expected_output
if diff test_output expected_output >/dev/null; then
    echo -e "${GREEN}✅ Test 7 pasado${NC}"
else
    echo -e "${RED}❌ Test 7 fallido${NC}"
    echo "Esperado:"
    cat expected_output
    echo "Obtenido:"
    cat test_output
    exit 1
fi

# Test 8: Ejemplo del subject - ababc
echo -e "${YELLOW}Test 8: Ejemplo del subject (ababc)${NC}"
echo 'ababcabababc' | ./filter ababc > test_output
echo '*****ab*****' > expected_output
if diff test_output expected_output >/dev/null; then
    echo -e "${GREEN}✅ Test 8 pasado${NC}"
else
    echo -e "${RED}❌ Test 8 fallido${NC}"
    echo "Esperado:"
    cat expected_output
    echo "Obtenido:"
    cat test_output
    exit 1
fi

# Test 9: Coincidencias solapadas
echo -e "${YELLOW}Test 9: Coincidencias solapadas${NC}"
echo 'aaa' | ./filter aa > test_output
echo '**a' > expected_output
if diff test_output expected_output >/dev/null; then
    echo -e "${GREEN}✅ Test 9 pasado${NC}"
else
    echo -e "${RED}❌ Test 9 fallido${NC}"
    echo "Esperado:"
    cat expected_output
    echo "Obtenido:"
    cat test_output
    exit 1
fi

# Test 10: Texto largo con múltiples coincidencias
echo -e "${YELLOW}Test 10: Texto largo${NC}"
echo 'the quick brown fox jumps over the lazy dog the end' | ./filter "the" > test_output
echo '*** quick brown fox jumps over *** lazy dog *** end' > expected_output
if diff test_output expected_output >/dev/null; then
    echo -e "${GREEN}✅ Test 10 pasado${NC}"
else
    echo -e "${RED}❌ Test 10 fallido${NC}"
    exit 1
fi

# Test 11: Un solo carácter
echo -e "${YELLOW}Test 11: Un solo carácter${NC}"
echo 'aaaabbbbcccc' | ./filter a > test_output
echo '****bbbbcccc' > expected_output
if diff test_output expected_output >/dev/null; then
    echo -e "${GREEN}✅ Test 11 pasado${NC}"
else
    echo -e "${RED}❌ Test 11 fallido${NC}"
    exit 1
fi

# Test 12: Input vacío
echo -e "${YELLOW}Test 12: Input vacío${NC}"
echo -n "" | ./filter "test" > test_output
echo -n "" > expected_output
if diff test_output expected_output >/dev/null; then
    echo -e "${GREEN}✅ Test 12 pasado${NC}"
else
    echo -e "${RED}❌ Test 12 fallido${NC}"
    exit 1
fi

# Limpiar archivos temporales
rm -f test_output expected_output filter

echo -e "${GREEN}🎉 ¡Todos los tests pasados!${NC}"
exit 0