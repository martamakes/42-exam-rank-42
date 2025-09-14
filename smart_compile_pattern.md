# Smart Compilation Pattern for 42 Test Scripts

## Problem
Test scripts need to handle student files that may or may not have a `main()` function, but only remove it when there's an actual compilation conflict.

## Solution Pattern

Replace this old pattern:
```bash
# OLD - BAD: Blindly removes main based on text search
if grep -q "^int main(" student_file.c; then
    sed '/^int main(/,$d' student_file.c > student_file_clean.c
    mv student_file_clean.c student_file.c
fi
gcc -Wall -Wextra -Werror student_file.c test_main.c -o test_program
```

With this new pattern:
```bash
# NEW - GOOD: Only removes main if compiler reports conflict
echo "📦 Compilando..."
if gcc -Wall -Wextra -Werror student_file.c test_main.c -o test_program 2>compile_error.txt; then
    echo "✅ Compilación exitosa"
else
    # Check if the error is specifically about multiple main functions
    if grep -q -E "(multiple definition.*main|duplicate symbol.*main)" compile_error.txt; then
        echo "⚠️  Conflicto de función main detectado - limpiando..."
        
        # Remove main function from student file
        sed '/^[[:space:]]*int[[:space:]]*main[[:space:]]*(/,$d' student_file.c > student_file_clean.c
        mv student_file_clean.c student_file.c
        
        # Try to compile again
        echo "📦 Recompilando sin función main..."
        if gcc -Wall -Wextra -Werror student_file.c test_main.c -o test_program 2>/dev/null; then
            echo "✅ Conflicto de main resuelto"
        else
            echo "❌ Error de compilación persiste después de limpiar main"
            echo "💡 Revisa tu implementación"
            rm -f compile_error.txt
            exit 1
        fi
    else
        echo "❌ Error de compilación (no relacionado con main)"
        echo "Detalles del error:"
        cat compile_error.txt
        echo "💡 Revisa que tu función esté correctamente implementada"
        echo "💡 Asegúrate de que compile con -Wall -Wextra -Werror"
        rm -f compile_error.txt
        exit 1
    fi
fi

# Clean up error file
rm -f compile_error.txt
```

## Benefits

1. **Respects commented main**: `/* int main() {} */` won't be removed unnecessarily
2. **Compiler-based detection**: Uses actual compilation errors, not text patterns
3. **Better error reporting**: Shows actual compilation errors when they occur
4. **Works for all cases**: Functions, programs, mixed exercises
5. **No false positives**: Only acts when there's a real conflict

## Scripts that need updating

- [x] `/04/level-1/ft_popen/grademe/test.sh` (already updated)
- [ ] `/04/level-1/picoshell/grademe/test.sh`
- [ ] `/04/level-1/sandbox/grademe/test.sh` 
- [ ] `/04/level-2/argo/grademe/test.sh`
- [ ] Any other scripts using the old pattern

## Template for quick updating

Replace `student_file` with the actual filename (e.g., `picoshell`, `sandbox`, `argo`):

```bash
# Find the old pattern
grep -n "if grep -q.*main.*;" your_test.sh

# Replace with the new pattern above, adjusting the filename
```