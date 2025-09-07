#!/bin/bash

# 🎓 PUNTO DE ENTRADA ÚNICO - EXAMSHELL SYSTEM
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

clear
echo -e "${BLUE}═══════════════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}                    🎓 EXAMSHELL - PUNTO DE ENTRADA ÚNICO              ${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════════════${NC}"
echo ""

echo -e "${YELLOW}Selecciona el sistema que quieres usar:${NC}"
echo ""
echo "1. 🚀 Sistema Avanzado (con tracking de progreso)"
echo "2. 📚 Sistema Tradicional (examshell clásico)"
echo "3. 🚪 Salir"
echo ""

read -p "Selecciona una opción (1-3): " choice

case $choice in
    1)
        echo -e "${GREEN}🚀 Iniciando Sistema Avanzado...${NC}"
        ./advanced_init.sh
        ;;
    2)
        echo -e "${GREEN}📚 Iniciando Sistema Tradicional...${NC}"
        echo "Selecciona el exam rank (03, 04, 05):"
        read -p "Exam rank: " rank
        ./examshell $rank
        ;;
    3)
        echo -e "${GREEN}¡Hasta luego! 🎓${NC}"
        exit 0
        ;;
    *)
        echo -e "${RED}Opción inválida${NC}"
        exit 1
        ;;
esac