#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

RENDU_PATH="../rendu/cpp_module_01"
TEST_NAME="cpp_module_01"
EXPECTED_OUTPUT="Richard: This looks like another boring day.\nRichard: I am Richard, the Titled!\nTarget Practice Dummy has been fwooshed!\nRichard: My job here is done!"

# Verificar existencia de archivos necesarios
REQUIRED_FILES=("Warlock.cpp" "Warlock.hpp" "ASpell.cpp" "ASpell.hpp" "ATarget.cpp" "ATarget.hpp" "Fwoosh.cpp" "Fwoosh.hpp" "Dummy.cpp" "Dummy.hpp")

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
    
    # Verificar que no se utilice "switch" en Warlock.cpp
    if grep -q "switch" Warlock.cpp; then
        echo -e "${RED}Error: Se está utilizando 'switch' en Warlock.cpp, lo cual está prohibido.${NC}"
        exit 1
    else
        echo -e "${GREEN}Buen trabajo: No se utiliza 'switch' en Warlock.cpp.${NC}"
    fi
    
    # Verificar que ASpell tenga un método clone() puro
    if grep -q "virtual.*ASpell.*clone.*=.*0" ASpell.hpp; then
        echo -e "${GREEN}ASpell contiene un método clone() puro.${NC}"
    else
        echo -e "${RED}Advertencia: ASpell podría no tener un método clone() puro.${NC}"
    fi
    
    # Verificar que ATarget tenga un método clone() puro
    if grep -q "virtual.*ATarget.*clone.*=.*0" ATarget.hpp; then
        echo -e "${GREEN}ATarget contiene un método clone() puro.${NC}"
    else
        echo -e "${RED}Advertencia: ATarget podría no tener un método clone() puro.${NC}"
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
