#!/bin/bash

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
RESET='\033[0m'
BOLD='\033[1m'

# Funciones 
parse_test_output() {
    local output="$1"
    local passed=$(echo "$output" | grep -c "✓")
    local failed=$(echo "$output" | grep -c "✗")
    local total=$(($passed + $failed))
    
    echo "$passed $failed $total"
}

# Verificamos que tengamos microshell
if [ ! -f "microshell" ]; then
    echo -e "${RED}Error: No se encuentra el ejecutable microshell${RESET}"
    echo -e "${YELLOW}Compilando...${RESET}"
    gcc -Wall -Wextra -Werror microshell.c -o microshell
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error de compilación.${RESET}"
        exit 1
    fi
fi

# Ejecutar las pruebas y capturar salida
echo -e "${BLUE}${BOLD}Ejecutando pruebas...${RESET}"
OUTPUT=$(./test.sh 2>&1)

# Análisis de resultados
RESULTS=$(parse_test_output "$OUTPUT")
PASSED=$(echo $RESULTS | cut -d' ' -f1)
FAILED=$(echo $RESULTS | cut -d' ' -f2)
TOTAL=$(echo $RESULTS | cut -d' ' -f3)

# Mostrar resultados
echo -e "${BLUE}${BOLD}===============================${RESET}"
echo -e "${BLUE}${BOLD}       RESULTADOS FINALES      ${RESET}"
echo -e "${BLUE}${BOLD}===============================${RESET}"
echo -e "${GREEN}Pruebas pasadas: ${PASSED}/${TOTAL}${RESET}"
echo -e "${RED}Pruebas fallidas: ${FAILED}/${TOTAL}${RESET}"

# Calcular nota aproximada (cada prueba vale 10 puntos)
SCORE=$((PASSED * 100 / TOTAL))
echo -e "${YELLOW}${BOLD}Nota aproximada: ${SCORE}/100${RESET}"

if [ $SCORE -eq 100 ]; then
    echo -e "${GREEN}${BOLD}¡PERFECTO! Tu implementación pasa todas las pruebas.${RESET}"
elif [ $SCORE -ge 80 ]; then
    echo -e "${GREEN}¡Bien hecho! Tu implementación funciona correctamente en la mayoría de los casos.${RESET}"
elif [ $SCORE -ge 50 ]; then
    echo -e "${YELLOW}Tu implementación funciona parcialmente. Revisa los casos que fallan.${RESET}"
else
    echo -e "${RED}Tu implementación necesita más trabajo. Revisa los requisitos del ejercicio.${RESET}"
fi

exit 0
