#!/bin/bash

# Función mejorada para el modo guiado de ft_printf
improved_guided_ft_printf() {
    clear
    echo -e "${BLUE}${BOLD}=== MODO GUIADO MEJORADO: FT_PRINTF ===${NC}\n"
    echo -e "Este modo te guiará paso a paso en la implementación de ft_printf,"
    echo -e "siguiendo el proceso natural de pensamiento de un programador.\n"
    
    # FASE 1: Entender el problema
    echo -e "${YELLOW}FASE 1: ENTENDER EL PROBLEMA${NC}"
    echo -e "Lo primero es entender qué necesitamos implementar:\n"
    echo -e "1. Una función llamada ft_printf que imite el comportamiento de printf"
    echo -e "2. Debe manejar solo 3 conversiones: %s (string), %d (decimal) y %x (hexadecimal)"
    echo -e "3. Debe devolver el número de caracteres impresos\n"
    
    echo -e "${CYAN}Paso mental:${NC} \"Necesito crear una función que lea un formato,"
    echo -e "detecte los especificadores % y los reemplace con el valor correspondiente\"\n"
    
    read -p "Presiona Enter para continuar..."
    
    # FASE 2: Planificar la estructura del código
    clear
    echo -e "${YELLOW}FASE 2: PLANIFICAR LA ESTRUCTURA${NC}"
    echo -e "Antes de empezar a escribir código, pensemos en la estructura necesaria:\n"
    
    echo -e "1. ${CYAN}Necesito incluir las bibliotecas adecuadas:${NC}"
    echo -e "   - stdarg.h para manejar los argumentos variables"
    echo -e "   - unistd.h para la función write\n"
    
    echo -e "2. ${CYAN}Necesito una función principal que:${NC}"
    echo -e "   - Acepte un formato y argumentos variables"
    echo -e "   - Recorra el formato caracter por caracter"
    echo -e "   - Procese cada especificador según su tipo"
    echo -e "   - Cuente los caracteres impresos\n"
    
    echo -e "3. ${CYAN}Necesito funciones auxiliares para:${NC}"
    echo -e "   - Imprimir strings (%s)"
    echo -e "   - Imprimir números en base 10 (%d)"
    echo -e "   - Imprimir números en base 16 (%x)\n"
    
    echo -e "${CYAN}Paso mental:${NC} \"Voy a empezar creando la estructura básica de la función principal\"\n"
    
    read -p "Presiona Enter para ver la estructura básica..."
    
    # FASE 3: Implementar la estructura básica
    clear
    echo -e "${YELLOW}FASE 3: IMPLEMENTAR LA ESTRUCTURA BÁSICA${NC}"
    echo -e "Comenzamos con la estructura básica de ft_printf:\n"
    
    # Crear archivo temporal con la estructura básica
    cat > "$RENDU_DIR/ft_printf/ft_printf.c.temp" << 'EOL'
#include <stdarg.h>
#include <unistd.h>

int ft_printf(const char *format, ...)
{
    int len = 0;        // Para contar caracteres impresos
    va_list args;       // Para manejar argumentos variables
    
    va_start(args, format);  // Inicializar lista de argumentos
    
    // Aquí recorreremos el formato
    
    va_end(args);       // Limpiar lista de argumentos
    return (len);       // Devolver total de caracteres
}
EOL

    echo -e "${CYAN}Estructura básica creada:${NC}\n"
    cat "$RENDU_DIR/ft_printf/ft_printf.c.temp"
    
    echo -e "\n${CYAN}Paso mental:${NC} \"Ahora necesito recorrer el formato e identificar caracteres normales y especificadores\"\n"
    
    read -p "Presiona Enter para implementar el recorrido del formato..."
    
    # FASE 4: Implementar el recorrido del formato
    clear
    echo -e "${YELLOW}FASE 4: IMPLEMENTAR EL RECORRIDO DEL FORMATO${NC}"
    echo -e "Ahora implementamos la lógica para recorrer el string de formato:\n"
    
    cat > "$RENDU_DIR/ft_printf/ft_printf.c.temp" << 'EOL'
#include <stdarg.h>
#include <unistd.h>

int ft_printf(const char *format, ...)
{
    int len = 0;
    va_list args;
    
    va_start(args, format);
    
    while (*format)  // Mientras no lleguemos al final de la string
    {
        if (*format == '%' && *(format + 1))  // Si encontramos un '%' y hay algo después
        {
            format++;  // Avanzamos al carácter después del '%'
            
            // Aquí procesaremos los especificadores
        }
        else  // Carácter normal
        {
            len += write(1, format, 1);  // Imprimimos el carácter
        }
        format++;  // Avanzamos al siguiente carácter
    }
    
    va_end(args);
    return (len);
}
EOL

    echo -e "${CYAN}Implementación del recorrido:${NC}\n"
    cat "$RENDU_DIR/ft_printf/ft_printf.c.temp"
    
    echo -e "\n${CYAN}Paso mental:${NC} \"Ahora necesito implementar cada uno de los especificadores: %s, %d y %x\"\n"
    
    read -p "Presiona Enter para implementar la función para imprimir strings (%s)..."
    
    # FASE 5: Implementar la impresión de strings
    clear
    echo -e "${YELLOW}FASE 5: IMPLEMENTAR LA IMPRESIÓN DE STRINGS (%s)${NC}"
    echo -e "Primero, creamos una función auxiliar para imprimir strings:\n"
    
    cat > "$RENDU_DIR/ft_printf/ft_printf.c.temp" << 'EOL'
#include <stdarg.h>
#include <unistd.h>

// Función auxiliar para imprimir strings
void ft_putstr(char *str, int *len)
{
    if (!str)  // Si la string es NULL
        str = "(null)";  // Usamos "(null)" como hace printf
    
    while (*str)  // Mientras no lleguemos al final de la string
    {
        *len += write(1, str, 1);  // Imprimimos un carácter y contamos
        str++;  // Avanzamos al siguiente carácter
    }
}

int ft_printf(const char *format, ...)
{
    int len = 0;
    va_list args;
    
    va_start(args, format);
    
    while (*format)
    {
        if (*format == '%' && *(format + 1))
        {
            format++;
            
            if (*format == 's')  // Si el especificador es 's'
            {
                ft_putstr(va_arg(args, char *), &len);  // Imprimimos la string
            }
            // Aquí agregaremos %d y %x
        }
        else
        {
            len += write(1, format, 1);
        }
        format++;
    }
    
    va_end(args);
    return (len);
}
EOL

    echo -e "${CYAN}Implementación de %s:${NC}\n"
    cat "$RENDU_DIR/ft_printf/ft_printf.c.temp"
    
    echo -e "\n${CYAN}Paso mental:${NC} \"Ahora necesito implementar una función para imprimir números\"\n"
    
    read -p "Presiona Enter para implementar la función para imprimir números (%d y %x)..."
    
    # FASE 6: Implementar la impresión de números
    clear
    echo -e "${YELLOW}FASE 6: IMPLEMENTAR LA IMPRESIÓN DE NÚMEROS (%d y %x)${NC}"
    echo -e "Ahora creamos una función que pueda imprimir números en cualquier base:\n"
    
    cat > "$RENDU_DIR/ft_printf/ft_printf.c.temp" << 'EOL'
#include <stdarg.h>
#include <unistd.h>

// Función auxiliar para imprimir strings
void ft_putstr(char *str, int *len)
{
    if (!str)
        str = "(null)";
    
    while (*str)
    {
        *len += write(1, str, 1);
        str++;
    }
}

// Función auxiliar para imprimir números en cualquier base
void ft_putnbr_base(long long n, int base, int *len)
{
    char *digits = "0123456789abcdef";  // Dígitos para cualquier base hasta 16
    
    // Manejar números negativos (solo para base 10)
    if (n < 0 && base == 10)
    {
        *len += write(1, "-", 1);  // Imprimimos el signo menos
        n = -n;  // Convertimos a positivo
    }
    
    // Imprimimos los dígitos en orden correcto usando recursión
    if (n >= base)
        ft_putnbr_base(n / base, base, len);  // Primero dígitos más significativos
    
    // Imprimimos el dígito actual
    *len += write(1, &digits[n % base], 1);
}

int ft_printf(const char *format, ...)
{
    int len = 0;
    va_list args;
    
    va_start(args, format);
    
    while (*format)
    {
        if (*format == '%' && *(format + 1))
        {
            format++;
            
            if (*format == 's')
            {
                ft_putstr(va_arg(args, char *), &len);
            }
            else if (*format == 'd')  // Si el especificador es 'd'
            {
                ft_putnbr_base(va_arg(args, int), 10, &len);  // Base 10 para decimal
            }
            else if (*format == 'x')  // Si el especificador es 'x'
            {
                ft_putnbr_base(va_arg(args, unsigned int), 16, &len);  // Base 16 para hexadecimal
            }
        }
        else
        {
            len += write(1, format, 1);
        }
        format++;
    }
    
    va_end(args);
    return (len);
}
EOL

    echo -e "${CYAN}Implementación completa:${NC}\n"
    cat "$RENDU_DIR/ft_printf/ft_printf.c.temp"
    
    echo -e "\n${CYAN}Paso mental:${NC} \"Ahora tengo una implementación completa de ft_printf que maneja %s, %d y %x\"\n"
    
    # FASE 7: Explicación final y consideraciones
    echo -e "${YELLOW}FASE 7: EXPLICACIÓN FINAL Y CONSIDERACIONES${NC}\n"
    
    echo -e "Esta implementación cumple con todos los requisitos del ejercicio:"
    echo -e "1. Maneja los tres especificadores requeridos: %s, %d y %x"
    echo -e "2. Cuenta correctamente los caracteres impresos"
    echo -e "3. Devuelve el número total de caracteres impresos"
    
    echo -e "\n${CYAN}Consideraciones adicionales:${NC}"
    echo -e "- Para %s: maneja correctamente strings NULL"
    echo -e "- Para %d: maneja correctamente números negativos"
    echo -e "- Para %x: usa unsigned int para evitar problemas con números negativos"
    echo -e "- La función es eficiente y no usa memoria extra innecesaria"
    
    echo -e "\n${YELLOW}¿Quieres guardar esta implementación como tu solución final? (s/n):${NC} "
    read -r save_choice
    
    if [[ "$save_choice" =~ ^[Ss]$ ]]; then
        mv "$RENDU_DIR/ft_printf/ft_printf.c.temp" "$RENDU_DIR/ft_printf/ft_printf.c"
        echo -e "\n${GREEN}Implementación guardada como $RENDU_DIR/ft_printf/ft_printf.c${NC}"
    else
        echo -e "\n${YELLOW}Implementación descartada. Puedes revisar los ejemplos para crear tu propia solución.${NC}"
        rm "$RENDU_DIR/ft_printf/ft_printf.c.temp"
    fi
    
    echo -e "\n${YELLOW}Presiona Enter para volver al menú...${NC}"
    read
}
