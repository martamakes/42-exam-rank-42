# 42 Exam Rank - Training System 🚀

Este repositorio contiene un sistema completo para practicar los Exámenes Rank de la escuela 42. El sistema simula el entorno de examen y proporciona ejercicios de práctica organizados por niveles y rangos de examen.

## 🎯 Características

- Sistema unificado para todos los exámenes Rank (02, 03, 04, 05, 06)
- Organización por niveles progresivos (para Rank 02)
- Ejercicios aleatorios 
- Test automáticos
- Tracking de progreso
- Interfaz amigable en terminal
- Modo práctica realista
- Guías y ejemplos para exámenes más avanzados

## 📋 Requisitos

- Sistema operativo Unix/Linux o macOS
- Compilador GCC
- Make

## 🛠 Instalación

```bash
# Clonar el repositorio
git clone https://github.com/martamakes/42-exam-rank-42.git

# Entrar al directorio
cd 42-Exam-Rank-02

# Dar permisos de ejecución (si es necesario)
chmod +x exam_master.sh
chmod +x 02/exam.sh
chmod +x 02/init.sh
chmod +x 03/exam.sh  # Si está disponible

# Iniciar el sistema
make
```

## 🎮 Uso

### Comandos básicos:
```bash
make            # Inicia el sistema de exámenes completo
make run_exam_02 # Inicia solo el examen Rank 02
make clean      # Limpia archivos temporales
make fclean     # Resetea todo el progreso
make re         # Reinstala todo el sistema
```

## 📦 Estructura del proyecto

El proyecto ahora tiene una estructura modular que permite practicar diferentes exámenes Rank:

```
.
├── exam_master.sh        # Script principal que permite elegir el examen
├── 02/                   # Exam Rank 02
│   ├── Level1/           # Ejercicios de nivel 1
│   ├── Level2/           # Ejercicios de nivel 2
│   ├── Level3/           # Ejercicios de nivel 3
│   ├── Level4/           # Ejercicios de nivel 4
│   ├── exam.sh           # Script específico del examen 02
│   └── ...
├── 03/                   # Exam Rank 03
│   ├── ft_printf/        # Ejercicio ft_printf
│   ├── get_next_line/    # Ejercicio get_next_line
│   ├── exam.sh           # Script específico del examen 03
│   └── ...
├── 04/                   # Exam Rank 04 (cuando esté disponible)
├── 05/                   # Exam Rank 05 (cuando esté disponible)
├── 06/                   # Exam Rank 06 (cuando esté disponible)
└── rendu/                # Directorio para tus soluciones del Rank 02
```

## 💻 Estructura de soluciones por examen

### Exam Rank 02
Tus soluciones deben estar en `/rendu/[nombre_ejercicio]/` siguiendo la guía del sistema.

### Exam Rank 03
Tus soluciones deben estar en `/03/rendu/` con la siguiente estructura:
- ft_printf: `/03/rendu/ft_printf/ft_printf.c`
- get_next_line: `/03/rendu/get_next_line/get_next_line.c` y `/03/rendu/get_next_line/get_next_line.h`

> **Nota importante**: El sistema te guiará sobre dónde colocar tus archivos para cada examen. Sigue siempre las instrucciones que se muestran en pantalla.

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
