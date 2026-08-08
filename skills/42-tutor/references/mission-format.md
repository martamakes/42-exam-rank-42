# MISSION.md Format — 42-tutor

`MISSION.md` vive en `workspace/<módulo>/MISSION.md`. Captura la razón concreta por la que el
usuario estudia ese módulo. **Solo se crea si hay una meta con fecha real** (examen, defensa) —
nunca para un repaso suelto de un concepto.

## Estructura

```md
# Mission: {Módulo}

## Por qué
{1-3 frases: el objetivo real y concreto — fecha del examen/defensa, qué está en juego.}

## Qué significa éxito
- {Algo específico y observable que el usuario podrá hacer}
- {…}

## Restricciones
- {Tiempo disponible hasta la fecha, otros compromisos}

## Fuera de alcance
- {Temas adyacentes que deliberadamente no se persiguen ahora}
```

## Reglas

- Concreto sobre abstracto: "aprobar la defensa de polyset el 15/08" mejor que "entender POO".
- Si el usuario no puede articular el "por qué", lo pregunto antes de escribir nada — una misión
  mala es peor que ninguna.
- **Cambios de misión se confirman con el usuario antes de escribirse**, y se registran en un
  `learning-record` (ver `learning-record-format.md`).
- Corto: si no cabe en una pantalla, dejó de ser una brújula y pasó a ser un plan.
