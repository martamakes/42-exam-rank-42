#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

RENDU_PATH="../rendu/cpp_module_02"
TEST_NAME="cpp_module_02"
EXPECTED_OUTPUT="Richard: This looks like another boring day.\nRichard: I am Richard, Hello, I'm Richard the Warlock!!\nInconspicuous Red-brick Wall has been turned into a critter!\nInconspicuous Red-brick Wall has been burnt to a crisp!\nRichard: My job here is done!"

# Verificar existencia de archivos necesarios
REQUIRED_FILES=(
    "Warlock.cpp" "Warlock.hpp" 
    "ASpell.cpp" "ASpell.hpp" 
    "ATarget.cpp" "ATarget.hpp" 
    "Fwoosh.cpp" "Fwoosh.hpp" 
    "Dummy.cpp" "Dummy.hpp"
    "Fireball.cpp" "Fireball.hpp"
    "Polymorph.cpp" "Polymorph.hpp"
    "BrickWall.cpp" "BrickWall.hpp"
    "SpellBook.cpp" "SpellBook.hpp"
    "TargetGenerator.cpp" "TargetGenerator.hpp"
)

for file in "${REQUIRED_FILES[@]}"; do
    if [ ! -f "$RENDU_PATH/$file" ]; then
        echo -e "${RED}Error: Falta el archivo requerido $file${NC}"
        exit 1
    fi
done

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
    
    # Verificar que no se utilice "switch" en clases prohibidas
    if grep -q "switch" Warlock.cpp SpellBook.cpp TargetGenerator.cpp; then
        echo -e "${RED}Error: Se está utilizando 'switch' en alguno de los archivos prohibidos.${NC}"
        exit 1
    else
        echo -e "${GREEN}Buen trabajo: No se utiliza 'switch' en archivos prohibidos.${NC}"
    fi
    
    # Verificar que SpellBook siga la forma canónica
    HAS_CONSTRUCTOR=0
    HAS_DESTRUCTOR=0
    HAS_COPY_CONSTRUCTOR=0
    HAS_ASSIGNMENT_OPERATOR=0
    
    # Verificar constructor
    if grep -q "SpellBook::SpellBook" SpellBook.cpp; then
        HAS_CONSTRUCTOR=1
    fi
    
    # Verificar destructor
    if grep -q "SpellBook::~SpellBook" SpellBook.cpp; then
        HAS_DESTRUCTOR=1
    fi
    
    # Verificar constructor de copia
    if grep -q "SpellBook::SpellBook.*SpellBook" SpellBook.cpp && grep -q "SpellBook.*const.*&" SpellBook.cpp; then
        HAS_COPY_CONSTRUCTOR=1
    fi
    
    # Verificar operador de asignación
    if grep -q "operator=" SpellBook.cpp || grep -q "operator =" SpellBook.cpp; then
        HAS_ASSIGNMENT_OPERATOR=1
    fi
    
    if [ $HAS_CONSTRUCTOR -eq 1 ] && [ $HAS_DESTRUCTOR -eq 1 ] && [ $HAS_COPY_CONSTRUCTOR -eq 1 ] && [ $HAS_ASSIGNMENT_OPERATOR -eq 1 ]; then
        echo -e "${GREEN}La clase SpellBook sigue la forma canónica.${NC}"
    else
        echo -e "${RED}Advertencia: La clase SpellBook no sigue completamente la forma canónica:${NC}"
        [ $HAS_CONSTRUCTOR -eq 0 ] && echo -e "${RED}  - Falta el constructor${NC}"
        [ $HAS_DESTRUCTOR -eq 0 ] && echo -e "${RED}  - Falta el destructor${NC}"
        [ $HAS_COPY_CONSTRUCTOR -eq 0 ] && echo -e "${RED}  - Falta el constructor de copia${NC}"
        [ $HAS_ASSIGNMENT_OPERATOR -eq 0 ] && echo -e "${RED}  - Falta el operador de asignación${NC}"
    fi
    
    # Similar verificación para TargetGenerator
    HAS_CONSTRUCTOR=0
    HAS_DESTRUCTOR=0
    HAS_COPY_CONSTRUCTOR=0
    HAS_ASSIGNMENT_OPERATOR=0
    
    # Verificar constructor
    if grep -q "TargetGenerator::TargetGenerator" TargetGenerator.cpp; then
        HAS_CONSTRUCTOR=1
    fi
    
    # Verificar destructor
    if grep -q "TargetGenerator::~TargetGenerator" TargetGenerator.cpp; then
        HAS_DESTRUCTOR=1
    fi
    
    # Verificar constructor de copia
    if grep -q "TargetGenerator::TargetGenerator.*TargetGenerator" TargetGenerator.cpp && grep -q "TargetGenerator.*const.*&" TargetGenerator.cpp; then
        HAS_COPY_CONSTRUCTOR=1
    fi
    
    # Verificar operador de asignación
    if grep -q "operator=" TargetGenerator.cpp || grep -q "operator =" TargetGenerator.cpp; then
        HAS_ASSIGNMENT_OPERATOR=1
    fi
    
    if [ $HAS_CONSTRUCTOR -eq 1 ] && [ $HAS_DESTRUCTOR -eq 1 ] && [ $HAS_COPY_CONSTRUCTOR -eq 1 ] && [ $HAS_ASSIGNMENT_OPERATOR -eq 1 ]; then
        echo -e "${GREEN}La clase TargetGenerator sigue la forma canónica.${NC}"
    else
        echo -e "${RED}Advertencia: La clase TargetGenerator no sigue completamente la forma canónica:${NC}"
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
