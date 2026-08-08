# Lessons, Reference & Assets Format — 42-tutor

Cómo generar los artefactos durables del workspace: `lessons/*.html`, `reference/*.html`, y los
componentes compartidos en `assets/`. Se lee al llegar al Paso 5 del protocolo de explicación
(bloque completo confirmado), no antes.

## Cuándo se genera una lección

Solo cuando un bloque **completo** del árbol de dependencias queda confirmado con evidencia
(checkpoint superado, no un "vale") — nunca por cada micro-intercambio. Cada lección debe ser
"una única victoria tangible".

## Lecciones (`lessons/000N-slug.html`)

- Un HTML autocontenido por lección, numeración secuencial dentro del módulo.
- Un único logro tangible, atado al bloque recién confirmado — nada de lecciones-enciclopedia.
- Diseño limpio, pensado para releerse/imprimirse antes de un examen — estilo Tufte, no
  plantilla genérica. **Cargo el skill `artifact-design` antes de escribir el HTML.**
- Enlaza a otras lecciones y a `reference/` relevantes del mismo módulo.
- Cita la fuente primaria más fiable (de `RESOURCES.md` — ver `resources-format.md`).
- Cierra siempre con un recordatorio: *"¿Algo no queda claro? Pregúntamelo directamente — soy tu
  profe, no solo esta lección."*
- Reutiliza `workspace/assets/style.css` y demás componentes — nunca reinventa estilos.
- Publicación por defecto: archivo local + `open "<ruta>"` (macOS). Si el usuario quiere un link
  para revisar desde el móvil o compartir, uso la herramienta `Artifact` en su lugar (cargando
  `artifact-design` igualmente).

## Referencia (`reference/000N-slug.html`)

Cheat sheets de sintaxis, algoritmos con pseudocódigo, diagramas, o un glosario renderizado
bonito. Se generan solo cuando algo se va a re-consultar bajo presión de examen — no por cada
concepto cubierto. `GLOSSARY.md` (markdown) sigue siendo la fuente canónica de términos; esto es
su versión pulida para repaso rápido.

## Componentes compartidos (`assets/`)

Reutilizar es el default, no la excepción. Antes de escribir una lección o referencia nueva,
reviso `workspace/assets/` y construyo sobre lo que ya existe. `style.css` es el primer
componente que se gana cualquier workspace — todas las lecciones lo enlazan para verse como un
único curso, no piezas sueltas. Un componente nuevo y reutilizable (widget de quiz, helper de
diagramas) se escribe en `assets/` y se enlaza — nunca se deja inline si una lección futura lo
va a necesitar también.

## Regla de quizzes

Cuando la pregunta de la simulación de eval admite una respuesta corta tipo opción/etiqueta (no
una explicación libre), todas las opciones/respuestas posibles deben tener el mismo número de
palabras y, si es posible, de caracteres — el formato no debe filtrar la respuesta correcta.

## Interleaving en la simulación de evaluación

En la simulación final (nunca en la explicación inicial de un bloque), mezclo preguntas de
bloques relacionados en vez de ir en el mismo orden en que se explicaron — se parece más a un
examen real y fuerza retrieval, no reconocimiento en caliente.

## Por qué esto importa: Fluency vs. Storage Strength

- **Fluency**: lo reconoces en caliente justo después de que te lo expliquen. Sensación de
  dominio engañosa.
- **Storage strength**: lo retienes semanas después, bajo presión de examen. El objetivo real.
- Por eso el checkpoint pide recordar/producir en vez de "¿tiene sentido?" (retrieval practice),
  por eso antes de reexplicar algo con workspace existente pido que lo recuperes primero
  (spacing), y por eso la eval final mezcla bloques (interleaving) en vez de ir en el mismo
  orden en que se enseñaron.
