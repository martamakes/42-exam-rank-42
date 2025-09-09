# Microshell

## Descripción
Escribe un programa que se comporte como la ejecución de un comando de shell.

## Archivos esperados
- `microshell.c`

## Funciones permitidas
- malloc, free, write, close, fork, waitpid, signal, kill, exit, chdir, execve, dup, dup2, pipe, strcmp, strncmp

## Requisitos

- La línea de comando a ejecutar será los argumentos de este programa
- La ruta del ejecutable será absoluta o relativa, pero tu programa no debe construir una ruta (desde la variable PATH, por ejemplo)
- Debes implementar `|` y `;` como en bash
  - Nunca probaremos un `|` inmediatamente seguido o precedido por nada o por `|` o `;`
- Tu programa debe implementar el comando integrado `cd` solo con una ruta como argumento (sin `-` o sin parámetros)
  - Si `cd` tiene un número incorrecto de argumentos, tu programa debe imprimir en STDERR `error: cd: bad arguments` seguido de un `\n`
  - Si `cd` falló, tu programa debe imprimir en STDERR `error: cd: cannot change directory to path_to_change` seguido de un `\n`, con path_to_change reemplazado por el argumento de cd
  - Un comando `cd` nunca estará inmediatamente seguido o precedido por un `|`
- No necesitas gestionar ningún tipo de comodín (`*`, `~`, etc...)
- No necesitas gestionar variables de entorno (`$BLA` ...)
- Si una llamada al sistema, excepto execve y chdir, devuelve un error, tu programa debe imprimir inmediatamente `error: fatal` en STDERR seguido de un `\n` y el programa debe salir
- Si execve falló, debes imprimir `error: cannot execute executable_that_failed` en STDERR seguido de un `\n` con executable_that_failed reemplazado por la ruta del ejecutable fallido (Debe ser el primer argumento de execve)
- Tu programa debe ser capaz de gestionar más de cientos de `|` incluso si limitamos el número de "archivos abiertos" a menos de 30.

## Ejemplo

```bash
$>./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
microshell
i love my microshell
$>
```

## Pistas
- No olvides pasar la variable de entorno a execve
- ¡No filtres descriptores de archivo!
