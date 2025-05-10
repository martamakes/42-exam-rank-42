# Exam Rank 05

Este examen consiste en tres módulos progresivos de C++ que se enfocan en la programación orientada a objetos, con énfasis en:
- Clases en Forma Canónica de Coplien
- Clases abstractas e interfaces
- Herencia y polimorfismo
- Gestión de memoria dinámica
- Contenedores STL (std::map, etc.)

## Estructura del Proyecto

```
/05
├── cpp_module_00/  # Primer ejercicio (clase Warlock básica)
├── cpp_module_01/  # Segundo ejercicio (añade ASpell, ATarget, etc.)
├── cpp_module_02/  # Tercer ejercicio (añade más clases y funcionalidades)
└── rendu/          # Aquí debes colocar tus soluciones
    ├── cpp_module_00/
    ├── cpp_module_01/
    └── cpp_module_02/
```

## Código Reutilizable

Los ejercicios son progresivos, lo que significa que puedes reutilizar código entre ellos:

### De cpp_module_00 a cpp_module_01:
- La clase `Warlock` se amplía, pero su estructura base se mantiene
- Los atributos `name` y `title` se conservan
- Los métodos `getName()`, `getTitle()`, `setTitle()` e `introduce()` se mantienen iguales
- El constructor y destructor son similares

### De cpp_module_01 a cpp_module_02:
- Las clases `Warlock`, `ASpell`, `ATarget`, `Fwoosh` y `Dummy` se conservan
- Deben añadirse modificaciones a `Warlock` para usar `SpellBook`
- Las nuevas clases (`Fireball`, `Polymorph`, etc.) siguen el mismo patrón que `Fwoosh` y `Dummy`
- `SpellBook` y `TargetGenerator` siguen patrones similares

## Puntos Importantes

1. **Forma Canónica de Coplien**: Todas las clases deben tener:
   - Constructor por defecto
   - Destructor
   - Constructor de copia 
   - Operador de asignación

2. **Clases No Copiables**: Varias clases (como `Warlock`, `SpellBook` y `TargetGenerator`) no deben ser copiables. Esto se logra haciendo privados y no implementados:
   - El constructor de copia
   - El operador de asignación

3. **Métodos Puros**: Las clases abstractas como `ASpell` y `ATarget` tienen métodos puros (`clone()`).

4. **Gestión de Memoria**: Se deben gestionar correctamente las copias y eliminación de objetos.

5. **Estructura de Comandos**: El uso de `switch` está **prohibido** en ciertas clases.

## Instrucciones de Ejecución

Para ejecutar las pruebas de cada módulo:

```bash
cd cpp_module_XX  # donde XX es 00, 01 o 02
./test.sh
```

## Consejos

- Asegúrate de que tus clases sigan la Forma Canónica de Coplien
- Verifica que los métodos que deben ser sobreescribibles sean virtuales
- Gestiona correctamente la memoria (no debe haber memory leaks)
- Las clases abstractas deben tener al menos un método puro (virtual = 0)
- ¡No uses `switch` donde está prohibido!
