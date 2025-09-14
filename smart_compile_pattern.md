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
# NEW - EDUCATIONAL: Show errors to student, let them fix main conflicts
echo "📦 Compilando..."
if gcc -Wall -Wextra -Werror student_file.c test_main.c -o test_program 2>compile_error.txt; then
    echo "✅ Compilación exitosa"
else
    echo "❌ Error de compilación"
    echo "Detalles del error:"
    cat compile_error.txt
    echo
    if grep -q -E "(multiple definition.*main|duplicate symbol.*main)" compile_error.txt; then
        echo "💡 Tienes una función main en tu código. Para este ejercicio, solo implementa la función."
        echo "💡 Elimina o comenta el main de tu archivo."
    else
        echo "💡 Revisa que tu función esté correctamente implementada"
        echo "💡 Asegúrate de que compile con -Wall -Wextra -Werror"
    fi
    rm -f compile_error.txt
    exit 1
fi

rm -f compile_error.txt
```

## Benefits

1. **Educational approach**: Student sees real compiler errors and learns to fix them
2. **Respects commented main**: `/* int main() {} */` compiles fine, no false errors
3. **Better error reporting**: Shows actual compilation errors when they occur
4. **Clear guidance**: Specific hints for main conflicts vs other compilation errors
5. **Student responsibility**: Teaches proper file management and error interpretation

## Scripts that have been updated

- [x] `/04/level-1/ft_popen/grademe/test.sh` ✅
- [x] `/04/level-1/picoshell/grademe/test.sh` ✅
- [x] `/04/level-1/sandbox/grademe/test.sh` ✅
- [x] `/04/level-2/argo/grademe/test.sh` ✅

## Other scripts that may need updating

- [ ] Any scripts in `/02/` directories that use the old pattern
- [ ] Any scripts in `/03/` or `/05/` directories that use the old pattern

## Template for quick updating

Replace `student_file` with the actual filename (e.g., `picoshell`, `sandbox`, `argo`):

```bash
# Find the old pattern
grep -n "if grep -q.*main.*;" your_test.sh

# Replace with the new pattern above, adjusting the filename
```