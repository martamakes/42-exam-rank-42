# Modo 2 — Implementación guiada desde el subject

Para cuando el objetivo no es aprender un concepto suelto (eso es Modo 1), sino la habilidad de
leer un enunciado de 42 que nunca has visto y salir con un plan de construcción ordenado — la
habilidad real que hace falta en examen.

## Diferencia con Modo 1

| | Modo 1 — Conceptos | Modo 2 — Implementación guiada |
|---|---|---|
| Pregunta que responde | ¿Qué es esto y por qué existe? | ¿Qué implemento primero y por qué en ese orden? |
| Tipo de grafo | Dependencias de CONCEPTOS (qué hay que entender antes) | Dependencias de COMPILACIÓN (qué tiene que existir antes para que enlace) |
| Verificación | Checkpoint de retrieval (explica/predice) | Compilar + probar cada pieza antes de la siguiente |
| Artefacto durable | `lessons/*.html` (una victoria conceptual) | `reference/*.html` tipo mapa de implementación |
| Meta final | Dominar el concepto para defenderlo en eval | Poder leer un subject nuevo solo, sin ayuda, en examen |

Si el usuario ya pasó por Modo 1 para el mismo módulo, reutilizo esa base (`GLOSSARY.md`,
`learning-records/`) — Modo 2 no reexplica conceptos ya confirmados, los da por hechos y se
centra en el orden y la verificación.

## Protocolo

### Paso 0 — Clasificar el tipo de subject

No todos los subjects dan lo mismo de partida. Antes de leer nada más a fondo:

- **Tipo A — esqueleto dado** (Polyset, tree_bag, la mayoría de exámenes de rank corto): 42 ya
  define las clases/interfaces (`bag`, `array_bag`...). El trabajo es extenderlas. El grafo de
  compilación se construye directamente sobre los ficheros que ya existen — ir a Paso 1.
- **Tipo B — solo protocolo/especificación** (ft_irc, webserv, minishell, cub3d/miniRT,
  Transcendence): 42 define QUÉ debe hacer el programa (protocolo, comportamiento, restricciones
  técnicas) — no CÓMO estructurarlo en clases. Nadie te da `Server.hpp`. Antes del grafo de
  compilación hace falta un paso extra: **diseñar el modelo de dominio**.

Para Tipo B, insertar esto entre el Paso 1 y el Paso 3:

#### Paso 1.5 (solo Tipo B) — Categorizar los requisitos, luego derivar el modelo de dominio

Los subjects Tipo B mezclan tipos de requisito muy distintos en el mismo texto — sepáralos antes
de diseñar nada, porque cada tipo restringe el diseño de forma diferente:

1. **Reglas técnicas duras** (binarias, pass/fail — no son features, son restricciones de
   arquitectura): ej. "un solo poll() para todo", "prohibido fork", "nunca debe crashear". Estas
   determinan la FORMA del programa (ej. un único bucle de eventos) antes de pensar en clases.
2. **Funciones/librerías permitidas**: define qué herramientas del SO tienes — y, por omisión,
   qué NO puedes usar (sin lista de threads → nada de hilos; sin librerías externas → sin Boost).
3. **Requisitos funcionales** (lo que el usuario final puede hacer — normalmente en una lista de
   viñetas de "features"): estos SÍ son candidatos directos a clases/métodos.
4. **Entregables no-código**: Makefile con reglas específicas, README con secciones específicas.

Con eso separado, **derivar el modelo de dominio**: qué sustantivos del enunciado (y del dominio
que describe — protocolo, RFC, etc.) se convierten en clases candidatas, y qué reglas técnicas
duras se convierten en un componente arquitectónico central (ej. el bucle de eventos) en vez de
en una clase cualquiera. Este modelo de dominio recién diseñado es lo que alimenta el Paso 3 —
los nodos del grafo de compilación ya no vienen dados por 42, los decidiste aquí.

### Paso 1 — Leer el subject completo, extraer el contrato

Del `subject.txt` (o `subject.en.txt`) real del ejercicio:
- **Expected files**: qué ficheros hay que entregar.
- **Allowed functions/globals**: qué se puede usar — condiciona el diseño (ej. si no está
  `malloc` permitido, no se puede resolver como en C).
- **Qué se da ya hecho**: código base, clases abstractas, un `main.cpp` de test que debe
  compilar tal cual con el código del alumno.
- **Restricciones explícitas**: forma canónica ortodoxa, normas de estilo, prohibiciones.

### Paso 2 — Mapear lo dado vs. lo pedido

Tabla simple: fichero → dado por 42 (no se toca) / a implementar por el alumno. Esto evita el
error común de intentar modificar código base que el evaluador espera intacto.

### Paso 3 — Grafo de dependencias de COMPILACIÓN

No es el árbol de conceptos de Modo 1. Aquí el orden lo dicta qué necesita existir para que lo
siguiente compile o enlace — que puede no coincidir con qué es más fácil de entender primero.

Ejemplo de forma (genérico, no memorizar un caso concreto — se construye leyendo cada subject):
```
[clases base dadas por 42, ya existen]
        │
        ▼
[clases que heredan de las bases — necesitan que las bases ya compilen]
        │
        ▼
[clases que envuelven/componen las anteriores — necesitan que existan primero]
        │
        ▼
[verificar contra el main dado — no se toca, debe compilar y comportarse como se espera]
```

### Paso 4 — Por cada clase, en el orden del grafo

1. **Contrato**: qué métodos exige la clase abstracta/interfaz que hereda o implementa.
2. **Esqueleto**: firmas vacías primero — que compile antes de tener lógica.
3. **Implementación mínima**: la lógica real, una función a la vez.
4. **Verificación incremental**: compilar y probar esa pieza contra el main dado (o un test
   mínimo) antes de pasar a la siguiente clase del grafo.

### Paso 5 — Checkpoint por clase

Antes de avanzar a la siguiente clase, confirmar que la actual compila y pasa una prueba mínima.
Mismo principio que Modo 1: no saltar un prerequisito porque "parece que va a funcionar".

## Artefacto durable

Al completar el recorrido de un ejercicio, genero `reference/000N-plan-implementacion.html` (no
`lessons/`) — un mapa de implementación tipo cheat sheet: el grafo de compilación + checklist por
clase, pensado para re-consultarse al enfrentarse a un subject nuevo bajo presión de examen, no
para releer la explicación de un concepto. Sigue las reglas de diseño de
`lessons-format.md` (mismo `assets/style.css`, cargar `artifact-design` antes de escribirlo).
