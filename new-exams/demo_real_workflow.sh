#!/bin/bash

# Demostración del flujo real del examshell con 3 terminales

echo "🎯 EXAMSHELL - Real Exam Workflow Demo"
echo "======================================"
echo ""
echo "This demonstrates the REAL examshell workflow used in 42 School exams:"
echo ""
echo "📺 TERMINAL SETUP (you need 3 terminals):"
echo ""
echo "Terminal 1: EXAMSHELL CONTROL"
echo "  ./examshell 03"
echo "  (This terminal controls the exam session)"
echo ""
echo "Terminal 2: READ SUBJECT"
echo "  cat subject/[exercise-name]/subject.txt"
echo "  (This terminal displays exercise instructions)"
echo ""
echo "Terminal 3: CODE SOLUTION"
echo "  vim [exercise-name].c"
echo "  gcc [exercise-name].c -o [exercise-name]"
echo "  ./[exercise-name]"
echo "  (This terminal is for coding and testing)"
echo ""
echo "🔄 TYPICAL EXAM WORKFLOW:"
echo ""
echo "1. Start examshell in Terminal 1"
echo "2. Exercise assigned (e.g., 'filter')"
echo "3. Subject directory created: subject/filter/subject.txt"
echo "4. In Terminal 2: cat subject/filter/subject.txt"
echo "5. In Terminal 3: vim filter.c"
echo "6. Code, compile, test in Terminal 3"
echo "7. In Terminal 1: [g] grademe when ready"
echo "8. Repeat for next level"
echo ""
echo "✅ Key differences from our old system:"
echo "  ❌ NO automatic subject display"
echo "  ✅ Must manually read subject from file"
echo "  ✅ Simulates real exam pressure"
echo "  ✅ Forces multi-terminal workflow"
echo ""
echo "🚀 Ready to test? Run: ./examshell 03"
echo "   Then open 2 more terminals for the full experience!"

# Compilar si no existe
if [ ! -f "./examshell" ]; then
    echo ""
    echo "📦 Compiling examshell first..."
    make > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo "✅ Compilation successful"
    else
        echo "❌ Compilation failed"
        exit 1
    fi
fi
