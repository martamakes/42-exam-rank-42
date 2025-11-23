# 🎓 Exam Rank 03 - Sistema de Práctica Automatizado

Sistema completo de práctica para el Exam Rank 03 de 42 School con validación automática, seguimiento de progreso y material educativo.

---

## 🚀 Inicio Rápido

```bash
./exam.sh
```

El sistema te guiará a través de ejercicios aleatorios o específicos, validará automáticamente tus soluciones y mantendrá registro de tu progreso.

---

## 📁 Estructura del Proyecto

```
42-exam-rank-03-automation/
├── exam.sh                           # Sistema principal de práctica
├── README.md                         # Este archivo
├── HERRAMIENTAS_DEBUGGING.md        # Guía de gdb, valgrind, debugging
├── REFERENCIA_RAPIDA.md             # Conceptos C rápidos (malloc, sizeof)
│
├── level-1/                          # Ejercicios nivel 1
│   ├── filter/
│   │   ├── grademe/test.sh          # Tests automáticos
│   │   ├── subject.txt              # Enunciado en inglés
│   │   └── subject-es.txt           # Enunciado en español
│   ├── ft_scanf/
│   │   ├── grademe/test.sh
│   │   ├── subject.txt
│   │   ├── subject-es.txt
│   │   └── explicaciones.md
│   └── broken_GNL/
│       ├── grademe/test.sh
│       ├── subject.txt
│       ├── subject-es.txt
│       └── given_code*.c/h          # Código proporcionado
│
├── level-2/                          # Ejercicios nivel 2
│   ├── permutations/
│   ├── powerset/
│   ├── n_queens/
│   ├── rip/
│   └── tsp/
│       └── grademe/test.sh
│
├── rendu3/                           # Soluciones validadas (referencia)
│   ├── COMPARATIVA_POWERSET_VS_PERMUTATIONS.md
│   ├── filter/filter.c
│   ├── ft_scanf/
│   │   ├── ft_scanf.c
│   │   └── readme_ft_scanf.md       # Explicación detallada
│   ├── broken_GNL/
│   │   ├── get_next_line.c
│   │   ├── get_next_line_comentado.c
│   │   └── readme_broken_gnl.md     # 8 errores explicados
│   ├── permutations/
│   │   ├── permutations.c
│   │   ├── permutations.h
│   │   └── readme_permutations.md   # Algoritmo backtracking
│   ├── powerset/
│   │   ├── powerset_bits.c          # Solución con máscaras
│   │   ├── powerset_bits.h
│   │   ├── powerset_backtraking.c   # Solución con backtracking
│   │   ├── powerset_backtraking.h
│   │   └── EXPLICACION_BACKTRACKING.md
│   ├── n_queens/
│   │   ├── n_queens.c
│   │   └── n_queens.h
│   ├── rip/rip.c                    # Balanceo de paréntesis
│   └── tsp/tsp.c                    # Traveling Salesman Problem
│
├── rendu/                            # Zona de trabajo (auto-generada, ignorada en git)
└── exam_progress/                    # Seguimiento de progreso
    ├── level1_done.txt
    └── level2_done.txt
```

---

## 🎯 Cómo Funciona

### 1. **Ejecutar el sistema**
```bash
./exam.sh
```

### 2. **Menú principal**
```
1. Practicar ejercicios (aleatorio de todos los niveles)
2. Practicar Level 1 (aleatorio)
3. Practicar Level 2 (aleatorio)
4. Seleccionar ejercicio específico
5. Ver progreso
6. Limpiar progreso
7. Salir
```

### 3. **Flujo de trabajo**

1. El sistema te asigna/muestras un ejercicio y su enunciado
2. Implementas tu solución en `level-X/ejercicio/`
3. El sistema ejecuta automáticamente `grademe/test.sh`
4. Si pasas → marca como completado y guarda en `rendu3/`
5. Si fallas → puedes reintentar o consultar soluciones en `rendu3/`

---

## 📚 Material Educativo

### **Guías Generales** (raíz del proyecto)
- **[HERRAMIENTAS_DEBUGGING.md](HERRAMIENTAS_DEBUGGING.md)** - gdb, valgrind, printf debugging, estrategias para el examen
- **[REFERENCIA_RAPIDA.md](REFERENCIA_RAPIDA.md)** - Conceptos C (malloc, sizeof, punteros)

### **Guías por Ejercicio** (rendu3/)
- **[readme_broken_gnl.md](rendu3/broken_GNL/readme_broken_gnl.md)** - 8 errores críticos explicados paso a paso
- **[readme_ft_scanf.md](rendu3/ft_scanf/readme_ft_scanf.md)** - Argumentos variables, parsing, dudas frecuentes
- **[readme_permutations.md](rendu3/permutations/readme_permutations.md)** - Backtracking con contador de frecuencias

### **Comparativas**
- **[COMPARATIVA_POWERSET_VS_PERMUTATIONS.md](rendu3/COMPARATIVA_POWERSET_VS_PERMUTATIONS.md)** - Cuándo usar máscaras vs recursión, checklist para el examen

---

## 💡 Ejercicios Disponibles

### **Level 1** (Básicos)

| Ejercicio | Descripción | Conceptos Clave |
|-----------|-------------|-----------------|
| **filter** | Reemplazo de strings con asteriscos | read, write, memmem, gestión memoria |
| **ft_scanf** | Implementación de scanf (%s, %d, %c) | va_list, parsing, streams, construcción de números |
| **broken_GNL** | Encontrar y reparar 8 errores en get_next_line | Debugging, punteros, unsigned/signed, EOF |

### **Level 2** (Avanzados)

| Ejercicio | Descripción | Conceptos Clave |
|-----------|-------------|-----------------|
| **permutations** | Generación de permutaciones en orden alfabético | Backtracking, contador de frecuencias, recursión |
| **powerset** | Subconjuntos que suman un valor objetivo | Máscaras de bits o backtracking recursivo |
| **n_queens** | Problema de las N reinas | Backtracking, validación de posiciones |
| **rip** | Balanceo mínimo de paréntesis | Backtracking en dos fases, validación |
| **tsp** | Traveling Salesman Problem | Permutaciones, distancia euclidiana, optimización |

---

## 🔒 Sistema de Protección

### **rendu3/** = Soluciones Validadas (Permanente)
- ✅ Todas las soluciones funcionales verificadas
- ✅ Versiones con y sin comentarios
- ✅ READMEs educativos con explicaciones detalladas
- ✅ **NUNCA se modifican** durante la práctica
- ✅ Tu referencia cuando te atascas

### **rendu/** = Zona de Trabajo (Temporal)
- 🔧 Se crea automáticamente al practicar
- 🔧 Espacio de trabajo para implementar soluciones
- 🔧 **Ignorada en git** (no se sube al repositorio)
- 🔧 Puede limpiarse sin perder nada importante

---

## 🧪 Sistema de Tests

Todos los ejercicios incluyen tests automáticos en `grademe/test.sh`:

### **Level 1** (Tests completos)
- **filter**: Casos de reemplazo, edge cases, validación de argumentos
- **ft_scanf**: Parsing de strings, integers, characters
- **broken_GNL**: Lectura línea por línea, diferentes casos

### **Level 2** (Tests básicos)
- Compilación + ejecución básica
- Verificación del output por el estudiante

---

## 📊 Seguimiento de Progreso

El sistema mantiene registro automático en:
- `exam_progress/level1_done.txt`
- `exam_progress/level2_done.txt`

**Ver progreso:**
```bash
./exam.sh → Opción 5
```

**Resetear progreso:**
```bash
./exam.sh → Opción 6
```

---

## 🛠️ Consejos para el Examen

### **Antes de empezar**
1. Lee completamente el enunciado
2. Identifica las funciones permitidas
3. Planifica tu solución antes de codificar

### **Durante el desarrollo**
1. Compila frecuentemente: `gcc -Wall -Wextra -Werror archivo.c`
2. Prueba casos edge: NULL, vacío, límites
3. Usa debugging: consulta [HERRAMIENTAS_DEBUGGING.md](HERRAMIENTAS_DEBUGGING.md)

### **Si te atascas**
1. Revisa el README del ejercicio en `rendu3/`
2. Consulta soluciones comentadas
3. Compara tu enfoque con las soluciones de referencia

---

## ⚙️ Compilación

### **Flags estándar**
```bash
gcc -Wall -Wextra -Werror archivo.c -o programa
```

### **Con debugging**
```bash
gcc -Wall -Wextra -Werror -g archivo.c -o programa
```

### **Con librerías matemáticas** (tsp)
```bash
gcc -Wall -Wextra -Werror archivo.c -lm -o programa
```

---

## 🆘 Resolución de Problemas

### **Error: "No se encontró script de test"**
```bash
chmod +x level-X/ejercicio/grademe/test.sh
```

### **Error de compilación**
- Verifica nombre del archivo (case-sensitive)
- Revisa funciones permitidas en el subject
- Compila con flags: `-Wall -Wextra -Werror`

### **Segmentation fault**
Consulta [HERRAMIENTAS_DEBUGGING.md](HERRAMIENTAS_DEBUGGING.md) para usar gdb/valgrind

---

## 🤝 Contribución

Este repositorio está estructurado para ser:
- ✅ Educativo: READMEs detallados por ejercicio
- ✅ Profesional: Código limpio con comentarios útiles
- ✅ Práctico: Sistema automatizado de validación
- ✅ Completo: Múltiples enfoques de solución donde aplica

---

## 📝 Notas Importantes

- **Protección de soluciones**: `rendu3/` está protegido, practica sin miedo
- **Zona de trabajo**: `rendu/` se ignora en git, es temporal
- **Tests automáticos**: Validan tu código igual que en el examen real
- **Material educativo**: READMEs explican conceptos difíciles paso a paso

---

## 🚀 ¡Comienza Ahora!

```bash
./exam.sh
```

**¡Buena suerte en tu preparación para el examen! 🍀**
