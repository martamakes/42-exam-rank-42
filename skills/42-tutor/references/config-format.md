# CONFIG.md Format — 42-tutor

`CONFIG.md` vive en `workspace/CONFIG.md` — como `NOTES.md`, es compartido entre todos los
módulos. Guarda las rutas LOCALES a esta máquina donde vive el código y los subjects del usuario,
para que la skill funcione igual en el ordenador de cualquier alumno sin hardcodear una ruta
concreta en `SKILL.md`.

## Por qué existe

Las rutas de disco son distintas para cada alumno (usuario, sistema operativo, dónde clona sus
repos). `CONFIG.md` es la única pieza de la skill con datos específicos de la máquina — vive en
`workspace/` (que no se distribuye ni se sube al repo de la skill) para que el resto de la skill
(`SKILL.md`, `references/`) siga siendo portable tal cual entre estudiantes.

## Cuándo se crea

La primera vez que el tutor necesita leer código o un subject real y `workspace/CONFIG.md` no
existe: pregunta las rutas una vez, las guarda, y no vuelve a preguntar. Si una ruta guardada ya
no existe en disco (repo movido, USB distinto), se pregunta de nuevo y se actualiza.

## Estructura

```md
# Config local — 42-tutor

- Código del alumno: {ruta absoluta al repo/carpeta con las implementaciones}
- Subjects/enunciados: {ruta absoluta a la carpeta con los PDFs/subject.txt, si existe}
```

## Reglas

- Nunca se asume una ruta — si `CONFIG.md` no existe o la ruta guardada ya no responde en disco,
  se pregunta antes de intentar leer nada.
- Este fichero es personal de cada instalación: **no se commitea** a un repo compartido de la
  skill (la copia distribuible en `skills/42-tutor/` de un repo de código no incluye
  `workspace/`).
- Si el alumno usa más de un ordenador (USB, dos máquinas), `CONFIG.md` puede tener rutas
  distintas en cada uno — es exactamente su propósito, no hay que sincronizarlo.
