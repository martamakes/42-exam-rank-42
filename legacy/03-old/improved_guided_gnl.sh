#!/bin/bash

# Función mejorada para el modo guiado de get_next_line
improved_guided_gnl() {
    clear
    echo -e "${BLUE}${BOLD}=== MODO GUIADO MEJORADO: GET_NEXT_LINE ===${NC}\n"
    echo -e "Este modo te guiará paso a paso en la implementación de get_next_line,"
    echo -e "siguiendo el proceso natural de pensamiento de un programador.\n"
    
    # FASE 1: Entender el problema
    echo -e "${YELLOW}FASE 1: ENTENDER EL PROBLEMA${NC}"
    echo -e "Lo primero es entender qué necesitamos implementar:\n"
    echo -e "1. Una función llamada get_next_line que lea una línea de un file descriptor"
    echo -e "2. Debe manejar múltiples llamadas consecutivas para leer líneas sucesivas"
    echo -e "3. Debe devolver la línea leída, incluyendo el \\n, o NULL si hay error o EOF\n"
    
    echo -e "${CYAN}Paso mental:${NC} \"Necesito crear una función que lea de un file descriptor"
    echo -e "hasta encontrar un salto de línea o el fin del archivo\"\n"
    
    read -p "Presiona Enter para continuar..."
    
    # FASE 2: Planificar la estructura del código
    clear
    echo -e "${YELLOW}FASE 2: PLANIFICAR LA ESTRUCTURA${NC}"
    echo -e "Antes de empezar a escribir código, pensemos en la estructura necesaria:\n"
    
    echo -e "1. ${CYAN}Necesito crear un archivo header con:${NC}"
    echo -e "   - Prototipo de la función"
    echo -e "   - Inclusiones necesarias (unistd.h para read, stdlib.h para malloc/free)\n"
    
    echo -e "2. ${CYAN}Necesito una función principal que:${NC}"
    echo -e "   - Lea bytes del file descriptor"
    echo -e "   - Busque un salto de línea (\\n)"
    echo -e "   - Guarde lo que sobra para futuras llamadas"
    echo -e "   - Devuelva la línea leída o NULL\n"
    
    echo -e "${CYAN}Paso mental:${NC} \"Voy a empezar creando el archivo header\"\n"
    
    read -p "Presiona Enter para implementar el archivo header..."
    
    # FASE 3: Implementar el archivo header
    clear
    echo -e "${YELLOW}FASE 3: IMPLEMENTAR EL ARCHIVO HEADER${NC}"
    echo -e "Primero, creamos el archivo get_next_line.h:\n"
    
    cat > "$RENDU_DIR/get_next_line/get_next_line.h.temp" << 'EOL'
#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>  // Para la función read
# include <stdlib.h>  // Para malloc y free

// Prototipo de la función principal
char *get_next_line(int fd);

#endif
EOL

    echo -e "${CYAN}Archivo header creado:${NC}\n"
    cat "$RENDU_DIR/get_next_line/get_next_line.h.temp"
    
    echo -e "\n${CYAN}Paso mental:${NC} \"Ahora necesito pensar en la estructura básica de la función\"\n"
    
    read -p "Presiona Enter para implementar la estructura básica de get_next_line..."
    
    # FASE 4: Implementar la estructura básica
    clear
    echo -e "${YELLOW}FASE 4: IMPLEMENTAR LA ESTRUCTURA BÁSICA${NC}"
    echo -e "Ahora, creamos la estructura básica de get_next_line.c:\n"
    
    cat > "$RENDU_DIR/get_next_line/get_next_line.c.temp" << 'EOL'
#include "get_next_line.h"

char *get_next_line(int fd)
{
    static char *remaining = NULL;  // Para guardar lo que sobra entre llamadas
    char *line = NULL;              // La línea que vamos a devolver
    
    // Verificar parámetros válidos
    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    
    // Aquí implementaremos la lógica de lectura
    
    return (line);
}
EOL

    echo -e "${CYAN}Estructura básica creada:${NC}\n"
    cat "$RENDU_DIR/get_next_line/get_next_line.c.temp"
    
    echo -e "\n${CYAN}Paso mental:${NC} \"Para el examen, puedo implementar una solución simple que lea caracter a caracter\"\n"
    
    read -p "Presiona Enter para implementar la lógica de lectura..."
    
    # FASE 5: Implementar la lógica de lectura simple (caracter a caracter)
    clear
    echo -e "${YELLOW}FASE 5: IMPLEMENTAR LA LÓGICA DE LECTURA${NC}"
    echo -e "Para el examen, podemos implementar una solución sencilla que lea caracter a caracter:\n"
    
    cat > "$RENDU_DIR/get_next_line/get_next_line.c.temp" << 'EOL'
#include "get_next_line.h"

char *get_next_line(int fd)
{
    int i = 0;                      // Índice para la línea
    char character;                  // Carácter actual leído
    int rd = read(fd, &character, 1); // Leer un carácter
    char *line = malloc(10000);     // Tamaño arbitrario grande
    
    // Verificar parámetros válidos y asignación de memoria
    if (fd < 0 || BUFFER_SIZE <= 0 || !line)
        return (NULL);
    
    // Leer caracteres hasta encontrar \n o EOF
    while (rd > 0)
    {
        line[i++] = character;          // Guardar el carácter
        if (character == '\n')          // Si es fin de línea, terminamos
            break;
        rd = read(fd, &character, 1);  // Leer el siguiente carácter
    }
    
    // Añadir terminador nulo
    line[i] = '\0';
    
    // Si no leímos nada o hubo un error, liberar memoria y devolver NULL
    if (rd <= 0 && i == 0)
    {
        free(line);
        return (NULL);
    }
    
    return (line);
}
EOL

    echo -e "${CYAN}Implementación simple (caracter a caracter):${NC}\n"
    cat "$RENDU_DIR/get_next_line/get_next_line.c.temp"
    
    echo -e "\n${CYAN}Paso mental:${NC} \"Esta solución es sencilla y funcional para el examen, pero"
    echo -e "no es eficiente para archivos grandes\"\n"
    
    # FASE 6: Explicación final y consideraciones
    echo -e "${YELLOW}FASE 6: EXPLICACIÓN FINAL Y CONSIDERACIONES${NC}\n"
    
    echo -e "Esta implementación cumple con los requisitos básicos del ejercicio:"
    echo -e "1. Lee una línea (hasta encontrar \\n o EOF) de un file descriptor"
    echo -e "2. Devuelve la línea incluyendo el \\n, o NULL si hay error o EOF"
    echo -e "3. Se puede llamar múltiples veces para leer líneas sucesivas"
    
    echo -e "\n${CYAN}Consideraciones adicionales:${NC}"
    echo -e "- Esta implementación es simple pero no eficiente para archivos grandes"
    echo -e "- Para una solución más eficiente, deberías leer en bloques de tamaño BUFFER_SIZE"
    echo -e "- En una implementación completa, deberías usar una variable estática para"
    echo -e "  guardar lo que sobra entre llamadas"
    echo -e "- La gestión de memoria es mínima (solo 1 malloc por línea)"
    
    echo -e "\n${YELLOW}¿Quieres guardar esta implementación como tu solución final? (s/n):${NC} "
    read -r save_choice
    
    if [[ "$save_choice" =~ ^[Ss]$ ]]; then
        mv "$RENDU_DIR/get_next_line/get_next_line.h.temp" "$RENDU_DIR/get_next_line/get_next_line.h"
        mv "$RENDU_DIR/get_next_line/get_next_line.c.temp" "$RENDU_DIR/get_next_line/get_next_line.c"
        echo -e "\n${GREEN}Implementación guardada en el directorio $RENDU_DIR/get_next_line/${NC}"
    else
        echo -e "\n${YELLOW}Implementación descartada. Puedes revisar los ejemplos para crear tu propia solución.${NC}"
        rm "$RENDU_DIR/get_next_line/get_next_line.h.temp" "$RENDU_DIR/get_next_line/get_next_line.c.temp"
    fi
    
    echo -e "\n${YELLOW}Presiona Enter para volver al menú...${NC}"
    read
}
