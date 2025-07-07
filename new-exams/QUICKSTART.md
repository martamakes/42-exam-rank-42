# 🚀 EXAMSHELL - Quick Start Guide

## 1. Instalación Rápida

```bash
# Hacer ejecutables los scripts
chmod +x install.sh test.sh

# Ejecutar instalación automática
./install.sh

# O compilar manualmente
make
```

## 2. Uso Inmediato

```bash
# Empezar con rank 03 (más fácil)
./examshell 03

# O elegir interactivamente
./examshell
```

## 3. Navegación Básica

Una vez en el examen:
- **Subject mostrado automáticamente** (como en el examen real)
- **`g`** → Marcar como aprobado (grademe)
- **`n`** → Siguiente ejercicio (mismo nivel)
- **`q`** → Salir

## 4. Ejemplo de Sesión

```
./examshell 03

EXAM MODE: 1 exercise per level (like real exam)
Status: Working on level 1 exercise

📚 Current Exercise:
   Name: filter
   Level: 1

=== SUBJECT ===

Assignment name: filter
Expected files: filter.c
...
[Aquí se muestra automáticamente todo el subject]
...

=== END SUBJECT ===

Press Enter to continue to menu...

[t] Run tester
[g] Grademe → Marca nivel 1 como aprobado, pasa a nivel 2
[n] Next → Cambia a otro ejercicio del nivel 1
[q] Quit → Salir del examen
```

## 5. Solución de Problemas

**Error "No exam ranks found":**
```bash
ls -la  # Verifica que tienes exam-rank-03, etc.
```

**Error de compilación:**
```bash
make clean && make  # Recompila desde cero
```

**Permisos:**
```bash
chmod +x examshell install.sh test.sh
```

## 6. Ejercicios Disponibles

### Rank 03 (Nivel Intermedio)
- **Level 1:** filter, scanf, broken_gnl
- **Level 2:** n_queens, permutations, powerset, rip, tsp

### Rank 04 (Nivel Avanzado)
- **Level 1:** ft_popen, picoshell, sandbox
- **Level 2:** argo, vbc

### Rank 05 (Nivel Experto)
- **Level 1:** string_bigint, vect2
- **Level 2:** bsq, game_of_life

## 7. Tips de Estudio

1. **Lee bien el subject** (`s` command)
2. **No uses libft** - implementa desde cero
3. **Maneja errores** - usa perror() cuando sea necesario
4. **Prueba edge cases** - entrada vacía, malloc fails, etc.
5. **Simula el tiempo real** - practica bajo presión

¡Happy coding! 🎯
