#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

RENDU_PATH="../rendu/cpp_module_00"
TEST_NAME="cpp_module_00"
EXPECTED_OUTPUT="Richard: This looks like another boring day.\nRichard: I am Richard, Mistress of Magma!\nRichard - Mistress of Magma\nJack: This looks like another boring day.\nJack: I am Jack, the Long!\nJack: I am Jack, the Mighty!\nJack: My job here is done!\nRichard: My job here is done!"

# Verificar existencia de archivos necesarios
if [ ! -f "$RENDU_PATH/Warlock.cpp" ] || [ ! -f "$RENDU_PATH/Warlock.hpp" ]; then
    echo -e "${RED}Error: Faltan archivos requeridos (Warlock.cpp y/o Warlock.hpp)${NC}"
    exit 1
fi

# Copiar main.cpp al directorio de respuestas
cp main.cpp "$RENDU_PATH/"

# Compilar
echo -e "${YELLOW}Compilando $TEST_NAME...${NC}"
cd "$RENDU_PATH"
g++ -Wall -Wextra -Werror -W -std=c++98 *.cpp -o "$TEST_NAME"
if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación${NC}"
    exit 1
fi
echo -e "${GREEN}Compilación exitosa${NC}"

# Ejecutar el programa y capturar la salida
echo -e "${YELLOW}Ejecutando prueba para $TEST_NAME...${NC}"
OUTPUT=$(./"$TEST_NAME" | tr -d '\r')

# Reemplazar saltos de línea literales por saltos reales en la salida esperada
EXPECTED_OUTPUT=$(echo -e "$EXPECTED_OUTPUT")

# Comparar salidas
if [ "$OUTPUT" = "$EXPECTED_OUTPUT" ]; then
    echo -e "${GREEN}¡Prueba superada! La salida coincide con la esperada.${NC}"
    
    # Verificar que no se utilice "using namespace std" en los archivos
    if grep -q "using namespace std" Warlock.cpp Warlock.hpp; then
        echo -e "${RED}Advertencia: Se está utilizando 'using namespace std', lo cual no es recomendado.${NC}"
    else
        echo -e "${GREEN}Buen trabajo: No se utiliza 'using namespace std'.${NC}"
    fi
    
    # Verificar que la clase siga la forma canónica de Coplien
    HAS_CONSTRUCTOR=0
    HAS_DESTRUCTOR=0
    HAS_COPY_CONSTRUCTOR=0
    HAS_ASSIGNMENT_OPERATOR=0
    
    # Verificar constructor
    if grep -q "Warlock::Warlock" Warlock.cpp; then
        HAS_CONSTRUCTOR=1
    fi
    
    # Verificar destructor
    if grep -q "Warlock::~Warlock" Warlock.cpp; then
        HAS_DESTRUCTOR=1
    fi
    
    # Verificar constructor de copia
    if grep -q "Warlock::Warlock.*Warlock" Warlock.cpp && grep -q "Warlock.*const.*&" Warlock.cpp; then
        HAS_COPY_CONSTRUCTOR=1
    fi
    
    # Verificar operador de asignación
    if grep -q "operator=" Warlock.cpp || grep -q "operator =" Warlock.cpp; then
        HAS_ASSIGNMENT_OPERATOR=1
    fi
    
    if [ $HAS_CONSTRUCTOR -eq 1 ] && [ $HAS_DESTRUCTOR -eq 1 ] && [ $HAS_COPY_CONSTRUCTOR -eq 1 ] && [ $HAS_ASSIGNMENT_OPERATOR -eq 1 ]; then
        echo -e "${GREEN}La clase sigue la forma canónica de Coplien.${NC}"
    else
        echo -e "${RED}Advertencia: La clase no sigue completamente la forma canónica de Coplien:${NC}"
        [ $HAS_CONSTRUCTOR -eq 0 ] && echo -e "${RED}  - Falta el constructor${NC}"
        [ $HAS_DESTRUCTOR -eq 0 ] && echo -e "${RED}  - Falta el destructor${NC}"
        [ $HAS_COPY_CONSTRUCTOR -eq 0 ] && echo -e "${RED}  - Falta el constructor de copia${NC}"
        [ $HAS_ASSIGNMENT_OPERATOR -eq 0 ] && echo -e "${RED}  - Falta el operador de asignación${NC}"
    fi
    
    exit 0
else
    echo -e "${RED}Prueba fallida. La salida no coincide con la esperada.${NC}"
    echo -e "${YELLOW}Salida esperada:${NC}"
    echo "$EXPECTED_OUTPUT"
    echo -e "${YELLOW}Salida obtenida:${NC}"
    echo "$OUTPUT"
    exit 1
fi
