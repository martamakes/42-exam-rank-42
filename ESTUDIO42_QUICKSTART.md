# 🎓 Estudio42 - Guía Rápida | Quick Start

## 🌍 Español

### ¿Qué es Estudio42?

Un tutor socrático interactivo para exámenes de 42 que te ayuda a **aprender haciendo**, guiándote con preguntas en lugar de darte código hecho.

### ⚡ Inicio Rápido

1. **Activa el modo:**
   ```bash
   /estudio42
   ```

2. **Claude detecta tu ejercicio automáticamente** y te confirma

3. **Sigue las instrucciones** del tutor socrático

### 🎯 Las 7 Reglas

El tutor **NO escribe código** por ti (salvo que digas "escríbelo tú"). En su lugar:

- ✅ Te guía con **preguntas**
- ✅ Da **pistas progresivas** (7 niveles)
- ✅ **Revisa** tu código cuando lo pidas
- ✅ **Explica errores** sin dar solución directa
- ✅ Te hace **pensar** antes de responder

### 💡 Comandos Durante la Sesión

| Comando | Qué hace |
|---------|----------|
| `dame una pista` | Siguiente nivel de pista (1→7) |
| `revisa mi código` | Análisis constructivo |
| `¿por qué este error?` | Modo debug (explica error) |
| `ayúdame con el main` | Te guía para escribir main de test |
| `añade mi tip: [texto]` | Guarda consejo personal |
| `escríbelo tú` | Excepción: Claude escribe |
| `mi progreso` | Muestra estadísticas |
| `cambia idioma a inglés` | Cambiar idioma |

### 📊 Sistema de Pistas (7 Niveles)

Cada vez que dices "dame una pista", avanzas un nivel:

1. **Pregunta diagnóstica** - "¿Qué hace esta función?"
2. **Pregunta específica** - "¿Qué estructura necesitas?"
3. **Concepto clave** - Explicación del concepto
4. **Estrategia** - Pasos generales
5. **Pseudocódigo** - Algoritmo en lenguaje natural
6. **Código parcial** - Estructura básica en C
7. **Edge cases** - Errores comunes + checklist

### 🔄 Ejemplo de Sesión

```
Tú: /estudio42

Claude: 🎓 MODO ESCUELA 42 ACTIVADO
        Detecté que estás en: ft_strlen (Rank 02, Level 1)
        ¿Es correcto? (s/n)

Tú: s

Claude: [Carga tips del ejercicio]
        ¿Qué quieres hacer? ¿Entender el ejercicio, revisar tu código,
        o necesitas una pista?

Tú: dame una pista

Claude: 💡 Pista nivel 1/7
        ¿Qué tipo de dato devuelve ft_strlen?
        ¿Qué parámetro recibe?

Tú: un int y recibe char *str

Claude: Muy bien. Ahora, ¿cómo podrías recorrer cada caracter del string?

Tú: [escribes tu código]

Tú: revisa mi código

Claude: [Lee tu código y da feedback constructivo SIN darte la solución]
        Veo que usas un while loop. Bien pensado.
        ¿Has considerado qué pasa si str es NULL?
```

### 📁 Archivos Creados

Después de usar el skill por primera vez:

```
tu-proyecto-42/
├── .estudio42/
│   ├── config.json       # Tu configuración (idioma, username)
│   ├── progress.json     # Tu progreso (ejercicios, pistas usadas)
│   └── tips/             # Tips por ejercicio
│       ├── rank-02/
│       ├── rank-04/
│       └── ...
```

### 👥 Sistema Colaborativo

Los tips se organizan por usuario. Cada uno tiene su sección:

```markdown
### @marta (2026-02-08)
- Mi consejo personal

### @otro_estudiante (2026-02-09)
- Su consejo diferente
```

Nadie sobreescribe los tips de otros.

### 🌱 Generación Automática

Si no hay tips para un ejercicio:
1. Busca el subject (README.md, subject.txt)
2. Lo lee y analiza
3. Genera tips automáticamente
4. Los guarda para próximas veces

### 🛠️ Configuración

**Cambiar idioma:**
```
cambia idioma a inglés
```

**Ver progreso:**
```
mi progreso
```

**Añadir tip personal:**
```
añade mi tip: para este ejercicio mira man pipe, tiene ejemplo útil
```

### 📖 Más Información

- **README completo del skill:** `skills/estudio42/README.md`
- **Configuración detallada:** `.estudio42/README.md`
- **Tips de ejemplo:** `.estudio42/tips/rank-04/level-1/ft_popen.md`

---

## 🌍 English

### What is Estudio42?

An interactive Socratic tutor for 42 exams that helps you **learn by doing**, guiding you with questions instead of giving you ready-made code.

### ⚡ Quick Start

1. **Activate the mode:**
   ```bash
   /estudio42
   ```

2. **Claude auto-detects your exercise** and confirms with you

3. **Follow the instructions** from the Socratic tutor

### 🎯 The 7 Rules

The tutor **DOES NOT write code** for you (unless you say "write it for me"). Instead:

- ✅ Guides you with **questions**
- ✅ Gives **progressive hints** (7 levels)
- ✅ **Reviews** your code when you ask
- ✅ **Explains errors** without giving direct solution
- ✅ Makes you **think** before answering

### 💡 Commands During Session

| Command | What it does |
|---------|--------------|
| `give me a hint` | Next hint level (1→7) |
| `review my code` | Constructive analysis |
| `why this error?` | Debug mode (explains error) |
| `help me with the main` | Guides you to write test main |
| `add my tip: [text]` | Save personal advice |
| `write it for me` | Exception: Claude writes |
| `my progress` | Show statistics |
| `change language to spanish` | Change language |

### 📊 Hint System (7 Levels)

Each time you say "give me a hint", you advance one level:

1. **Diagnostic question** - "What does this function do?"
2. **Specific question** - "What structure do you need?"
3. **Key concept** - Concept explanation
4. **Strategy** - General steps
5. **Pseudocode** - Algorithm in natural language
6. **Partial code** - Basic structure in C
7. **Edge cases** - Common errors + checklist

### 🔄 Example Session

```
You: /estudio42

Claude: 🎓 42 SCHOOL MODE ACTIVATED
        Detected you're working on: ft_strlen (Rank 02, Level 1)
        Is this correct? (y/n)

You: y

Claude: [Loads exercise tips]
        What do you want to do? Understand the exercise, review your code,
        or need a hint?

You: give me a hint

Claude: 💡 Hint level 1/7
        What data type does ft_strlen return?
        What parameter does it receive?

You: an int and receives char *str

Claude: Very good. Now, how could you iterate through each character?

You: [write your code]

You: review my code

Claude: [Reads your code and gives constructive feedback WITHOUT solution]
        I see you're using a while loop. Good thinking.
        Have you considered what happens if str is NULL?
```

### 📁 Created Files

After using the skill for the first time:

```
your-42-project/
├── .estudio42/
│   ├── config.json       # Your config (language, username)
│   ├── progress.json     # Your progress (exercises, hints used)
│   └── tips/             # Tips per exercise
│       ├── rank-02/
│       ├── rank-04/
│       └── ...
```

### 👥 Collaborative System

Tips are organized by user. Each has their section:

```markdown
### @marta (2026-02-08)
- My personal advice

### @another_student (2026-02-09)
- Their different advice
```

Nobody overwrites others' tips.

### 🌱 Auto-Generation

If no tips exist for an exercise:
1. Searches for subject (README.md, subject.txt)
2. Reads and analyzes it
3. Auto-generates tips
4. Saves them for next time

### 🛠️ Configuration

**Change language:**
```
change language to spanish
```

**View progress:**
```
my progress
```

**Add personal tip:**
```
add my tip: for this exercise check man pipe, has useful example
```

### 📖 More Information

- **Full skill README:** `skills/estudio42/README.md`
- **Detailed configuration:** `.estudio42/README.md`
- **Example tips:** `.estudio42/tips/rank-04/level-1/ft_popen.md`

---

## 🚀 Ready to Start?

```bash
/estudio42
```

¡Buena suerte! | Good luck! 🎓
