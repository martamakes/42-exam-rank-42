/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marta <marta@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-05 16:50:41 by marta            #+#    #+#             */
/*   Updated: 2025-05-05 16:50:41 by marta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/*
 * Este tester compara la salida de la función ft_printf con la del printf real
 * Solo prueba los casos mencionados en el enunciado: conversiones %s, %d y %x
 */

// Prototipo de la función a testear
int ft_printf(const char *format, ...);

// Contador global de errores
int g_tests_failed = 0;

// Función para capturar la salida de cualquier función a un archivo
void capture_output_to_file(const char *file_name, int (*func)(const char *, ...), const char *format, ...) {
    // Guardar stdout original
    int original_stdout = dup(STDOUT_FILENO);
    
    // Redirigir stdout al archivo
    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    
    // Configurar argumentos variables y llamar a la función
    va_list args;
    va_start(args, format);
    
    // Llamar a vfprintf o ft_printf según el puntero de función
    int result;
    if (func == (int (*)(const char *, ...))printf) {
        result = vprintf(format, args);
    } else {
        // Llamar a ft_printf con los argumentos individuales
        // Esto es un hack ya que ft_printf no tiene vft_printf
        char buffer[1024];
        vsprintf(buffer, format, args);
        
        // Ahora reconfigurar args para cada tipo específico
        va_list new_args;
        va_start(new_args, format);
        result = func(format, va_arg(new_args, void *), va_arg(new_args, void *), va_arg(new_args, void *));
        va_end(new_args);
    }
    
    va_end(args);
    
    // Flush stdout para asegurar que toda la salida está en el archivo
    fflush(stdout);
    
    // Restaurar stdout original
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdout);
    close(fd);
}

// Función para test de string simple
void test_string_simple() {
    const char *test_name = "String simple";
    printf("\033[0;34m=== Test: %s ===\033[0m\n", test_name);
    
    // Capturar salida de printf original
    FILE *original_file = fopen("original.txt", "w");
    if (!original_file) {
        perror("fopen");
        g_tests_failed++;
        return;
    }
    
    int stdout_backup = dup(STDOUT_FILENO);
    dup2(fileno(original_file), STDOUT_FILENO);
    int original_ret = printf("%s\n", "toto");
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    fclose(original_file);
    
    // Capturar salida de ft_printf
    FILE *custom_file = fopen("custom.txt", "w");
    if (!custom_file) {
        perror("fopen");
        g_tests_failed++;
        return;
    }
    
    dup2(fileno(custom_file), STDOUT_FILENO);
    int custom_ret = ft_printf("%s\n", "toto");
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    fclose(custom_file);
    close(stdout_backup);
    
    // Leer y comparar archivos
    original_file = fopen("original.txt", "r");
    custom_file = fopen("custom.txt", "r");
    
    if (!original_file || !custom_file) {
        perror("fopen para lectura");
        g_tests_failed++;
        if (original_file) fclose(original_file);
        if (custom_file) fclose(custom_file);
        return;
    }
    
    char original_buffer[4096] = {0};
    char custom_buffer[4096] = {0};
    
    fread(original_buffer, 1, sizeof(original_buffer) - 1, original_file);
    fread(custom_buffer, 1, sizeof(custom_buffer) - 1, custom_file);
    
    fclose(original_file);
    fclose(custom_file);
    
    // Comparar resultados
    if (strcmp(original_buffer, custom_buffer) == 0 && original_ret == custom_ret) {
        printf("\033[0;32m[OK] Salida correcta ✓\033[0m\n");
        printf("Valor de retorno: %d\n", custom_ret);
    } else {
        g_tests_failed++;
        printf("\033[0;31m[KO] Salida incorrecta ✗\033[0m\n");
        
        if (strcmp(original_buffer, custom_buffer) != 0) {
            printf("Salida esperada: \"%s\"\n", original_buffer);
            printf("Salida obtenida: \"%s\"\n", custom_buffer);
        }
        
        if (original_ret != custom_ret) {
            printf("Valor de retorno esperado: %d\n", original_ret);
            printf("Valor de retorno obtenido: %d\n", custom_ret);
        }
    }
    
    // Limpiar archivos temporales
    remove("original.txt");
    remove("custom.txt");
    
    printf("\n");
}

// Función para test de string con entero
void test_string_with_integer() {
    const char *test_name = "String con entero";
    printf("\033[0;34m=== Test: %s ===\033[0m\n", test_name);
    
    // Capturar salida de printf original
    FILE *original_file = fopen("original.txt", "w");
    if (!original_file) {
        perror("fopen");
        g_tests_failed++;
        return;
    }
    
    int stdout_backup = dup(STDOUT_FILENO);
    dup2(fileno(original_file), STDOUT_FILENO);
    int original_ret = printf("Magic %s is %d", "number", 42);
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    fclose(original_file);
    
    // Capturar salida de ft_printf
    FILE *custom_file = fopen("custom.txt", "w");
    if (!custom_file) {
        perror("fopen");
        g_tests_failed++;
        return;
    }
    
    dup2(fileno(custom_file), STDOUT_FILENO);
    int custom_ret = ft_printf("Magic %s is %d", "number", 42);
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    fclose(custom_file);
    close(stdout_backup);
    
    // Leer y comparar archivos
    original_file = fopen("original.txt", "r");
    custom_file = fopen("custom.txt", "r");
    
    if (!original_file || !custom_file) {
        perror("fopen para lectura");
        g_tests_failed++;
        if (original_file) fclose(original_file);
        if (custom_file) fclose(custom_file);
        return;
    }
    
    char original_buffer[4096] = {0};
    char custom_buffer[4096] = {0};
    
    fread(original_buffer, 1, sizeof(original_buffer) - 1, original_file);
    fread(custom_buffer, 1, sizeof(custom_buffer) - 1, custom_file);
    
    fclose(original_file);
    fclose(custom_file);
    
    // Comparar resultados
    if (strcmp(original_buffer, custom_buffer) == 0 && original_ret == custom_ret) {
        printf("\033[0;32m[OK] Salida correcta ✓\033[0m\n");
        printf("Valor de retorno: %d\n", custom_ret);
    } else {
        g_tests_failed++;
        printf("\033[0;31m[KO] Salida incorrecta ✗\033[0m\n");
        
        if (strcmp(original_buffer, custom_buffer) != 0) {
            printf("Salida esperada: \"%s\"\n", original_buffer);
            printf("Salida obtenida: \"%s\"\n", custom_buffer);
        }
        
        if (original_ret != custom_ret) {
            printf("Valor de retorno esperado: %d\n", original_ret);
            printf("Valor de retorno obtenido: %d\n", custom_ret);
        }
    }
    
    // Limpiar archivos temporales
    remove("original.txt");
    remove("custom.txt");
    
    printf("\n");
}

// Función para test de entero y hexadecimal
void test_integer_and_hex() {
    const char *test_name = "Entero y hexadecimal";
    printf("\033[0;34m=== Test: %s ===\033[0m\n", test_name);
    
    // Capturar salida de printf original
    FILE *original_file = fopen("original.txt", "w");
    if (!original_file) {
        perror("fopen");
        g_tests_failed++;
        return;
    }
    
    int stdout_backup = dup(STDOUT_FILENO);
    dup2(fileno(original_file), STDOUT_FILENO);
    int original_ret = printf("Hexadecimal for %d is %x\n", 42, 42);
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    fclose(original_file);
    
    // Capturar salida de ft_printf
    FILE *custom_file = fopen("custom.txt", "w");
    if (!custom_file) {
        perror("fopen");
        g_tests_failed++;
        return;
    }
    
    dup2(fileno(custom_file), STDOUT_FILENO);
    int custom_ret = ft_printf("Hexadecimal for %d is %x\n", 42, 42);
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    fclose(custom_file);
    close(stdout_backup);
    
    // Leer y comparar archivos
    original_file = fopen("original.txt", "r");
    custom_file = fopen("custom.txt", "r");
    
    if (!original_file || !custom_file) {
        perror("fopen para lectura");
        g_tests_failed++;
        if (original_file) fclose(original_file);
        if (custom_file) fclose(custom_file);
        return;
    }
    
    char original_buffer[4096] = {0};
    char custom_buffer[4096] = {0};
    
    fread(original_buffer, 1, sizeof(original_buffer) - 1, original_file);
    fread(custom_buffer, 1, sizeof(custom_buffer) - 1, custom_file);
    
    fclose(original_file);
    fclose(custom_file);
    
    // Comparar resultados
    if (strcmp(original_buffer, custom_buffer) == 0 && original_ret == custom_ret) {
        printf("\033[0;32m[OK] Salida correcta ✓\033[0m\n");
        printf("Valor de retorno: %d\n", custom_ret);
    } else {
        g_tests_failed++;
        printf("\033[0;31m[KO] Salida incorrecta ✗\033[0m\n");
        
        if (strcmp(original_buffer, custom_buffer) != 0) {
            printf("Salida esperada: \"%s\"\n", original_buffer);
            printf("Salida obtenida: \"%s\"\n", custom_buffer);
        }
        
        if (original_ret != custom_ret) {
            printf("Valor de retorno esperado: %d\n", original_ret);
            printf("Valor de retorno obtenido: %d\n", custom_ret);
        }
    }
    
    // Limpiar archivos temporales
    remove("original.txt");
    remove("custom.txt");
    
    printf("\n");
}

// Función para test casos adicionales
void test_additional_cases() {
    const char *test_name = "Casos adicionales";
    printf("\033[0;34m=== Test: %s ===\033[0m\n", test_name);
    
    // Lista de casos a probar
    struct {
        const char *desc;
        const char *format;
        void *arg1;
        void *arg2;
        void *arg3;
    } cases[] = {
        {"String vacía", "%s", "", NULL, NULL},
        {"String NULL", "%s", NULL, NULL, NULL},
        {"Entero positivo grande", "%d", (void *)(intptr_t)2147483647, NULL, NULL},
        {"Entero negativo", "%d", (void *)(intptr_t)-42, NULL, NULL},
        {"Entero negativo grande", "%d", (void *)(intptr_t)-2147483648, NULL, NULL},
        {"Hexadecimal 0", "%x", (void *)(intptr_t)0, NULL, NULL},
        {"Hexadecimal grande", "%x", (void *)(intptr_t)0xffff, NULL, NULL},
        {"Conversiones mixtas", "%s %d %x", "test", (void *)(intptr_t)42, (void *)(intptr_t)255},
    };
    
    int total_cases = sizeof(cases) / sizeof(cases[0]);
    int case_failures = 0;
    
    for (int i = 0; i < total_cases; i++) {
        printf("\033[0;34m  Subcaso: %s\033[0m\n", cases[i].desc);
        
        // Capturar salida de printf original
        FILE *original_file = fopen("original.txt", "w");
        if (!original_file) {
            perror("fopen");
            g_tests_failed++;
            case_failures++;
            continue;
        }
        
        int stdout_backup = dup(STDOUT_FILENO);
        dup2(fileno(original_file), STDOUT_FILENO);
        
        int original_ret;
        if (cases[i].arg3) {
            original_ret = printf(cases[i].format, cases[i].arg1, cases[i].arg2, cases[i].arg3);
        } else if (cases[i].arg2) {
            original_ret = printf(cases[i].format, cases[i].arg1, cases[i].arg2);
        } else {
            original_ret = printf(cases[i].format, cases[i].arg1);
        }
        
        fflush(stdout);
        dup2(stdout_backup, STDOUT_FILENO);
        fclose(original_file);
        
        // Capturar salida de ft_printf
        FILE *custom_file = fopen("custom.txt", "w");
        if (!custom_file) {
            perror("fopen");
            g_tests_failed++;
            case_failures++;
            continue;
        }
        
        dup2(fileno(custom_file), STDOUT_FILENO);
        
        int custom_ret;
        if (cases[i].arg3) {
            custom_ret = ft_printf(cases[i].format, cases[i].arg1, cases[i].arg2, cases[i].arg3);
        } else if (cases[i].arg2) {
            custom_ret = ft_printf(cases[i].format, cases[i].arg1, cases[i].arg2);
        } else {
            custom_ret = ft_printf(cases[i].format, cases[i].arg1);
        }
        
        fflush(stdout);
        dup2(stdout_backup, STDOUT_FILENO);
        fclose(custom_file);
        
        // Leer y comparar archivos
        original_file = fopen("original.txt", "r");
        custom_file = fopen("custom.txt", "r");
        
        if (!original_file || !custom_file) {
            perror("fopen para lectura");
            g_tests_failed++;
            case_failures++;
            if (original_file) fclose(original_file);
            if (custom_file) fclose(custom_file);
            continue;
        }
        
        char original_buffer[4096] = {0};
        char custom_buffer[4096] = {0};
        
        fread(original_buffer, 1, sizeof(original_buffer) - 1, original_file);
        fread(custom_buffer, 1, sizeof(custom_buffer) - 1, custom_file);
        
        fclose(original_file);
        fclose(custom_file);
        
        // Comparar resultados
        if (strcmp(original_buffer, custom_buffer) == 0 && original_ret == custom_ret) {
            printf("    \033[0;32m[OK] Salida correcta ✓\033[0m\n");
        } else {
            g_tests_failed++;
            case_failures++;
            printf("    \033[0;31m[KO] Salida incorrecta ✗\033[0m\n");
            
            if (strcmp(original_buffer, custom_buffer) != 0) {
                printf("    Salida esperada: \"%s\"\n", original_buffer);
                printf("    Salida obtenida: \"%s\"\n", custom_buffer);
            }
            
            if (original_ret != custom_ret) {
                printf("    Valor de retorno esperado: %d\n", original_ret);
                printf("    Valor de retorno obtenido: %d\n", custom_ret);
            }
        }
    }
    
    printf("\nResumen del subcaso: ");
    if (case_failures == 0) {
        printf("\033[0;32m%d/%d OK ✓\033[0m\n", total_cases, total_cases);
    } else {
        printf("\033[0;31m%d/%d OK, %d KO ✗\033[0m\n", total_cases - case_failures, total_cases, case_failures);
    }
    
    // Limpiar archivos temporales
    remove("original.txt");
    remove("custom.txt");
}

int main(void) {
    // Inicializar contador
    g_tests_failed = 0;
    
    printf("\033[0;34m=========================================\033[0m\n");
    printf("\033[0;34m=            FT_PRINTF TESTER           =\033[0m\n");
    printf("\033[0;34m=========================================\033[0m\n\n");
    
    // Pruebas del enunciado
    test_string_simple();
    test_string_with_integer();
    test_integer_and_hex();
    
    // Pruebas adicionales
    test_additional_cases();
    
    // Resumen
    printf("\033[0;34m=========================================\033[0m\n");
    printf("\033[0;34m=               RESUMEN                 =\033[0m\n");
    printf("\033[0;34m=========================================\033[0m\n\n");
    
    if (g_tests_failed == 0) {
        printf("\033[0;32mTodos los tests pasaron correctamente ✓\033[0m\n");
    } else {
        printf("\033[0;31mSe encontraron %d errores ✗\033[0m\n", g_tests_failed);
    }
    
    return g_tests_failed;
}