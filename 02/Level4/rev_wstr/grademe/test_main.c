// test_main_improved.c para rev_wstr
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int g_tests_failed = 0;

typedef struct s_test_case {
    char *input;
    char *expected;
    char *description;
} t_test_case;

// Función para capturar la salida del programa
char *capture_program_output(char *input) {
    int pipefd[2];
    char *output = NULL;
    size_t output_size = 0;
    
    // Calcular tamaño inicial del buffer
    output_size = input ? strlen(input) + 100 : 100;
    output = malloc(output_size);
    if (!output) {
        perror("malloc");
        exit(1);
    }
    memset(output, 0, output_size);
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        free(output);
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        free(output);
        exit(1);
    }
    
    if (pid == 0) {
        // Proceso hijo
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        if (input) {
            char *args[] = {"./rev_wstr", input, NULL};
            execv("./rev_wstr", args);
        } else {
            char *args[] = {"./rev_wstr", NULL};
            execv("./rev_wstr", args);
        }
        perror("execv");
        exit(1);
    } else {
        // Proceso padre
        close(pipefd[1]);
        
        size_t total_read = 0;
        ssize_t bytes_read;
        char buffer[1024];
        
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            if (total_read + bytes_read >= output_size) {
                output_size = total_read + bytes_read + 1;
                char *new_output = realloc(output, output_size);
                if (!new_output) {
                    perror("realloc");
                    free(output);
                    exit(1);
                }
                output = new_output;
            }
            memcpy(output + total_read, buffer, bytes_read);
            total_read += bytes_read;
        }
        
        if (total_read < output_size)
            output[total_read] = '\0';
        
        close(pipefd[0]);
        
        int status;
        waitpid(pid, &status, 0);
    }
    
    return output;
}

// Función para ejecutar un test
void run_test(t_test_case test) {
    printf("Test: %s\n", test.description);
    printf("Input: [%s]\n", test.input ? test.input : "NULL");
    
    char *output = capture_program_output(test.input);
    
    printf("Expected: [%s]\n", test.expected);
    printf("Got:      [%s]\n", output);
    
    if (strcmp(output, test.expected) == 0) {
        printf("\033[0;32m[OK]\033[0m\n\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n\n");
        g_tests_failed++;
    }
    
    free(output);
}

int main(void)
{
    printf("\n=== Testing rev_wstr - VERSIÓN EXAMEN ===\n\n");

    t_test_case tests[] = {
        // Tests originales del subject
        {"You hate people! But I love gatherings. Isn't it ironic?",
         "ironic? it Isn't gatherings. love I But people! hate You\n",
         "Test complejo del subject"},
        
        {"abcdefghijklm",
         "abcdefghijklm\n",
         "Test palabra única"},
        
        {"Wingardium Leviosa",
         "Leviosa Wingardium\n",
         "Test dos palabras"},

        // Tests básicos adicionales
        {"Hello World",
         "World Hello\n",
         "Test básico dos palabras"},
        
        {"The Quick Brown Fox",
         "Fox Brown Quick The\n",
         "Test múltiples palabras"},
        
        {"a",
         "a\n",
         "Test una letra"},
        
        {"a b c",
         "c b a\n",
         "Test letras separadas"},

        // *** CASOS CRÍTICOS QUE FALTABAN ***

        // Test con tabs (CRÍTICO - el subject dice espacios/tabs)
        {"Hello\tWorld",
         "World Hello\n",
         "Test con tab"},

        {"Hello\tWorld\tTest",
         "Test World Hello\n",
         "Test múltiples tabs"},

        {"Word1\t\tWord2",
         "Word2 Word1\n",
         "Test tabs dobles"},

        // Test mezclando espacios y tabs
        {"Hello \t World",
         "World Hello\n",
         "Test espacio + tab"},

        {"Word1\t Word2 \tWord3",
         "Word3 Word2 Word1\n",
         "Test espacios y tabs mezclados"},

        // Test con palabras muy largas
        {"Supercalifragilisticexpialidocious hello",
         "hello Supercalifragilisticexpialidocious\n",
         "Test palabra muy larga"},

        {"short VeryVeryVeryLongWordThatShouldStillWork medium",
         "medium VeryVeryVeryLongWordThatShouldStillWork short\n",
         "Test palabras de diferentes longitudes"},

        // Test con caracteres especiales
        {"Hello! @World? #Test",
         "#Test @World? Hello!\n",
         "Test caracteres especiales"},

        {"user@domain.com password123",
         "password123 user@domain.com\n",
         "Test símbolos comunes"},

        // Test con números
        {"123 456 789",
         "789 456 123\n",
         "Test solo números"},

        {"word1 123 word2",
         "word2 123 word1\n",
         "Test números y palabras"},

        // Test con un solo carácter especial
        {"!",
         "!\n",
         "Test un carácter especial"},

        {"@ # $",
         "$ # @\n",
         "Test símbolos separados"},

        // Test palabras que contienen espacios dentro (no debería pasar según subject)
        // pero vamos a testearlo por si acaso
        
        // Test con mayúsculas y minúsculas mezcladas
        {"MiXeD cAsE WoRdS",
         "WoRdS cAsE MiXeD\n",
         "Test mayúsculas/minúsculas"},

        // Test con string que contiene solo un espacio
        {" ",
         "\n",
         "Test solo un espacio"},

        // Test con string que contiene solo un tab
        {"\t",
         "\n",
         "Test solo un tab"},

        // Test string vacía
        {"",
         "\n",
         "Test string vacía"},

        // Test con muchas palabras (stress test)
        {"one two three four five six seven eight nine ten",
         "ten nine eight seven six five four three two one\n",
         "Test muchas palabras"},

        // Test con palabras de una letra
        {"a b c d e f g h i j",
         "j i h g f e d c b a\n",
         "Test palabras de una letra"},

        // Test palindromo de palabras
        {"level deed radar",
         "radar deed level\n",
         "Test palabras palindrómicas"},

        // Test con números muy largos
        {"123456789 987654321",
         "987654321 123456789\n",
         "Test números largos"},

        // Test con acentos y caracteres especiales (según ASCII)
        {"café naïve résumé",
         "résumé naïve café\n",
         "Test caracteres especiales"},

        // Test con puntuación compleja
        {"Hello, World! How are you?",
         "you? are How World! Hello,\n",
         "Test puntuación compleja"},

        // Test con paréntesis y otros símbolos
        {"(hello) [world] {test}",
         "{test} [world] (hello)\n",
         "Test paréntesis y brackets"},

        // Test con comillas
        {"\"hello\" 'world'",
         "'world' \"hello\"\n",
         "Test comillas"},

        // Test con caracteres de escape (aunque no deberían estar en input según subject)
        {"hello\\nworld test",
         "test hello\\nworld\n",
         "Test backslashes"},

        // Test números negativos
        {"-123 +456 789",
         "789 +456 -123\n",
         "Test números con signos"},

        // Test decimales
        {"3.14 2.71 1.41",
         "1.41 2.71 3.14\n",
         "Test números decimales"},

        // Test con underscores
        {"hello_world test_case final_word",
         "final_word test_case hello_world\n",
         "Test con underscores"},

        // Test con URLs
        {"https://www.example.com http://test.org",
         "http://test.org https://www.example.com\n",
         "Test URLs"},

        // Test con extensiones de archivo
        {"file.txt document.pdf image.jpg",
         "image.jpg document.pdf file.txt\n",
         "Test extensiones archivo"},
    };

    // Test sin argumentos (caso especial)
    printf("Test: Sin argumentos\n");
    printf("Input: [NULL]\n");
    char *no_args_output = capture_program_output(NULL);
    printf("Expected: [\\n]\n");
    printf("Got:      [%s]\n", no_args_output);
    if (strcmp(no_args_output, "\n") == 0) {
        printf("\033[0;32m[OK]\033[0m\n\n");
    } else {
        printf("\033[0;31m[KO]\033[0m\n\n");
        g_tests_failed++;
    }
    free(no_args_output);

    // Ejecutar todos los tests
    size_t num_tests = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < num_tests; i++)
        run_test(tests[i]);

    printf("=== Resumen de tests ===\n");
    if (g_tests_failed == 0)
        printf("\033[0;32m✅ Todos los tests pasaron correctamente! Tu rev_wstr está listo para el examen.\033[0m\n");
    else
        printf("\033[0;31m❌ Se encontraron %d errores. Revisa los casos fallidos.\033[0m\n", g_tests_failed);

    printf("\n\033[0;33m📝 CASOS CRÍTICOS CUBIERTOS:\033[0m\n");
    printf("✅ Sin argumentos\n");
    printf("✅ String vacía\n");
    printf("✅ Una palabra\n");
    printf("✅ Tabs (\\t) como separadores\n");
    printf("✅ Espacios y tabs mezclados\n");
    printf("✅ Palabras muy largas\n");
    printf("✅ Caracteres especiales (!@#$%%)\n");
    printf("✅ Números y símbolos\n");
    printf("✅ Solo espacios/tabs\n");
    printf("✅ Muchas palabras (stress test)\n");
    printf("✅ URLs y extensiones de archivo\n");
    
    printf("\n\033[0;31m⚠️  PUNTOS CRÍTICOS PARA EL EXAMEN:\033[0m\n");
    printf("• argc != 2: imprimir solo \\n\n");
    printf("• Separadores: SOLO espacios y tabs\n");
    printf("• Imprimir palabras en orden inverso\n");
    printf("• Una palabra: imprimir la misma palabra\n");
    printf("• String vacía: imprimir solo \\n\n");
    printf("• Terminar siempre con \\n\n");

    printf("\n\033[0;36m💡 ALGORITMO RECOMENDADO:\033[0m\n");
    printf("```c\n");
    printf("#include <unistd.h>\n");
    printf("\n");
    printf("int is_space(char c) {\n");
    printf("    return (c == ' ' || c == '\\t');\n");
    printf("}\n");
    printf("\n");
    printf("void rev_wstr(char *str) {\n");
    printf("    int i = 0;\n");
    printf("    int end, start;\n");
    printf("    \n");
    printf("    // Ir al final\n");
    printf("    while (str[i]) i++;\n");
    printf("    i--;\n");
    printf("    \n");
    printf("    while (i >= 0) {\n");
    printf("        // Saltar espacios/tabs\n");
    printf("        while (i >= 0 && is_space(str[i])) i--;\n");
    printf("        end = i + 1;\n");
    printf("        \n");
    printf("        // Encontrar inicio de palabra\n");
    printf("        while (i >= 0 && !is_space(str[i])) i--;\n");
    printf("        start = i + 1;\n");
    printf("        \n");
    printf("        // Imprimir palabra si existe\n");
    printf("        if (end > start) {\n");
    printf("            write(1, &str[start], end - start);\n");
    printf("            if (i > 0) write(1, \" \", 1);\n");
    printf("        }\n");
    printf("    }\n");
    printf("}\n");
    printf("\n");
    printf("int main(int argc, char **argv) {\n");
    printf("    if (argc == 2)\n");
    printf("        rev_wstr(argv[1]);\n");
    printf("    write(1, \"\\n\", 1);\n");
    printf("    return 0;\n");
    printf("}\n");
    printf("```\n");

    return g_tests_failed;
}

/*
CASOS CRÍTICOS AÑADIDOS PARA REV_WSTR:

🔴 CRÍTICOS PARA EXAMEN:
1. Sin argumentos - Debe imprimir solo \n
2. Tabs como separadores - El subject dice espacios/tabs
3. String vacía - Caso edge común
4. Una palabra - Caso trivial

🟡 IMPORTANTES:
5. Espacios y tabs mezclados
6. Palabras muy largas - Test de buffer
7. Caracteres especiales - !@#$%
8. Solo espacios/tabs

🟢 ADICIONALES:
9. Números y símbolos
10. URLs y paths
11. Stress test con muchas palabras
12. Casos de puntuación compleja

ERRORES COMUNES QUE DETECTA:
- No manejar argc != 2
- No reconocer tabs como separadores
- Problemas con string vacía
- No imprimir espacios entre palabras
- No terminar con \n
- Problemas con caracteres especiales

CASOS TÍPICOS DEL EXAMEN:
- "" (string vacía)
- "word" (una palabra)
- "hello\tworld" (con tab)
- Sin argumentos
- Palabras con caracteres especiales

TIP EXAMEN:
El subject especifica espacios Y tabs como separadores.
Siempre verificar argc == 2 al inicio.
*/