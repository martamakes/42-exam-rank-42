# Estudio42 Skill | 42 School Study Mode

🎓 Tutor socrático interactivo para exámenes de la Escuela 42
🎓 Interactive Socratic tutor for 42 School exams

---

## 🌍 Español

### ¿Qué es esto?

**Estudio42** es un skill para Claude Code que te ayuda a aprender programación al estilo de la Escuela 42: descubriendo las soluciones por ti mismo, guiado por preguntas en lugar de recibir código hecho.

### ✨ Características

- 🤖 **Tutor Socrático**: Te guía con preguntas, no con respuestas directas
- 🎯 **Detección Automática**: Identifica qué ejercicio estás haciendo
- 💡 **Sistema de Pistas**: 7 niveles progresivos de ayuda
- 📚 **Tips Personalizados**: Cada ejercicio tiene consejos específicos
- 🌱 **Generación Automática**: Si no hay tips, los genera del subject
- 👥 **Colaborativo**: Múltiples usuarios pueden añadir tips sin sobreescribir
- 🌍 **Multilingüe**: Español e Inglés
- 📊 **Tracking**: Sigue tu progreso y pistas usadas

### 🚀 Instalación

#### Opción 1: Global (Recomendado)

```bash
cd ~/.claude/skills/
git clone https://github.com/tu-repo/estudio42.git
cd estudio42
npm install
```

#### Opción 2: Por Proyecto

```bash
cd /ruta/a/tu-proyecto-42/
mkdir -p skills
cd skills
git clone https://github.com/tu-repo/estudio42.git
cd estudio42
npm install
```

### 📖 Uso

#### Activar el modo estudio

```bash
cd /ruta/a/tu-proyecto-42/
claude
> /estudio42
```

El skill:
1. Detecta automáticamente el ejercicio actual
2. Te confirma la detección
3. Busca o genera tips del ejercicio
4. Activa el modo tutor socrático

#### Comandos durante la sesión

- **"dame una pista"** → Siguiente nivel de pista (1-7)
- **"revisa mi código"** → Análisis constructivo sin dar solución
- **"¿por qué este error?"** → Explica errores de compilación
- **"ayúdame con el main"** → Te guía para escribir el main de test (modo socrático)
- **"añade mi tip: [texto]"** → Guarda un consejo personal
- **"escríbelo tú"** → Excepción: Claude escribe código
- **"siguiente ejercicio"** → Cambia de ejercicio
- **"mi progreso"** → Muestra estadísticas
- **"cambia idioma a inglés"** → Cambiar idioma

### 🎯 Las 7 Reglas del Modo Tutor

1. ❌ **NO** escribir código por ti (salvo que digas "escríbelo tú")
2. ✅ Guiarte con **preguntas** para que descubras la solución
3. ✅ **Revisar** tu código cuando lo pidas
4. ✅ Dar **pistas progresivas** (7 niveles) si te bloqueas
5. ✅ **Desglosar** problemas complejos en pasos pequeños
6. ✅ **Preguntarte** qué estás pensando antes de dar respuestas
7. ✅ **Explicar errores** sin darte la solución directa

### 💡 Sistema de Pistas (7 Niveles)

Cada ejercicio tiene pistas progresivas:

1. **Pregunta Diagnóstica**: "¿Qué hace esta función?"
2. **Pregunta Específica**: "¿Qué estructura de datos necesitas?"
3. **Concepto Clave**: Explicación del concepto principal
4. **Estrategia**: Pasos generales para resolver
5. **Pseudocódigo**: Algoritmo en lenguaje natural
6. **Código Parcial**: Estructura básica en C
7. **Edge Cases**: Errores comunes y checklist

### 📁 Estructura de Archivos

Después de usar el skill por primera vez:

```
tu-proyecto-42/
├── .estudio42/
│   ├── config.json          # Configuración (idioma, usuario)
│   ├── progress.json         # Tracking de progreso
│   └── tips/
│       ├── rank-02/
│       │   ├── level-1/
│       │   │   ├── ft_strlen.md
│       │   │   └── first_word.md
│       │   └── level-4/
│       │       ├── flood_fill.md
│       │       └── ft_list_foreach.md
│       └── rank-04/
│           └── level-1/
│               └── ft_popen.md
```

### 🔄 Flujo de Trabajo

#### Primera vez en un ejercicio:

```
1. /estudio42
2. Claude detecta ejercicio → "¿Es correcto?"
3. No hay tips → Busca subject
4. Genera tips automáticamente
5. Activa modo tutor
6. "¿Qué quieres hacer?"
```

#### Siguientes veces:

```
1. /estudio42
2. Claude detecta ejercicio
3. Carga tips existentes
4. Activa modo tutor
5. "¿Sigues en [ejercicio]?"
```

### 👥 Sistema Colaborativo

Los tips tienen una sección por usuario:

```markdown
## 👤 Tips de Usuarios

### @marta (2026-02-08)
- Para popen mira el man de pipe, tiene ejemplo útil
- No olvides cerrar los file descriptors

### @otro_estudiante (2026-02-09)
- Yo lo hice con un approach diferente
- Dibuja el flujo en papel primero
```

**Nadie sobreescribe los tips de otros**, cada uno tiene su sección.

### 🌱 Generación Automática

Si no hay tips para un ejercicio:

1. Busca el subject (README.md, subject.txt)
2. Extrae: prototipo, descripción, allowed functions
3. Genera tips con estructura completa
4. Guarda en `.estudio42/tips/`

Si no encuentra el subject, te pide que lo proporciones.

### 🌐 Portable y Reutilizable

El skill funciona en:
- ✅ Repos de examen (Rank 02-06)
- ✅ Proyectos grandes (libft, minishell, etc.)
- ✅ Cualquier repo de 42

Cada proyecto tiene su propia configuración y tips.

### 🧪 Ejemplos

#### Ejemplo 1: Primera vez con ft_strlen

```
> /estudio42

🎓 MODO ESCUELA 42 ACTIVADO

Detecté que estás en: ft_strlen (Rank 02, Level 1)
¿Es correcto? (s/n)

> s

Generando tips automáticamente desde el subject...
✅ Tips generados y guardados

¿Qué quieres hacer?
```

#### Ejemplo 2: Pedir pista

```
> dame una pista

💡 Pista nivel 1/7

¿Qué tipo de dato devuelve ft_strlen?
¿Qué parámetro recibe?
```

#### Ejemplo 3: Revisar código

```
> revisa mi código

[Claude lee tu código]

Veo que estás usando un while loop. Bien pensado.

¿Qué condición estás usando para parar el loop?
¿Has considerado qué pasa si str es NULL?
```

#### Ejemplo 4: Añadir tip personal

```
> añade mi tip: para este ejercicio ayuda dibujar la lista en papel

✅ Tip personal añadido a tu sección
```

### 🛠️ Desarrollo

#### Estructura del código:

- `index.js` - Lógica principal del skill
- `prompts.js` - Textos multilingües
- `detect.js` - Detección de ejercicios
- `generator.js` - Generación de tips
- `package.json` - Configuración npm

#### Contribuir:

1. Fork el repo
2. Crea una rama: `git checkout -b feature/nueva-funcionalidad`
3. Commit: `git commit -m 'Add: nueva funcionalidad'`
4. Push: `git push origin feature/nueva-funcionalidad`
5. Abre un Pull Request

---

## 🌍 English

### What is this?

**Estudio42** is a Claude Code skill that helps you learn programming the 42 School way: discovering solutions yourself, guided by questions instead of receiving ready-made code.

### ✨ Features

- 🤖 **Socratic Tutor**: Guides you with questions, not direct answers
- 🎯 **Auto-Detection**: Identifies which exercise you're working on
- 💡 **Hint System**: 7 progressive levels of help
- 📚 **Custom Tips**: Each exercise has specific advice
- 🌱 **Auto-Generation**: If no tips exist, generates them from subject
- 👥 **Collaborative**: Multiple users can add tips without overwriting
- 🌍 **Multilingual**: Spanish and English
- 📊 **Tracking**: Follows your progress and hints used

### 🚀 Installation

#### Option 1: Global (Recommended)

```bash
cd ~/.claude/skills/
git clone https://github.com/your-repo/estudio42.git
cd estudio42
npm install
```

#### Option 2: Per Project

```bash
cd /path/to/your-42-project/
mkdir -p skills
cd skills
git clone https://github.com/your-repo/estudio42.git
cd estudio42
npm install
```

### 📖 Usage

#### Activate study mode

```bash
cd /path/to/your-42-project/
claude
> /estudio42
```

The skill:
1. Auto-detects current exercise
2. Confirms detection with you
3. Finds or generates exercise tips
4. Activates Socratic tutor mode

#### Commands during session

- **"give me a hint"** → Next hint level (1-7)
- **"review my code"** → Constructive analysis without giving solution
- **"why this error?"** → Explains compilation errors
- **"help me with the main"** → Guides you to write test main (Socratic mode)
- **"add my tip: [text]"** → Saves a personal tip
- **"write it for me"** → Exception: Claude writes code
- **"next exercise"** → Change exercise
- **"my progress"** → Shows statistics
- **"change language to english"** → Change language

### 🎯 The 7 Tutor Mode Rules

1. ❌ **DON'T** write code for you (unless you say "write it for me")
2. ✅ Guide you with **questions** to discover the solution
3. ✅ **Review** your code when you ask
4. ✅ Give **progressive hints** (7 levels) if you're stuck
5. ✅ **Break down** complex problems into small steps
6. ✅ **Ask** what you're thinking before giving answers
7. ✅ **Explain errors** without giving direct solution

### 💡 Hint System (7 Levels)

Each exercise has progressive hints:

1. **Diagnostic Question**: "What does this function do?"
2. **Specific Question**: "What data structure do you need?"
3. **Key Concept**: Explanation of main concept
4. **Strategy**: General steps to solve
5. **Pseudocode**: Algorithm in natural language
6. **Partial Code**: Basic structure in C
7. **Edge Cases**: Common errors and checklist

### 📁 File Structure

After using the skill for the first time:

```
your-42-project/
├── .estudio42/
│   ├── config.json          # Configuration (language, user)
│   ├── progress.json         # Progress tracking
│   └── tips/
│       ├── rank-02/
│       │   ├── level-1/
│       │   │   ├── ft_strlen.md
│       │   │   └── first_word.md
│       │   └── level-4/
│       │       ├── flood_fill.md
│       │       └── ft_list_foreach.md
│       └── rank-04/
│           └── level-1/
│               └── ft_popen.md
```

### 🛠️ Development

#### Code structure:

- `index.js` - Main skill logic
- `prompts.js` - Multilingual texts
- `detect.js` - Exercise detection
- `generator.js` - Tips generation
- `package.json` - npm configuration

#### Contributing:

1. Fork the repo
2. Create branch: `git checkout -b feature/new-feature`
3. Commit: `git commit -m 'Add: new feature'`
4. Push: `git push origin feature/new-feature`
5. Open Pull Request

---

## 📝 License

MIT License - See LICENSE file for details

## 👥 Authors

- mvigara-
- Contributions welcome!

## 🔗 Links

- [42 School](https://www.42.fr/)
- [Claude Code](https://github.com/anthropics/claude-code)

---

**¿Preguntas? ¿Sugerencias?**
**Questions? Suggestions?**

Abre un issue en el repo | Open an issue in the repo
