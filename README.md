# 🎓 42 Exam Practice 🚀

Sistema para practicar los Exámenes Rank de 42 (02 a 06): menú guiado, ejercicios aleatorios por rango/nivel, y validación automática.

## ⚡ **Inicio rápido**

```bash
./exam_master.sh
# o, equivalente:
make
```

## 🎯 **Características**

- Acceso unificado a los ranks 02, 03, 04, 05 y 06 desde un único menú
- Workflow por rango: elige examen → nivel → ejercicio, lee el subject y escribe tu solución
- Progreso por nivel guardado en `<rango>/exam_progress/`
- Validación automática vía `grademe/test.sh` por ejercicio (donde exista)

## 📋 Requisitos

- Sistema operativo Unix/Linux o macOS
- Compilador GCC/Clang
- Make

## 🛠 **Instalación rápida**

```bash
# Clonar el repositorio
git clone https://github.com/martamakes/42-exam-rank-42.git

# Entrar al directorio
cd 42-exam-rank-42

# Iniciar el sistema
./exam_master.sh
```

## 🎮 **Uso**

### **Método recomendado:**
```bash
./exam_master.sh       # Menú: elige rango 02-06, luego nivel/ejercicio
# o
make                   # Equivalente vía Makefile (make run_exam_05, etc.)
```

### **Acceso directo a un rango:**
```bash
cd 02 && ./exam.sh
cd 03 && ./exam.sh
cd 04 && ./exam.sh
cd 05 && ./exam.sh
cd 06 && ./exam.sh
```

> Nota: existen también las carpetas `new-exams/` (un prototipo EXAMSHELL, solo con `exam-rank-03/`) y `legacy/` (intentos antiguos `03-old`/`04-old`/`05-old`). No forman parte del flujo activo — el sistema real es el descrito arriba (`exam_master.sh` → `<rango>/exam.sh`).

## 📦 **Estructura del proyecto**

```
42-exam-rank-42/
├── exam_master.sh        # Menú principal: elige rango 02-06
├── Makefile               # make / make run_exam_0X (llama a exam_master.sh o al exam.sh del rango)
│
├── 02/ 03/ 04/ 05/ 06/    # Un directorio por rango
│   ├── exam.sh            # Selecciona nivel/ejercicio, corre grademe/test.sh, guarda progreso
│   ├── exam_progress/
│   ├── level-1/ level-2/… # Un subdirectorio por ejercicio: subject + código dado + grademe/test.sh
│   └── rendu/              # (por rango; no usado en la práctica — ver nota abajo)
│
├── rendu/                 # Área de práctica REAL, compartida por todos los rangos.
│                           # exam.sh de cada rango escribe/lee aquí (PROJECT_ROOT/rendu/<ejercicio>).
│                           # Debe quedar vacía entre sesiones: aquí escribes en fresco cuando practicas.
│
└── rendu2/ rendu4/ rendu5/…  # Archivo personal de soluciones ya resueltas, por rango, solo para
                               # consulta/estudio. No lo lee exam.sh. Ver 05/CONTEXT.md para el detalle
                               # del modelo given/rendu/rendu5 usado en el rango 05.
```

## 🎯 **Cómo se usa**

**Workflow estándar:**
1. Lanza `./exam_master.sh` (o `make`)
2. Elige tu rango de examen (02, 03, 04, 05 o 06)
3. El sistema te asigna/deja elegir un ejercicio y muestra el subject
4. Escribe tu solución dentro de `rendu/<ejercicio>/` (en la raíz del proyecto)
5. Valida con la opción del menú — internamente ejecuta `<rango>/level-X/<ejercicio>/grademe/test.sh`

**Tips:**
- ✅ El progreso por nivel se guarda automáticamente en `<rango>/exam_progress/`
- ✅ Si un ejercicio no tiene `grademe/test.sh` todavía, `exam.sh` genera un placeholder que siempre "pasa" — no confundir con una validación real

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

### Exam Rank 04
Disponible en el directorio `04/`, con ejercicios organizados por niveles.

### Exam Rank 05
Consta de 2 niveles con ejercicios avanzados de C y C++:

#### Nivel 1 (3 ejercicios — C++)
- **bigint**: clase de precisión arbitraria para enteros sin signo. Soporta suma, comparación y "digitshift" (desplazar dígitos en base 10, p. ej. `42 << 3 == 42000`). Debe implementar `+`, `+=`, `<<`, `>>`, `<<=`, `>>=`, `++`, `--`, comparadores y el operador de salida `<<`.
- **vect2**: vector matemático 2D de enteros. Suma, resta, multiplicación por escalar, `[]` para acceder a componentes, operador de salida `<<`, comparadores, `+=`/`-=`/`*=` e incremento/decremento.
- **polyset**: implementación de Set y Bag con array y árbol binario de búsqueda. Se crean `searchable_array_bag` y `searchable_tree_bag` (heredan de `array_bag`/`tree_bag`, dados) implementando `searchable_bag`; luego `set`, que envuelve un searchable_bag para no permitir duplicados. Forma canónica completa y const-correctness en las 6 clases que se escriben. Ver `05/CONTEXT.md` para el modelo de carpetas (given/rendu/rendu5) usado en este rango.

#### Nivel 2 (2 ejercicios — C)
- **bsq**: encuentra el mayor cuadrado posible en un mapa evitando obstáculos, leyendo el mapa de fichero o stdin.
- **game_of_life**: simulación del Juego de la Vida de Conway; el tablero inicial se dibuja con comandos tipo "pluma" (`w a s d x`) leídos de stdin, y luego se simulan N iteraciones.

### Exam Rank 06
Disponible en el directorio `06/`.

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
