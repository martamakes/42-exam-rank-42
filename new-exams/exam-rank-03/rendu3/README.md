# Rendu3 - Soluciones del Exam Rank 03

Esta carpeta almacena automáticamente las soluciones de los ejercicios completados durante la práctica del Exam Rank 03.

## Estructura

```
rendu3/
├── exercise_name/
│   ├── exercise_name.c
│   └── exercise_name.h (si existe)
└── ...
```

## Funcionamiento

Cuando completas un ejercicio exitosamente usando `./exam.sh`:

1. **Validación**: El sistema ejecuta los tests del ejercicio
2. **Copia automática**: Si los tests pasan, tu solución se copia aquí
3. **Preservación**: Las soluciones se mantienen como referencia

## Propósito

- **Backup**: Conservar las soluciones validadas
- **Referencia**: Revisar implementaciones previas cuando te atascas
- **Práctica**: Comparar diferentes enfoques al rehacer ejercicios
- **Estudio**: Consultar soluciones correctas si fallas los tests

## Soluciones Disponibles

### Level 1
- ✅ **filter** - Reemplazo de cadenas con asteriscos
- ✅ **scanf** - Implementación de scanf con %s, %d, %c (dos versiones)
- ✅ **broken_gnl** - Versión reparada de get_next_line

## Nota

Estas soluciones se copian automáticamente de:
- `level-X/exercise_name/exercise_name.c` (si existe)
- `level-X/exercise_name/solutions/exercise_name.c` (alternativa)
- Primer archivo `.c` encontrado en el directorio del ejercicio

No necesitas gestionar esta carpeta manualmente - el sistema `exam.sh` se encarga de todo.
