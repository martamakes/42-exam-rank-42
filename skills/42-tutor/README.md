# 42-tutor

Skill de Claude Code para estudiar el Common Core de 42 con un enfoque bottom-up, estado
persistente por módulo y dos modos de trabajo: entender conceptos o que te guíe implementando un
subject real. / Claude Code skill for studying the 42 Common Core with a bottom-up approach,
persistent per-module state, and two working modes: understanding concepts or guided
implementation of a real subject.

---

## Español

### Qué es

`42-tutor` es una skill instalable en `~/.claude/skills/42-tutor/`. Cuando la invocas, Claude Code
actúa como un tutor que:

- Nunca salta prerequisitos: mapea qué conceptos previos hacen falta antes de explicar uno nuevo.
- Verifica antes de reexplicar: revisa tu progreso guardado (`workspace/<módulo>/`) antes de
  repetir algo que ya demostraste que sabes.
- Prioriza fuentes citables sobre su memoria — si no hay una fuente fiable, te lo dice
  explícitamente en vez de sonar seguro sin estarlo.
- Usa retrieval practice (recordar/producir) en vez de "¿tiene sentido?" — checkpoints cortos por
  concepto, simulación de evaluador al final de cada bloque.
- Guarda todo el progreso en disco, así que puedes retomar un módulo semanas después sin repetir
  desde cero.

### Instalación

Copia la carpeta completa `42-tutor/` a tu directorio de skills de usuario:

```bash
cp -r 42-tutor ~/.claude/skills/42-tutor
```

No necesitas configurar nada más — Claude Code detecta la skill automáticamente por su
`SKILL.md`.

### Cómo invocarla

Simplemente pide ayuda con cualquier módulo del curriculum y Claude Code la activará solo, o
invócala explícitamente:

```
/42-tutor
```

o en lenguaje natural:

```
Ayúdame a entender el módulo CPP08 de 42
Quiero preparar el examen de rank 05
Guíame para implementar el subject de minishell
```

### Los dos modos

**Modo 1 — Entender conceptos**
Para cuando quieres dominar el *porqué* y el *cómo* de un tema (por ejemplo, "no entiendo
`dynamic_cast`" o "quiero repasar RAII antes del examen"). El tutor construye el árbol de
dependencias de conceptos, te explica el más bajo del árbol que aún no domines, te hace un
checkpoint de retrieval, y solo avanza con evidencia real de que lo aprendiste. Al confirmar un
bloque completo, genera una lección HTML y un registro de aprendizaje.

*Ejemplo de conversación:*

```
Tú: quiero entender los templates de CPP07, nunca los he pillado del todo

Tutor: [revisa tu workspace, no encuentra progreso previo en CPP07]
       Antes de templates hace falta que domines sobrecarga de funciones y por qué
       el compilador no puede generalizar tipos sin ellos. ¿Te sientes segura ahí,
       o empezamos por eso?

Tú: empecemos por sobrecarga

Tutor: [explica sobrecarga: qué es / por qué existe / cómo funciona / ejemplo /
       cómo aparece en tu código real / fuente primaria]
       Checkpoint: sin mirar el código, explícame por qué el compilador elige una
       sobrecarga concreta en tiempo de compilación y no en runtime.
```

**Modo 2 — Implementación guiada**
Para cuando tienes un subject real delante y no sabes por dónde empezar (por ejemplo, "tengo el
PDF de polyset y no sé qué clase implementar primero"). El tutor lee el subject completo, separa
qué está dado de qué hay que escribir, construye el grafo de dependencias de *compilación* (no de
conceptos) y te guía clase por clase: contrato → esqueleto → implementación mínima → verificación
antes de pasar a la siguiente.

*Ejemplo de conversación:*

```
Tú: tengo que hacer el subject de ft_irc, guíame con el orden de implementación

Tutor: [lee el subject en /Volumes/BIWIN/42 - subjects/]
       Para que compile algo mínimo primero necesitas: Server (socket + bind + listen),
       luego Client (una conexión aceptada), luego el parser de comandos IRC.
       Empezamos por Server. Contrato: debe abrir un socket TCP en el puerto dado
       y aceptar conexiones sin bloquear el resto del bucle. ¿Cómo lo plantearías?
```

### Estado persistente por módulo

Cada módulo que trabajas genera su propio workspace:

```
~/.claude/skills/42-tutor/workspace/<módulo-slug>/
├── MISSION.md            # solo si hay una meta con fecha real (examen, defensa)
├── GLOSSARY.md           # términos que ya demostraste que dominas
├── RESOURCES.md          # fuentes citadas + huecos de conocimiento pendientes
├── learning-records/     # historial de qué se confirmó y cuándo
├── lessons/              # lecciones HTML generadas al confirmar un bloque
└── reference/            # cheat sheets / mapas de implementación (Modo 2)
```

Todo se crea de forma perezosa — nada se escribe hasta que hay algo real que registrar. Esto
significa que puedes cerrar la sesión y, semanas después, decir "sigamos con CPP08" y el tutor
retoma exactamente donde lo dejaste, sin volver a explicarte lo que ya confirmaste.

### Consejos de uso

- Sé específica con el módulo y el objetivo: "quiero aprobar la defensa de polyset el viernes" da
  mejores resultados que "ayúdame con C++".
- Si te bloqueas en un checkpoint, pide una pista en vez de la respuesta — el tutor está diseñado
  para no dártela directamente.
- El curriculum cubierto está en `references/curriculum.md`; si tu campus tiene círculos distintos,
  díselo al tutor y lo ajusta.
- Puedes alternar entre Modo 1 y Modo 2 dentro del mismo módulo: entender los conceptos primero y
  luego pedir la guía de implementación del subject.

---

## English

### What it is

`42-tutor` is a skill you install under `~/.claude/skills/42-tutor/`. Once invoked, Claude Code
acts as a tutor that:

- Never skips prerequisites: it maps which prior concepts you need before explaining a new one.
- Verifies before re-explaining: it checks your saved progress (`workspace/<module>/`) before
  repeating something you've already demonstrated you know.
- Prioritizes citable sources over its own memory — if there's no reliable source, it says so
  explicitly instead of sounding confident without being so.
- Uses retrieval practice (recall/produce) instead of "does this make sense?" — short checkpoints
  per concept, evaluator simulation at the end of each block.
- Saves all progress to disk, so you can resume a module weeks later without starting over.

### Installation

Copy the full `42-tutor/` folder into your user skills directory:

```bash
cp -r 42-tutor ~/.claude/skills/42-tutor
```

No further configuration needed — Claude Code auto-detects the skill from its `SKILL.md`.

### How to invoke it

Just ask for help with any curriculum module and Claude Code will activate it on its own, or
invoke it explicitly:

```
/42-tutor
```

or in natural language:

```
Help me understand 42's CPP08 module
I want to prepare for the rank 05 exam
Guide me through implementing the minishell subject
```

### The two modes

**Mode 1 — Understanding concepts**
For when you want to master the *why* and *how* of a topic (e.g. "I never really understood
`dynamic_cast`" or "I want to review RAII before the exam"). The tutor builds the concept
dependency tree, explains the lowest concept in the tree you haven't mastered yet, runs a
retrieval checkpoint, and only advances with real evidence you've learned it. Once a full block is
confirmed, it generates an HTML lesson and a learning record.

*Example conversation:*

```
You: I want to understand CPP07 templates, I've never fully gotten them

Tutor: [checks your workspace, finds no prior progress on CPP07]
       Before templates you need to be solid on function overloading and why the
       compiler can't generalize types without it. Are you confident there, or
       should we start with that?

You: let's start with overloading

Tutor: [explains overloading: what it is / why it exists / how it works / example /
       how it shows up in your real code / primary source]
       Checkpoint: without looking at the code, explain why the compiler picks a
       specific overload at compile time rather than at runtime.
```

**Mode 2 — Guided implementation**
For when you have a real subject in front of you and don't know where to start (e.g. "I have the
polyset PDF and don't know which class to implement first"). The tutor reads the full subject,
separates what's given from what you need to write, builds the *compilation* dependency graph
(not the concept one), and guides you class by class: contract → skeleton → minimal
implementation → verification before moving to the next.

*Example conversation:*

```
You: I need to do the ft_irc subject, guide me through the implementation order

Tutor: [reads the subject in /Volumes/BIWIN/42 - subjects/]
       To get something minimal compiling first you need: Server (socket + bind +
       listen), then Client (one accepted connection), then the IRC command parser.
       Let's start with Server. Contract: it must open a TCP socket on the given
       port and accept connections without blocking the rest of the loop. How would
       you approach it?
```

### Persistent per-module state

Every module you work on generates its own workspace:

```
~/.claude/skills/42-tutor/workspace/<module-slug>/
├── MISSION.md            # only if there's a goal with a real date (exam, defense)
├── GLOSSARY.md           # terms you've already demonstrated you master
├── RESOURCES.md          # cited sources + pending knowledge gaps
├── learning-records/     # history of what was confirmed and when
├── lessons/              # HTML lessons generated when a block is confirmed
└── reference/            # cheat sheets / implementation maps (Mode 2)
```

Everything is created lazily — nothing gets written until there's something real to record. That
means you can close the session and, weeks later, say "let's continue with CPP08" and the tutor
picks up exactly where you left off, without re-explaining what you already confirmed.

### Usage tips

- Be specific about the module and the goal: "I want to pass the polyset defense on Friday" gets
  better results than "help me with C++".
- If you get stuck on a checkpoint, ask for a hint instead of the answer — the tutor is designed
  not to give it to you directly.
- The covered curriculum lives in `references/curriculum.md`; if your campus has different
  circles/ranks, tell the tutor and it adjusts.
- You can switch between Mode 1 and Mode 2 within the same module: understand the concepts first,
  then ask for the subject implementation guide.
