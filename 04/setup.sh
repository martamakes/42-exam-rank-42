#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

# Directorios
EXAM_DIR="$(cd "$(dirname "$0")" && pwd)"
MICROSHELL_DIR="$EXAM_DIR/microshell"
GRADEME_DIR="$MICROSHELL_DIR/grademe"

# Crear directorios necesarios
mkdir -p "$GRADEME_DIR"
mkdir -p "$EXAM_DIR/rendu/microshell"

echo -e "${BLUE}Configurando permisos...${NC}"

# Dar permisos de ejecución
chmod +x "$EXAM_DIR/exam.sh"
chmod +x "$MICROSHELL_DIR/microshell_guide" 2>/dev/null || echo -e "${YELLOW}Advertencia: microshell_guide no encontrado${NC}"
chmod +x "$GRADEME_DIR/test.sh" 2>/dev/null || echo -e "${YELLOW}Advertencia: test.sh no encontrado${NC}"
chmod +x "$GRADEME_DIR/grademe.sh" 2>/dev/null || echo -e "${YELLOW}Advertencia: grademe.sh no encontrado${NC}"

# Compilar guía si no existe
if [ ! -x "$MICROSHELL_DIR/microshell_guide" ]; then
    echo -e "${YELLOW}Compilando guía interactiva...${NC}"
    gcc -Wall -Wextra -Werror "$MICROSHELL_DIR/microshell_guide.c" "$MICROSHELL_DIR/guide_utils.c" -o "$MICROSHELL_DIR/microshell_guide" 2>/dev/null
    if [ $? -eq 0 ]; then
        chmod +x "$MICROSHELL_DIR/microshell_guide"
        echo -e "${GREEN}Guía compilada correctamente${NC}"
    else
        echo -e "${RED}Error al compilar la guía${NC}"
    fi
fi

echo -e "${GREEN}Configuración completada. Ahora puedes ejecutar ./exam.sh${NC}"
