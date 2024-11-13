#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Contador global de errores
int g_tests_failed = 0;

// Prototipo de la función a testear
void print_bits(unsigned char octet);

// Función para capturar la salida de print_bits
void get_output(unsigned char input, char *output) 
{
    int     pipefd[2];
    int     stdout_backup;
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }
    
    // Guardar stdout original
    stdout_backup = dup(STDOUT_FILENO);
    if (stdout_backup == -1) {
        perror("dup");
        return;
    }
    
    // Redirigir stdout al pipe
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        perror("dup2");
        return;
    }
    
    // Llamar a la función
    print_bits(input);
    fflush(stdout);
    
    // Restaurar stdout
    if (dup2(stdout_backup, STDOUT_FILENO) == -1) {
        perror("dup2");
        return;
    }
    
    close(pipefd[1]);
    
    // Leer la salida capturada
    int bytes_read = read(pipefd[0], output, 8);
    if (bytes_read == -1) {
        perror("read");
        return;
    }
    output[bytes_read] = '\0';
    
    close(pipefd[0]);
    close(stdout_backup);
}

// Función de test individual
void run_test(char *test_name, unsigned char input, const char *expected) 
{
    char output[9] = {0};  // 8 bits + null terminator
    
    printf("Test %s: ", test_name);
    get_output(input, output);
    
    if (strcmp(output, expected) == 0) {
        printf("\033[0;32m[OK]\033[0m\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("  Input    : %u\n", input);
        printf("  Expected : \"%s\"\n", expected);
        printf("  Got      : \"%s\"\n", output);
        g_tests_failed++;
    }
}

int main(void) 
{
    printf("\n=== Testing print_bits ===\n\n");

    // Tests básicos
    run_test("Subject example", 2, "00000010");
    run_test("Zero", 0, "00000000");
    run_test("All ones", 255, "11111111");
    run_test("One", 1, "00000001");
    run_test("Powers of 2", 128, "10000000");
    run_test("Alternating", 170, "10101010");
    run_test("Random byte", 42, "00101010");
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }

    return g_tests_failed;
}