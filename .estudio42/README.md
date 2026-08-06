# .estudio42 - Directorio de Configuración

Este directorio contiene la configuración y datos del skill **Estudio42**.

## 📁 Estructura

```
.estudio42/
├── config.json          # Configuración del usuario (idioma, nombre)
├── progress.json        # Tracking de progreso (sesiones, pistas usadas)
├── README.md           # Este archivo
└── tips/               # Tips organizados por rank/level
    ├── rank-02/
    │   ├── level-1/
    │   ├── level-2/
    │   ├── level-3/
    │   └── level-4/
    ├── rank-03/
    ├── rank-04/
    ├── rank-05/
    └── rank-06/
```

## 🔧 config.json

Configuración personal del skill:

```json
{
  "language": "es",           // Idioma: "es" o "en"
  "username": "marta",        // Tu username (para tips personales)
  "created": "2026-02-08",    // Fecha de primera configuración
  "lastUsed": "2026-02-08"    // Última vez que usaste el skill
}
```

Para cambiar el idioma:
- Desde Claude: `cambia idioma a inglés` o `change language to english`
- Manualmente: Edita `language` en config.json

## 📊 progress.json

Tracking de tu progreso:

```json
{
  "sessions": [
    {
      "date": "2026-02-08T10:30:00Z",
      "exercise": "ft_strlen",
      "rank": "02",
      "level": "1",
      "hints_used": 3,
      "completed": true,
      "time_minutes": 25
    }
  ],
  "total_hints": 47,
  "exercises_completed": 12,
  "exercises_started": ["ft_strlen", "first_word", ...]
}
```

## 📚 tips/

Directorio de tips organizados por rank y level.

### Estructura de un archivo de tips:

Cada ejercicio tiene su archivo `.md` con:

1. **📋 Subject**: El enunciado del ejercicio
2. **💡 Tips Generales**: Conceptos, recursos, complejidad
3. **🎯 Enfoque Sugerido**: 7 niveles de pistas progresivas
4. **👤 Tips de Usuarios**: Consejos personales por usuario

### Generación automática:

Si no existe un archivo de tips, el skill:
1. Busca el subject (README.md, subject.txt)
2. Lo lee y extrae información
3. Genera el archivo con tips básicos
4. Lo guarda en tips/rank-XX/level-Y/

### Sistema colaborativo:

Cada usuario tiene su propia sección en los tips:

```markdown
## 👤 Tips de Usuarios

### @marta (2026-02-08)
- Tip personal 1
- Tip personal 2

### @otro_estudiante (2026-02-09)
- Su tip personal
```

Nadie sobreescribe los tips de otros.

## ⚙️ Comandos del Skill

Durante una sesión de `/estudio42`:

- `dame una pista` → Siguiente nivel (1-7)
- `revisa mi código` → Análisis sin solución
- `¿por qué este error?` → Modo debug
- `añade mi tip: [texto]` → Guarda tip personal
- `escríbelo tú` → Excepción: Claude escribe
- `mi progreso` → Muestra stats
- `cambia idioma a [idioma]` → Cambiar idioma

## 🗑️ Limpiar datos

Si quieres empezar de cero:

```bash
# Borrar solo progreso (mantener tips)
rm .estudio42/progress.json

# Borrar todo
rm -rf .estudio42/
```

Los tips se regenerarán automáticamente cuando vuelvas a usar el skill.

## 🔒 Git

Este directorio está incluido en `.gitignore` por defecto (debería estarlo).
Si quieres compartir tus tips con otros, puedes:

```bash
# En .gitignore, comentar:
# .estudio42/

# O solo commitear tips:
git add -f .estudio42/tips/
```

## 📝 Notas

- Los tips se crean **bajo demanda** (solo cuando los necesitas)
- El tracking es **local** (no se sincroniza entre repos)
- Cada proyecto 42 tiene su propio `.estudio42/`
- Portable: el skill funciona en cualquier proyecto 42

---

**Para más info:** Lee el README.md del skill en `skills/estudio42/`
