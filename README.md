# 🎓 42 Exam Practice - UNIFIED SYSTEM 🚀

¡Sistema completo e integrado para practicar **TODOS** los Exámenes Rank de 42! Combina el mejor de ambos mundos: sistema legacy para principiantes y EXAMSHELL auténtico para experiencia real.

## ⚡ **NUEVO: Sistema Unificado**

```bash
# 🚀 ACCESO ÚNICO A TODO
./unified_exam.sh
```

## 🎯 **Características Principales**

**🔧 Sistema Legacy (02, 06):**
- Ideal para principiantes
- Menús guiados paso a paso
- 4 niveles progresivos (Rank 02)
- Tests automáticos con validación

**🚀 Sistema EXAMSHELL (03, 04, 05):**
- **Réplica exacta del examshell real de 42**
- Workflow idéntico al examen oficial
- Ejercicios aleatorios por sesión
- Tracking de tiempo real
- Estructura auténtica subject/ y rendu/

**🎯 Integración Total:**
- Acceso unificado a TODOS los exam ranks
- Detección automática de sistemas disponibles
- Acceso directo por exam rank
- Compilación automática de EXAMSHELL

## 📋 Requisitos

- Sistema operativo Unix/Linux o macOS
- Compilador GCC
- Make

## 🛠 **Instalación Rápida**

```bash
# Clonar el repositorio
git clone https://github.com/martamakes/42-exam-rank-42.git

# Entrar al directorio
cd 42-exam-rank-42

# 🚀 INICIAR SISTEMA UNIFICADO
./unified_exam.sh
```

## 🎮 **Uso del Sistema Unificado**

### **Método Recomendado (Nuevo):**
```bash
./unified_exam.sh      # Acceso a TODOS los exam ranks
```

### **Métodos Alternativos:**
```bash
# Sistema Legacy específico
./exam_master.sh       # Solo ranks 02, 03, 04, 05, 06 (legacy)

# Sistema EXAMSHELL específico  
cd new-exams && ./init.sh    # Solo ranks 03, 04, 05 (EXAMSHELL)

# Métodos clásicos
make                   # Sistema legacy
make run_exam_02      # Solo Rank 02
```

## 📦 **Estructura Unificada del Proyecto**

```
42-exam-rank-42/
├── unified_exam.sh       # 🚀 NUEVO: Acceso unificado a todo
├── exam_master.sh        # Sistema legacy original
│
├── 02/                   # 🔧 SISTEMA LEGACY
│   ├── Level1/ Level2/ Level3/ Level4/
│   ├── exam.sh
│   └── rendu/
├── 06/                   # 🔧 SISTEMA LEGACY  
│   ├── exam.sh
│   └── ...
│
├── new-exams/            # 🚀 SISTEMA EXAMSHELL
│   ├── examshell         # Binario EXAMSHELL (compilado)
│   ├── init.sh           # Menu EXAMSHELL
│   ├── exam-rank-03/
│   │   ├── level-1/ level-2/
│   │   └── [exercises]
│   ├── exam-rank-04/
│   │   ├── level-1/ level-2/
│   │   └── [exercises]
│   ├── exam-rank-05/
│   │   ├── level-1/ level-2/
│   │   └── [exercises]
│   ├── rendu/            # Soluciones EXAMSHELL
│   └── subject/          # Subjects temporales
│
├── 03/ 04/ 05/           # Legacy duplicados (compatibilidad)
└── rendu/                # Soluciones legacy
```

## 🎯 **Guía de Uso por Sistema**

### **🚀 Sistema EXAMSHELL (Ranks 03, 04, 05) - RECOMENDADO**

**Workflow auténtico de 42:**
1. Lanza: `./unified_exam.sh` → Opción 2 o 3
2. El sistema te asigna un ejercicio aleatorio
3. Lee: `cat subject/[ejercicio]/subject.txt`
4. Programa: `mkdir rendu/[ejercicio]` y crea tu solución
5. Valida: `grademe` en el sistema

**Ventajas:**
- ✅ Experiencia **exacta** al examen real
- ✅ Tracking de tiempo real
- ✅ Workflow idéntico a 42

### **🔧 Sistema Legacy (Ranks 02, 06) - Para Principiantes**

**Workflow guiado:**
1. Lanza: `./unified_exam.sh` → Opción 1 o 3
2. Menú te guía paso a paso
3. Ejercicios organizados por niveles
4. Tests automáticos incluidos

**Ventajas:**
- ✅ Ideal para empezar
- ✅ Progresión por niveles
- ✅ Tests automáticos

## 📚 Contenido de los exámenes

### Exam Rank 02
Consta de 4 niveles, cada uno con ejercicios de dificultad incremental:

#### Nivel 1 (12 ejercicios)
- first_word, fizzbuzz, ft_putstr, ft_strcpy, ft_strlen, ft_swap, repeat_alpha, rev_print, rot_13, rotone, search_and_replace, ulstr

#### Nivel 2 (20 ejercicios)
- alpha_mirror, camel_to_snake, do_op, ft_atoi, ft_strcmp, ft_strcspn, ft_strdup, ft_strpbrk, ft_strspn, ft_strrev, inter, is_power_of_2, last_word, max, print_bits, reverse_bits, snake_to_camel, swap_bits, union, wdmatch

#### Nivel 3 (15 ejercicios)
- add_prime_sum, epur_str, expand_str, ft_atoi_base, ft_list_size, ft_range, ft_rrange, hidenp, lcm, paramsum, pgcd, print_hex, rstr_capitalizer, str_capitalizer, tab_mult

#### Nivel 4 (10 ejercicios)
- flood_fill, fprime, ft_itoa, ft_list_foreach, ft_list_remove_if, ft_split, rev_wstr, rostring, sort_int_tab, sort_list

### Exam Rank 03
Consta de dos ejercicios principales:
- **ft_printf**: Implementación simplificada de la función printf
- **get_next_line**: Función para leer líneas de un archivo

### Exámenes Rank 04, 05 y 06
Se irán añadiendo a medida que estén disponibles. El sistema está preparado para incorporarlos fácilmente.

## 📝 Tips para los exámenes

1. **Práctica constante**: Intenta resolver cada ejercicio varias veces hasta que puedas hacerlo sin consultar la solución.

2. **Gestión del tiempo**: Los exámenes reales tienen un límite de tiempo, así que practica resolviéndolos con presión de tiempo.

3. **Norminette**: No incluye Norminette porque en el examen tampoco se aplica.

4. **Depuración**: Aprende a depurar tu código sin depurador (usando prints estratégicos). En 42 Madrid tienes disponible Valgrind y gdb, úsalos.

5. **Memoria**: Comprueba siempre las fugas de memoria en funciones que usan malloc.

6. **Ejercicios frecuentes**: Para el Rank 03, ft_printf y get_next_line son los ejercicios más frecuentes. El sistema incluye guías paso a paso y ejemplos para facilitar su comprensión.

## 🤝 Contribuir

Las contribuciones son bienvenidas:

1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## ⚠️ Disclaimer

Este proyecto no está oficialmente afiliado con 42 School. Es una herramienta de práctica creada por y para estudiantes.

## 📜 Licencia

Este proyecto está bajo la licencia MIT. Ver `LICENSE` para más información.
Todos los enunciados pertenecen a 42 School.

## 🙏 Agradecimientos

- A la comunidad de 42
- A todos los estudiantes que han contribuido con ejercicios y mejoras
- A los creadores del sistema de exámenes original

---
Made with ❤️ by mvigara- estudiante de 42 School Madrid
