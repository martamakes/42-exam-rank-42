#!/bin/bash

# Test script para broken_GNL

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${YELLOW}=== TESTING BROKEN_GNL ===${NC}"

# Limpiar archivos previos
rm -f gnl_test test_main.c test_file*.txt

# Verificar que existen los archivos necesarios
if [ ! -f "get_next_line.c" ]; then
    echo -e "${RED}❌ Error: get_next_line.c no encontrado${NC}"
    exit 1
fi

if [ ! -f "get_next_line.h" ]; then
    echo -e "${RED}❌ Error: get_next_line.h no encontrado${NC}"
    exit 1
fi

# VALIDACIÓN DE FUNCIONES PROHIBIDAS
echo -e "${CYAN}Verificando funciones prohibidas...${NC}"
ALLOWED_FUNCS="read|free|malloc"

# Eliminar comentarios multilínea /* */ y comentarios de línea //
CODE_NO_COMMENTS=$(sed -e ':a' -e 's|/\*.*\*/||g' -e '/\/\*/,/\*\//d' -e 's|//.*||g' get_next_line.c)

FORBIDDEN=$(echo "$CODE_NO_COMMENTS" | grep -oE '\b[a-z_][a-z0-9_]*\s*\(' | \
            grep -v "^main\|^get_next_line\|^ft_\|^str_" | \
            grep -vE "^($ALLOWED_FUNCS)\s*\(" | \
            grep -vE "^(if|while|return|switch|for|sizeof)\s*\(" | \
            sort -u)

if [ ! -z "$FORBIDDEN" ]; then
    echo -e "${RED}❌ Funciones prohibidas detectadas:${NC}"
    echo "$FORBIDDEN"
    echo -e "${YELLOW}Funciones permitidas: read, free, malloc${NC}"
    exit 1
fi
echo -e "${GREEN}✅ No se detectaron funciones prohibidas${NC}"

# Variables para contar tests
PASSED=0
TOTAL=0

# Test 1: Lectura básica de múltiples líneas
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Lectura básica de 3 líneas${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;
    int count = 0;

    fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "Line 1\nLine 2\nLine 3\n", 21);
    close(fd);

    fd = open("test_file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL) {
        count++;
        free(line);
    }
    close(fd);
    remove("test_file.txt");

    if (count == 3)
        printf("OK");
    else
        printf("FAIL:%d", count);
    return 0;
}
EOF

if ! gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_main.c get_next_line.c -o gnl_test 2>/dev/null; then
    echo -e "${RED}❌ Error de compilación${NC}"
    gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_main.c get_next_line.c -o gnl_test
    exit 1
fi

output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Test 2: Línea sin \n al final (EOF)
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Línea sin newline al final${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;
    int count = 0;

    fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "Line without newline", 20);
    close(fd);

    fd = open("test_file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL) {
        count++;
        free(line);
    }
    close(fd);
    remove("test_file.txt");

    if (count == 1)
        printf("OK");
    else
        printf("FAIL:%d", count);
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_main.c get_next_line.c -o gnl_test 2>/dev/null
output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Test 3: Archivo vacío
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Archivo vacío${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;

    fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    close(fd);

    fd = open("test_file.txt", O_RDONLY);
    line = get_next_line(fd);
    close(fd);
    remove("test_file.txt");

    if (line == NULL)
        printf("OK");
    else {
        printf("FAIL");
        free(line);
    }
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_main.c get_next_line.c -o gnl_test 2>/dev/null
output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Test 4: BUFFER_SIZE pequeño (1)
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: BUFFER_SIZE=1${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;
    int count = 0;

    fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "ABC\nDEF\n", 8);
    close(fd);

    fd = open("test_file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL) {
        count++;
        free(line);
    }
    close(fd);
    remove("test_file.txt");

    if (count == 2)
        printf("OK");
    else
        printf("FAIL:%d", count);
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=1 test_main.c get_next_line.c -o gnl_test 2>/dev/null
output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Test 5: BUFFER_SIZE grande (9999)
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: BUFFER_SIZE=9999${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;
    int count = 0;

    fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "First\nSecond\nThird\n", 19);
    close(fd);

    fd = open("test_file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL) {
        count++;
        free(line);
    }
    close(fd);
    remove("test_file.txt");

    if (count == 3)
        printf("OK");
    else
        printf("FAIL:%d", count);
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=9999 test_main.c get_next_line.c -o gnl_test 2>/dev/null
output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Test 6: Línea muy larga
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Línea muy larga${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;
    char long_line[1001];
    int i;

    // Crear línea de 1000 caracteres
    for (i = 0; i < 1000; i++)
        long_line[i] = 'A';
    long_line[1000] = '\0';

    fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, long_line, 1000);
    write(fd, "\n", 1);
    close(fd);

    fd = open("test_file.txt", O_RDONLY);
    line = get_next_line(fd);
    close(fd);
    remove("test_file.txt");

    if (line && strlen(line) == 1001) {
        printf("OK");
        free(line);
    } else {
        printf("FAIL");
        if (line)
            free(line);
    }
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=10 test_main.c get_next_line.c -o gnl_test 2>/dev/null
output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Test 7: Solo newlines
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Solo newlines${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;
    int count = 0;

    fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "\n\n\n", 3);
    close(fd);

    fd = open("test_file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL) {
        count++;
        free(line);
    }
    close(fd);
    remove("test_file.txt");

    if (count == 3)
        printf("OK");
    else
        printf("FAIL:%d", count);
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_main.c get_next_line.c -o gnl_test 2>/dev/null
output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Test 8: FD inválido
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: FD inválido${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int main(void)
{
    char *line;

    line = get_next_line(-1);

    if (line == NULL)
        printf("OK");
    else {
        printf("FAIL");
        free(line);
    }
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_main.c get_next_line.c -o gnl_test 2>/dev/null
output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Test 9: Mezcla de líneas con y sin \n
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Mezcla de líneas${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;
    int count = 0;

    fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "Line1\n\nLine3\nLast", 18);
    close(fd);

    fd = open("test_file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL) {
        count++;
        free(line);
    }
    close(fd);
    remove("test_file.txt");

    if (count == 4)
        printf("OK");
    else
        printf("FAIL:%d", count);
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_main.c get_next_line.c -o gnl_test 2>/dev/null
output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Test 10: Verificar contenido exacto
TOTAL=$((TOTAL + 1))
echo -e "${YELLOW}Test $TOTAL: Verificar contenido exacto${NC}"

cat > test_main.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "get_next_line.h"

int main(void)
{
    int fd;
    char *line;

    fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "Hello\n", 6);
    close(fd);

    fd = open("test_file.txt", O_RDONLY);
    line = get_next_line(fd);
    close(fd);
    remove("test_file.txt");

    if (line && strcmp(line, "Hello\n") == 0) {
        printf("OK");
        free(line);
    } else {
        printf("FAIL");
        if (line)
            free(line);
    }
    return 0;
}
EOF

gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 test_main.c get_next_line.c -o gnl_test 2>/dev/null
output=$(./gnl_test 2>/dev/null)
if [ "$output" == "OK" ]; then
    echo -e "${GREEN}✅ Test $TOTAL pasado${NC}"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}❌ Test $TOTAL fallido (output: $output)${NC}"
fi

# Limpiar archivos temporales
rm -f gnl_test test_main.c test_file*.txt

# Resultado final
echo
echo -e "${CYAN}======================================${NC}"
echo -e "${CYAN}Tests pasados: $PASSED/$TOTAL${NC}"
echo -e "${CYAN}======================================${NC}"

if [ $PASSED -eq $TOTAL ]; then
    echo -e "${GREEN}🎉 ¡Todos los tests pasados!${NC}"
    exit 0
else
    echo -e "${RED}❌ Algunos tests fallaron${NC}"
    exit 1
fi
