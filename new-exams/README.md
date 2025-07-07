# 🎓 EXAMSHELL - 42 School Exam Practice System (Modular)

Un sistema de práctica **modular** que simula el entorno real del **examshell** de 42 School, con **persistencia completa** y **estadísticas avanzadas**.

## 📋 Características Nuevas

- ✅ **Arquitectura modular** - Código organizado en módulos especializados
- ✅ **Persistencia completa** - Guarda progreso automáticamente
- ✅ **Estadísticas avanzadas** - Tiempo, ejercicios completados, progreso
- ✅ **Directorios subject reales** - Como en examshell: `/subject/[exercise-name]/subject.txt`
- ✅ **Flujo de 3 terminales** - Como en examen real: control + subject + coding
- ✅ **Sin subject automático** - Hay que ir a buscarlo como en examen real
- ✅ **Limpieza automática** - Borra directorios subject al salir
- ✅ **Detección automática** de exam-rank disponibles (03, 04, 05)
- 🎲 **Selección aleatoria** de ejercicios sin repetición
- 📊 **Sistema de progreso** por niveles
- 🖥️ **Interfaz idéntica** al examshell real
- 📚 **Subject automático** - Se muestra sin necesidad de solicitarlo
- 🔧 **Preparado para testers** (próximamente)

## 🏢 **Arquitectura Modular**

```
examshell/
├── examshell.h        # Header principal con prototipos
├── main.c             # Punto de entrada y argumentos
├── ui.c               # Interfaz de usuario y visualización
├── persistence.c      # Sistema de persistencia y estadísticas
├── exercises.c        # Gestión de ejercicios y ranks
├── exam_session.c     # Lógica principal del examen
├── Makefile          # Sistema de compilación modular
└── .examshell_progress # Archivo de progreso (auto-generado)
```

### 📁 **Módulos del Sistema**

| Módulo | Responsabilidad | Funciones Clave |
|--------|----------------|------------------|
| **main.c** | Entrada del programa | Argumentos, validación, coordinación |
| **ui.c** | Interfaz visual | Headers, menús, mostrar subjects |
| **persistence.c** | Datos y progreso | Guardar/cargar, estadísticas, tiempo |
| **exercises.c** | Gestión de contenido | Detectar ranks, cargar ejercicios |
| **exam_session.c** | Lógica del examen | Flujo principal, niveles, grademe |

## 🚀 Instalación y Uso

### Compilación Modular

```bash
# Compilar todo el sistema modular
make

# Ver estructura del proyecto
make structure

# Compilar con debug
make debug

# Ver ayuda completa
make help
```

### Uso del Sistema

```bash
# Ejecutar con rank específico
./examshell 03
./examshell 04
./examshell 05

# Ejecutar sin argumentos (selección interactiva)
./examshell

# Instalación global (opcional)
make install
examshell 03  # Ahora funciona desde cualquier directorio
```

## 🎮 Nuevas Funcionalidades

### 1. **Sistema de Persistencia**
- **Progreso automático**: Se guarda al salir y al completar ejercicios
- **Sesiones múltiples**: Continúa donde lo dejaste
- **Opción de reset**: Empezar sesión nueva cuando quieras

### 2. **Estadísticas Avanzadas**
```
=== EXAM STATISTICS ===
Exam Rank: 03
Current Level: 2

Progress:
  Level 1: ✅ COMPLETED
  Level 2: ❌ Not completed

Exercises completed: 2
  filter, n_queens

Time tracking:
  Session time: 00:15:32
  Total time: 01:23:45
  Overall progress: 50%
======================
```

### 3. **Menú Mejorado**
- **`[t]`** Run tester (próximamente)
- **`[g]`** Grademe - Marca como aprobado y guarda progreso
- **`[n]`** Next exercise (evita ejercicios ya completados)
- **`[s]`** Show statistics - Muestra progreso detallado
- **`[q]`** Quit - Guarda progreso automáticamente

### 4. **Experiencia Mejorada**
- **Subject automático**: Como en examshell real, no necesitas pedirlo
- **Sin repeticiones**: Evita ejercicios ya completados
- **Continuidad**: Retoma exactamente donde lo dejaste
- **Feedback claro**: Confirmaciones y mensajes informativos
- **Directorios subject reales**: Crea `/subject/[exercise-name]/subject.txt` como el examshell oficial

### 5. **Experiencia 100% Real del Examshell**

#### 🖥️ **Workflow de 3 Terminales (Como el Examen Real)**
```bash
# Terminal 1: CONTROL DEL EXAMEN
./examshell 03
# - Navegación ([g], [n], [q])
# - NO muestra subject automáticamente
# - Te dice dónde encontrar el subject

# Terminal 2: LEER SUBJECT  
cat subject/filter/subject.txt
# - Abres cuando necesites leer las instrucciones
# - Consultas las veces que quieras

# Terminal 3: DESARROLLAR
vim filter.c
gcc filter.c && ./a.out
# - Codeas tu solución
# - Compilas y testas
```

#### 📁 **Estructura de Directorios Exacta**
```bash
# Cuando seleccionas un ejercicio, se crea:
./subject/
└── [exercise-name]/
    └── subject.txt    # Instrucciones del ejercicio

# Ejemplo con ejercicio "filter":
./subject/
└── filter/
    └── subject.txt    # cat subject/filter/subject.txt
```

**🎯 Diferencias con Sistema Anterior:**
- ❌ **Antes**: Subject se mostrába automáticamente  
- ✅ **Ahora**: Debes ir a buscarlo a `subject/[name]/subject.txt`
- ❌ **Antes**: Todo en 1 terminal  
- ✅ **Ahora**: Workflow real de 3 terminales
- ❌ **Antes**: Fácil acceso al subject  
- ✅ **Ahora**: Como en examen real - tienes que gestionarlo

## 🛠️ Conceptos Técnicos (Modular)

### Gestión Modular de Datos
```c
// persistence.c - Ejemplo de función de guardado
int save_progress(t_exam_session *session) {
    FILE *file = fopen(PROGRESS_FILE, "w");
    // Guarda rank, niveles, ejercicios completados, tiempo
    fprintf(file, "RANK=%d\n", session->progress.rank);
    fprintf(file, "LEVEL1_COMPLETED=%d\n", session->progress.level1_completed);
    // ...
}
```

### Arquitectura de Sesión
```c
// exam_session.c - Control de flujo principal
void run_exam_session(int rank) {
    // 1. Cargar progreso previo o inicializar
    // 2. Cargar ejercicios de ambos niveles
    // 3. Seleccionar ejercicio apropiado
    // 4. Ejecutar bucle principal del examen
    // 5. Guardar progreso al salir
}
```

### Sistema de Ejercicios Inteligente
```c
// exercises.c - Evitar repeticiones
static t_exercise *select_random_exercise(t_exercise *exercises, int count, 
                                         t_progress_data *progress) {
    // Busca ejercicios no completados
    // Selecciona aleatoriamente entre disponibles
}
```

## 🎯 Ejercicios Disponibles

### Exam Rank 03 (Nivel Intermedio)
- **Level 1:** filter, scanf, broken_gnl
- **Level 2:** n_queens, permutations, powerset, rip, tsp

### Exam Rank 04 (Nivel Avanzado)
- **Level 1:** ft_popen, picoshell, sandbox
- **Level 2:** argo, vbc

### Exam Rank 05 (Nivel Experto)
- **Level 1:** string_bigint, vect2
- **Level 2:** bsq, game_of_life

## 💡 Ventajas de la Arquitectura Modular

### ✅ **Mantenibilidad**
- Cada módulo tiene una responsabilidad específica
- Fácil de debuggear y modificar
- Código más limpio y organizado

### ✅ **Escalabilidad**
- Agregar nuevas funcionalidades es simple
- Cada módulo puede evolucionar independientemente
- Estructura preparada para testers automáticos

### ✅ **Legibilidad**
- Funciones más cortas y enfocadas
- Separación clara de responsabilidades
- Más fácil para nuevos desarrolladores

### ✅ **Testing**
- Cada módulo se puede testear por separado
- Menos dependencias entre componentes
- Debugging más eficiente

## 📝 Comparación: Sistema Anterior vs Modular

| Aspecto | Sistema Anterior | Sistema Modular |
|---------|------------------|-----------------|
| **Archivos** | 1 archivo grande (800+ líneas) | 5 archivos especializados |
| **Funciones** | Funciones muy largas | Funciones cortas y enfocadas |
| **Mantenimiento** | Difícil de modificar | Fácil de mantener |
| **Debugging** | Complejo | Módulo por módulo |
| **Features** | Básico | Persistencia + estadísticas |
| **Escalabilidad** | Limitada | Muy escalable |

## 🚀 Roadmap Modular

### Versión Actual (v2.0 - Modular)
- ✅ Arquitectura modular completa
- ✅ Sistema de persistencia
- ✅ Estadísticas avanzadas
- ✅ Experiencia mejorada

### Próximas Versiones
- 🔄 **v2.1**: Módulo de testers automáticos (`testers.c`)
- 🔄 **v2.2**: Módulo de configuración (`config.c`)
- 🔄 **v2.3**: Módulo de reportes (`reports.c`)
- 🔄 **v2.4**: Módulo de red para rankings online (`network.c`)

## 📞 Desarrollo y Contribución

### Agregar Nuevo Módulo
```bash
# 1. Crear nuevo archivo .c
# 2. Agregar prototipos a examshell.h
# 3. Actualizar Makefile SRCS
# 4. Compilar y testear
make re
```

### Modificar Módulo Existente
```bash
# Solo recompila archivos modificados
make
```

---

**¡Sistema modular completo! 🎊**

*Arquitectura profesional para la comunidad de 42 Madrid*
