# picoshell

## 📋 Subject (Auto-extraído)

```
Assignment name: picoshell
Expected files: picoshell.c
Allowed functions: close, fork, wait, exit, execvp, dup2, pipe

Write the following function:

int picoshell(char **cmds[]);

The goal of this function is to execute a pipeline. It must execute each
commands of cmds and connect the output of one to the input of the
next command (just like a shell).

Cmds contains a null-terminated list of valid commands. Each row
of cmds is an argv array directly usable for a call to execvp.

If any error occur, The function must return 1 (you must close all open fds).
Otherwise wait all child processes and return 0.

Examples:
./picoshell /bin/ls "|" /usr/bin/grep picoshell
  → picoshell
./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
  → squblblb
```

## 💡 Tips Generales (Auto-generados)

**Conceptos clave detectados:**
- Pipeline (múltiples pipes conectados)
- Fork múltiple (un proceso hijo por comando)
- Redirección encadenada (output → input)
- Wait (esperar a todos los hijos)
- Manejo de file descriptors múltiples

**Recursos útiles:**
- `man pipe` ⭐
- `man fork`
- `man dup2` ⭐⭐ (clave para entender redirección)
- `man wait` / `man waitpid`
- `man execvp`

**Complejidad:** Alta (pipeline completo, múltiples procesos)

**Allowed functions:** close, fork, wait, exit, execvp, dup2, pipe

## 🎯 Enfoque Sugerido (7 Niveles de Pistas)

### Nivel 1: Pregunta Diagnóstica
- ¿Qué hace un pipeline en shell? (ej: `ls | grep pic | wc -l`)
- ¿Cuántos procesos hijos necesitas? (uno por comando)
- ¿Cómo se conectan los comandos entre sí?
- ¿Qué debe recibir cada comando como stdin y stdout?

### Nivel 2: Pregunta Más Específica
- Si tienes 3 comandos (`ls | grep | wc`), ¿cuántos pipes necesitas?
- ¿Qué proceso debe esperar a los demás?
- ¿En qué orden debes crear los procesos?
- ¿Qué FDs debe cerrar cada proceso hijo?

### Nivel 3: Concepto Clave
```
PIPELINE = Cadena de procesos conectados por pipes

Ejemplo: cmd1 | cmd2 | cmd3

Proceso padre crea:
- pipe1[0,1] → conecta cmd1 y cmd2
- pipe2[0,1] → conecta cmd2 y cmd3

Hijo 1 (cmd1):
  stdout → pipe1[1] (escribe)
  (lee de stdin normal)

Hijo 2 (cmd2):
  stdin  → pipe1[0] (lee de cmd1)
  stdout → pipe2[1] (escribe)

Hijo 3 (cmd3):
  stdin  → pipe2[0] (lee de cmd2)
  (escribe a stdout normal)

CLAVE: Cada hijo cierra TODOS los FDs que no usa
       (incluyendo pipes de otros comandos!)
```

### Nivel 4: Estrategia
```
1. Contar comandos en cmds[]
2. Crear array de pipes (N-1 pipes para N comandos)
3. Para cada comando:
   a) Hacer fork()
   b) En el hijo:
      - Si no es el primero: dup2(pipe_anterior[0], STDIN)
      - Si no es el último: dup2(pipe_actual[1], STDOUT)
      - Cerrar TODOS los file descriptors de pipes
      - execvp(cmd[0], cmd)
   c) En el padre: continuar al siguiente comando
4. Padre cierra TODOS los pipes
5. Padre hace wait() por cada hijo
6. Return 0 si todo OK, 1 si error
```

### Nivel 5: Pseudocódigo
```
int picoshell(char **cmds[]) {
    int num_cmds = contar_comandos(cmds);
    int pipes[num_cmds - 1][2];  // N-1 pipes
    pid_t pids[num_cmds];

    // Crear todos los pipes
    for (i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) == -1)
            return 1;
    }

    // Crear proceso por cada comando
    for (i = 0; i < num_cmds; i++) {
        pids[i] = fork();
        if (pids[i] == -1)
            return limpiar_y_return_1();

        if (pids[i] == 0) {
            // HIJO
            // Si no es el primero: dup2(pipes[i-1][0], STDIN)
            // Si no es el último:  dup2(pipes[i][1], STDOUT)

            // Cerrar TODOS los pipes (bucle)
            for (j = 0; j < num_cmds - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
    }

    // PADRE: Cerrar todos los pipes
    for (i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Esperar a todos los hijos
    for (i = 0; i < num_cmds; i++) {
        wait(NULL);
    }

    return 0;
}
```

### Nivel 6: Código Parcial
```c
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int count_cmds(char **cmds[]) {
    int i = 0;
    while (cmds[i])
        i++;
    return i;
}

int picoshell(char **cmds[]) {
    int num_cmds = count_cmds(cmds);
    int pipes[num_cmds - 1][2];
    pid_t pid;
    int i, j;

    // Crear pipes
    for (i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) == -1)
            return 1;  // TODO: cerrar pipes previos
    }

    // Fork para cada comando
    for (i = 0; i < num_cmds; i++) {
        pid = fork();
        if (pid == -1)
            return 1;  // TODO: cleanup

        if (pid == 0) {
            // HIJO: tu código aquí
            // 1. Redirigir stdin si no es primero
            // 2. Redirigir stdout si no es último
            // 3. Cerrar TODOS los pipes
            // 4. execvp
            // 5. exit(1)
        }
    }

    // PADRE: tu código aquí
    // 1. Cerrar todos los pipes
    // 2. Wait por todos los hijos
    // 3. Return 0

    return 0;
}
```

### Nivel 7: Análisis de Edge Cases
```
⚠️ Errores MUY comunes:

1. NO CERRAR TODOS LOS PIPES EN LOS HIJOS
   ✗ Solo cerrar el pipe actual
   ✓ Cerrar TODOS los pipes (bucle con close)
   → Si no cierras todos: programas se quedan colgados esperando EOF

2. CERRAR PIPES ANTES DE DUP2
   ✗ close(pipes[i][1]); dup2(pipes[i][1], STDOUT);
   ✓ dup2 primero, close después

3. NO CERRAR PIPES EN EL PADRE
   ✗ Solo cerrar en hijos
   ✓ Padre también debe cerrar (si no: leak + procesos bloqueados)

4. CONFUNDIR ÍNDICES DE PIPES
   ✗ Comando i usa pipes[i][0] y pipes[i][1]
   ✓ Comando i usa pipes[i-1][0] (entrada) y pipes[i][1] (salida)

   Ejemplo con 3 comandos:
   - cmd[0]: escribe a pipes[0][1]
   - cmd[1]: lee de pipes[0][0], escribe a pipes[1][1]
   - cmd[2]: lee de pipes[1][0]

5. NO MANEJAR PRIMER/ÚLTIMO COMANDO
   ✗ Redirigir stdin del primero (no debe)
   ✗ Redirigir stdout del último (no debe)
   ✓ Primer comando lee stdin normal
   ✓ Último comando escribe stdout normal

6. NO HACER WAIT POR TODOS
   ✗ Solo un wait() cuando hay 3 comandos
   ✓ wait() en bucle (num_cmds veces)

7. LEAK EN CASO DE ERROR
   ✗ return 1 sin cerrar pipes ya creados
   ✓ Cerrar pipes antes de retornar

✅ CHECKLIST DETALLADO:

Fase 1: Setup
- [ ] Contar comandos correctamente
- [ ] Crear array de pipes (size = num_cmds - 1)
- [ ] Crear todos los pipes con verificación de errores

Fase 2: Fork y redirección
Para cada comando:
- [ ] fork() con verificación de error
- [ ] Si pid == 0 (HIJO):
  - [ ] Si i > 0: dup2(pipes[i-1][0], STDIN_FILENO)
  - [ ] Si i < num_cmds-1: dup2(pipes[i][1], STDOUT_FILENO)
  - [ ] Bucle: cerrar TODOS pipes[j][0] y pipes[j][1]
  - [ ] execvp(cmds[i][0], cmds[i])
  - [ ] exit(1) si execvp falla

Fase 3: Padre limpia
- [ ] Bucle: cerrar todos pipes[i][0] y pipes[i][1]
- [ ] Bucle: wait(NULL) num_cmds veces
- [ ] return 0

Manejo de errores:
- [ ] Si pipe() falla: cerrar pipes previos, return 1
- [ ] Si fork() falla: cerrar todos pipes, return 1
- [ ] Cerrar FDs antes de cada return 1

🔍 DEBUGGING:

Problema: "Programa se cuelga"
→ Probablemente no estás cerrando todos los pipes
→ Algún proceso espera leer pero nadie cierra el extremo de escritura

Problema: "Primer comando no ejecuta"
→ Revisa que no estés redirigiendo su stdin

Problema: "Último comando no muestra output"
→ Revisa que no estés redirigiendo su stdout a un pipe

Problema: "Norminette: 25 lines"
→ Separa en funciones auxiliares (count_cmds, close_all_pipes, etc.)

Problema: "Leak de FDs"
→ Usa strace o lsof para ver qué FDs quedan abiertos
→ Asegúrate de cerrar en padre E hijos
```

## 🧪 Casos de Prueba del Subject

El subject muestra ejemplos específicos que DEBEN funcionar:

### Ejemplo 1: Pipeline simple (2 comandos)
```bash
./picoshell /bin/ls "|" /usr/bin/grep picoshell
# Output esperado: picoshell
```

**¿Qué prueba?**
- 2 comandos: ls | grep
- Output de ls va al input de grep
- Rutas completas (/bin/ls, /usr/bin/grep)

### Ejemplo 2: Pipeline de 3 comandos
```bash
./picoshell echo 'squalala' "|" cat "|" sed 's/a/b/g'
# Output esperado: squblblb
```

**¿Qué prueba?**
- 3 comandos encadenados: echo | cat | sed
- Transformación de datos a través del pipe
- El sed debe recibir correctamente el output

### Tu Main de Test - Guía Socrática

**El subject dice: "you will find in this directory a file main.c"**

1. **Primero:** Busca si hay main.c en el directorio del ejercicio
2. **Si existe:** Úsalo como base (ya convierte argv en cmds[])
3. **Si no existe:** Te guiaré para crearlo

**Cuando digas "ayúdame con el main", te preguntaré:**

1. ¿Encontraste el main.c de ejemplo?
2. ¿Entiendes cómo convierte argv en cmds[]?
3. ¿Qué casos del subject necesitas probar? (los 2 ejemplos de arriba)
4. ¿Cómo parsea el "|" para separar comandos?

**NO necesitas probar:**
- Pipes con 10 comandos
- Comandos que no existen
- Todos los edge cases imaginables

**SÍ necesitas probar:**
- Los 2 ejemplos del subject (2 y 3 comandos)
- Que el output sea el esperado
- Que los pipes conecten correctamente

### Estructura del cmds[] según el subject

```c
// Para: ls | grep picoshell
char **cmds[] = {
    (char *[]){"/bin/ls", NULL},
    (char *[]){"grep", "picoshell", NULL},
    NULL
};
```

Cada comando es un array de strings (argv para execvp).
cmds[] es un array de estos arrays, terminado en NULL.

## 📚 Recursos Adicionales

### Visualización del Pipeline

```
cmd1 | cmd2 | cmd3

     pipes[0]        pipes[1]
    [0]  [1]        [0]  [1]
     ↓    ↑         ↓    ↑
   read write     read write

┌──────────┐     ┌──────────┐     ┌──────────┐
│   cmd1   │────▶│   cmd2   │────▶│   cmd3   │
└──────────┘     └──────────┘     └──────────┘
  stdout →        ← stdin →        ← stdin
             pipes[0][1]     pipes[1][1]
                   ↓             ↓
             pipes[0][0]     pipes[1][0]
```

### Orden de Operaciones en Hijo

```c
// SIEMPRE en este orden:
1. dup2(...)        // Redirigir primero
2. close(...)       // Cerrar después
3. execvp(...)      // Ejecutar al final
4. exit(1)          // Si execvp falla
```

### Test Manual

```bash
# Compila
gcc -Wall -Wextra -Werror picoshell.c -o picoshell

# Test simple (2 comandos)
./picoshell /bin/ls "|" /usr/bin/grep pic

# Test complejo (3 comandos)
./picoshell echo 'hello world' "|" cat "|" /usr/bin/wc -w

# Test con /bin/cat (tu implementación)
# Si se cuelga → no estás cerrando pipes
```

### Debugging con printf

```c
// En el hijo, ANTES de execvp:
fprintf(stderr, "Hijo %d ejecutando: %s\n", i, cmds[i][0]);

// En el padre, después de fork:
fprintf(stderr, "Creé hijo %d con PID %d\n", i, pid);
```

## 👤 Tips de Usuarios

### @marta (2026-02-08)
[Esperando tu primer tip personal]

[Otros usuarios añadirán sus tips aquí sin borrar los tuyos]
