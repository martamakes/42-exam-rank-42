# Learning Record Format — 42-tutor

Viven en `workspace/<módulo>/learning-records/`, numeración secuencial: `0001-slug.md`,
`0002-slug.md`... La carpeta se crea de forma perezosa, al primer registro.

Son el equivalente pedagógico de un ADR: capturan comprensión no obvia, no un log de actividad.

## Plantilla

```md
# {Título corto de lo que se estableció}

{1-3 frases: qué se aprendió (o qué conocimiento previo se estableció), y por qué importa para
sesiones futuras.}
```

Eso es todo el formato. Un registro puede ser un único párrafo.

## Cuándo escribo uno

1. El usuario responde bien a un checkpoint o pregunta de evaluador — no un simple "vale, entendido".
2. Declara conocimiento previo explícito ("esto ya lo sabía de X") — anoto también la
   profundidad declarada.
3. Se corrige un malentendido claro — estos predicen dónde tropezará en temas relacionados.
4. Cambia la misión del módulo — cross-link a `MISSION.md` (ver `mission-format.md`).

## Lo que NO cuenta

- Que yo haya explicado algo. Cobertura no es aprendizaje — hace falta evidencia de que el
  usuario puede usarlo correctamente.
- Actividad sesión a sesión. No es un diario.
- Algo que ya está capturado con precisión en `GLOSSARY.md` como definición de término — no
  duplicar.

## Supersesión

Si un registro nuevo contradice uno anterior (el entendimiento se profundizó o se corrigió), el
antiguo se marca `Status: superseded by LR-NNNN` — no se borra. El historial de cómo evolucionó
el entendimiento es señal útil.
