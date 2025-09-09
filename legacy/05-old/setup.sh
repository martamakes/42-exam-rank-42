#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

# Directorios
EXAM_DIR="$(cd "$(dirname "$0")" && pwd)"
CPP_MODULES=("00" "01" "02")

# Crear directorios necesarios
mkdir -p "$EXAM_DIR/rendu/cpp_module_00"
mkdir -p "$EXAM_DIR/rendu/cpp_module_01"
mkdir -p "$EXAM_DIR/rendu/cpp_module_02"

echo -e "${BLUE}Configurando permisos...${NC}"

# Dar permisos de ejecución a los scripts
chmod +x "$EXAM_DIR/exam.sh"

for module in "${CPP_MODULES[@]}"; do
    chmod +x "$EXAM_DIR/cpp_module_$module/test.sh" 2>/dev/null || echo -e "${YELLOW}Advertencia: test.sh no encontrado en cpp_module_$module${NC}"
done

echo -e "${GREEN}Configuración completada.${NC}"
echo -e "${YELLOW}Ahora puedes ejecutar ./exam.sh para iniciar el tester.${NC}"
