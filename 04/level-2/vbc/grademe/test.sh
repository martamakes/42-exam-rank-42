#!/bin/bash

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

# Configuración de directorios
EXERCISE="vbc"
GRADEME_DIR="$(cd "$(dirname "$0")" && pwd)"
STUDENT_DIR="../../../../rendu/$EXERCISE"

# Contadores
TESTS_PASSED=0
TESTS_FAILED=0

echo -e "${BLUE}🧪 VBC COMPREHENSIVE TESTER (Math Expression Calculator)${NC}"
echo -e "${BLUE}======================================================${NC}"

# Verificar que existe el directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}❌ Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    echo -e "${YELLOW}💡 Crea tu solución: mkdir -p rendu/vbc && cp your_vbc.c rendu/vbc/vbc.c${NC}"
    exit 1
fi

echo -e "${BLUE}📁 Testing solution at: $STUDENT_DIR${NC}"
echo ""

# Crear directorio temporal
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

# Copiar archivos necesarios
cp -r "$STUDENT_DIR"/* "$TEMP_DIR"

# Ir al directorio temporal
cd "$TEMP_DIR"

# Compilar
echo -e "${BLUE}📦 Compilando vbc...${NC}"
gcc -Wall -Wextra -Werror *.c -o vbc 2>/dev/null

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Error de compilación${NC}"
    echo -e "${YELLOW}💡 Revisa que tu programa vbc compile correctamente${NC}"
    echo -e "${YELLOW}💡 Asegúrate de que compile con -Wall -Wextra -Werror${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Compilación exitosa${NC}"
echo ""

# Función para ejecutar test
run_test() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    local should_fail="$4"
    
    printf "Testing %-35s ... " "$test_name"
    
    local output
    local exit_code
    
    # Ejecutar con timeout
    output=$(timeout 5 ./vbc "$input" 2>&1)
    exit_code=$?
    
    if [ "$should_fail" = "true" ]; then
        # Test should fail
        if [ $exit_code -ne 0 ]; then
            if [[ "$output" == *"$expected"* ]]; then
                echo -e "${GREEN}✓ PASS${NC}"
                ((TESTS_PASSED++))
            else
                echo -e "${RED}✗ FAIL (wrong error message)${NC}"
                echo -e "${RED}  Expected: $expected${NC}"
                echo -e "${RED}  Got: $output${NC}"
                ((TESTS_FAILED++))
            fi
        else
            echo -e "${RED}✗ FAIL (should have failed)${NC}"
            echo -e "${RED}  Expected error but got: $output${NC}"
            ((TESTS_FAILED++))
        fi
    else
        # Test should succeed
        if [ $exit_code -eq 0 ]; then
            if [ "$output" = "$expected" ]; then
                echo -e "${GREEN}✓ PASS${NC}"
                ((TESTS_PASSED++))
            else
                echo -e "${RED}✗ FAIL (wrong output)${NC}"
                echo -e "${RED}  Expected: '$expected'${NC}"
                echo -e "${RED}  Got: '$output'${NC}"
                ((TESTS_FAILED++))
            fi
        else
            echo -e "${RED}✗ FAIL (exit code $exit_code)${NC}"
            echo -e "${RED}  Output: $output${NC}"
            ((TESTS_FAILED++))
        fi
    fi
}

# Ejecutar tests
echo -e "${BLUE}🚀 Ejecutando tests...${NC}"
echo ""

echo -e "${YELLOW}📋 BASIC TESTS:${NC}"
run_test "Simple number" "1" "1"
run_test "Simple addition" "2+3" "5"
run_test "Addition chain" "1+2+3+4+5" "15"

echo ""
echo -e "${YELLOW}🔢 MULTIPLICATION TESTS:${NC}"
run_test "Simple multiplication" "3*4" "12"
run_test "Mult + add (precedence 1)" "3*4+5" "17"
run_test "Add + mult (precedence 2)" "3+4*5" "23"

echo ""
echo -e "${YELLOW}🔧 PARENTHESIS TESTS:${NC}"
run_test "Simple parenthesis" "(1)" "1"
run_test "Nested parenthesis" "(((((((3)))))))" "3"
run_test "Parenthesis precedence" "(3+4)*5" "35"
run_test "Complex parenthesis 1" "(1+2)*3" "9"
run_test "Complex parenthesis 2" "((6*6+7+5+8)*(1+0+4*8+7)+2)+4*(1+2)" "2254"

echo ""
echo -e "${YELLOW}🧮 COMPLEX EXPRESSIONS:${NC}"
run_test "Very nested" "(((((2+2)*2+2)*2+2)*2+2)*2+2)*2" "188"
run_test "Long expression (partial)" "2*4+9+3+2*1+5+1+6+6*1*1+8*0+0+5" "45"
run_test "Very long expr (from subject)" "2*4+9+3+2*1+5+1+6+6*1*1+8*0+0+5+0*4*9*5*8+9*7+5*1+3+1+4*5*7*3+0*3+4*8+8+8+4*0*5*3+5+4+5*7+9+6*6+7+9*2*6*9+2+1*3*7*1*1*5+1+2+7+4+3*4*2+0+4*4*2*2+6+7*5+9+0+8*4+6*7+5+4*4+2+5*5+1+6+3*5*9*9+7*4*3+7+4*9+3+0+1*8+1+2*9*4*5*1+0*1*9+5*3*5+9*6+5*4+5+5*8*6*4*9*2+0+0+1*5*3+6*8*0+0+2*3+7*5*6+8+6*6+9+3+7+0*0+5+2*8+2*7*2+3+9*1*4*8*7*9+2*0+1*6*4*2+8*8*3*1+8+2*4+8*3+8*3+9*5+2*3+9*5*6*4+3*6*6+7+4*8+0+2+9*8*0*6*8*1*2*7+0*5+6*5+0*2+7+2+3+8*7+6+1*3+5+4*5*4*6*1+4*7+9*0+4+9*8+7+5+6+2+6+1+1+1*6*0*9+7+6*2+4*4+1*6*2*9+3+0+0*1*8+4+6*2+6+2*7+7+0*9+6+2*1+6*5*2*3*5*2*6*4+2*9*2*4*5*2*2*3+8+8*3*2*3+0*5+9*6+8+3*1+6*9+8+9*2*0+2" "94305"

echo ""
echo -e "${YELLOW}🛡️  ERROR HANDLING TESTS:${NC}"
run_test "Incomplete expression" "1+" "Unexpected end of input" true
run_test "Extra closing parenthesis" "1+2)" "Unexpected token ')'" true
run_test "Invalid number in parenthesis" "((1+3)*12+(3*(2+6))" "Unexpected token '2'" true

echo ""
echo -e "${YELLOW}📊 RESULTS:${NC}"
TOTAL_TESTS=$((TESTS_PASSED + TESTS_FAILED))
echo -e "Total tests: ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"

echo ""
if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡TODOS LOS TESTS PASARON!${NC}"
    echo -e "${GREEN}Tu programa vbc funciona correctamente.${NC}"
    echo -e "${GREEN}✓ Maneja números simples y operaciones básicas${NC}"
    echo -e "${GREEN}✓ Respeta precedencia de operadores (* antes que +)${NC}"
    echo -e "${GREEN}✓ Procesa paréntesis correctamente${NC}"
    echo -e "${GREEN}✓ Maneja expresiones complejas y anidadas${NC}"
    echo -e "${GREEN}✓ Detecta errores de sintaxis apropiadamente${NC}"
    exit 0
else
    echo -e "${RED}❌ $TESTS_FAILED test(s) fallaron.${NC}"
    echo -e "${YELLOW}💡 Revisa tu implementación del parser de expresiones.${NC}"
    echo -e "${YELLOW}💡 Verifica precedencia de operadores y manejo de paréntesis.${NC}"
    exit 1
fi