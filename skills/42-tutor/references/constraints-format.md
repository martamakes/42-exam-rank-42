# CONSTRAINTS.md Format — 42-tutor

`CONSTRAINTS.md` vive en `workspace/<módulo>/CONSTRAINTS.md`. Captura las restricciones TÉCNICAS
del subject — lo que el enunciado prohíbe, permite o exige explícitamente — para que ni el tutor
ni el alumno propongan nunca algo que el evaluador rechazaría. Es memoria persistente: se escribe
la primera vez que se lee el subject (Modo 2, Paso 1) o en cuanto el usuario indica una
restricción en Modo 1, y se consulta SIEMPRE antes de sugerir una función, técnica o estructura.

## Cuándo se crea

- **Modo 2, Paso 1** (lectura del subject): se extraen las restricciones del enunciado real.
- **Modo 1, en cualquier momento**: si el usuario menciona una restricción de su subject ("no
  puedo usar STL aquí", "está prohibido malloc en este ejercicio"), se registra aunque no se esté
  en Modo 2.
- Se actualiza (nunca se sobrescribe sin más) si aparece una restricción nueva o se corrige una
  mal registrada — con nota de cuándo cambió.

## Estructura

```md
# Constraints: {Módulo}

## Funciones y librerías permitidas
- {función/librería} — {para qué, si el subject lo aclara}

## Prohibido explícitamente
- {función, técnica o patrón prohibido} — {cita o paráfrasis corta del subject}

## Reglas técnicas duras (arquitectura)
- {regla binaria pass/fail que determina la forma del programa, ej. "un solo poll() para todo"}

## Norma / estilo
- {Norminette, forma canónica ortodoxa, límites de líneas, lo que aplique}

## Entregables no-código
- {Makefile con reglas específicas, README exigido, etc.}

## Fuente
- {subject.txt / subject.en.txt / ruta o cómo se obtuvo, con fecha de lectura}
```

## Fichero común: `workspace/CONSTRAINTS-COMMON.md`

Si el mismo texto de restricción aparece IDÉNTICO en 2+ subjects (típicamente el capítulo
"Instrucciones generales" de 42: Norma, cero leaks, `-Wall -Werror -Wextra`, reglas del
Makefile, estructura del README, política de IA), se extrae UNA VEZ a
`workspace/CONSTRAINTS-COMMON.md` (mismo nivel que `NOTES.md`/`CONFIG.md`, no por módulo). El
`CONSTRAINTS.md` de cada módulo entonces:
- Empieza con `Ver también ../CONSTRAINTS-COMMON.md` en vez de repetir esas reglas.
- Solo lista lo ESPECÍFICO de ese subject (funciones permitidas propias, prohibiciones
  particulares, flags de compilación concretos, requisitos de README que difieran).
- Si un subject contradice algo del común (ej. "aquí NO se permite libft" cuando el común dice
  "si el subject lo permite..."), se anota como excepción en el `CONSTRAINTS.md` del módulo, no
  se toca el común.

## Reglas

- Solo restricciones VERIFICABLES contra el subject real — nunca una restricción "por si acaso" o
  inventada de memoria paramétrica. Si no está claro, se marca como `(sin confirmar — preguntar)`
  en vez de asumir.
- Se consulta ANTES de proponer un contrato de clase, una función o una técnica — tanto en Modo 1
  (checkpoints y ejemplos) como en Modo 2 (cada paso del grafo de compilación).
- No duplica lo que ya está en el subject completo — es un resumen operativo para consulta
  rápida, no una copia del enunciado.
- Corto y escaneable: si una sección no aplica al módulo, se omite, no se deja vacía.
