// test_main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 4096
int g_tests_failed = 0;

void capture_output(char *const args[], char *output)
{
    int pipefd[2];
    int status;
    
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
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        execv("./inter", args);
        perror("execv");
        exit(1);
    }
    
    close(pipefd[1]);
    
    int total = 0;
    int n;
    while ((n = read(pipefd[0], output + total, BUFFER_SIZE - total - 1)) > 0)
        total += n;
    
    output[total] = '\0';
    close(pipefd[0]);
    
    waitpid(pid, &status, 0);
}

void run_test(char *test_name, char *const args[], const char *expected)
{
    char output[BUFFER_SIZE] = {0};
    
    printf("Test %s: ", test_name);
    capture_output(args, output);
    
    if (strcmp(output, expected) == 0)
        printf("\033[0;32m[OK]\033[0m\n");
    else {
        printf("\033[0;31m[KO]\033[0m\n");
        printf("Expected: \"%s\"\n", expected);
        printf("Got     : \"%s\"\n", output);
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing inter ===\n\n");
    
    // Test 1: Sin argumentos
    {
        char *args[] = {"./inter", NULL};
        run_test("No arguments", args, "\n");
    }
    
    // Test 2: Un solo argumento
    {
        char *args[] = {"./inter", "test", NULL};
        run_test("One argument", args, "\n");
    }
    
    // Test 3: Caso básico del subject
    {
        char *args[] = {"./inter", "padinton", 
                       "paqefwtdjetyiytjneytjoeyjnejeyj", NULL};
        run_test("Subject example 1", args, "padinto\n");
    }
    
    // Test 4: Segundo caso del subject
    {
        char *args[] = {"./inter", "ddf6vewg64f", 
                       "gtwthgdwthdwfteewhrtag6h4ffdhsd", NULL};
        run_test("Subject example 2", args, "df6ewg4\n");
    }
    
    // Test 5: Tercer caso del subject
    {
        char *args[] = {"./inter", "rien", 
                       "cette phrase ne cache rien", NULL};
        run_test("Subject example 3", args, "rien\n");
    }
    
    // Test 6: Strings vacíos
    {
        char *args[] = {"./inter", "", "", NULL};
        run_test("Empty strings", args, "\n");
    }
    
    // Test 7: Sin caracteres en común
    {
        char *args[] = {"./inter", "abc", "def", NULL};
        run_test("No common chars", args, "\n");
    }
    
    // Test 8: Caracteres duplicados
    {
        char *args[] = {"./inter", "aabbcc", "abcabc", NULL};
        run_test("Duplicated chars", args, "abc\n");
    }
    
    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    else
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    
    return g_tests_failed;
}