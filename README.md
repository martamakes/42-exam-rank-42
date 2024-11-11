```markdown
# 42 Exam Rank 02 - Training System 🚀

Este repositorio contiene un sistema completo para practicar el Exam Rank 02 de la escuela 42. El sistema simula el entorno de examen y proporciona ejercicios de práctica organizados por niveles.

## 🎯 Características

- Sistema de niveles progresivos (4 niveles)
- Ejercicios aleatorios por nivel
- Test automáticos
- Tracking de progreso
- Interfaz amigable en terminal
- Modo práctica realista

## 📋 Requisitos

- Sistema operativo Unix/Linux o macOS
- Compilador GCC
- Make

## 🛠 Instalación

```bash
# Clonar el repositorio
git clone https://github.com/martamakes/42-exam-rank-02.git

# Entrar al directorio
cd 42-exam-rank-02

# Dar permisos de ejecución (si es necesario)
chmod +x 02/exam.sh
chmod +x 02/init.sh

# Iniciar el sistema
make
```

## 🎮 Uso

### Comandos básicos:
```bash
make        # Inicia el sistema de exámenes
make clean  # Limpia archivos temporales
make fclean # Resetea todo el progreso
make re     # Reinstala todo el sistema
```

## 📚 Estructura de ejercicios

El examen consta de 4 niveles, cada uno con ejercicios de dificultad incremental:

### Nivel 1 (12 ejercicios)
- first_word
- fizzbuzz
- ft_putstr
- ft_strcpy
- ft_strlen
- ft_swap
- repeat_alpha
- rev_print
- rot_13
- rotone
- search_and_replace
- ulstr

### Nivel 2 (20 ejercicios)
- alpha_mirror
- camel_to_snake
- do_op
- ft_atoi
- ft_strcmp
- ft_strcspn
- ft_strdup
- ft_strpbrk
- ft_strspn
- ft_strrev
- inter
- is_power_of_2
- last_word
- max
- print_bits
- reverse_bits
- snake_to_camel
- swap_bits
- union
- wdmatch

### Nivel 3 (15 ejercicios)
- add_prime_sum
- epur_str
- expand_str
- ft_atoi_base
- ft_list_size
- ft_range
- ft_rrange
- hidenp
- lcm
- paramsum
- pgcd
- print_hex
- rstr_capitalizer
- str_capitalizer
- tab_mult

### Nivel 4 (10 ejercicios)
- flood_fill
- fprime
- ft_itoa
- ft_list_foreach
- ft_list_remove_if
- ft_split
- rev_wstr
- rostring
- sort_int_tab
- sort_list

## 📝 Tips para el examen

1. **Práctica constante**: Intenta resolver cada ejercicio varias veces hasta que puedas hacerlo sin consultar la solución.

2. **Gestión del tiempo**: El examen real tiene un límite de tiempo, así que practica resolviéndolos con presión de tiempo.

3. **Norminette**: No incluye Norminette porque en el examen tampoco se aplica.

4. **Depuración**: Aprende a depurar tu código sin depurador (usando prints estratégicos). En 42 Madrid tienes disponible Valgrind y gdb, úsalos.

5. **Memoria**: Comprueba siempre las fugas de memoria en funciones que usan malloc.

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

## 🙏 Agradecimientos

- A la comunidad de 42
- A todos los estudiantes que han contribuido con ejercicios y mejoras
- A los creadores del sistema de exámenes original

---
Made with ❤️ by mvigara- estudiante de 42 School Madrid
```
