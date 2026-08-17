# Modo 3 — Preparación de examen offline

Para exámenes de Rank (02–06) donde el alumno se enfrenta al subject en blanco, sin internet, sin
código propio a la vista. La habilidad real no es recordar una solución, sino reconocer la
familia del problema y reconstruir un esqueleto válido desde cero, verificado con casos límite.

Distinción central: **ESTUDIO** puede contrastar contra `rendu*` (soluciones ya resueltas por el
alumno); **SIMULACIÓN** se comporta como si solo existiera el subject y las herramientas locales
permitidas en examen. Confundir los dos contextos convierte las soluciones en una muleta — ver
regla en `SKILL.md` ("Reglas que SIEMPRE aplico").

## Protocolo

1. **Localizar el Rank y el ejercicio**: leer primero su subject real en `02/`…`06/` (usando
   `workspace/CONFIG.md` para la ruta, igual que Modo 2).
2. **Extraer el contrato**: entrada, salida, funciones autorizadas, errores, restricciones y
   comportamiento en edge cases.
3. **Clasificar el ejercicio por familia de patrón, no por nombre**: parsing, recorrido de
   strings, aritmética, listas enlazadas, recursión, backtracking, parsing de expresiones,
   pipes/procesos, etc. El nombre del ejercicio no es la unidad de estudio — la familia sí,
   porque es lo que se transfiere a un subject nuevo en examen.
4. **Solo en ESTUDIO**, leer una solución `rendu*` como evidencia, nunca como respuesta para
   copiar. Extraer:
   - invariante central;
   - esqueleto mínimo de la solución;
   - operaciones que se repiten;
   - errores de compilación o lógica previsibles;
   - tests manuales mínimos;
   - una regla nemotécnica breve y comprobable (apunta a un invariante, un orden de operaciones o
     un edge case concreto — nunca una frase vaga).
5. **Ficha de patrón** en `workspace/<módulo>/reference/000N-patron-<nombre>.html` — vinculada al
   ejercicio pero escrita con palabras propias y pseudocódigo, no con una reproducción completa
   de la solución. Sigue las reglas de diseño de `lessons-format.md` (mismo `assets/style.css`,
   cargar `artifact-design` antes de escribirlo).
6. **Retrieval**: mostrar solo el subject o una versión resumida del contrato; el alumno dice qué
   familia es, propone el orden de construcción y escribe el esqueleto.
7. **Simulación**: prohibido abrir `rendu*`, apuntes o internet. Una pista máxima por bloqueo,
   centrada en el invariante o en el siguiente test a probar — nunca en pegar código.
8. **Debrief**: comparar después con la solución de estudio, registrar únicamente los fallos, la
   causa, y una señal de reconocimiento para la próxima vez (en `learning-records/`).

## Señal de reconocimiento

Para cada ejercicio trabajado en Modo 3, se registra explícitamente: qué detalle concreto del
subject indica la familia de problema, y qué estructura inicial conviene crear a partir de ahí.
Esto es lo que el alumno debe poder recordar de un vistazo al subject en examen, antes de
diseñar nada.

## Criterio de dominio

Antes de declarar un patrón dominado: el alumno lo reconstruye sin mirar `rendu*` ni la ficha, lo
compila, y explica al menos un caso límite sin ayuda. Igual que Modo 1/Modo 2, no se marca
dominado por cobertura ("lo vimos") sino por evidencia de recuperación activa bajo las mismas
condiciones que el examen real.

## Relación con Modo 2

Modo 2 ya cubre "leer un subject nuevo y derivar el orden de construcción" para ejercicios
Tipo A/B (`guided-implementation-format.md`) — Modo 3 no lo sustituye, añade encima el ciclo
completo estudio→patrón→simulación→debrief, y generaliza más allá de clases C++ a cualquier
familia algorítmica de Rank 02–06. Si el módulo ya tiene un `GLOSSARY.md`/`RESOURCES.md` de Modo 1
o un `reference/0001-plan-implementacion.html` de Modo 2, Modo 3 los reutiliza en vez de
reexplicar — mismo principio de "verifico, no asumo" de `SKILL.md`.
