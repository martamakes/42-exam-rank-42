#!/bin/bash

# Test script para ft_scanf

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${YELLOW}=== TESTING FT_SCANF ===${NC}"

# Limpiar archivos previos
rm -f ft_scanf test_program test_main.c test*.txt

# Verificar que existe el archivo fuente
if [ ! -f "ft_scanf.c" ]; then
    echo -e "${RED}❌ Error: ft_scanf.c no encontrado${NC}"
    exit 1
fi

# VALIDACIÓN DE FUNCIONES PROHIBIDAS
echo -e "${CYAN}Verificando funciones prohibidas...${NC}"
ALLOWED_FUNCS="fgetc|ungetc|ferror|feof|isspace|isdigit|va_start|va_arg|va_copy|va_end"
FORBIDDEN=$(grep -oE '\b[a-z_][a-z0-9_]*\s*\(' ft_scanf.c | \
            grep -v "^main\|^ft_\|^match_\|^scan_\|^stdin" | \
            grep -vE "^($ALLOWED_FUNCS)\s*\(" | \
            sort -u)

if [ ! -z "$FORBIDDEN" ]; then
    echo -e "${RED}❌ Funciones prohibidas detectadas:${NC}"
    echo "$FORBIDDEN"
    echo -e "${YELLOW}Funciones permitidas: fgetc, ungetc, ferror, feof, isspace, isdigit, stdin, va_start, va_arg, va_copy, va_end${NC}"
    exit 1
fi
echo -e "${GREEN}✅ No se detectaron funciones prohibidas${NC}"

# Variables para contar tests
PASSED=0
TOTAL=0

# Función para ejecutar un test
run_test() {
    local test_name="$1"
    local input="$2"
    local format="$3"
    local expected_result="$4"
    local expected_output="$5"

    TOTAL=$((TOTAL + 1))
    echo -e "${YELLOW}Test $TOTAL: $test_name${NC}"

    # Crear archivo de test
    cat > test_main.c << EOF
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ft_scanf(const char *format, ...);

int main(void)
{
    char str[100];
    int num1, num2;
    char c;
    int result;

    // Crear archivo temporal con input
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "$input");
    fclose(f);
    freopen("test_input.txt", "r", stdin);

    // Ejecutar ft_scanf
    result = ft_scanf("$format", $expected_output);

    // Imprimir resultado
    printf("%d", result);

    remove("test_input.txt");
    return 0;
}
EOF

    # Compilar
    if ! gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null; then
        echo -e "${RED}❌ Error de compilación en test${NC}"
        rm -f test_main.c
        return 1
    fi

    # Ejecutar y capturar resultado
    actual_result=$(./test_program 2>/dev/null)

    if [ "$actual_result" == "$expected_result" ]; then
        echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}❌ Test $TOTAL fallido${NC}"
        echo "  Input: '$input'"
        echo "  Format: '$format'"
        echo "  Esperado: $expected_result"
        echo "  Obtenido: $actual_result"
        return 1
    fi
}

# Test 1: String simple
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    char str[100];
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "Hello");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%s", str);
    printf("%d:%s", result, str);
    remove("test_input.txt");
    return 0;
}
EOF

echo -e "${YELLOW}Compilando ft_scanf.c...${NC}"
if ! gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null; then
    echo -e "${RED}❌ Error de compilación${NC}"
    gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program
    rm -f test_main.c
    exit 1
fi

echo -e "${GREEN}✅ Compilación exitosa${NC}"
echo

# Test 1: String simple
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: String simple${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "1:Hello" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: 1:Hello, obtenido: $output)${NC}"
fi

# Test 2: Integer positivo
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    int num;
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "42");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%d", &num);
    printf("%d:%d", result, num);
    remove("test_input.txt");
    return 0;
}
EOF
gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null

TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Integer positivo${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "1:42" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: 1:42, obtenido: $output)${NC}"
fi

# Test 3: Integer negativo
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    int num;
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "-123");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%d", &num);
    printf("%d:%d", result, num);
    remove("test_input.txt");
    return 0;
}
EOF
gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null

TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Integer negativo${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "1:-123" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: 1:-123, obtenido: $output)${NC}"
fi

# Test 4: Character
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    char c;
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "A");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%c", &c);
    printf("%d:%c", result, c);
    remove("test_input.txt");
    return 0;
}
EOF
gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null

TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Character${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "1:A" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: 1:A, obtenido: $output)${NC}"
fi

# Test 5: Múltiples valores (string y número)
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    char str[100];
    int num;
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "Hello 42");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%s %d", str, &num);
    printf("%d:%s:%d", result, str, num);
    remove("test_input.txt");
    return 0;
}
EOF
gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null

TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: String + Integer${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "2:Hello:42" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: 2:Hello:42, obtenido: $output)${NC}"
fi

# Test 6: Espacios en blanco antes de string
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    char str[100];
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "   World");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%s", str);
    printf("%d:%s", result, str);
    remove("test_input.txt");
    return 0;
}
EOF
gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null

TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Espacios antes de string${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "1:World" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: 1:World, obtenido: $output)${NC}"
fi

# Test 7: EOF inmediato
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    int num;
    FILE *f = fopen("test_input.txt", "w");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%d", &num);
    printf("%d", result);
    remove("test_input.txt");
    return 0;
}
EOF
gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null

TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: EOF inmediato${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "-1" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: -1, obtenido: $output)${NC}"
fi

# Test 8: Integer con espacios
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    int num;
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "   99");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%d", &num);
    printf("%d:%d", result, num);
    remove("test_input.txt");
    return 0;
}
EOF
gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null

TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Integer con espacios${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "1:99" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: 1:99, obtenido: $output)${NC}"
fi

# Test 9: Character con espacios (NO debe saltarlos)
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    char c;
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "   B");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%c", &c);
    printf("%d:%d", result, (int)c);
    remove("test_input.txt");
    return 0;
}
EOF
gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null

TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Character con espacios (NO saltarlos)${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "1:32" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: 1:32 [espacio], obtenido: $output)${NC}"
fi

# Test 10: Cero
cat > test_main.c << 'EOF'
#include <stdio.h>
int ft_scanf(const char *format, ...);
int main(void) {
    int num;
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "0");
    fclose(f);
    freopen("test_input.txt", "r", stdin);
    int result = ft_scanf("%d", &num);
    printf("%d:%d", result, num);
    remove("test_input.txt");
    return 0;
}
EOF
gcc -Wall -Wextra -Werror test_main.c ft_scanf.c -o test_program 2>/dev/null

TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Cero${NC}"
output=$(./test_program 2>/dev/null)
if [ "$output" == "1:0" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (esperado: 1:0, obtenido: $output)${NC}"
fi

# Limpiar archivos temporales
rm -f test_program test_main.c test*.txt

# Resultado final
echo
echo -e "${CYAN}======================================${NC}"
echo -e "${CYAN}Tests pasados: $PASSED/$TOTAL${NC}"
echo -e "${CYAN}======================================${NC}"

if [ $PASSED -eq $TOTAL ]; then
    echo -e "${GREEN}🎉 ¡Todos los tests pasados!${NC}"
    exit 0
else
    echo -e "${RED}❌ Algunos tests fallaron${NC}"
    exit 1
fi
