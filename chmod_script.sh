#!/bin/bash

# Script que da permisos de ejecución a todos los scripts utilizados por el sistema de examen

# Lista de directorios donde buscar scripts
script_dirs=(
    "."
    "./02"
    "./03"
    "./04"
    "./05"
    "./06"
    "./03/ft_printf"
    "./03/get_next_line"
)

# Tipo de archivos para dar permisos
script_patterns=(
    "*.sh"
    "exam*.sh"
    "test*.sh"
    "setup*.sh"
    "init*.sh"
)

# Colores para mensajes
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${YELLOW}=== Configurando permisos de ejecución para scripts ===${NC}"

total_files=0

# Recorrer cada directorio
for dir in "${script_dirs[@]}"; do
    if [ -d "$dir" ]; then
        echo -e "${GREEN}Revisando directorio: $dir${NC}"
        
        # Recorrer cada patrón
        for pattern in "${script_patterns[@]}"; do
            files=$(find "$dir" -name "$pattern" -type f 2>/dev/null)
            
            if [ -n "$files" ]; then
                for file in $files; do
                    if [ -f "$file" ]; then
                        chmod +x "$file"
                        echo "  - Permisos de ejecución dados a: $file"
                        ((total_files++))
                    fi
                done
            fi
        done
    fi
done

echo -e "${GREEN}Finalizado: $total_files archivos actualizados${NC}"