// test_main.c para fprime - VERSIÓN LIMPIA (solo inputs válidos)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

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
        
        execv("./fprime", args);
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
    printf("\n=== Testing fprime - VERSIÓN EXAMEN (Solo inputs válidos) ===\n\n");
    
    // Tests de argc (no son inputs, sino número de argumentos)
    {
        char *args[] = {"./fprime", NULL};
        run_test("Sin argumentos", args, "\n");
    }
    
    {
        char *args[] = {"./fprime", "42", "21", NULL};
        run_test("Múltiples argumentos", args, "\n");
    }
    
    // Tests originales del subject (todos con inputs válidos)
    {
        char *args[] = {"./fprime", "1", NULL};
        run_test("Número 1", args, "1\n");
    }
    
    {
        char *args[] = {"./fprime", "2", NULL};
        run_test("Número 2 (primer primo)", args, "2\n");
    }
    
    {
        char *args[] = {"./fprime", "3", NULL};
        run_test("Número 3", args, "3\n");
    }
    
    {
        char *args[] = {"./fprime", "4", NULL};
        run_test("Número 4", args, "2*2\n");
    }
    
    {
        char *args[] = {"./fprime", "5", NULL};
        run_test("Número 5", args, "5\n");
    }
    
    {
        char *args[] = {"./fprime", "6", NULL};
        run_test("Número 6", args, "2*3\n");
    }
    
    {
        char *args[] = {"./fprime", "7", NULL};
        run_test("Número 7", args, "7\n");
    }
    
    {
        char *args[] = {"./fprime", "8", NULL};
        run_test("Potencia de 2 (8)", args, "2*2*2\n");
    }
    
    {
        char *args[] = {"./fprime", "9", NULL};
        run_test("Número 9", args, "3*3\n");
    }
    
    {
        char *args[] = {"./fprime", "10", NULL};
        run_test("Número 10", args, "2*5\n");
    }
    
    {
        char *args[] = {"./fprime", "16", NULL};
        run_test("16 (2^4)", args, "2*2*2*2\n");
    }
    
    {
        char *args[] = {"./fprime", "25", NULL};
        run_test("25 (5^2)", args, "5*5\n");
    }
    
    {
        char *args[] = {"./fprime", "27", NULL};
        run_test("27 (3^3)", args, "3*3*3\n");
    }
    
    {
        char *args[] = {"./fprime", "28", NULL};
        run_test("28 (número perfecto)", args, "2*2*7\n");
    }
    
    {
        char *args[] = {"./fprime", "30", NULL};
        run_test("30 (2*3*5)", args, "2*3*5\n");
    }
    
    {
        char *args[] = {"./fprime", "42", NULL};
        run_test("Número pequeño (42)", args, "2*3*7\n");
    }
    
    {
        char *args[] = {"./fprime", "49", NULL};
        run_test("49 (7^2)", args, "7*7\n");
    }
    
    {
        char *args[] = {"./fprime", "60", NULL};
        run_test("60 (muchos factores)", args, "2*2*3*5\n");
    }
    
    {
        char *args[] = {"./fprime", "72", NULL};
        run_test("72 (potencias mixtas)", args, "2*2*2*3*3\n");
    }
    
    {
        char *args[] = {"./fprime", "77", NULL};
        run_test("77 (7*11)", args, "7*11\n");
    }
    
    {
        char *args[] = {"./fprime", "91", NULL};
        run_test("91 (7*13)", args, "7*13\n");
    }
    
    {
        char *args[] = {"./fprime", "97", NULL};
        run_test("Primo grande (97)", args, "97\n");
    }
    
    {
        char *args[] = {"./fprime", "100", NULL};
        run_test("Número 100", args, "2*2*5*5\n");
    }
    
    {
        char *args[] = {"./fprime", "121", NULL};
        run_test("121 (11*11)", args, "11*11\n");
    }
    
    {
        char *args[] = {"./fprime", "169", NULL};
        run_test("169 (13*13)", args, "13*13\n");
    }
    
    {
        char *args[] = {"./fprime", "210", NULL};
        run_test("210 (2*3*5*7)", args, "2*3*5*7\n");
    }
    
    {
        char *args[] = {"./fprime", "997", NULL};
        run_test("Primo muy grande (997)", args, "997\n");
    }
    
    {
        char *args[] = {"./fprime", "1000", NULL};
        run_test("Número 1000", args, "2*2*2*5*5*5\n");
    }
    
    {
        char *args[] = {"./fprime", "9539", NULL};
        run_test("Número primo (9539)", args, "9539\n");
    }
    
    {
        char *args[] = {"./fprime", "225225", NULL};
        run_test("Factores múltiples (225225)", args, "3*3*5*5*7*11*13\n");
    }

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32m✅ Todos los tests pasaron correctamente! Tu fprime está listo para el examen.\033[0m\n");
    else
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);
    
    printf("\n\033[0;33m📝 CASOS CRÍTICOS CUBIERTOS (Solo inputs válidos):\033[0m\n");
    printf("✅ Números pequeños (1-10)\n");
    printf("✅ Potencias de primos (4, 8, 16, 25, 27, 49)\n");
    printf("✅ Primos pequeños y grandes (2, 3, 5, 7, 97, 997, 9539)\n");
    printf("✅ Números compuestos típicos (42, 100, 1000)\n");
    printf("✅ Cuadrados de primos (25, 49, 121, 169)\n");
    printf("✅ Números con muchos factores (60, 72, 210)\n");
    printf("✅ Casos del subject original (225225, 9539, 42)\n");
    
    printf("\n\033[0;31m⚠️  PUNTOS CRÍTICOS PARA EL EXAMEN:\033[0m\n");
    printf("• argc != 2: imprimir solo \\n\n");
    printf("• Número 1: imprimir \"1\\n\"\n");
    printf("• Factores en orden ascendente separados por '*'\n");
    printf("• Todos los inputs serán números positivos válidos\n");
    printf("• Terminar siempre con \\n\n");

    printf("\n\033[0;36m💡 ALGORITMO RECOMENDADO:\033[0m\n");
    printf("```c\n");
    printf("#include <stdio.h>\n");
    printf("#include <stdlib.h>\n");
    printf("\n");
    printf("void fprime(int n)\n");
    printf("{\n");
    printf("    int factor = 2;\n");
    printf("    int first = 1;\n");
    printf("    \n");
    printf("    if (n == 1) {\n");
    printf("        printf(\"1\");\n");
    printf("        return;\n");
    printf("    }\n");
    printf("    \n");
    printf("    while (n > 1) {\n");
    printf("        if (n %% factor == 0) {\n");
    printf("            if (!first)\n");
    printf("                printf(\"*\");\n");
    printf("            printf(\"%%d\", factor);\n");
    printf("            first = 0;\n");
    printf("            n /= factor;\n");
    printf("        } else {\n");
    printf("            factor++;\n");
    printf("        }\n");
    printf("    }\n");
    printf("}\n");
    printf("\n");
    printf("int main(int argc, char **argv)\n");
    printf("{\n");
    printf("    if (argc != 2) {\n");
    printf("        printf(\"\\n\");\n");
    printf("        return 0;\n");
    printf("    }\n");
    printf("    \n");
    printf("    int n = atoi(argv[1]);\n");
    printf("    fprime(n);\n");
    printf("    printf(\"\\n\");\n");
    printf("    return 0;\n");
    printf("}\n");
    printf("```\n");

    printf("\n\033[0;35m🧮 CASOS ESPECIALES A RECORDAR:\033[0m\n");
    printf("• 1: siempre imprime \"1\"\n");
    printf("• 2: primer número primo, imprime \"2\"\n");
    printf("• Potencias: 8 -> \"2*2*2\", 27 -> \"3*3*3\"\n");
    printf("• Primos: 97 -> \"97\"\n");
    printf("• Productos: 42 -> \"2*3*7\"\n");

    return g_tests_failed;
}

/*
TESTS ELIMINADOS (inputs no válidos según subject):

❌ ELIMINADOS - NO SON INPUTS VÁLIDOS:
1. Número 0 - No es un entero positivo válido
2. Números negativos (-42) - No son enteros positivos
3. Strings no numéricos ("abc", "", "   ") - No son números válidos
4. Strings mixtos ("42abc", " 42", "+42") - No son enteros válidos limpios
5. Números muy grandes que causan overflow - Fuera del rango válido

✅ MANTENIDOS - SON INPUTS VÁLIDOS:
1. Tests de argc (0 o >1 argumentos) - No son inputs, sino casos de uso
2. Todos los números enteros positivos (1, 2, 3, ..., 225225)
3. Casos del subject original

JUSTIFICACIÓN:
El subject dice claramente: "The input, when there is one, will be valid"
Esto significa que solo debemos testear con números enteros positivos válidos.
Los casos de argc != 2 siguen siendo válidos porque no son inputs sino casos de uso.

CASOS TÍPICOS DEL EXAMEN (Solo válidos):
- 1 (caso especial)
- 2-10 (números pequeños)
- 42 (número favorito de 42)
- Potencias: 4, 8, 16, 25, 27, 49
- Primos: 97, 997, 9539
- Compuestos: 100, 225225

TIP EXAMEN:
Con inputs válidos, el algoritmo es más simple.
No necesitas manejar casos de error de atoi.
Solo enfócate en la factorización correcta.
*/