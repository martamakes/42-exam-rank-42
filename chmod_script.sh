#!/bin/bash

# Script para dar permisos de ejecución a todos los scripts relevantes

# Directorios
EXAM_DIR="/Users/marta/Documents/PROJECTS/42-exam-practice/42-Exam-Rank-02/04"

# Dar permisos al script principal
chmod +x $EXAM_DIR/exam.sh

# Dar permisos a los scripts de microshell
mkdir -p $EXAM_DIR/microshell/grademe
chmod +x $EXAM_DIR/microshell/microshell_guide 2>/dev/null || echo "No existe microshell_guide compilado"
chmod +x $EXAM_DIR/microshell/grademe/test.sh 2>/dev/null || echo "No existe test.sh"
chmod +x $EXAM_DIR/microshell/grademe/grademe.sh 2>/dev/null || echo "No existe grademe.sh"

echo "Permisos actualizados correctamente."
