# 🎓 Exam Rank 03 - Sistema de Práctica Automatizado 🚀

Sistema interactivo de preparación para el Exam Rank 03 de 42, con validación automática y seguimiento de progreso.

## 📋 Requisitos

- Sistema operativo Unix/Linux o macOS
- Compilador C (gcc/clang)
- Make (para algunos ejercicios)
- Bash shell
- Git

## 🛠 Instalación Rápida

```bash
# Clonar el repositorio
git clone https://github.com/tu-usuario/42-exam-rank-03-automation.git

# Entrar al directorio
cd 42-exam-rank-03-automation

# Iniciar el sistema
./exam.sh
```

## 🚀 Inicio Rápido

```bash
# 1. Ejecutar el script principal
./exam.sh

# 2. Crear tu espacio de trabajo (cuando se asigne un ejercicio)
cd rendu
mkdir nombre_ejercicio
cd nombre_ejercicio
touch nombre_ejercicio.c

# 3. Programar tu solución
# Edita el archivo .c con tu código

# 4. Desde el menú del sistema, selecciona "Validar ejercicio"
```

## 🎯 Características

- Sistema interactivo de práctica con menú de opciones
- Selección aleatoria de ejercicios por nivel o global
- Validación automática mediante tests (grademe)
- Seguimiento de progreso por ejercicio
- Almacenamiento de soluciones completadas
- Soporte para rehacer ejercicios ya completados
- Subjects disponibles en español e inglés
- Sistema de workspace separado (rendu/) para trabajo del usuario

## 📦 Estructura del Proyecto

```
42-exam-rank-03-automation/
├── exam.sh                 # Script principal del sistema
├── level-1/               # Ejercicios de nivel 1
│   ├── broken_GNL/
│   ├── filter/
│   └── ft_scanf/
├── level-2/               # Ejercicios de nivel 2
│   ├── n_queens/
│   ├── permutations/
│   ├── powerset/
│   ├── rip/
│   └── tsp/
├── rendu/                 # Workspace de trabajo (crear manualmente)
├── rendu3/                # Soluciones completadas (auto-generado)
└── exam_progress/         # Archivos de seguimiento de progreso
```

## 📚 Ejercicios Disponibles

### Level 1
- **broken_GNL**: Implementación de get_next_line con manejo de errores
- **filter**: Función de filtrado de arrays
- **ft_scanf**: Implementación simplificada de scanf

### Level 2
- **n_queens**: Problema de las N reinas (backtracking)
- **permutations**: Generación de permutaciones
- **powerset**: Generación del conjunto potencia
- **rip**: Routing Information Protocol
- **tsp**: Travelling Salesman Problem

## 🎮 Uso Detallado

### Menú Principal

El menú principal te ofrece las siguientes opciones:
   - **Opción 1**: Practicar ejercicios aleatorios de todos los niveles
   - **Opción 2**: Practicar solo ejercicios de Level 1 (aleatorio)
   - **Opción 3**: Practicar solo ejercicios de Level 2 (aleatorio)
   - **Opción 4**: Seleccionar un ejercicio específico
   - **Opción 5**: Ver progreso actual
   - **Opción 6**: Limpiar progreso (reset)
   - **Opción 7**: Salir

### 🔄 Workflow Recomendado

1. **Seleccionar ejercicio**: El sistema te presenta un ejercicio aleatorio o seleccionado
2. **Leer el subject**: Se muestra automáticamente el enunciado
3. **Crear workspace**:
   ```bash
   cd rendu
   mkdir nombre_ejercicio
   cd nombre_ejercicio
   touch nombre_ejercicio.c
   ```
4. **Implementar solución**: Escribe tu código en `rendu/nombre_ejercicio/nombre_ejercicio.c`
5. **Validar**: Desde el menú, selecciona "Validar ejercicio"
   - El sistema copia tu código al directorio del ejercicio
   - Ejecuta los tests automáticos (grademe)
   - Si pasa, marca el ejercicio como completado
   - Guarda tu solución en `rendu3/`

### ⚙️ Opciones Durante la Práctica

Cuando trabajas en un ejercicio, puedes:
- **Validar ejercicio**: Ejecuta los tests automáticos
- **Marcar como completado sin validar**: Útil para saltar ejercicios
- **Ver subject de nuevo**: Remuestra el enunciado
- **Limpiar ejercicio**: Borra tu código para empezar de cero
- **Siguiente ejercicio**: Pasa al siguiente ejercicio aleatorio
- **Volver al menú principal**: Regresa al menú inicial

## ⚠️ Recordatorios Importantes

Antes de validar, el sistema te recordará:
- Hacer `git add` de tus archivos
- Verificar que tu código compila sin errores
- Revisar que cumples todos los requisitos del subject
- Ejecutar `grademe` en el examen real antes de enviar

## 📂 Estructura de Directorios

### rendu/ (Workspace de Trabajo)
Tu carpeta de trabajo personal. Debes crear manualmente la estructura:
```
rendu/
└── nombre_ejercicio/
    ├── nombre_ejercicio.c
    └── nombre_ejercicio.h (si es necesario)
```

**Nota especial**: Para `broken_GNL`, el archivo debe llamarse `get_next_line.c` en lugar de `broken_GNL.c`.

### rendu3/ (Soluciones de Referencia)
Generada automáticamente. Contiene las soluciones que han pasado los tests:
```
rendu3/
├── filter/
│   └── filter.c
├── n_queens/
│   ├── n_queens.c
│   └── n_queens.h
└── ...
```

### exam_progress/ (Seguimiento de Progreso)
Generado automáticamente. Archivos que registran ejercicios completados:
- `level1_done.txt`: Ejercicios completados de Level 1
- `level2_done.txt`: Ejercicios completados de Level 2

## ✅ Características de Validación

Cada ejercicio incluye:
- **Subject**: Enunciado del ejercicio (español/inglés)
- **Carpeta grademe/**: Tests automáticos
- **Script test.sh**: Validación automática de la solución

El sistema:
1. Copia tu código desde `rendu/` al directorio del ejercicio
2. Ejecuta el script de test correspondiente
3. Muestra resultados en color (verde = éxito, rojo = error)
4. Guarda soluciones exitosas en `rendu3/`

## 💡 Consejos y Tips

### Para el Examen Real
- Practica creando tu workspace desde cero cada vez
- Familiarízate con el proceso de `git add` y validación
- Revisa las soluciones en `rendu3/` después de completar ejercicios
- Practica bajo presión usando el modo aleatorio

### Gestión de Progreso
- Usa "Ver progreso" frecuentemente para seguir tu avance
- Puedes rehacer ejercicios completados usando "Seleccionar ejercicio específico"
- Usa "Limpiar progreso" si quieres empezar desde cero

### Organización
- Mantén `rendu/` limpio, solo con ejercicios en los que estás trabajando
- Consulta `rendu3/` para ver soluciones que ya funcionaron
- Los archivos `.h` se copian automáticamente si existen

## 🔧 Desarrollo y Personalización

El script `exam.sh` está completamente comentado y puede ser modificado para:
- Añadir nuevos niveles de ejercicios
- Personalizar colores y mensajes
- Modificar el sistema de validación
- Integrar con otros sistemas de testing

## 🤝 Contribuir

Las contribuciones son bienvenidas:

1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## ⚠️ Aviso legal

Este proyecto no está oficialmente afiliado con 42 School. Es una herramienta de práctica creada por y para estudiantes.

Las soluciones en `rendu3/` son de referencia y pueden no ser las únicas soluciones válidas.

## 📜 Licencia

Este proyecto está bajo la licencia MIT.
Todos los enunciados pertenecen a 42 School.

## 🙏 Agradecimientos

- A la comunidad de 42
- A todos los estudiantes que han contribuido con ejercicios y mejoras
- A los creadores de los sistemas de testing originales

---

**¡Buena suerte en tu preparación para el Exam Rank 03!** 🚀

Made with ❤️ for 42 students
