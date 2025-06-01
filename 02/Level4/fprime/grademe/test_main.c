// test_main_improved.c para fprime
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
    printf("\n=== Testing fprime - VERSIÓN EXAMEN ===\n\n");
    
    // Tests originales del subject
    {
        char *args[] = {"./fprime", NULL};
        run_test("Sin argumentos", args, "\n");
    }
    
    {
        char *args[] = {"./fprime", "42", "21", NULL};
        run_test("Múltiples argumentos", args, "\n");
    }
    
    {
        char *args[] = {"./fprime", "1", NULL};
        run_test("Número 1", args, "1\n");
    }
    
    {
        char *args[] = {"./fprime", "9539", NULL};
        run_test("Número primo", args, "9539\n");
    }
    
    {
        char *args[] = {"./fprime", "225225", NULL};
        run_test("Factores múltiples", args, "3*3*5*5*7*11*13\n");
    }
    
    {
        char *args[] = {"./fprime", "42", NULL};
        run_test("Número pequeño", args, "2*3*7\n");
    }

    // *** CASOS CRÍTICOS QUE FALTABAN ***

    // Test con número 0 (CRÍTICO)
    {
        char *args[] = {"./fprime", "0", NULL};
        run_test("Número 0", args, "0\n");
    }

    // Test con número 2 (primer primo)
    {
        char *args[] = {"./fprime", "2", NULL};
        run_test("Número 2 (primer primo)", args, "2\n");
    }

    // Test con número 3
    {
        char *args[] = {"./fprime", "3", NULL};
        run_test("Número 3", args, "3\n");
    }

    // Test con número 4 (primera potencia)
    {
        char *args[] = {"./fprime", "4", NULL};
        run_test("Número 4", args, "2*2\n");
    }

    // Test con número 8 (potencia de 2)
    {
        char *args[] = {"./fprime", "8", NULL};
        run_test("Potencia de 2", args, "2*2*2\n");
    }

    // Test con número 16 (potencia alta de 2)
    {
        char *args[] = {"./fprime", "16", NULL};
        run_test("16 (2^4)", args, "2*2*2*2\n");
    }

    // Test con número 100
    {
        char *args[] = {"./fprime", "100", NULL};
        run_test("Número 100", args, "2*2*5*5\n");
    }

    // Test con número 1000
    {
        char *args[] = {"./fprime", "1000", NULL};
        run_test("Número 1000", args, "2*2*2*5*5*5\n");
    }

    // Test con potencia de 3
    {
        char *args[] = {"./fprime", "27", NULL};
        run_test("27 (3^3)", args, "3*3*3\n");
    }

    // Test con producto de primos pequeños
    {
        char *args[] = {"./fprime", "30", NULL};
        run_test("30 (2*3*5)", args, "2*3*5\n");
    }

    // Test con número primo grande
    {
        char *args[] = {"./fprime", "97", NULL};
        run_test("Primo grande (97)", args, "97\n");
    }

    // Test con número primo más grande
    {
        char *args[] = {"./fprime", "997", NULL};
        run_test("Primo muy grande (997)", args, "997\n");
    }

    // Test casos con atoi edge cases

    // Test con espacios al inicio
    {
        char *args[] = {"./fprime", " 42", NULL};
        run_test("Espacios al inicio", args, "2*3*7\n");
    }

    // Test con espacios al final
    {
        char *args[] = {"./fprime", "42 ", NULL};
        run_test("Espacios al final", args, "2*3*7\n");
    }

    // Test con ceros a la izquierda
    {
        char *args[] = {"./fprime", "0042", NULL};
        run_test("Ceros a la izquierda", args, "2*3*7\n");
    }

    // Test con signo + (positivo explícito)
    {
        char *args[] = {"./fprime", "+42", NULL};
        run_test("Signo + explícito", args, "2*3*7\n");
    }

    // Test con número muy pequeño (casos triviales)
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
        char *args[] = {"./fprime", "9", NULL};
        run_test("Número 9", args, "3*3\n");
    }

    {
        char *args[] = {"./fprime", "10", NULL};
        run_test("Número 10", args, "2*5\n");
    }

    // Test con números que son productos de primos grandes
    {
        char *args[] = {"./fprime", "77", NULL};
        run_test("77 (7*11)", args, "7*11\n");
    }

    {
        char *args[] = {"./fprime", "91", NULL};
        run_test("91 (7*13)", args, "7*13\n");
    }

    // Test con número que parece primo pero no lo es
    {
        char *args[] = {"./fprime", "121", NULL};
        run_test("121 (11*11)", args, "11*11\n");
    }

    {
        char *args[] = {"./fprime", "169", NULL};
        run_test("169 (13*13)", args, "13*13\n");
    }

    // Test con casos especiales de atoi

    // Test con string no numérico (comportamiento de atoi)
    {
        char *args[] = {"./fprime", "abc", NULL};
        run_test("String no numérico", args, "1\n"); // atoi("abc") = 0, pero tu código trata 0 como caso especial
    }

    // Test con string que empieza con número
    {
        char *args[] = {"./fprime", "42abc", NULL};
        run_test("Número seguido de letras", args, "2*3*7\n");
    }

    // Test con string vacío
    {
        char *args[] = {"./fprime", "", NULL};
        run_test("String vacío", args, "1\n");
    }

    // Test con solo espacios
    {
        char *args[] = {"./fprime", "   ", NULL};
        run_test("Solo espacios", args, "1\n");
    }

    // CASOS CRÍTICOS CON NÚMEROS NEGATIVOS (según atoi)
    // Nota: atoi convierte números negativos, pero la factorización prima
    // matemáticamente solo se define para números positivos

    // Test con número negativo
    {
        char *args[] = {"./fprime", "-42", NULL};
        run_test("Número negativo", args, "1\n"); // Comportamiento indefinido, depende de implementación
    }

    // Test con número muy grande (cerca del límite de int)
    {
        char *args[] = {"./fprime", "2147483647", NULL}; // INT_MAX
        run_test("INT_MAX (primo)", args, "2147483647\n");
    }

    // Test con factorización de número grande
    {
        char *args[] = {"./fprime", "2147483646", NULL}; // INT_MAX - 1
        run_test("INT_MAX - 1", args, "2*3*7*11*31*151*331\n");
    }

    // Test números perfectos pequeños
    {
        char *args[] = {"./fprime", "28", NULL}; // 2^2 * 7
        run_test("28 (número perfecto)", args, "2*2*7\n");
    }

    // Test con números con muchos factores pequeños
    {
        char *args[] = {"./fprime", "60", NULL}; // 2^2 * 3 * 5
        run_test("60 (muchos factores)", args, "2*2*3*5\n");
    }

    {
        char *args[] = {"./fprime", "72", NULL}; // 2^3 * 3^2
        run_test("72 (potencias mixtas)", args, "2*2*2*3*3\n");
    }

    // Test con número que es producto de muchos primos distintos
    {
        char *args[] = {"./fprime", "210", NULL}; // 2*3*5*7
        run_test("210 (2*3*5*7)", args, "2*3*5*7\n");
    }

    // Test edge case: número que es cuadrado de primo
    {
        char *args[] = {"./fprime", "49", NULL}; // 7^2
        run_test("49 (7^2)", args, "7*7\n");
    }

    {
        char *args[] = {"./fprime", "25", NULL}; // 5^2
        run_test("25 (5^2)", args, "5*5\n");
    }

    // Resumen
    printf("\n=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32m✅ Todos los tests pasaron correctamente! Tu fprime está listo para el examen.\033[0m\n");
    else
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);
    
    printf("\n\033[0;33m📝 CASOS CRÍTICOS CUBIERTOS:\033[0m\n");
    printf("✅ Número 0 (caso especial)\n");
    printf("✅ Números pequeños (2-10)\n");
    printf("✅ Potencias de primos (4, 8, 16, 27)\n");
    printf("✅ Primos grandes (97, 997)\n");
    printf("✅ Casos de atoi (espacios, +, ceros)\n");
    printf("✅ Strings no numéricos\n");
    printf("✅ Números negativos\n");
    printf("✅ Números muy grandes (INT_MAX)\n");
    printf("✅ Cuadrados de primos\n");
    printf("✅ Números con muchos factores\n");
    
    printf("\n\033[0;31m⚠️  PUNTOS CRÍTICOS PARA EL EXAMEN:\033[0m\n");
    printf("• argc != 2: imprimir solo \\n\n");
    printf("• Número 1: imprimir \"1\\n\"\n");
    printf("• Número 0: comportamiento puede variar según implementación\n");
    printf("• Factores en orden ascendente separados por '*'\n");
    printf("• atoi maneja espacios y signos automáticamente\n");
    printf("• Números negativos: comportamiento indefinido\n");
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
    printf("• 0: puede imprimir \"0\" o \"1\" según implementación\n");
    printf("• 1: siempre imprime \"1\"\n");
    printf("• 2: primer número primo, imprime \"2\"\n");
    printf("• Potencias: 8 -> \"2*2*2\", 27 -> \"3*3*3\"\n");
    printf("• Primos: 97 -> \"97\"\n");
    printf("• Productos: 42 -> \"2*3*7\"\n");

    return g_tests_failed;
}

/*
CASOS CRÍTICOS AÑADIDOS PARA FPRIME:

🔴 CRÍTICOS PARA EXAMEN:
1. Número 0 - Caso especial muy común en exámenes
2. Números 2-10 - Casos básicos que siempre testean
3. Potencias de primos - 4, 8, 16, 27 (casos típicos)
4. Strings no numéricos - Comportamiento de atoi

🟡 IMPORTANTES:
5. Espacios y signos - atoi maneja automáticamente
6. Primos grandes - Verificar algoritmo
7. Números negativos - Comportamiento indefinido
8. Cuadrados de primos - 25, 49, 121

🟢 ADICIONALES:
9. Números con muchos factores - 60, 72, 210
10. Números muy grandes - INT_MAX
11. Casos edge de atoi - strings vacíos, mixtos

ERRORES COMUNES QUE DETECTA:
- No manejar argc != 2 correctamente
- No imprimir "1" para n = 1
- Orden incorrecto de factores
- No separar factores con '*'
- No manejar casos especiales de atoi
- Problemas con números negativos o 0
- No terminar con '\n'

CASOS TÍPICOS DEL EXAMEN:
- 0, 1, 2 (casos especiales)
- 4, 8, 9, 16 (potencias pequeñas)
- 42 (número favorito de 42 school)
- Primos como 97
- Strings como "abc", " 42", "+42"

TIP EXAMEN:
El algoritmo es simple pero hay muchos edge cases.
Practica especialmente el manejo de 0, 1 y strings inválidos.
*/