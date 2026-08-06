# ft_popen

## 📋 Subject (Auto-extraído)

```
Assignment name  : ft_popen
Expected files   : ft_popen.c
Allowed functions: pipe, fork, dup2, execvp, close, exit

Write the following function:

int ft_popen(const char *file, char *const argv[], char type);

The function must launch the executable file with the arguments argv (using execvp).
If type is 'r' the function must return a file descriptor connected to the output of the command.
If type is 'w' the function must return a file descriptor connected to the input of the command.
In case of error or invalid parameter the function must return -1.

⚠️ Hints del subject:
Do not leak file descriptors!
This exercise is inspired by the libc's popen().
```

## 💡 Tips Generales (Auto-generados)

**Conceptos clave detectados:**
- Pipes (comunicación entre procesos)
- Fork (crear proceso hijo)
- Redirección de file descriptors (dup2)
- Ejecución de comandos (execvp)

**Recursos útiles:**
- `man pipe` ⭐ (tiene función de ejemplo que te puede servir de chuleta)
- `man fork`
- `man dup2`
- `man execvp`

**Complejidad:** Alta (manejo de procesos + file descriptors)

**Allowed functions:** pipe, fork, dup2, execvp, close, exit

## 🎯 Enfoque Sugerido (7 Niveles de Pistas)

### Nivel 1: Pregunta Diagnóstica
- ¿Qué hace exactamente popen en C estándar?
- ¿Por qué necesitamos un proceso hijo?
- ¿Cuál es la diferencia entre type='r' y type='w'?
- ¿Qué significa "conectado al output" vs "conectado al input"?

### Nivel 2: Pregunta Más Específica
- ¿Cómo comunicarías datos entre proceso padre e hijo?
- Si type='r', ¿quién escribe y quién lee?
- Si type='w', ¿quién escribe y quién lee?
- ¿Qué file descriptor necesitas retornar en cada caso?

### Nivel 3: Concepto Clave
```
PIPE: Crea un "tubo" de comunicación
pipe() crea dos FDs:
- fds[0] = extremo de LECTURA
- fds[1] = extremo de ESCRITURA

type='r': Padre LEE lo que hijo ESCRIBE
  → Hijo redirige su STDOUT al pipe
  → Padre retorna el fd de lectura

type='w': Padre ESCRIBE, hijo LEE
  → Hijo redirige su STDIN al pipe
  → Padre retorna el fd de escritura

El proceso hijo ejecuta el comando (ls, grep, etc.)
El proceso padre se queda con el fd para leer/escribir
```

### Nivel 4: Estrategia
```
1. Validar parámetros (type debe ser 'r' o 'w')
2. Crear pipe con pipe(fds)
3. Hacer fork() para crear proceso hijo
4. En el proceso HIJO (pid == 0):
   a) Redirigir STDOUT (si type='r') o STDIN (si type='w')
   b) Cerrar file descriptors no usados
   c) Ejecutar comando con execvp(file, argv)
5. En el proceso PADRE (pid != 0):
   a) Cerrar file descriptor no usado
   b) Retornar el file descriptor útil
6. Manejar errores en cada paso (pipe, fork, dup2)
```

### Nivel 5: Pseudocódigo + Referencia Man Page
```
⭐ LEE `man pipe` - tiene un ejemplo completo que te sirve de base

int ft_popen(const char *file, char *const argv[], char type) {
    int fds[2];  // fds[0]=lectura, fds[1]=escritura
    pid_t pid;

    // 1. Validar type
    if (type != 'r' && type != 'w')
        return -1;

    // 2. Crear pipe
    if (pipe(fds) == -1)
        return -1;

    // 3. Hacer fork
    pid = fork();
    if (pid == -1)
        return -1;

    if (pid == 0) {
        // PROCESO HIJO: ejecutar comando
        if (type == 'r') {
            // El hijo escribe en el pipe (su stdout va al pipe)
            dup2(fds[1], STDOUT_FILENO);
            close(fds[0]);  // No necesita leer
        } else {
            // El hijo lee del pipe (su stdin viene del pipe)
            dup2(fds[0], STDIN_FILENO);
            close(fds[1]);  // No necesita escribir
        }
        close(fds[...]);  // Cerrar el otro también
        execvp(file, argv);
        exit(1);  // Si execvp falla
    }

    // PROCESO PADRE: retornar fd apropiado
    if (type == 'r') {
        close(fds[1]);  // No va a escribir
        return fds[0];  // Retorna fd de lectura
    } else {
        close(fds[0]);  // No va a leer
        return fds[1];  // Retorna fd de escritura
    }
}
```

### Nivel 6: Código Parcial con Detalles
```c
#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type) {
    int fds[2];
    pid_t pid;

    // Validación de tipo
    if (type != 'r' && type != 'w')
        return -1;

    // Crear pipe
    if (pipe(fds) == -1)
        return -1;

    // Crear proceso hijo
    pid = fork();
    if (pid == -1) {
        close(fds[0]);
        close(fds[1]);
        return -1;
    }

    if (pid == 0) {
        // HIJO: Tu código aquí
        // 1. dup2 según type
        // 2. close ambos fds
        // 3. execvp
        // 4. exit si execvp falla
    }

    // PADRE: Tu código aquí
    // 1. close fd no usado
    // 2. return fd útil
}
```

### Nivel 7: Análisis de Edge Cases
```
⚠️ Errores MUY comunes:

1. LEAK DE FILE DESCRIPTORS
   ✗ No cerrar fds[0] y fds[1] en el hijo
   ✓ Cerrar AMBOS después de dup2

2. CONFUNDIR fds[0] y fds[1]
   ✗ type='r' con dup2(fds[0], STDOUT)
   ✓ type='r' con dup2(fds[1], STDOUT) → hijo ESCRIBE

3. NO VALIDAR type
   ✗ Asumir que type siempre es válido
   ✓ Return -1 si type != 'r' && type != 'w'

4. NO VERIFICAR ERRORES
   ✗ No verificar return de pipe(), fork()
   ✓ Return -1 en cada caso de error

5. CERRAR FDs EN ORDEN INCORRECTO
   ✗ Cerrar antes de dup2
   ✓ dup2 primero, luego close

6. NO HACER EXIT EN EL HIJO
   ✗ Si execvp falla, hijo continúa ejecutando
   ✓ exit(1) después de execvp

✅ CHECKLIST COMPLETO:

Validación:
- [ ] Verificar type == 'r' || type == 'w'

Creación pipe:
- [ ] Verificar return de pipe() != -1

Fork:
- [ ] Verificar return de fork() != -1
- [ ] Cerrar ambos fds si fork falla

Hijo (type='r'):
- [ ] dup2(fds[1], STDOUT_FILENO)
- [ ] close(fds[0])
- [ ] close(fds[1])
- [ ] execvp(file, argv)
- [ ] exit(1) si execvp falla

Hijo (type='w'):
- [ ] dup2(fds[0], STDIN_FILENO)
- [ ] close(fds[1])
- [ ] close(fds[0])
- [ ] execvp(file, argv)
- [ ] exit(1) si execvp falla

Padre (type='r'):
- [ ] close(fds[1])
- [ ] return fds[0]

Padre (type='w'):
- [ ] close(fds[0])
- [ ] return fds[1]

🔍 DEBUGGING:
- Si el programa se cuelga → Probablemente leak de FDs
- Si lee/escribe basura → Confundiste fds[0] y fds[1]
- Si "command not found" → execvp no encuentra el comando
- Si norminette falla → Revisa longitud de línea
```

## 🧪 Casos de Prueba del Subject

El subject muestra dos ejemplos de uso:

### Ejemplo 1: Leer output de comando
```c
int main() {
    int  fd;
    char *line;

    fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    while ((line = get_next_line(fd)))
        ft_putstr(line);
    return (0);
}
```

**¿Qué prueba este caso?**
- type='r' (lectura del output)
- Comando simple: `ls`
- Leer línea por línea con get_next_line
- El padre LEE lo que el hijo ESCRIBE

### Ejemplo 2: Pipeline con grep
```c
int main() {
    int  fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    dup2(fd, 0);  // Conecta output de ls al stdin
    fd = ft_popen("grep", (char *const []){"grep", "c", NULL}, 'r');
    char *line;
    while ((line = get_next_line(fd)))
        printf("%s", line);
}
```

**¿Qué prueba este caso?**
- Encadenar dos ft_popen (ls | grep)
- dup2 para redirigir output de uno al stdin del otro
- type='r' en ambos casos

### Tu Main de Test - Guía Socrática

**Cuando digas "ayúdame con el main", te preguntaré:**

1. ¿Qué casos del subject necesitas probar?
2. ¿Tienes get_next_line implementado? (lo necesitas para leer)
3. ¿Cómo verificarás que funciona type='r'?
4. ¿Necesitas probar type='w'?
5. ¿Qué comando simple usarás para probar? (ls, echo, cat...)

**NO necesitas probar:**
- Todos los edge cases imaginables
- NULL pointers (el subject no lo menciona)
- Comandos complejos

**SÍ necesitas probar:**
- Los casos que están en el subject
- Que el fd retornado funcione
- Que puedas leer/escribir según type

## 📚 Recursos Adicionales

### Man Pages Críticas

**`man pipe` (⭐ MUY IMPORTANTE)**
```bash
man pipe
# Busca la sección EXAMPLES
# Tiene una función completa que te sirve de referencia
```

**`man dup2`**
- Entiende qué hace: "duplica" un FD en otro número
- `dup2(old_fd, new_fd)` → new_fd apunta a donde apuntaba old_fd

**`man fork`**
- Return value: 0 en hijo, PID en padre, -1 en error

**`man execvp`**
- Diferencia entre exec, execl, execv, execvp
- execvp busca en PATH (por eso funciona "ls", "grep")

### Conceptos a Dominar

1. **File Descriptors**
   - 0 = STDIN
   - 1 = STDOUT
   - 2 = STDERR
   - Los demás son FDs abiertos

2. **Pipe**
   - Unidireccional (datos van en un solo sentido)
   - Tiene dos extremos: lectura y escritura
   - Si cierras todos los extremos de escritura, read() retorna 0 (EOF)

3. **Fork**
   - Crea copia exacta del proceso
   - Hijo y padre son independientes
   - Hereda FDs abiertos (¡por eso hay que cerrar!)

4. **dup2**
   - Cambia a dónde apunta un FD
   - Útil para redirigir stdin/stdout/stderr

## 👤 Tips de Usuarios

### @marta (2026-02-08)
[Esperando tu primer tip personal]

[Otros usuarios añadirán sus tips aquí sin borrar los tuyos]
