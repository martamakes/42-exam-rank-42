#!/bin/bash

# Script de prueba rápida del sistema examshell
# Ejecuta tests básicos para verificar que todo funciona

echo "🧪 Testing examshell system..."
echo "================================"

# Verificar archivos necesarios
echo "📁 Checking required files..."
required_files=("examshell.c" "examshell.h" "Makefile")

for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "✅ $file found"
    else
        echo "❌ $file missing"
        exit 1
    fi
done

# Compilar
echo ""
echo "🔨 Compiling..."
if make; then
    echo "✅ Compilation successful"
else
    echo "❌ Compilation failed"
    exit 1
fi

# Test básico
echo ""
echo "🔍 Running basic tests..."

# Test sin argumentos
echo "Testing without arguments..."
if ./examshell 2>/dev/null; then
    echo "❓ Interactive mode works"
else
    echo "✅ Correctly handles no arguments"
fi

# Test con argumento inválido
echo "Testing with invalid argument..."
if ./examshell 99 2>/dev/null; then
    echo "❌ Should reject invalid rank"
else
    echo "✅ Correctly rejects invalid rank"
fi

echo ""
echo "🎉 Basic tests completed!"
echo "Run './examshell 03' to start practicing!"
