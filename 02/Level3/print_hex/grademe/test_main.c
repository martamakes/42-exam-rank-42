#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int g_tests_failed = 0;

// Versión mejorada de capture_output que garantiza capturar toda la salida
void capture_output(char *const args[], char *output, size_t size)
{
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        execv("./print_hex", args);
        perror("execv");
        exit(1);
    }
    
    // Proceso padre
    close(pipefd[1]); // Cerramos el extremo de escritura
    
    // Leemos en un bucle para asegurar capturar toda la salida
    int total_read = 0;
    int bytes_read;
    char buffer[128];
    
    // Usamos select para esperar la disponibilidad de datos con timeout
    fd_set read_fds;
    struct timeval timeout;
    
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(pipefd[0], &read_fds);
        
        // Establecemos un pequeño timeout
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // 100ms
        
        int ready = select(pipefd[0] + 1, &read_fds, NULL, NULL, &timeout);
        
        if (ready == -1) {
            perror("select");
            break;
        }
        
        if (ready == 0) {
            // Timeout - probablemente ya no hay más datos
            break;
        }
        
        // Hay datos disponibles para leer
        bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        
        if (bytes_read <= 0) {
            // Fin de archivo o error
            break;
        }
        
        // Aseguramos no desbordar el buffer de salida
        if (total_read + bytes_read >= size - 1) {
            memcpy(output + total_read, buffer, size - 1 - total_read);
            total_read = size - 1;
            break;
        }
        
        // Copiamos los datos leídos al buffer de salida
        memcpy(output + total_read, buffer, bytes_read);
        total_read += bytes_read;
    }
    
    // Terminamos la cadena
    output[total_read] = '\0';
    
    // Cerramos el extremo de lectura
    close(pipefd[0]);
    
    // Esperamos a que el proceso hijo termine
    int status;
    waitpid(pid, &status, 0);
    
    // Pequeña pausa para asegurar que todos los datos sean procesados
    usleep(10000); // 10ms
}

void run_test(char *test_name, char *const args[], const char *expected)
{
    char output[256] = {0};
    
    printf("Test %s: ", test_name);
    capture_output(args, output, sizeof(output));
    
    if (strcmp(output, expected) == 0)
        printf("\033[0;32m[OK]\033[0m\n");
    else {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("Expected: \"");
        for (int i = 0; expected[i]; i++) {
            if (expected[i] == '\n')
                printf("\\n");
            else
                printf("%c", expected[i]);
        }
        printf("\"\n");
        
        printf("Got     : \"");
        for (int i = 0; output[i]; i++) {
            if (output[i] == '\n')
                printf("\\n");
            else
                printf("%c", output[i]);
        }
        printf("\"\n");
        
        // Mostrar donde difieren
        int i = 0;
        while (expected[i] && output[i] && expected[i] == output[i])
            i++;
        
        if (expected[i] || output[i]) {
            printf("Difference at position %d: ", i);
            if (expected[i])
                printf("expected '%c' (%d) ", expected[i], expected[i]);
            else
                printf("expected end of string ");
            
            if (output[i])
                printf("got '%c' (%d)\n", output[i], output[i]);
            else
                printf("got end of string\n");
        }
        
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing print_hex ===\n\n");
    
    // Test 1: Ejemplo del subject
    {
        char *args[] = {"./print_hex", "10", NULL};
        run_test("Subject example 1", args, "a\n");
    }
    
    // Test 2: Otro ejemplo del subject
    {
        char *args[] = {"./print_hex", "255", NULL};
        run_test("Subject example 2", args, "ff\n");
    }
    
    // Test 3: Ejemplo grande del subject
    {
        char *args[] = {"./print_hex", "5156454", NULL};
        run_test("Subject example 3", args, "4eae66\n");
    }
    
    // Test 4: Sin argumentos
    {
        char *args[] = {"./print_hex", NULL};
        run_test("No arguments", args, "\n");
    }
    
    // Test 5: Cero
    {
        char *args[] = {"./print_hex", "0", NULL};
        run_test("Zero", args, "0\n");
    }
    
    // Test 6: Número pequeño
    {
        char *args[] = {"./print_hex", "16", NULL};
        run_test("Small number", args, "10\n");
    }
    
    // Test 7: Número grande
    {
        char *args[] = {"./print_hex", "1000000", NULL};
        run_test("Big number", args, "f4240\n");
    }
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}