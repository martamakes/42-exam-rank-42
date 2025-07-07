#!/bin/bash

# Test script para verificar la funcionalidad de directorios subject

echo "🧪 Testing subject directory functionality..."

# Compilar el proyecto
echo "📦 Compiling examshell..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1

if [ ! -f "./examshell" ]; then
    echo "❌ Compilation failed"
    exit 1
fi

echo "✅ Compilation successful"

# Verificar que no existe directorio subject antes
if [ -d "subject" ]; then
    echo "🧹 Cleaning up existing subject directory..."
    rm -rf subject
fi

echo "📁 Subject directory status before running: $([ -d "subject" ] && echo "EXISTS" || echo "NOT FOUND")"

echo ""
echo "🎯 To test the subject directory functionality:"
echo "1. Run: ./examshell 03"
echo "2. When an exercise is selected, check if 'subject/[exercise-name]/' directory is created"
echo "3. Verify that 'subject/[exercise-name]/subject.txt' contains the exercise instructions"
echo "4. When you quit with [q], the subject directory should be automatically cleaned up"
echo ""
echo "📋 Manual test commands:"
echo "   ls -la subject/                    # Check if subject directory exists"
echo "   ls -la subject/*/                  # Check exercise subdirectories"
echo "   cat subject/*/subject.txt          # View copied subject file"
echo ""
echo "Ready to test! Run './examshell 03' to begin."
