# RESOURCES.md Format — 42-tutor

`RESOURCES.md` vive en `workspace/<módulo>/RESOURCES.md`. El conocimiento de las lecciones sale
de aquí, no de memoria paramétrica cuando existe una fuente citable.

## Estructura

```md
# {Módulo} Resources

## Knowledge

- [cppreference: Access specifiers](https://en.cppreference.com/w/cpp/language/access)
  Qué cubre y cuándo usarlo, en una línea.
- Enunciado real: `ruta/al/subject.txt` (este repo)
  Qué cubre y cuándo usarlo.

## Gaps

- {Tema para el que no hay fuente citable todavía — dispara búsqueda futura}
```

## Reglas

- Solo fuentes de alta confianza: documentación oficial (cppreference, RFCs, man pages), el
  propio `subject.txt`/`GUIA.md` del repo, la Norminette. Nada de marketing disfrazado de docs.
- Cada entrada lleva una línea de uso — un enlace pelado no sirve dentro de tres meses.
- **Gaps explícitos**: si no hay buena fuente para algo que el módulo necesita, se anota — no se
  rellena con memoria paramétrica sin avisar que lo estoy haciendo.
- Poda: una fuente que resultó floja o poco fiable se borra, no se deja enterrada.
