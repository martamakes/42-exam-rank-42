# Exam Rank 04 - Microshell

## Descripción
Este examen consiste en la implementación de un microshell (una versión simplificada de un shell) que maneje:
- Ejecución de comandos con argumentos
- Comando integrado `cd` con gestión de errores
- Tuberías (`|`) para conectar la salida de un comando con la entrada del siguiente
- Separadores de comandos (`;`)

## Estructura del Proyecto
- `microshell/`: Directorio con el ejercicio y recursos
  - `README.md`: Subject del ejercicio
  - `microshell_guide`: Guía interactiva
  - `microshell_steps_*.c`: Implementación paso a paso
  - `grademe/`: Scripts de prueba

- `rendu/`: Directorio donde debes colocar tu solución
  - `microshell/`: Tu implementación de microshell

## Uso del Tester
1. Ejecuta `./exam.sh` para acceder al menú principal
2. Selecciona una opción:
   - Ver subject
   - Abrir la guía interactiva
   - Ejecutar tests sobre tu implementación
   - Compilar tu solución

## Requisitos
- Tu solución debe estar en el directorio `rendu/microshell/`
- Debes implementar el microshell según las especificaciones del subject
- Solo puedes usar las funciones permitidas
- Tu código debe compilar con `-Wall -Wextra -Werror`

## Ejecución del Examen
```bash
./exam.sh
```

## Limpieza
```bash
./exam.sh clean
```
