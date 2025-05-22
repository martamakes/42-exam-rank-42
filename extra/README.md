# 🎯 Sistema de Práctica por Patrones - 42 School Exam Rank 02

## 📖 Descripción

Este sistema reorganiza los ejercicios de 42 School Exam Rank 02 por **patrones conceptuales** en lugar de niveles arbitrarios de dificultad. Está diseñado para optimizar el aprendizaje y la preparación para el examen mediante una progresión científica basada en conceptos de programación.

## 🚀 Inicio Rápido

```bash
cd extra/
make start
```

## 📚 Los 10 Patrones Conceptuales

| Patrón | Concepto | Ejercicios Clave |
|--------|----------|------------------|
| 🔤 **01** | Manipulación básica de caracteres | rot_13, ulstr, print_bits |
| 📝 **02** | Procesamiento palabra por palabra | first_word, camelCase, rostring |
| 🔄 **03** | Conversión de tipos | ft_atoi, ft_itoa, ft_split |
| 🔍 **04** | Algoritmos de búsqueda | wdmatch, hidenp, strcspn |
| 📊 **05** | Operaciones con conjuntos | union, inter |
| 💾 **06** | Memoria y punteros | malloc, strdup, arrays |
| 🧮 **07** | Algoritmos matemáticos | primos, MCM, MCD |
| 🔗 **08** | Listas enlazadas | navegación, modificación |
| 📈 **09** | Algoritmos de ordenación | bubble sort, sort_list |
| 🌊 **10** | Recursividad avanzada | flood_fill |

## 🎯 Ventajas del Sistema

- ✅ **Aprendizaje progresivo**: Domina un concepto antes de pasar al siguiente
- ✅ **Mejor retención**: Agrupa ejercicios similares para reforzar patrones
- ✅ **Preparación eficiente**: Enfoque sistemático para el examen
- ✅ **Tracking de progreso**: Ve exactamente qué has dominado
- ✅ **Explicaciones detalladas**: Cada patrón tiene su propia documentación

## 📋 Comandos Disponibles

```bash
make start      # Inicia el sistema de práctica
make menu       # Muestra el menú principal
make progress   # Ver progreso detallado
make status     # Estado del sistema
make setup      # Configurar enlaces simbólicos
make help       # Mostrar ayuda
make clean      # Limpiar archivos temporales
make reset      # Reiniciar todo el progreso (¡CUIDADO!)
```

## 🏗️ Estructura del Proyecto

```
extra/
├── Makefile                    # Comandos principales
├── README.md                   # Esta documentación
├── exercise_patterns.conf      # Configuración de ejercicios por patrón
├── patterns/                   # Directorios organizados por patrón
│   ├── 01-manipulacion-caracteres/
│   ├── 02-procesamiento-palabras/
│   └── ...
├── progress/                   # Archivos de seguimiento
│   ├── pattern_01.txt
│   ├── pattern_02.txt
│   └── ...
├── explanations/              # Explicaciones de cada patrón
│   ├── pattern_01.md
│   ├── pattern_02.md
│   └── ...
├── scripts/                   # Scripts del sistema
│   ├── main_menu.sh
│   ├── pattern_practice.sh
│   ├── setup_links.sh
│   ├── show_progress.sh
│   ├── show_help.sh
│   └── system_status.sh
└── links/                     # Enlaces simbólicos a ejercicios originales
```

## 🎓 Estrategia de Estudio Recomendada

### Fase 1: Fundamentos (Semanas 1-2)
- **Patrón 01**: Manipulación básica de caracteres
- **Patrón 02**: Procesamiento de cadenas palabra por palabra
- **Patrón 03**: Funciones de conversión de tipos

### Fase 2: Algoritmos Básicos (Semanas 3-4)
- **Patrón 04**: Algoritmos de búsqueda
- **Patrón 05**: Operaciones con conjuntos

### Fase 3: Memoria y Matemáticas (Semanas 5-6)
- **Patrón 06**: Manipulación de memoria y punteros
- **Patrón 07**: Algoritmos matemáticos

### Fase 4: Estructuras Avanzadas (Semanas 7-8)
- **Patrón 08**: Manipulación de listas enlazadas
- **Patrón 09**: Algoritmos de ordenación

### Fase 5: Recursividad y Repaso (Semanas 9-10)
- **Patrón 10**: Recursividad y algoritmos de inundación
- Repaso general con ejercicios aleatorios

## 💡 Consejos para el Examen

### Antes del Examen
1. **Domina los patrones básicos** (01-03) antes de avanzar
2. **Practica 3-5 ejercicios por sesión** de estudio
3. **Revisa patrones completados semanalmente**
4. **Usa `make progress`** para tracking regular

### Durante el Examen
1. **Lee todo el enunciado** antes de empezar a codificar
2. **Identifica el patrón** - ¿a cuál de los 10 pertenece?
3. **Implementa paso a paso** - no intentes hacerlo todo de una
4. **Prueba con casos simples** antes de casos complejos
5. **Gestiona tu tiempo** - 3 horas para 4 ejercicios

## 🔧 Instalación y Configuración

### Prerrequisitos
- Sistema Unix/Linux/macOS
- Compilador GCC
- Bash shell
- El repositorio original de ejercicios en `../02/`

### Configuración Inicial
```bash
# 1. Navegar al directorio
cd /path/to/42-Exam-Rank-02/extra/

# 2. Configurar el sistema
make setup

# 3. Iniciar
make start
```

## 🐛 Solución de Problemas

### El sistema no encuentra los ejercicios originales
```bash
# Verificar estructura
ls ../02/  # Debe mostrar Level1, Level2, Level3, Level4

# Reconfigurar
make setup
```

### Los scripts no son ejecutables
```bash
# Corregir permisos
make setup
# o manualmente
chmod +x scripts/*.sh
```

### Se perdió el progreso
- Los archivos están en `progress/pattern_XX.txt`
- Si se borraron, se recrearán automáticamente
- Para resetear un patrón específico, usa la opción 'r' en su menú

## 🤝 Contribuciones

Este sistema está diseñado para ser extensible:

- **Añadir explicaciones**: Edita `explanations/pattern_XX.md`
- **Reorganizar ejercicios**: Modifica `exercise_patterns.conf`
- **Personalizar progreso**: Los archivos en `progress/` son texto plano

## 📞 Soporte

Para problemas o sugerencias:
1. Usa `make help` para documentación completa
2. Verifica el estado con `make status`
3. Consulta la sección de troubleshooting en la ayuda

---

## 🏆 Sobre el Sistema

Desarrollado por un estudiante de 42 School para estudiantes de 42 School. 

**Objetivo**: Transformar la preparación para exámenes de un proceso memorístico a un aprendizaje científico basado en patrones.

**Filosofía**: "No practiques ejercicios, domina patrones."

---

¡Buena suerte en tu examen! 🚀
