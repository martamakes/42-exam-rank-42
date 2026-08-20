#!/bin/bash

MICROSHELL="./microshell"
COLOR_RED="\033[0;31m"
COLOR_GREEN="\033[0;32m"
COLOR_YELLOW="\033[0;33m"
COLOR_RESET="\033[0m"

echo -e "${COLOR_YELLOW}=== Pruebas básicas ===${COLOR_RESET}"

# Prueba 1: Comando simple
echo -e "\n${COLOR_YELLOW}Prueba 1: Comando simple${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL /bin/ls"
$MICROSHELL /bin/ls
if [ $? -eq 0 ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 1 pasada${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 1 fallida${COLOR_RESET}"
fi

# Prueba 2: Comando con argumentos
echo -e "\n${COLOR_YELLOW}Prueba 2: Comando con argumentos${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL /bin/ls -la"
$MICROSHELL /bin/ls -la
if [ $? -eq 0 ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 2 pasada${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 2 fallida${COLOR_RESET}"
fi

# Prueba 3: Comando inexistente
echo -e "\n${COLOR_YELLOW}Prueba 3: Comando inexistente${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL /comando_inexistente"
$MICROSHELL /comando_inexistente
if [ $? -ne 0 ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 3 pasada (el comando fallido devuelve código diferente a 0)${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 3 fallida (el comando fallido debería devolver código diferente a 0)${COLOR_RESET}"
fi

echo -e "\n${COLOR_YELLOW}=== Pruebas de cd ===${COLOR_RESET}"

# Prueba 4: cd básico
echo -e "\n${COLOR_YELLOW}Prueba 4: cd básico${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL cd .. ';' /bin/pwd"
CURRENT_DIR=$(pwd)
$MICROSHELL cd .. ";" /bin/pwd
if [ "$(pwd)" = "$CURRENT_DIR" ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 4 pasada (cd no afecta al proceso padre)${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 4 fallida (cd afectó al proceso padre)${COLOR_RESET}"
    cd "$CURRENT_DIR"
fi

# Prueba 5: cd con error
echo -e "\n${COLOR_YELLOW}Prueba 5: cd con error${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL cd /directorio_inexistente"
$MICROSHELL cd /directorio_inexistente
if [ $? -ne 0 ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 5 pasada (cd a directorio inexistente devuelve error)${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 5 fallida (cd a directorio inexistente debería devolver error)${COLOR_RESET}"
fi

echo -e "\n${COLOR_YELLOW}=== Pruebas de pipes ===${COLOR_RESET}"

# Prueba 6: Pipe simple
echo -e "\n${COLOR_YELLOW}Prueba 6: Pipe simple${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL /bin/ls '|' /bin/grep microshell"
$MICROSHELL /bin/ls "|" /bin/grep microshell
if [ $? -eq 0 ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 6 pasada${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 6 fallida${COLOR_RESET}"
fi

# Prueba 7: Múltiples pipes
echo -e "\n${COLOR_YELLOW}Prueba 7: Múltiples pipes${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL /bin/ls '|' /bin/grep microshell '|' /usr/bin/wc -l"
$MICROSHELL /bin/ls "|" /bin/grep microshell "|" /usr/bin/wc -l
if [ $? -eq 0 ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 7 pasada${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 7 fallida${COLOR_RESET}"
fi

echo -e "\n${COLOR_YELLOW}=== Pruebas de punto y coma ===${COLOR_RESET}"

# Prueba 8: Comandos separados por ;
echo -e "\n${COLOR_YELLOW}Prueba 8: Comandos separados por ;${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL /bin/ls ';' /bin/echo 'Prueba de punto y coma'"
$MICROSHELL /bin/ls ";" /bin/echo "Prueba de punto y coma"
if [ $? -eq 0 ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 8 pasada${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 8 fallida${COLOR_RESET}"
fi

echo -e "\n${COLOR_YELLOW}=== Pruebas complejas ===${COLOR_RESET}"

# Prueba 9: Combinación de pipes y punto y coma
echo -e "\n${COLOR_YELLOW}Prueba 9: Combinación de pipes y punto y coma${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL /bin/ls '|' /bin/grep microshell ';' /bin/echo 'Prueba compleja'"
$MICROSHELL /bin/ls "|" /bin/grep microshell ";" /bin/echo "Prueba compleja"
if [ $? -eq 0 ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 9 pasada${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 9 fallida${COLOR_RESET}"
fi

# Prueba 10: Caso de error en pipe
echo -e "\n${COLOR_YELLOW}Prueba 10: Caso de error en pipe${COLOR_RESET}"
echo -e "Ejecutando: $MICROSHELL /bin/ls '|' /comando_inexistente"
$MICROSHELL /bin/ls "|" /comando_inexistente
if [ $? -ne 0 ]; then
    echo -e "${COLOR_GREEN}✓ Prueba 10 pasada (error en comando de pipe devuelve error)${COLOR_RESET}"
else
    echo -e "${COLOR_RED}✗ Prueba 10 fallida (error en comando de pipe debería devolver error)${COLOR_RESET}"
fi

echo -e "\n${COLOR_YELLOW}Todas las pruebas completadas.${COLOR_RESET}"
