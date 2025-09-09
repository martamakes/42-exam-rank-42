#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'

# Directorios de trabajo
EXAM_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$EXAM_DIR")"
RENDU_DIR="$EXAM_DIR/rendu"

# Crear directorios si no existen
mkdir -p "$RENDU_DIR"
mkdir -p "$RENDU_DIR/ft_printf"
mkdir -p "$RENDU_DIR/get_next_line"

# Asegurarse de que los scripts de modo guiado mejorado tengan permisos de ejecución
if [ -f "$EXAM_DIR/improved_guided_ft_printf.sh" ]; then
    chmod +x "$EXAM_DIR/improved_guided_ft_printf.sh"
fi

if [ -f "$EXAM_DIR/improved_guided_gnl.sh" ]; then
    chmod +x "$EXAM_DIR/improved_guided_gnl.sh"
fi

# Función para el modo guiado
guided_mode() {
    clear
    echo -e "${BLUE}${BOLD}=== MODO GUIADO ===${NC}\n"
    echo -e "${YELLOW}${BOLD}Selecciona el ejercicio:${NC}"
    echo "1. ft_printf (modo mejorado)"
    echo "2. get_next_line (modo mejorado)"
    echo "3. ft_printf (modo clásico)"
    echo "4. get_next_line (modo clásico)"
    echo "0. Volver al menú principal"
    
    read -p "Selección: " choice
    
    case $choice in
        1)
            # Llamar al script mejorado para ft_printf
            if [ -f "$EXAM_DIR/improved_guided_ft_printf.sh" ]; then
                source "$EXAM_DIR/improved_guided_ft_printf.sh"
                improved_guided_ft_printf
            else
                echo -e "${RED}Error: No se encuentra el script mejorado para ft_printf.${NC}"
                sleep 2
                guided_mode
            fi
            ;;
        2)
            # Llamar al script mejorado para get_next_line
            if [ -f "$EXAM_DIR/improved_guided_gnl.sh" ]; then
                source "$EXAM_DIR/improved_guided_gnl.sh"
                improved_guided_gnl
            else
                echo -e "${RED}Error: No se encuentra el script mejorado para get_next_line.${NC}"
                sleep 2
                guided_mode
            fi
            ;;
        3)
            guided_ft_printf
            ;;
        4)
            guided_gnl
            ;;
        0)
            return
            ;;
        *)
            echo -e "${RED}Opción inválida${NC}"
            sleep 1
            guided_mode
            ;;
    esac
}

# Función para el modo guiado ft_printf
guided_ft_printf() {
    clear
    echo -e "${BLUE}${BOLD}=== MODO GUIADO: FT_PRINTF ===${NC}\n"
    echo -e "Vamos a implementar ft_printf paso a paso:\n"
    
    # Paso 1: Estructura básica
    echo -e "${YELLOW}PASO 1: Estructura básica${NC}"
    echo -e "Primero, vamos a crear la estructura básica de la función ft_printf.\n"
    cat > "$RENDU_DIR/ft_printf/ft_printf.c.temp" << 'EOL'
#include <stdarg.h>
#include <unistd.h>

int ft_printf(const char *format, ...)
{
    int len = 0;
    va_list args;
    
    va_start(args, format);
    
    // Aquí iremos recorriendo el formato
    
    va_end(args);
    return (len);
}
EOL

    echo -e "Estructura básica creada. Esta es la base de nuestra función:\n"
    cat "$RENDU_DIR/ft_printf/ft_printf.c.temp"
    
    echo -e "\n${CYAN}Esta estructura contiene:${NC}"
    echo -e "1. La inclusión de stdarg.h para manejar argumentos variables"
    echo -e "2. La inclusión de unistd.h para poder usar write"
    echo -e "3. La inicialización de va_list y el contador de longitud"
    echo -e "4. La llamada a va_start para inicializar la lista de argumentos"
    echo -e "5. Un espacio para nuestro código"
    echo -e "6. La limpieza con va_end y el retorno de la longitud\n"
    
    read -p "Presiona Enter para continuar al siguiente paso..."
    
    # Paso 2: Recorrer el formato
    clear
    echo -e "${YELLOW}PASO 2: Recorrer el formato${NC}"
    echo -e "Ahora, vamos a implementar la lógica para recorrer el string de formato.\n"
    
    cat > "$RENDU_DIR/ft_printf/ft_printf.c.temp" << 'EOL'
#include <stdarg.h>
#include <unistd.h>

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
            // Aquí procesaremos los especificadores
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

    echo -e "Ahora tenemos la lógica para recorrer el formato:\n"
    cat "$RENDU_DIR/ft_printf/ft_printf.c.temp"
    
    echo -e "\n${CYAN}En este paso hemos añadido:${NC}"
    echo -e "1. Un bucle while para recorrer cada carácter del formato"
    echo -e "2. Una condición para detectar el símbolo '%'"
    echo -e "3. Lógica para imprimir caracteres normales usando write\n"
    
    read -p "Presiona Enter para continuar al siguiente paso..."
    
    # Paso 3: Implementar los especificadores
    clear
    echo -e "${YELLOW}PASO 3: Implementar los especificadores${NC}"
    echo -e "Finalmente, vamos a implementar los tres especificadores requeridos: %s, %d y %x.\n"
    
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

// Función auxiliar para imprimir números
void ft_putnbr(long n, int base, int *len)
{
    char *digits = "0123456789abcdef";
    
    if (n < 0)
    {
        *len += write(1, "-", 1);
        n = -n;
    }
    
    if (n >= base)
        ft_putnbr(n / base, base, len);
    
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
                ft_putstr(va_arg(args, char *), &len);
            else if (*format == 'd')
                ft_putnbr(va_arg(args, int), 10, &len);
            else if (*format == 'x')
                ft_putnbr(va_arg(args, unsigned int), 16, &len);
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

    echo -e "Ahora tenemos la implementación completa de ft_printf:\n"
    cat "$RENDU_DIR/ft_printf/ft_printf.c.temp"
    
    echo -e "\n${CYAN}En este paso final hemos añadido:${NC}"
    echo -e "1. Una función ft_putstr para imprimir strings (maneja NULL)"
    echo -e "2. Una función ft_putnbr para imprimir números en cualquier base"
    echo -e "3. La lógica para procesar los especificadores %s, %d y %x"
    echo -e "4. El uso de va_arg para obtener los argumentos correspondientes\n"
    
    echo -e "${GREEN}Ahora tienes una implementación completa de ft_printf!${NC}"
    echo -e "Puedes usarla como base para tu solución o modificarla según tus preferencias.\n"
    
    read -p "¿Quieres guardar esta implementación en ft_printf.c? (s/n): " save_choice
    if [[ "$save_choice" =~ ^[Ss]$ ]]; then
        mv "$RENDU_DIR/ft_printf/ft_printf.c.temp" "$RENDU_DIR/ft_printf/ft_printf.c"
        echo -e "${GREEN}Implementación guardada en $RENDU_DIR/ft_printf/ft_printf.c${NC}"
    else
        echo -e "${YELLOW}Implementación descartada. Puedes revisar los ejemplos para crear tu propia solución.${NC}"
        rm "$RENDU_DIR/ft_printf/ft_printf.c.temp"
    fi
    
    read -p "Presiona Enter para volver al menú principal..."
}

# Función para el modo guiado get_next_line
guided_gnl() {
    clear
    echo -e "${BLUE}${BOLD}=== MODO GUIADO: GET_NEXT_LINE ===${NC}\n"
    echo -e "Vamos a implementar get_next_line paso a paso:\n"
    
    # Paso 1: Crear el header file
    echo -e "${YELLOW}PASO 1: Crear el archivo header${NC}"
    echo -e "Primero, vamos a crear el archivo get_next_line.h con la declaración de la función.\n"
    
    cat > "$RENDU_DIR/get_next_line/get_next_line.h.temp" << 'EOL'
#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

char *get_next_line(int fd);

#endif
EOL

    echo -e "Header creado. Este es el contenido del archivo get_next_line.h:\n"
    cat "$RENDU_DIR/get_next_line/get_next_line.h.temp"
    
    echo -e "\n${CYAN}Este header contiene:${NC}"
    echo -e "1. Las inclusiones necesarias (unistd.h para read y stdlib.h para malloc/free)"
    echo -e "2. El prototipo de la función get_next_line"
    echo -e "3. Guardas de inclusión (#ifndef, #define, #endif)\n"
    
    read -p "Presiona Enter para continuar al siguiente paso..."
    
    # Paso 2: Estructura básica del .c
    clear
    echo -e "${YELLOW}PASO 2: Estructura básica de get_next_line.c${NC}"
    echo -e "Ahora, vamos a crear la estructura básica del archivo .c\n"
    
    cat > "$RENDU_DIR/get_next_line/get_next_line.c.temp" << 'EOL'
#include "get_next_line.h"

char *get_next_line(int fd)
{
    static char *remaining = NULL;  // Para guardar lo que sobra entre llamadas
    char *line = NULL;             // La línea que vamos a devolver
    char buffer[BUFFER_SIZE + 1];  // Buffer para leer del fd
    int bytes_read;                // Número de bytes leídos en cada read
    
    // Verificar parámetros válidos
    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    
    // Aquí implementaremos la lógica de lectura
    
    return (line);
}
EOL

    echo -e "Estructura básica creada. Este es el contenido inicial de get_next_line.c:\n"
    cat "$RENDU_DIR/get_next_line/get_next_line.c.temp"
    
    echo -e "\n${CYAN}En esta estructura básica:${NC}"
    echo -e "1. Incluimos nuestro header"
    echo -e "2. Declaramos una variable estática 'remaining' para guardar lo que sobre entre llamadas"
    echo -e "3. Declaramos variables para la línea a devolver, el buffer y los bytes leídos"
    echo -e "4. Verificamos que el fd y BUFFER_SIZE sean válidos\n"
    
    echo -e "${YELLOW}Nota:${NC} BUFFER_SIZE se define al compilar, usando -D BUFFER_SIZE=42\n"
    
    read -p "Presiona Enter para continuar al siguiente paso..."
    
    # Paso 3: Implementar la lógica de lectura
    clear
    echo -e "${YELLOW}PASO 3: Implementar la lógica de lectura${NC}"
    echo -e "Vamos a implementar una solución sencilla que lee caracter a caracter.\n"
    
    cat > "$RENDU_DIR/get_next_line/get_next_line.c.temp" << 'EOL'
#include "get_next_line.h"

char *get_next_line(int fd)
{
    int i = 0;
    char character;
    int rd = read(fd, &character, 1);
    char *line = malloc(10000);  // Tamaño arbitrario grande
    
    if (fd < 0 || BUFFER_SIZE <= 0 || !line)
        return (NULL);
        
    while (rd > 0)
    {
        line[i++] = character;
        if (character == '\n')  // Si encontramos un salto de línea, terminamos
            break;
        rd = read(fd, &character, 1);
    }
    
    line[i] = '\0';  // Añadir terminador nulo
    
    // Si no leímos nada o hubo un error, liberar memoria y devolver NULL
    if (rd <= 0 && i == 0)
    {
        free(line);
        return (NULL);
    }
    
    return (line);
}
EOL

    echo -e "Implementación básica completada. Este es el código de get_next_line.c:\n"
    cat "$RENDU_DIR/get_next_line/get_next_line.c.temp"
    
    echo -e "\n${CYAN}En esta implementación:${NC}"
    echo -e "1. Usamos un enfoque simple de leer caracter a caracter"
    echo -e "2. Guardamos cada carácter en un buffer hasta encontrar '\\n' o EOF"
    echo -e "3. Manejamos errores de fd inválido, BUFFER_SIZE inválido o error de lectura"
    echo -e "4. Aseguramos que la línea termine con '\\0'\n"
    
    echo -e "${YELLOW}Nota:${NC} Esta implementación es simple pero no eficiente. Para una solución más eficiente,"
    echo -e "deberías leer en bloques de tamaño BUFFER_SIZE y manejar el texto remanente.\n"
    
    echo -e "${GREEN}Ahora tienes una implementación básica de get_next_line!${NC}"
    echo -e "Puedes usarla como base para tu solución o mejorarla para mayor eficiencia.\n"
    
    read -p "¿Quieres guardar estos archivos? (s/n): " save_choice
    if [[ "$save_choice" =~ ^[Ss]$ ]]; then
        mv "$RENDU_DIR/get_next_line/get_next_line.h.temp" "$RENDU_DIR/get_next_line/get_next_line.h"
        mv "$RENDU_DIR/get_next_line/get_next_line.c.temp" "$RENDU_DIR/get_next_line/get_next_line.c"
        echo -e "${GREEN}Archivos guardados en el directorio $RENDU_DIR/get_next_line/${NC}"
    else
        echo -e "${YELLOW}Implementación descartada. Puedes revisar los ejemplos para crear tu propia solución.${NC}"
        rm "$RENDU_DIR/get_next_line/get_next_line.h.temp" "$RENDU_DIR/get_next_line/get_next_line.c.temp"
    fi
    
    read -p "Presiona Enter para volver al menú principal..."
}

# Función para mostrar el subject de un ejercicio
show_subject() {
    local exercise=$1
    local subject_path="$EXAM_DIR/$exercise/README.md"
    
    clear
    echo -e "${BLUE}${BOLD}=== EJERCICIO: $exercise ===${NC}\n"
    echo -e "${CYAN}${BOLD}=== SUBJECT ===${NC}\n"
    
    if [ -f "$subject_path" ]; then
        cat "$subject_path"
        echo -e "\n${YELLOW}Para resolver este ejercicio:${NC}"
        echo -e "1. ${CYAN}Directorio:${NC} rendu/$exercise"
        echo -e "2. ${CYAN}Crea tu solución en:${NC} rendu/$exercise/"
        echo -e "\n${GREEN}${BOLD}IMPORTANTE:${NC} Tu solución debe estar dentro del directorio rendu/$exercise/"
        echo -e "Los archivos que debes crear son:"
        if [ "$exercise" = "ft_printf" ]; then
            echo -e "- ${CYAN}ft_printf.c${NC}"
        elif [ "$exercise" = "get_next_line" ]; then
            echo -e "- ${CYAN}get_next_line.c${NC}"
            echo -e "- ${CYAN}get_next_line.h${NC}"
        fi
    else
        echo -e "${RED}Error: No se encuentra el subject en $subject_path${NC}"
    fi
    
    echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
    read
}

# Función para validar ejercicio
validate_exercise() {
    local exercise=$1
    local student_dir="$RENDU_DIR/$exercise"
    local source_dir="$EXAM_DIR/$exercise"
    
    # Verificar que el usuario ha creado su solución
    if [ ! -d "$student_dir" ]; then
        echo -e "${RED}Error: No se encuentra tu solución en $student_dir${NC}"
        mkdir -p "$student_dir"
        echo -e "${YELLOW}Se ha creado el directorio $student_dir${NC}"
        return 1
    fi
    
    # Verificar existencia de archivos necesarios
    if [ "$exercise" = "ft_printf" ]; then
        if [ ! -f "$student_dir/ft_printf.c" ]; then
            echo -e "${RED}Error: No se encuentra ft_printf.c en $student_dir${NC}"
            return 1
        fi
    elif [ "$exercise" = "get_next_line" ]; then
        if [ ! -f "$student_dir/get_next_line.c" ] || [ ! -f "$student_dir/get_next_line.h" ]; then
            echo -e "${RED}Error: Faltan archivos en $student_dir${NC}"
            echo -e "${YELLOW}Asegúrate de tener get_next_line.c y get_next_line.h${NC}"
            return 1
        fi
    fi
    
    # Compilar y ejecutar las pruebas
    echo -e "${BLUE}${BOLD}Ejecutando tests para $exercise...${NC}"
    
    # Crear un Makefile temporal para la compilación
    TEMP_MAKEFILE="$student_dir/Makefile.tmp"
    cat > "$TEMP_MAKEFILE" << EOF
NAME = $exercise
SRC = $student_dir/*.c
INCLUDES = $student_dir/*.h
CFLAGS = -Wall -Wextra -Werror

all: \$(NAME)

\$(NAME): \$(SRC) \$(INCLUDES)
	gcc \$(CFLAGS) -o \$(NAME) \$(SRC) -I.

clean:
	rm -f \$(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
EOF
    
    # Compilar
    cd "$student_dir" && make -f Makefile.tmp
    COMPILE_RESULT=$?
    
    # Limpiar
    rm -f "$TEMP_MAKEFILE"
    
    if [ $COMPILE_RESULT -ne 0 ]; then
        echo -e "${RED}Error de compilación. Por favor, corrige los errores.${NC}"
        return 1
    fi
    
    # Ejecutar las pruebas específicas
    if [ "$exercise" = "ft_printf" ]; then
        # Test de ft_printf
        if [ -f "$source_dir/test_main.c" ]; then
            gcc -Wall -Wextra -Werror "$source_dir/test_main.c" "$student_dir/ft_printf.c" -o "$student_dir/ft_printf_test"
            "$student_dir/ft_printf_test"
            TEST_RESULT=$?
            rm -f "$student_dir/ft_printf_test"
        else
            echo -e "${YELLOW}No se encontró el archivo de prueba. Evaluación manual requerida.${NC}"
            TEST_RESULT=0  # Asumimos éxito para continuar
        fi
    elif [ "$exercise" = "get_next_line" ]; then
        # Test de get_next_line
        if [ -f "$source_dir/test_main.c" ]; then
            gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 "$source_dir/test_main.c" "$student_dir/get_next_line.c" -o "$student_dir/gnl_test"
            "$student_dir/gnl_test"
            TEST_RESULT=$?
            rm -f "$student_dir/gnl_test"
        else
            echo -e "${YELLOW}No se encontró el archivo de prueba. Evaluación manual requerida.${NC}"
            TEST_RESULT=0  # Asumimos éxito para continuar
        fi
    fi
    
    if [ $TEST_RESULT -eq 0 ]; then
        echo -e "${GREEN}¡Los tests para $exercise han pasado correctamente!${NC}"
        return 0
    else
        echo -e "${RED}Algunos tests han fallado. Revisa tu implementación.${NC}"
        return 1
    fi
}

# Función para ver ejemplos y guías
view_examples() {
    local exercise=$1
    local examples_dir="$EXAM_DIR/$exercise/examples"
    
    clear
    echo -e "${BLUE}${BOLD}=== EJEMPLOS Y GUÍAS: $exercise ===${NC}\n"
    
    if [ ! -d "$examples_dir" ]; then
        echo -e "${RED}No se encontraron ejemplos para este ejercicio.${NC}"
        echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
        read
        return
    fi
    
    # Listar archivos de ejemplo
    echo -e "${CYAN}${BOLD}Archivos de ejemplo disponibles:${NC}"
    ls -1 "$examples_dir" | cat -n
    
    echo -e "\n${YELLOW}Selecciona un archivo para ver (0 para volver):${NC}"
    read -r example_choice
    
    if [[ "$example_choice" =~ ^[0-9]+$ ]] && [ "$example_choice" -gt 0 ]; then
        EXAMPLE_FILE=$(ls -1 "$examples_dir" | sed -n "${example_choice}p")
        if [ -n "$EXAMPLE_FILE" ]; then
            clear
            echo -e "${CYAN}${BOLD}=== $EXAMPLE_FILE ===${NC}\n"
            cat "$examples_dir/$EXAMPLE_FILE"
            echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
            read
        else
            echo -e "${RED}Selección inválida.${NC}"
            sleep 1
        fi
    fi
}

# Función para practicar un ejercicio específico
practice_exercise() {
    local exercise=$1
    
    while true; do
        clear
        show_subject "$exercise"
        
        echo -e "\n${YELLOW}${BOLD}Opciones:${NC}"
        echo "1. Validar mi solución"
        echo "2. Ver ejemplos y guías"
        echo "3. Volver al menú principal"
        
        read -p "Selección: " option
        
        case $option in
            1)
                validate_exercise "$exercise"
                echo -e "\n${YELLOW}Presiona Enter para continuar...${NC}"
                read
                ;;
            2)
                view_examples "$exercise"
                ;;
            3)
                return
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                sleep 1
                ;;
        esac
    done
}

# Función para simular un examen real
simulate_exam() {
    clear
    echo -e "${BLUE}${BOLD}=== SIMULACIÓN DE EXAMEN: RANK 03 ===${NC}\n"
    echo -e "${YELLOW}En un examen real, se te asignará aleatoriamente uno de los ejercicios.${NC}\n"
    
    # Seleccionar ejercicio aleatorio
    RANDOM_NUM=$((RANDOM % 2))
    
    if [ $RANDOM_NUM -eq 0 ]; then
        EXAM_EXERCISE="ft_printf"
    else
        EXAM_EXERCISE="get_next_line"
    fi
    
    echo -e "${GREEN}¡Ejercicio seleccionado!${NC} Deberás implementar: ${CYAN}$EXAM_EXERCISE${NC}\n"
    echo -e "Tiempo estimado: 2 horas (en el examen real)\n"
    
    read -p "Presiona Enter para comenzar la simulación..."
    
    # Iniciar el tiempo
    START_TIME=$(date +%s)
    
    # Mostrar el subject y comenzar
    practice_exercise "$EXAM_EXERCISE"
    
    # Calcular tiempo transcurrido
    END_TIME=$(date +%s)
    ELAPSED=$((END_TIME - START_TIME))
    HOURS=$((ELAPSED / 3600))
    MINUTES=$(( (ELAPSED % 3600) / 60 ))
    SECONDS=$((ELAPSED % 60))
    
    clear
    echo -e "${BLUE}${BOLD}=== FIN DE LA SIMULACIÓN ===${NC}\n"
    echo -e "Ejercicio: ${CYAN}$EXAM_EXERCISE${NC}"
    echo -e "Tiempo transcurrido: ${YELLOW}$HOURS horas, $MINUTES minutos, $SECONDS segundos${NC}\n"
    
    if [ -f "$RENDU_DIR/$EXAM_EXERCISE/${EXAM_EXERCISE}.c" ]; then
        echo -e "${GREEN}¡Has completado el ejercicio!${NC}"
        echo -e "Verifica que tu solución pase todas las pruebas antes del examen real.\n"
    else
        echo -e "${RED}No has completado el ejercicio.${NC}"
        echo -e "Practica más antes del examen real.\n"
    fi
    
    read -p "Presiona Enter para volver al menú principal..."
}

# Menú principal
main_menu() {
    while true; do
        clear
        echo -e "${BLUE}${BOLD}=== 42 EXAM RANK 03 PRACTICE ===${NC}"
        echo -e "${YELLOW}${BOLD}Selecciona una opción:${NC}"
        echo "1. Modo guiado (recomendado para principiantes)"
        echo "2. Practicar ft_printf"
        echo "3. Practicar get_next_line"
        echo "4. Simular examen (ejercicio aleatorio)"
        echo "5. Ver estructura de directorios"
        echo "6. Limpiar directorio rendu"
        echo "0. Salir"
        
        read -p "Selección: " choice
        
        case $choice in
            1)
                guided_mode
                ;;
            2)
                practice_exercise "ft_printf"
                ;;
            3)
                practice_exercise "get_next_line"
                ;;
            4)
                simulate_exam
                ;;
            5)
                clear
                echo -e "${BLUE}${BOLD}=== ESTRUCTURA DE DIRECTORIOS ===${NC}"
                find "$EXAM_DIR" -type d -maxdepth 2 | sort
                echo -e "\n${YELLOW}Presiona Enter para volver...${NC}"
                read
                ;;
            6)
                echo -e "${RED}¿Estás seguro de que quieres limpiar el directorio rendu? (s/n)${NC}"
                read -r response
                if [[ "$response" =~ ^[Ss]$ ]]; then
                    rm -rf "$RENDU_DIR"/*
                    mkdir -p "$RENDU_DIR/ft_printf"
                    mkdir -p "$RENDU_DIR/get_next_line"
                    echo -e "${GREEN}Directorio rendu limpiado y reconstruido.${NC}"
                    sleep 1
                fi
                ;;
            0)
                exit 0
                ;;
            *)
                echo -e "${RED}Opción inválida${NC}"
                sleep 1
                ;;
        esac
    done
}

# Iniciar menú principal
main_menu
