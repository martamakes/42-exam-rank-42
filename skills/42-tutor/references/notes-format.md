# NOTES.md Format — 42-tutor

`NOTES.md` vive en `workspace/NOTES.md` — es el único fichero compartido entre TODOS los módulos
(no por módulo). Captura preferencias del usuario que aplican a cualquier tema que se estudie,
para no tener que redescubrirlas cada vez.

## Qué va aquí

- **Preferencias de explicación**: cómo prefiere que se le explique algo (p.ej. desglose de
  sintaxis fragmento a fragmento, evitar analogías, ejemplos ejecutables siempre).
- **Restricciones de proceso** (no técnicas del subject — esas van en `CONSTRAINTS.md` del
  módulo, ver `constraints-format.md`): reglas sobre CÓMO quiere trabajar, no sobre qué permite
  el enunciado. P.ej. "nunca me des la solución directa, dame pistas", "avísame antes de generar
  una lección HTML", "prefiere sesiones cortas de un solo concepto".
- **Contexto transversal**: cosas que afectan a cualquier módulo, como el campus/cohorte si
  difiere del curriculum por defecto en `curriculum.md`.

## Qué NO va aquí

- Restricciones técnicas de un subject concreto → `workspace/<módulo>/CONSTRAINTS.md`.
- Progreso o términos dominados de un módulo concreto → `GLOSSARY.md` / `learning-records/`.
- Rutas locales a esta máquina (código, subjects) → `workspace/CONFIG.md` (`config-format.md`).

## Estructura

```md
# Notas transversales — 42-tutor

Preferencias del usuario que aplican a cualquier módulo, no solo a uno.

- **{Preferencia}** ("{cita textual del usuario si existe}") — {por qué / cuándo aplica}.
```

## Reglas

- Se añade en cuanto el usuario expresa una preferencia de proceso que se repetiría en otro
  módulo — no hace falta que lo pida explícitamente, pero si hay duda de si es una preferencia
  durable o algo puntual de esa sesión, se pregunta antes de escribir.
- Nunca se borra una entrada salvo que el usuario diga que ya no aplica.
