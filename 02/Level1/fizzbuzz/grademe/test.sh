#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

# Función para detectar el sistema operativo
get_os() {
    case "$(uname -s)" in
        Darwin*)    echo 'Darwin';;
        Linux*)     echo 'Linux';;
        *)         echo 'Unknown';;
    esac
}

# Función para ejecutar con timeout compatible con ambos sistemas
run_with_timeout() {
    local timeout_duration=$1
    shift
    local command=("$@")
    
    OS=$(get_os)
    
    if [ "$OS" = "Linux" ]; then
        timeout "$timeout_duration" "${command[@]}"
        return $?
    elif [ "$OS" = "Darwin" ]; then
        perl -e '
            eval {
                local $SIG{ALRM} = sub { die "timeout\n" };
                alarm '"$timeout_duration"';
                system(@ARGV);
                alarm 0;
            };
            if ($@) {
                exit 124 if $@ eq "timeout\n";
                die $@;
            }
            exit $? >> 8;
        ' -- "${command[@]}"
        return $?
    else
        echo "Sistema operativo no soportado"
        exit 1
    fi
}

# Configuración de directorios
EXERCISE="fizzbuzz"
GRADEME_DIR="$(cd "$(dirname "$0")" && pwd)"
EXERCISE_DIR="$(dirname "$GRADEME_DIR")"
LEVEL_DIR="$(dirname "$EXERCISE_DIR")"
EXAM_DIR="$(dirname "$LEVEL_DIR")"
PROJECT_ROOT="$(dirname "$EXAM_DIR")"
RENDU_DIR="$PROJECT_ROOT/rendu/$EXERCISE"
REFERENCE_FILE="$EXERCISE_DIR/$EXERCISE.c"

# Verificación inicial de directorios y archivos
echo "Verificando estructura de directorios..."
echo "GRADEME_DIR: $GRADEME_DIR"
echo "EXERCISE_DIR: $EXERCISE_DIR"
echo "LEVEL_DIR: $LEVEL_DIR"
echo "EXAM_DIR: $EXAM_DIR"
echo "PROJECT_ROOT: $PROJECT_ROOT"
echo "RENDU_DIR: $RENDU_DIR"
echo "REFERENCE_FILE: $REFERENCE_FILE"

if [ ! -d "$RENDU_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio de tu implementación: $RENDU_DIR${NC}"
    echo -e "${YELLOW}Creando directorio...${NC}"
    mkdir -p "$RENDU_DIR"
fi

if [ ! -f "$RENDU_DIR/$EXERCISE.c" ]; then
    echo -e "${RED}Error: No se encuentra tu implementación en: $RENDU_DIR/$EXERCISE.c${NC}"
    echo -e "${YELLOW}Por favor, asegúrate de colocar tu código en: $RENDU_DIR/$EXERCISE.c${NC}"
    exit 1
fi

if [ ! -f "$REFERENCE_FILE" ]; then
    echo -e "${RED}Error: No se encuentra el archivo de referencia en: $REFERENCE_FILE${NC}"
    exit 1
fi

# Crear directorio temporal de trabajo
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

# Copiar archivos necesarios
cp "$RENDU_DIR/$EXERCISE.c" "$TEMP_DIR/student_$EXERCISE.c"
cp "$REFERENCE_FILE" "$TEMP_DIR/reference_$EXERCISE.c"

cd "$TEMP_DIR" || exit 1

# Compilar implementación del estudiante
echo -e "${BLUE}Compilando tu implementación...${NC}"
gcc -Wall -Wextra -Werror "student_$EXERCISE.c" -o "student_prog" 2>student_compile_error.txt
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación en tu código:${NC}"
    cat student_compile_error.txt
    exit 1
fi

# Compilar implementación de referencia
echo -e "${BLUE}Compilando código de referencia...${NC}"
gcc -Wall -Wextra -Werror "reference_$EXERCISE.c" -o "reference_prog" 2>reference_compile_error.txt
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación en el código de referencia:${NC}"
    cat reference_compile_error.txt
    exit 1
fi

# Ejecutar ambos programas y capturar salidas
echo -e "${BLUE}Ejecutando pruebas...${NC}"

# Ejecutar programa del estudiante
run_with_timeout 2 ./student_prog > student_output.txt 2>student_error.txt
student_status=$?
if [ $student_status -eq 124 ]; then
    echo -e "${RED}Error: Tu programa excedió el tiempo límite${NC}"
    exit 1
elif [ $student_status -ne 0 ]; then
    echo -e "${RED}Error: Tu programa terminó con código de error $student_status${NC}"
    cat student_error.txt
    exit 1
fi

# Ejecutar programa de referencia
run_with_timeout 2 ./reference_prog > reference_output.txt 2>reference_error.txt
reference_status=$?
if [ $reference_status -ne 0 ]; then
    echo -e "${RED}Error interno: El programa de referencia falló${NC}"
    exit 1
fi

# Comparar salidas byte a byte
if cmp -s student_output.txt reference_output.txt; then
    echo -e "${GREEN}¡Test pasado! ✓${NC}"
    echo "Tu implementación coincide exactamente con la salida esperada"
    exit 0
else
    echo -e "${RED}Test fallido ✗${NC}"
    echo -e "${YELLOW}Comparación detallada:${NC}"
    
    # Mostrar tamaños de archivo
    student_size=$(wc -c < student_output.txt)
    reference_size=$(wc -c < reference_output.txt)
    echo "Tamaño de tu output: $student_size bytes"
    echo "Tamaño del output esperado: $reference_size bytes"
    
    # Mostrar diferencias en hexadecimal y texto
    echo -e "\nPrimeros bytes de las diferencias:"
    diff -u student_output.txt reference_output.txt | head -n 10
    
    echo -e "\nRepresentación hexadecimal:"
    echo "Tu output:"
    xxd -l 64 student_output.txt
    echo "Output esperado:"
    xxd -l 64 reference_output.txt
    
    exit 1
fi