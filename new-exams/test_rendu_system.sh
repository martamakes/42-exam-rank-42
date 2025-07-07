#!/bin/bash

# Test del sistema de directorios rendu
echo "🧪 Testing RENDU system implementation..."
echo

# Compilar el proyecto
echo "📦 Compiling examshell..."
make clean && make
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed"
    exit 1
fi

echo "✅ Compilation successful"
echo

# Crear algunos archivos de prueba en rendu para simular trabajo del estudiante
echo "📁 Creating test structure..."

# Simular que el usuario trabajó en algunos ejercicios
mkdir -p rendu/filter
echo '// Test filter solution' > rendu/filter/filter.c
echo '// Test filter header' > rendu/filter/filter.h

mkdir -p rendu/scanf
echo '// Test scanf solution' > rendu/scanf/ft_scanf.c

echo "📋 Current directory structure:"
ls -la
echo

if [ -d "rendu" ]; then
    echo "📁 rendu/ directory contents:"
    find rendu -type f -exec echo "  {}" \;
    echo
fi

echo "🎯 Test scenarios:"
echo "1. Run ./examshell 03 and test [r] option to see rendu files"
echo "2. Test rank change (03 -> 04) to verify rendu cleanup"
echo "3. Test new session option to verify rendu cleanup"
echo

echo "🚀 Ready to test! Run: ./examshell 03"
