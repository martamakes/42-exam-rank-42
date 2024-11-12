#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'
YELLOW='\033[1;33m'

# Configuración de directorios
EXERCISE="ft_list_size"
GRADEME_DIR="$(cd "$(dirname "$0")" && pwd)"
EXERCISE_DIR="$(dirname "$GRADEME_DIR")"
LEVEL_DIR="$(dirname "$EXERCISE_DIR")"
EXAM_DIR="$(dirname "$LEVEL_DIR")"
PROJECT_ROOT="$(dirname "$EXAM_DIR")"
RENDU_DIR="$PROJECT_ROOT/rendu/$EXERCISE"

# Verificación inicial de directorios y archivos
echo "Verificando estructura de directorios..."
echo "GRADEME_DIR: $GRADEME_DIR"
echo "EXERCISE_DIR: $EXERCISE_DIR"
echo "LEVEL_DIR: $LEVEL_DIR"
echo "EXAM_DIR: $EXAM_DIR"
echo "PROJECT_ROOT: $PROJECT_ROOT"
echo "RENDU_DIR: $RENDU_DIR"

# Verificar directorio del estudiante
if [ ! -d "$RENDU_DIR" ]; then
    echo -e "${RED}Error: No se encuentra el directorio $RENDU_DIR${NC}"
    echo -e "${YELLOW}Creando directorio...${NC}"
    mkdir -p "$RENDU_DIR"
fi

# Verificar archivos necesarios
if [ ! -f "$RENDU_DIR/$EXERCISE.c" ] || [ ! -f "$RENDU_DIR/ft_list.h" ]; then
    echo -e "${RED}Error: Faltan archivos requeridos en $RENDU_DIR${NC}"
    echo -e "${YELLOW}Asegúrate de tener tanto ft_list_size.c como ft_list.h${NC}"
    exit 1
fi

# Crear directorio temporal
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

# Copiar archivos necesarios
cp "$RENDU_DIR/$EXERCISE.c" "$TEMP_DIR/"
cp "$RENDU_DIR/ft_list.h" "$TEMP_DIR/"
cp "$GRADEME_DIR/test_main.c" "$TEMP_DIR/"

# Crear test_main.c
cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include "ft_list.h"

// Contador global de errores
int g_tests_failed = 0;

// Función auxiliar para crear un nuevo nodo
t_list *create_node(void *data)
{
    t_list *node = malloc(sizeof(t_list));
    if (!node)
        return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

// Función auxiliar para liberar una lista
void free_list(t_list *list)
{
    t_list *current;
    while (list)
    {
        current = list;
        list = list->next;
        free(current);
    }
}

// Función auxiliar para ejecutar un test
void run_test(t_list *list, int expected, const char *test_name)
{
    int result = ft_list_size(list);
    
    printf("Test %s: ", test_name);
    
    if (result == expected)
    {
        printf("\033[0;32m[OK]\033[0m\n");
    }
    else
    {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Expected: %d\n", expected);
        printf("  Got: %d\n", result);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing ft_list_size ===\n\n");

    // Test 1: Lista vacía
    run_test(NULL, 0, "empty list");

    // Test 2: Lista con un elemento
    t_list *single = create_node("one");
    run_test(single, 1, "single element");
    free_list(single);

    // Test 3: Lista con múltiples elementos
    t_list *multi = create_node("one");
    multi->next = create_node("two");
    multi->next->next = create_node("three");
    multi->next->next->next = create_node("four");
    multi->next->next->next->next = create_node("five");
    run_test(multi, 5, "five elements");
    free_list(multi);

    // Test 4: Lista grande
    t_list *large = NULL;
    t_list *current = NULL;
    int i;
    int size = 100;

    for (i = 0; i < size; i++)
    {
        if (!large)
        {
            large = create_node(&i);
            current = large;
        }
        else
        {
            current->next = create_node(&i);
            current = current->next;
        }
    }
    run_test(large, 100, "large list (100 elements)");
    free_list(large);

    // Test 5: Lista con elementos NULL
    t_list *null_data = create_node(NULL);
    null_data->next = create_node(NULL);
    null_data->next->next = create_node(NULL);
    run_test(null_data, 3, "three elements with NULL data");
    free_list(null_data);

    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
    {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    }
    else
    {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}
EOF
cd "$TEMP_DIR" || exit 1


# Compilar
echo -e "${BLUE}Compilando...${NC}"
gcc -Wall -Wextra -Werror test_main.c "$EXERCISE.c" -o test_prog 2>compile_error.txt

if [ $? -ne 0 ]; then
    echo -e "${RED}Error de compilación:${NC}"
    cat compile_error.txt
    exit 1
fi

# Ejecutar tests
echo -e "${BLUE}Ejecutando tests...${NC}"
./test_prog
test_result=$?

if [ $test_result -eq 0 ]; then
    echo -e "${GREEN}¡Todos los tests pasaron correctamente!${NC}"
else
    echo -e "${RED}Algunos tests han fallado${NC}"
fi

# Retornar resultado de los tests
exit $test_result