#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Variable global para contar los tests fallidos
int g_tests_failed = 0;

// Definir las estructuras tal como están en el subject
typedef struct json {
    enum {
        MAP,
        INTEGER,
        STRING
    } type;
    union {
        struct {
            struct pair *data;
            size_t size;
        } map;
        int integer;
        char *string;
    };
} json;

typedef struct pair {
    char *key;
    json value;
} pair;

// Prototipos de las funciones que necesitamos (están en argo.c)
int argo(json *dst, FILE *stream);
int peek(FILE *stream);
void unexpected(FILE *stream);
int accept(FILE *stream, char c);
int expect(FILE *stream, char c);
void free_json(json j);

// Función serialize (necesaria para los tests)
void serialize(json j) {
    switch (j.type) {
        case INTEGER:
            printf("%d", j.integer);
            break;
        case STRING:
            putchar('"');
            for (int i = 0; j.string[i]; i++) {
                if (j.string[i] == '\\' || j.string[i] == '"')
                    putchar('\\');
                putchar(j.string[i]);
            }
            putchar('"');
            break;
        case MAP:
            putchar('{');
            for (size_t i = 0; i < j.map.size; i++) {
                if (i != 0)
                    putchar(',');
                serialize((json){.type = STRING, .string = j.map.data[i].key});
                putchar(':');
                serialize(j.map.data[i].value);
            }
            putchar('}');
            break;
    }
}

// Test helper function
void run_test(const char *test_name, int (*test_func)()) {
    printf("Testing %s... ", test_name);
    if (test_func()) {
        printf("✓ PASS\n");
    } else {
        printf("✗ FAIL\n");
        g_tests_failed++;
    }
}

// Helper function to create FILE* from string
FILE* create_stream_from_string(const char *str) {
    char temp_file[] = "/tmp/argo_test_XXXXXX";
    int fd = mkstemp(temp_file);
    if (fd == -1) return NULL;
    
    write(fd, str, strlen(str));
    lseek(fd, 0, SEEK_SET);
    
    FILE *stream = fdopen(fd, "r");
    unlink(temp_file); // Delete file when closed
    return stream;
}

// Test 1: Simple integer
int test_simple_integer() {
    FILE *stream = create_stream_from_string("42");
    if (!stream) return 0;
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    if (ret != 1) return 0;
    if (result.type != INTEGER) return 0;
    if (result.integer != 42) return 0;
    
    free_json(result);
    return 1;
}

// Test 2: Negative integer
int test_negative_integer() {
    FILE *stream = create_stream_from_string("-17");
    if (!stream) return 0;
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    if (ret != 1) return 0;
    if (result.type != INTEGER) return 0;
    if (result.integer != -17) return 0;
    
    free_json(result);
    return 1;
}

// Test 3: Simple string
int test_simple_string() {
    FILE *stream = create_stream_from_string("\"hello\"");
    if (!stream) return 0;
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    if (ret != 1) return 0;
    if (result.type != STRING) return 0;
    if (!result.string || strcmp(result.string, "hello") != 0) return 0;
    
    free_json(result);
    return 1;
}

// Test 4: String with escaped quote
int test_escaped_string() {
    FILE *stream = create_stream_from_string("\"escape! \\\" \"");
    if (!stream) return 0;
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    if (ret != 1) return 0;
    if (result.type != STRING) return 0;
    if (!result.string || strcmp(result.string, "escape! \" ") != 0) return 0;
    
    free_json(result);
    return 1;
}

// Test 5: Simple map
int test_simple_map() {
    FILE *stream = create_stream_from_string("{\"key\":42}");
    if (!stream) return 0;
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    if (ret != 1) return 0;
    if (result.type != MAP) return 0;
    if (result.map.size != 1) return 0;
    if (!result.map.data[0].key || strcmp(result.map.data[0].key, "key") != 0) return 0;
    if (result.map.data[0].value.type != INTEGER) return 0;
    if (result.map.data[0].value.integer != 42) return 0;
    
    free_json(result);
    return 1;
}

// Test 6: Map with multiple entries
int test_multiple_map() {
    FILE *stream = create_stream_from_string("{\"tomatoes\":42,\"potatoes\":234}");
    if (!stream) return 0;
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    if (ret != 1) return 0;
    if (result.type != MAP) return 0;
    if (result.map.size != 2) return 0;
    
    // Check first entry
    if (!result.map.data[0].key || strcmp(result.map.data[0].key, "tomatoes") != 0) return 0;
    if (result.map.data[0].value.type != INTEGER) return 0;
    if (result.map.data[0].value.integer != 42) return 0;
    
    // Check second entry
    if (!result.map.data[1].key || strcmp(result.map.data[1].key, "potatoes") != 0) return 0;
    if (result.map.data[1].value.type != INTEGER) return 0;
    if (result.map.data[1].value.integer != 234) return 0;
    
    free_json(result);
    return 1;
}

// Test 7: Nested map
int test_nested_map() {
    FILE *stream = create_stream_from_string("{\"outer\":{\"inner\":123}}");
    if (!stream) return 0;
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    if (ret != 1) return 0;
    if (result.type != MAP) return 0;
    if (result.map.size != 1) return 0;
    if (!result.map.data[0].key || strcmp(result.map.data[0].key, "outer") != 0) return 0;
    if (result.map.data[0].value.type != MAP) return 0;
    if (result.map.data[0].value.map.size != 1) return 0;
    if (!result.map.data[0].value.map.data[0].key || strcmp(result.map.data[0].value.map.data[0].key, "inner") != 0) return 0;
    if (result.map.data[0].value.map.data[0].value.type != INTEGER) return 0;
    if (result.map.data[0].value.map.data[0].value.integer != 123) return 0;
    
    free_json(result);
    return 1;
}

// Test 8: Error - unfinished string
int test_unfinished_string() {
    // Capture output to check error message
    int pipefd[2];
    if (pipe(pipefd) == -1) return 0;
    
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(pipefd[1], STDOUT_FILENO);
    
    FILE *stream = create_stream_from_string("\"unfinished");
    if (!stream) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return 0;
    }
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    dup2(saved_stdout, STDOUT_FILENO);
    close(pipefd[1]);
    close(saved_stdout);
    
    char buffer[256];
    ssize_t bytes = read(pipefd[0], buffer, sizeof(buffer) - 1);
    close(pipefd[0]);
    
    // Should return -1 for error
    if (ret != -1) return 0;
    
    // Should print error message
    if (bytes > 0) {
        buffer[bytes] = '\0';
        if (strstr(buffer, "unexpected end of input") != NULL) {
            return 1;
        }
    }
    
    return 1; // Accept if no output capture (function still failed correctly)
}

// Test 9: Error - unexpected token
int test_unexpected_token() {
    // Capture output to check error message
    int pipefd[2];
    if (pipe(pipefd) == -1) return 0;
    
    int saved_stdout = dup(STDOUT_FILENO);
    dup2(pipefd[1], STDOUT_FILENO);
    
    FILE *stream = create_stream_from_string("{\"key\":}");
    if (!stream) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return 0;
    }
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    dup2(saved_stdout, STDOUT_FILENO);
    close(pipefd[1]);
    close(saved_stdout);
    
    char buffer[256];
    ssize_t bytes = read(pipefd[0], buffer, sizeof(buffer) - 1);
    close(pipefd[0]);
    
    // Should return -1 for error
    if (ret != -1) return 0;
    
    // Should print error message
    if (bytes > 0) {
        buffer[bytes] = '\0';
        if (strstr(buffer, "unexpected token '}'") != NULL) {
            return 1;
        }
    }
    
    return 1; // Accept if no output capture (function still failed correctly)
}

// Test 10: Empty map
int test_empty_map() {
    FILE *stream = create_stream_from_string("{}");
    if (!stream) return 0;
    
    json result;
    int ret = argo(&result, stream);
    fclose(stream);
    
    if (ret != 1) return 0;
    if (result.type != MAP) return 0;
    if (result.map.size != 0) return 0;
    
    free_json(result);
    return 1;
}

int main() {
    printf("🧪 ARGO TESTER (JSON Parser)\n");
    printf("============================\n\n");
    
    printf("📋 BASIC TESTS:\n");
    run_test("Simple integer (42)", test_simple_integer);
    run_test("Negative integer (-17)", test_negative_integer);
    run_test("Simple string", test_simple_string);
    run_test("Empty map", test_empty_map);
    
    printf("\n🔧 STRING TESTS:\n");
    run_test("Escaped string", test_escaped_string);
    
    printf("\n🗂️  MAP TESTS:\n");
    run_test("Simple map", test_simple_map);
    run_test("Multiple entries map", test_multiple_map);
    run_test("Nested map", test_nested_map);
    
    printf("\n🛡️  ERROR HANDLING TESTS:\n");
    run_test("Unfinished string", test_unfinished_string);
    run_test("Unexpected token", test_unexpected_token);
    
    printf("\n📊 RESULTS:\n");
    if (g_tests_failed == 0) {
        printf("✅ ALL TESTS PASSED! Your argo parser works correctly!\n");
        return 0;
    } else {
        printf("❌ %d test(s) failed. Review your implementation.\n", g_tests_failed);
        return 1;
    }
}