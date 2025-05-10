#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

# Directorios
TESTDIR=$(dirname $0)
EXERCISE="get_next_line"
STUDENT_DIR="../../rendu/$EXERCISE"

# Valores de BUFFER_SIZE a probar
BUFFER_SIZES=("1" "5" "42" "9999")

# Mostrar ayuda si se solicita
if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
    echo "Uso: ./test.sh [opciones]"
    echo ""
    echo "Opciones:"
    echo "  --help, -h     Muestra esta ayuda"
    echo "  --valgrind     Ejecuta tests con Valgrind para detectar fugas de memoria"
    echo "  --memcheck     Alias para --valgrind"
    echo "  --buffer=N     Ejecuta tests solo con BUFFER_SIZE=N"
    echo ""
    echo "Ejemplos:"
    echo "  ./test.sh              # Ejecuta tests con todos los BUFFER_SIZE"
    echo "  ./test.sh --valgrind   # Ejecuta con detección de fugas de memoria"
    echo "  ./test.sh --buffer=42  # Ejecuta solo con BUFFER_SIZE=42"
    exit 0
fi

# Verificar si se debe usar valgrind
USE_VALGRIND=0
if [ "$1" == "--valgrind" ] || [ "$1" == "--memcheck" ]; then
    if command -v valgrind &> /dev/null; then
        USE_VALGRIND=1
        echo -e "${BLUE}Modo Valgrind activado${NC}"
    else
        echo -e "${RED}Valgrind no está instalado. Ejecutando tests normales.${NC}"
    fi
fi

# Verificar si se especificó un BUFFER_SIZE específico
CUSTOM_BUFFER=0
CUSTOM_BUFFER_SIZE=""
if [[ "$1" == --buffer=* ]]; then
    CUSTOM_BUFFER=1
    CUSTOM_BUFFER_SIZE="${1#*=}"
    
    if ! [[ "$CUSTOM_BUFFER_SIZE" =~ ^[0-9]+$ ]]; then
        echo -e "${RED}Error: El valor de BUFFER_SIZE debe ser un número entero positivo${NC}"
        exit 1
    fi
    
    echo -e "${BLUE}Usando BUFFER_SIZE=$CUSTOM_BUFFER_SIZE${NC}"
    BUFFER_SIZES=("$CUSTOM_BUFFER_SIZE")
fi

# Información
echo -e "${BLUE}Testing $EXERCISE${NC}"
echo -e "${BLUE}=======================${NC}"

# Verificar directorio del estudiante
if [ ! -d "$STUDENT_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $STUDENT_DIR${NC}"
    echo -e "${BLUE}Asegúrate de que la estructura de directorios es correcta:${NC}"
    echo -e "${BLUE}  - Tu solución debe estar en: $STUDENT_DIR/${NC}"
    
    # Crear el directorio para el estudiante
    echo -e "${BLUE}¿Quieres crear el directorio $STUDENT_DIR? (s/n)${NC}"
    read -n 1 answer
    echo
    
    if [ "$answer" = "s" ] || [ "$answer" = "S" ]; then
        mkdir -p "$STUDENT_DIR"
        echo -e "${GREEN}Directorio creado.${NC}"
        echo -e "${BLUE}Por favor, copia tus get_next_line.c y get_next_line.h en: $STUDENT_DIR/${NC}"
        exit 0
    else
        exit 1
    fi
fi

# Copiar archivos necesarios
cp "$STUDENT_DIR/$EXERCISE.c" .
cp "$STUDENT_DIR/$EXERCISE.h" .

# Verificar si los archivos existen
if [ ! -f "$EXERCISE.c" ]; then
    echo -e "${RED}Error: No se encuentra el archivo $EXERCISE.c en $STUDENT_DIR${NC}"
    exit 1
fi

if [ ! -f "$EXERCISE.h" ]; then
    echo -e "${RED}Error: No se encuentra el archivo $EXERCISE.h en $STUDENT_DIR${NC}"
    exit 1
fi

# Verificar errores totales
total_errors=0
declare -A errors_by_size

# Probar con diferentes BUFFER_SIZE
for size in "${BUFFER_SIZES[@]}"; do
    echo -e "\n${BLUE}Testing with BUFFER_SIZE=$size${NC}"
    echo -e "${BLUE}===========================${NC}\n"
    
    # Compilar los tests
    gcc -Wall -Wextra -Werror -D BUFFER_SIZE=$size test_main.c "$EXERCISE.c" -o test_prog
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error de compilación con BUFFER_SIZE=$size${NC}"
        total_errors=$((total_errors + 1))
        errors_by_size[$size]=1
        continue
    fi
    
    # Ejecutar tests con o sin valgrind
    if [ $USE_VALGRIND -eq 1 ]; then
        echo -e "${BLUE}Ejecutando tests con valgrind...${NC}"
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test_prog
        test_result=$?
        
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}No se detectaron fugas de memoria${NC}"
        else
            echo -e "${RED}Se detectaron fugas de memoria${NC}"
        fi
    else
        # Ejecutar tests
        ./test_prog
        test_result=$?
    fi
    
    total_errors=$((total_errors + test_result))
    errors_by_size[$size]=$test_result
    
    # Limpiar
    rm -f test_prog
done

# Resumen final
echo -e "\n${BLUE}=======================${NC}"
echo -e "${BLUE}    RESUMEN FINAL      ${NC}"
echo -e "${BLUE}=======================${NC}\n"

# Mostrar resultados por BUFFER_SIZE
echo -e "${BLUE}Resultados por BUFFER_SIZE:${NC}"
for size in "${BUFFER_SIZES[@]}"; do
    if [ "${errors_by_size[$size]}" -eq 0 ]; then
        echo -e "  BUFFER_SIZE=$size: ${GREEN}OK ✓${NC}"
    else
        echo -e "  BUFFER_SIZE=$size: ${RED}${errors_by_size[$size]} errores ✗${NC}"
    fi
done

echo -e "\n${BLUE}Resultado global:${NC}"
if [ $total_errors -eq 0 ]; then
    echo -e "${GREEN}Todos los tests pasaron correctamente con todos los BUFFER_SIZE ✓${NC}"
else
    echo -e "${RED}Se encontraron $total_errors errores en total ✗${NC}"
fi

# Limpiar
rm -f "$EXERCISE.c" "$EXERCISE.h"

# Salir con el código de resultado final
exit $total_errors