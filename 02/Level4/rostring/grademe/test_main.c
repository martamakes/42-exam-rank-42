// test_main_improved.c para rostring
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 4096
int g_tests_failed = 0;

// Función para capturar la salida del programa
void capture_output(char *const args[], char *output)
{
    int pipefd[2];
    int status;
    
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        execv("./rostring", args);
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

// Función auxiliar para ejecutar un test individual
void run_test(char *test_name, char *const args[], const char *expected)
{
    char output[BUFFER_SIZE] = {0};
    
    printf("Test %s:\n", test_name);
    printf("Input: ");
    if (args[1])
        printf("\"%s\"\n", args[1]);
    else
        printf("NULL\n");
        
    capture_output(args, output);
    
    printf("Expected: \"%s\"\n", expected);
    printf("Got:      \"%s\"\n", output);
    
    if (strcmp(output, expected) == 0)
        printf("\033[0;32m[OK]\033[0m\n\n");
    else
    {
        printf("\033[0;31m[KO]\033[0m\n\n");
        g_tests_failed++;
    }
}

int main(void)
{
    printf("\n=== Testing rostring - VERSIÓN EXAMEN ===\n\n");
    
    // Tests originales del subject
    {
        char *args[] = {"./rostring", "Que la lumiere soit et la lumiere fut", NULL};
        run_test("Caso básico del subject", args, "la lumiere soit et la lumiere fut Que\n");
    }
    
    {
        char *args[] = {"./rostring", "     AkjhZ zLKIJz , 23y", NULL};
        run_test("Múltiples espacios iniciales", args, "zLKIJz , 23y AkjhZ\n");
    }
    
    {
        char *args[] = {"./rostring", "first", NULL};
        run_test("Una sola palabra", args, "first\n");
    }
    
    {
        char *args[] = {"./rostring", "abc   ", NULL};
        run_test("Espacios al final", args, "abc\n");
    }
    
    {
        char *args[] = {"./rostring", NULL};
        run_test("Sin argumentos", args, "\n");
    }

    // *** CASOS CRÍTICOS QUE FALTABAN ***

    // Test con string vacía
    {
        char *args[] = {"./rostring", "", NULL};
        run_test("String vacía", args, "\n");
    }
    
    // Test con solo espacios
    {
        char *args[] = {"./rostring", "   ", NULL};
        run_test("Solo espacios", args, "\n");
    }

    // Test con tabs (CRÍTICO - el subject dice espacios/tabs)
    {
        char *args[] = {"./rostring", "Hello\tWorld", NULL};
        run_test("Con tab", args, "World Hello\n");
    }

    {
        char *args[] = {"./rostring", "\tHello\tWorld\t", NULL};
        run_test("Tabs al inicio y final", args, "World Hello\n");
    }

    {
        char *args[] = {"./rostring", "Word1\t\tWord2\tWord3", NULL};
        run_test("Múltiples tabs", args, "Word2 Word3 Word1\n");
    }

    // Test mezclando espacios y tabs
    {
        char *args[] = {"./rostring", " \t Hello \t World \t ", NULL};
        run_test("Espacios y tabs mezclados", args, "World Hello\n");
    }

    {
        char *args[] = {"./rostring", "\t  \t  First   \t  Second  \t  Third   \t  ", NULL};
        run_test("Separadores complejos", args, "Second Third First\n");
    }

    // Test con solo tabs
    {
        char *args[] = {"./rostring", "\t\t\t", NULL};
        run_test("Solo tabs", args, "\n");
    }

    // Test dos palabras básico
    {
        char *args[] = {"./rostring", "Hello World", NULL};
        run_test("Dos palabras básico", args, "World Hello\n");
    }

    // Test tres palabras
    {
        char *args[] = {"./rostring", "One Two Three", NULL};
        run_test("Tres palabras", args, "Two Three One\n");
    }

    // Test muchas palabras
    {
        char *args[] = {"./rostring", "A B C D E F G", NULL};
        run_test("Muchas palabras", args, "B C D E F G A\n");
    }

    // Test con palabras largas
    {
        char *args[] = {"./rostring", "Supercalifragilisticexpialidocious short", NULL};
        run_test("Palabra muy larga", args, "short Supercalifragilisticexpialidocious\n");
    }

    // Test con caracteres especiales
    {
        char *args[] = {"./rostring", "Hello! @World? #Test", NULL};
        run_test("Caracteres especiales", args, "@World? #Test Hello!\n");
    }

    {
        char *args[] = {"./rostring", "user@domain.com password123", NULL};
        run_test("Email y password", args, "password123 user@domain.com\n");
    }

    // Test con números
    {
        char *args[] = {"./rostring", "123 456 789", NULL};
        run_test("Solo números", args, "456 789 123\n");
    }

    {
        char *args[] = {"./rostring", "word1 123 word2", NULL};
        run_test("Números y palabras", args, "123 word2 word1\n");
    }

    // Test con puntuación
    {
        char *args[] = {"./rostring", "Hello, World! How are you?", NULL};
        run_test("Puntuación compleja", args, "World! How are you? Hello,\n");
    }

    // Test con símbolos
    {
        char *args[] = {"./rostring", "(hello) [world] {test}", NULL};
        run_test("Paréntesis y brackets", args, "[world] {test} (hello)\n");
    }

    // Test con comillas
    {
        char *args[] = {"./rostring", "\"first\" 'second' third", NULL};
        run_test("Comillas", args, "'second' third \"first\"\n");
    }

    // Test con underscores
    {
        char *args[] = {"./rostring", "hello_world test_case final_word", NULL};
        run_test("Con underscores", args, "test_case final_word hello_world\n");
    }

    // Test con guiones
    {
        char *args[] = {"./rostring", "first-word second-word third-word", NULL};
        run_test("Con guiones", args, "second-word third-word first-word\n");
    }

    // Test con URLs
    {
        char *args[] = {"./rostring", "https://www.example.com http://test.org", NULL};
        run_test("URLs", args, "http://test.org https://www.example.com\n");
    }

    // Test con paths
    {
        char *args[] = {"./rostring", "/usr/bin/gcc /home/user file.txt", NULL};
        run_test("Paths de archivo", args, "/home/user file.txt /usr/bin/gcc\n");
    }

    // Test con extensiones
    {
        char *args[] = {"./rostring", "file.txt document.pdf image.jpg", NULL};
        run_test("Extensiones archivo", args, "document.pdf image.jpg file.txt\n");
    }

    // Test con números decimales
    {
        char *args[] = {"./rostring", "3.14 2.71 1.41", NULL};
        run_test("Números decimales", args, "2.71 1.41 3.14\n");
    }

    // Test con números negativos
    {
        char *args[] = {"./rostring", "-123 +456 789", NULL};
        run_test("Números con signos", args, "+456 789 -123\n");
    }

    // Test con una letra como primera palabra
    {
        char *args[] = {"./rostring", "a very long sentence", NULL};
        run_test("Primera palabra una letra", args, "very long sentence a\n");
    }

    // Test palabras de una letra
    {
        char *args[] = {"./rostring", "a b c d e", NULL};
        run_test("Palabras de una letra", args, "b c d e a\n");
    }

    // Test con mayúsculas y minúsculas
    {
        char *args[] = {"./rostring", "MiXeD cAsE WoRdS", NULL};
        run_test("Mayúsculas/minúsculas", args, "cAsE WoRdS MiXeD\n");
    }

    // Test con acentos (según ASCII)
    {
        char *args[] = {"./rostring", "café naïve résumé", NULL};
        run_test("Caracteres acentuados", args, "naïve résumé café\n");
    }

    // Test múltiples argumentos (debería imprimir solo \n)
    {
        char *args[] = {"./rostring", "first", "second", "third", NULL};
        run_test("Múltiples argumentos", args, "\n");
    }

    // Test con espacios múltiples entre palabras
    {
        char *args[] = {"./rostring", "word1     word2     word3", NULL};
        run_test("Espacios múltiples entre palabras", args, "word2 word3 word1\n");
    }

    // Test con tabs múltiples entre palabras
    {
        char *args[] = {"./rostring", "word1\t\t\tword2\t\t\tword3", NULL};
        run_test("Tabs múltiples entre palabras", args, "word2 word3 word1\n");
    }

    // Test mezclando todo tipo de separadores
    {
        char *args[] = {"./rostring", "  \t  First  \t \t  Second   \t   Third  \t  ", NULL};
        run_test("Separadores extremos", args, "Second Third First\n");
    }

    // Test con caracteres de control (aunque no deberían estar)
    {
        char *args[] = {"./rostring", "hello\\nworld test", NULL};
        run_test("Con backslashes", args, "test hello\\nworld\n");
    }

    // Test con comillas dentro de palabras
    {
        char *args[] = {"./rostring", "don't can't won't", NULL};
        run_test("Contracciones", args, "can't won't don't\n");
    }

    // Test stress con muchas palabras
    {
        char *args[] = {"./rostring", "one two three four five six seven eight nine ten eleven twelve", NULL};
        run_test("Stress test - muchas palabras", args, "two three four five six seven eight nine ten eleven twelve one\n");
    }

    // Test con solo una palabra y espacios
    {
        char *args[] = {"./rostring", "   onlyword   ", NULL};
        run_test("Una palabra con espacios", args, "onlyword\n");
    }

    // Test con solo una palabra y tabs
    {
        char *args[] = {"./rostring", "\t\tonlyword\t\t", NULL};
        run_test("Una palabra con tabs", args, "onlyword\n");
    }

    // Test palindromo de palabras
    {
        char *args[] = {"./rostring", "level deed radar civic", NULL};
        run_test("Palabras palindrómicas", args, "deed radar civic level\n");
    }

    // Resumen
    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32m✅ Todos los tests pasaron correctamente! Tu rostring está listo para el examen.\033[0m\n");
    else
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);
    
    printf("\n\033[0;33m📝 CASOS CRÍTICOS CUBIERTOS:\033[0m\n");
    printf("✅ Sin argumentos\n");
    printf("✅ String vacía\n");
    printf("✅ Una palabra\n");
    printf("✅ Solo espacios/tabs\n");
    printf("✅ Tabs como separadores\n");
    printf("✅ Espacios y tabs mezclados\n");
    printf("✅ Múltiples argumentos\n");
    printf("✅ Caracteres especiales\n");
    printf("✅ Separadores múltiples\n");
    printf("✅ Stress test\n");
    
    printf("\n\033[0;31m⚠️  PUNTOS CRÍTICOS PARA EL EXAMEN:\033[0m\n");
    printf("• argc != 2: imprimir solo \\n\n");
    printf("• Rotar una palabra a la izquierda\n");
    printf("• Primera palabra va al final\n");
    printf("• Separadores: espacios Y tabs\n");
    printf("• Una sola palabra: imprimir igual\n");
    printf("• String vacía/solo separadores: solo \\n\n");
    printf("• Palabras separadas por UN solo espacio en output\n");

    printf("\n\033[0;36m💡 ALGORITMO RECOMENDADO:\033[0m\n");
    printf("```c\n");
    printf("#include <unistd.h>\n");
    printf("\n");
    printf("int is_space(char c) {\n");
    printf("    return (c == ' ' || c == '\\t');\n");
    printf("}\n");
    printf("\n");
    printf("int main(int argc, char **argv) {\n");
    printf("    if (argc != 2) {\n");
    printf("        write(1, \"\\n\", 1);\n");
    printf("        return 0;\n");
    printf("    }\n");
    printf("    \n");
    printf("    int i = 0;\n");
    printf("    int first_start, first_end;\n");
    printf("    int word_printed = 0;\n");
    printf("    \n");
    printf("    // Saltar espacios iniciales\n");
    printf("    while (argv[1][i] && is_space(argv[1][i])) i++;\n");
    printf("    first_start = i;\n");
    printf("    \n");
    printf("    // Encontrar final de primera palabra\n");
    printf("    while (argv[1][i] && !is_space(argv[1][i])) i++;\n");
    printf("    first_end = i;\n");
    printf("    \n");
    printf("    // Saltar espacios\n");
    printf("    while (argv[1][i] && is_space(argv[1][i])) i++;\n");
    printf("    \n");
    printf("    // Imprimir resto de palabras\n");
    printf("    while (argv[1][i]) {\n");
    printf("        if (!is_space(argv[1][i])) {\n");
    printf("            if (word_printed) write(1, \" \", 1);\n");
    printf("            while (argv[1][i] && !is_space(argv[1][i])) {\n");
    printf("                write(1, &argv[1][i], 1);\n");
    printf("                i++;\n");
    printf("            }\n");
    printf("            word_printed = 1;\n");
    printf("        } else {\n");
    printf("            i++;\n");
    printf("        }\n");
    printf("    }\n");
    printf("    \n");
    printf("    // Imprimir primera palabra al final\n");
    printf("    if (first_end > first_start) {\n");
    printf("        if (word_printed) write(1, \" \", 1);\n");
    printf("        write(1, &argv[1][first_start], first_end - first_start);\n");
    printf("    }\n");
    printf("    \n");
    printf("    write(1, \"\\n\", 1);\n");
    printf("    return 0;\n");
    printf("}\n");
    printf("```\n");

    return g_tests_failed;
}

/*
CASOS CRÍTICOS AÑADIDOS PARA ROSTRING:

🔴 CRÍTICOS PARA EXAMEN:
1. Sin argumentos - Debe imprimir solo \n
2. String vacía - Caso edge común
3. Tabs como separadores - Subject dice espacios/tabs
4. Una palabra - No debe rotar

🟡 IMPORTANTES:
5. Solo espacios/tabs - Solo \n
6. Múltiples argumentos - Solo \n
7. Espacios y tabs mezclados
8. Separadores múltiples

🟢 ADICIONALES:
9. Caracteres especiales
10. URLs y paths
11. Stress test con muchas palabras
12. Casos de puntuación

ERRORES COMUNES QUE DETECTA:
- No manejar argc != 2
- No reconocer tabs como separadores
- No rotar correctamente (primera al final)
- Problemas con string vacía
- No separar palabras con un solo espacio
- No terminar con \n

PUNTOS CLAVE:
- Rotar = primera palabra va AL FINAL
- Separadores: espacios Y tabs
- Output: palabras separadas por UN espacio
- argc != 2: solo \n
- Una palabra: imprimir igual
- String vacía/solo separadores: solo \n

TIP EXAMEN:
La rotación significa que la primera palabra va al final,
no que todas las palabras se mueven una posición.
Es fácil confundirse con el concepto de "rotación".
*/