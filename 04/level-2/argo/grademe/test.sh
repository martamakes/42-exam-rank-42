#!/bin/bash

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

# Configuración de directorios
EXERCISE="argo"
GRADEME_DIR="$(cd "$(dirname "$0")" && pwd)"
STUDENT_DIR="../../../../rendu/$EXERCISE"

echo -e "${BLUE}🧪 ARGO COMPREHENSIVE TESTER (JSON Parser)${NC}"
echo -e "${BLUE}===========================================${NC}"

# Verificar que existe el directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}❌ Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    echo -e "${YELLOW}💡 Crea tu solución: mkdir -p rendu/argo && cp your_argo.c rendu/argo/argo.c${NC}"
    exit 1
fi

# Verificar que existe el archivo del estudiante
if [ ! -f "$STUDENT_DIR/argo.c" ]; then
    echo -e "${RED}❌ Error: No se encuentra argo.c en $STUDENT_DIR${NC}"
    exit 1
fi

echo -e "${BLUE}📁 Testing solution at: $STUDENT_DIR/argo.c${NC}"
echo ""

# Crear directorio temporal
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

# Copiar archivos necesarios
cp "$STUDENT_DIR"/* "$TEMP_DIR"
cp test_main.c "$TEMP_DIR"

# Ir al directorio temporal
cd "$TEMP_DIR"

# Intentar compilar directamente - que el estudiante vea los errores
echo -e "${BLUE}📦 Compilando...${NC}"
if gcc -Wall -Wextra -Werror argo.c test_main.c -o test_program 2>compile_error.txt; then
    echo -e "${GREEN}✅ Compilación exitosa${NC}"
else
    echo -e "${RED}❌ Error de compilación${NC}"
    echo -e "${YELLOW}Detalles del error:${NC}"
    cat compile_error.txt
    echo
    if grep -q -E "(multiple definition.*main|duplicate symbol.*main)" compile_error.txt; then
        echo -e "${YELLOW}💡 Tienes una función main en tu código. Para este ejercicio, solo implementa la función argo.${NC}"
        echo -e "${YELLOW}💡 Elimina o comenta el main de tu archivo.${NC}"
    else
        echo -e "${YELLOW}💡 Revisa que tu función argo esté correctamente implementada${NC}"
        echo -e "${YELLOW}💡 Asegúrate de que compile con -Wall -Wextra -Werror${NC}"
    fi
    rm -f compile_error.txt
    exit 1
fi

rm -f compile_error.txt

echo -e "${GREEN}✓ Compilación exitosa${NC}"
echo ""

# Ejecutar tests
echo -e "${BLUE}🚀 Ejecutando tests...${NC}"
echo ""

# Ejecutar el programa de test con timeout
timeout 15 ./test_program
test_result=$?

echo ""
if [ $test_result -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡TODOS LOS TESTS PASARON!${NC}"
    echo -e "${GREEN}Tu implementación de argo funciona correctamente.${NC}"
    exit 0
elif [ $test_result -eq 124 ]; then
    echo -e "${RED}⏰ Tests timeout - posible bucle infinito o deadlock${NC}"
    exit 1
else
    echo -e "${RED}❌ Algunos tests fallaron.${NC}"
    echo -e "${YELLOW}💡 Revisa tu implementación y vuelve a intentar.${NC}"
    exit 1
fi