# GLOSSARY.md Format — 42-tutor

`GLOSSARY.md` vive en `workspace/<módulo>/GLOSSARY.md`. Es el lenguaje canónico de ese módulo —
toda explicación y lección se adhiere a su terminología una vez existe.

## Estructura

```md
# {Módulo} Glossary

## Terms

**protected**:
Definición propia, ajustada, en 1-2 frases. Define QUÉ ES, no cómo se usa ni cómo se implementa.
_Avoid_: alias sueltos a evitar ("medio privado", etc.)
```

## Reglas

- Un término entra **solo** cuando el usuario lo ha demostrado en un checkpoint o pregunta de
  evaluador — nunca solo por haber sido cubierto en una explicación (ver disciplina de
  `learning-record-format.md`).
- Definiciones tight: 1-2 frases, no un ensayo.
- Si hay varias palabras para lo mismo, elijo una y las demás quedan listadas como `_Avoid_` —
  así es como el lenguaje se comprime.
- Uso los propios términos del glosario dentro de otras definiciones, una vez existen — hace que
  los conceptos complejos siguientes sean más fáciles de agarrar.
- Se revisa según se profundiza el entendimiento — se actualiza in place, no se deja obsoleta.
- Agrupar bajo subtítulos (`## Herencia`, `## Memoria`) si aparecen clusters naturales; una
  lista plana está bien si los términos cohesionan sin ellos.
