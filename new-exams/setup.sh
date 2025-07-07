#!/bin/bash

# 🚀 EXAMSHELL - Quick Setup Script
# This script will set up the 42 School Exam Practice System

echo "🎓 42 SCHOOL EXAMSHELL - Quick Setup"
echo "======================================"
echo

# Check if we're in the right directory
if [ ! -f "examshell.h" ]; then
    echo "❌ Error: Please run this script from the examshell directory"
    echo "   Make sure you're in the directory containing examshell.h"
    exit 1
fi

# Check for required tools
echo "🔍 Checking system requirements..."

# Check for GCC
if ! command -v gcc &> /dev/null; then
    echo "❌ GCC compiler not found. Please install GCC first."
    exit 1
fi

# Check for Make
if ! command -v make &> /dev/null; then
    echo "❌ Make not found. Please install Make first."
    exit 1
fi

echo "✅ GCC and Make found"
echo

# Check for exercise directories
echo "🔍 Checking for exercise content..."
found_ranks=0

for rank in 03 04 05; do
    if [ -d "exam-rank-$rank" ]; then
        echo "✅ Found exam-rank-$rank"
        found_ranks=$((found_ranks + 1))
    else
        echo "⚠️  exam-rank-$rank not found"
    fi
done

if [ $found_ranks -eq 0 ]; then
    echo "❌ No exam-rank directories found!"
    echo "   Please make sure you have at least one of: exam-rank-03, exam-rank-04, exam-rank-05"
    exit 1
fi

echo "✅ Found $found_ranks exam ranks"
echo

# Clean any previous builds
echo "🧹 Cleaning previous builds..."
make fclean > /dev/null 2>&1

# Compile the project
echo "🔨 Compiling examshell..."
if make > /dev/null 2>&1; then
    echo "✅ Compilation successful!"
else
    echo "❌ Compilation failed. Running with verbose output:"
    make
    exit 1
fi

# Check if executable was created
if [ ! -f "examshell" ]; then
    echo "❌ Executable not created. Something went wrong."
    exit 1
fi

# Make executable
chmod +x examshell

echo
echo "🎉 SETUP COMPLETE!"
echo "=================="
echo
echo "📖 Quick Start:"
echo "   ./examshell 03    # Start Exam Rank 03 (Intermediate)"
echo "   ./examshell 04    # Start Exam Rank 04 (Advanced)"
echo "   ./examshell 05    # Start Exam Rank 05 (Expert)"
echo "   ./examshell       # Interactive rank selection"
echo
echo "📚 Available exam ranks:"
for rank in 03 04 05; do
    if [ -d "exam-rank-$rank" ]; then
        level1_count=$(find "exam-rank-$rank/level-1" -maxdepth 1 -type d | wc -l)
        level2_count=$(find "exam-rank-$rank/level-2" -maxdepth 1 -type d | wc -l)
        level1_count=$((level1_count - 1))  # Remove the level-1 directory itself
        level2_count=$((level2_count - 1))  # Remove the level-2 directory itself
        echo "   Rank $rank: $level1_count exercises (level 1), $level2_count exercises (level 2)"
    fi
done

echo
echo "🎯 Remember:"
echo "   • Use 3 terminals like in real exam"
echo "   • Read subject: cat subject/[exercise]/subject.txt"
echo "   • Create rendu/[exercise]/ manually"
echo "   • Put your .c and .h files in rendu/[exercise]/"
echo "   • Use [r] option to check your files"
echo
echo "🍀 Good luck with your practice!"
echo
echo "▶️  Ready to start? Run: ./examshell 03"
