# Ejemplo: "Ayúdame con el main"

## 🎯 El Problema

Uno de los mayores dolores de cabeza en los exámenes de 42 es **crear el main de test** cuando:
- No te dan uno
- El que te dan hay que modificarlo
- No sabes qué casos probar

## ✨ Solución: Modo Tutor para Mains

Con `/estudio42`, el comando `"ayúdame con el main"` activa el **modo tutor socrático para tests**.

### Filosofía

❌ **NO hace:** Generar el main automáticamente
✅ **SÍ hace:** Guiarte con preguntas para que TÚ lo escribas

**Razón:** Aprendes más escribiéndolo tú mismo con guía.

---

## 📝 Ejemplo Real: ft_popen

### Contexto

El subject de ft_popen muestra dos ejemplos de uso, pero no da un main completo para compilar y probar.

### Conversación con el Tutor

```
Tú: ayúdame con el main

Claude: Veo que el subject de ft_popen tiene dos ejemplos.

        ¿Cuál de los dos quieres implementar primero?
        1. El ejemplo simple con ls
        2. El ejemplo con ls | grep

Tú: el simple primero

Claude: Perfecto. Miremos el ejemplo:

        fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
        while ((line = get_next_line(fd)))
            ft_putstr(line);

        ¿Qué necesitas tener implementado para que este main funcione?

Tú: necesito get_next_line y ft_putstr

Claude: Exacto. Tienes esas funciones disponibles?

Tú: tengo get_next_line de otro ejercicio

Claude: Bien. Para ft_putstr, ¿cómo la implementarías rápido?
        (Es solo para testing, no necesita ser perfecta)

Tú: un while con write

Claude: Perfecto. Ahora empecemos con el main.

        ¿Qué prototipo necesitas?
        int main(void) o int main(int argc, char **argv)?

Tú: int main(void) es suficiente

Claude: Ok. ¿Qué variables necesitas declarar?

Tú: int fd y char *line

Claude: Muy bien. Ahora escribe la estructura básica:
        - Declarar variables
        - Llamar ft_popen
        - Loop con get_next_line
        - Escribir cada línea

        Avísame cuando lo tengas y lo revisamos.

[Tú escribes el código]

Tú: listo, revisa mi código

Claude: [Lee tu código]

        Muy bien. Veo que:
        ✓ Declaras fd y line
        ✓ Llamas ft_popen con los argumentos correctos
        ✓ Loop mientras get_next_line retorna línea

        ¿Has probado compilarlo?

Tú: sí, compila pero no muestra nada

Claude: Mmm. ¿Verificaste que fd != -1 después de ft_popen?
        Si ft_popen falla, retorna -1 y get_next_line no funcionará.

Tú: ah, no lo verificaba

Claude: Añade una verificación. ¿Cómo harías un check básico?

Tú: if (fd == -1) return 1;

Claude: Perfecto. Prueba ahora.

[Funciona]

Tú: funciona! ahora quiero probar el segundo ejemplo

Claude: Excelente. El segundo ejemplo es más complejo:

        fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
        dup2(fd, 0);
        fd = ft_popen("grep", (char *const []){"grep", "c", NULL}, 'r');

        ¿Entiendes qué hace el dup2(fd, 0)?

Tú: redirige el fd al stdin?

Claude: Exacto. El output de ls va al stdin del proceso actual.
        Luego grep lee de stdin (que ahora tiene el output de ls).

        ¿Quieres añadir esto a tu main o crear un main2.c separado?
```

---

## 📝 Ejemplo Real: picoshell

### Contexto

El subject menciona: "you will find in this directory a file main.c"

### Conversación con el Tutor

```
Tú: ayúdame con el main

Claude: El subject dice que debe haber un main.c en el directorio.

        ¿Lo encontraste?

Tú: sí, está aquí

Claude: Perfecto. ¿Qué hace ese main?
        (Léelo y dime qué entiendes)

Tú: convierte los argumentos en un array cmds[]

Claude: Exacto. Parsea argv y separa por "|".

        Ese main ya está hecho para ti. Lo único que necesitas
        es compilar tu picoshell.c con ese main.c

        ¿Has probado compilar?

Tú: no, cómo lo compilo?

Claude: gcc picoshell.c main.c -o picoshell

        Luego prueba con los ejemplos del subject:
        ./picoshell /bin/ls "|" /usr/bin/grep picoshell

Tú: compila pero da segfault

Claude: Hmm. Antes de ejecutar, verifiquemos tu código.

        Cuando picoshell() recibe cmds[], ¿qué es cmds[0]?

Tú: el primer comando

Claude: Sí, pero específicamente, ¿qué tipo de dato?
        cmds es char **cmds[], así que cmds[0] es...?

Tú: un char **

Claude: Exacto. Es un array de strings (argv del comando).

        ¿Cómo iteras sobre todos los comandos?

[Continúa la conversación guiando paso a paso]
```

---

## 🔑 Puntos Clave

### Lo que el tutor hace:

1. **Identifica los ejemplos del subject** - Solo prueba lo que pide el enunciado
2. **Pregunta qué tienes** - get_next_line? funciones auxiliares?
3. **Guía la estructura** - ¿Qué variables? ¿Qué prototipo?
4. **Paso a paso** - No da todo de golpe, va preguntando
5. **Verifica errores** - ¿Compiló? ¿Funciona? ¿Qué output da?
6. **Explica conceptos** - Si no entiendes algo, explica sin dar código

### Lo que el tutor NO hace:

❌ Generar el main completo
❌ Inventar edge cases que no están en el subject
❌ Darte el código sin que entiendas
❌ Modificar tu código directamente (salvo que digas "escríbelo tú")

---

## 💡 Tips para Usar "Ayúdame con el main"

### 1. Lee el subject primero
```
Antes de pedir ayuda con el main, lee el subject completo.
Identifica la sección "Examples:" o casos de prueba.
```

### 2. Busca main.c de ejemplo
```
Algunos ejercicios incluyen main.c en el directorio.
Mira si existe antes de crear uno desde cero.
```

### 3. Solo prueba lo del subject
```
No te compliques con edge cases que no se mencionan.
Si el subject muestra 2 ejemplos, prueba esos 2.
```

### 4. Usa funciones auxiliares básicas
```
Si necesitas ft_putstr para testing, no hace falta que sea perfecta.
Un while con write(1, &str[i++], 1) es suficiente.
```

### 5. Compila paso a paso
```
No escribas todo el main de golpe.
Escribe, compila, prueba, itera.
```

---

## 🚀 Empieza Ahora

```bash
cd tu-ejercicio/
/estudio42

# Cuando estés listo:
> ayúdame con el main

# El tutor te guiará paso a paso
```

---

## 🎓 Filosofía

> "Dame un main y probaré mi código una vez.
> Enséñame a crear mains y probaré código toda mi vida."

El objetivo no es que tengas el main rápido, sino que **aprendas a crear tus propios tests** para cualquier ejercicio futuro.

Por eso el tutor **pregunta** en lugar de **dar**.

---

**¿Listo para probarlo?** 🚀

```
/estudio42
```
