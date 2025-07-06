/*
 * EJERCICIO: FT_POPEN
 * 
 * DESCRIPCIÓN:
 * Implementar una versión simplificada de popen() que ejecute comandos
 * y retorne un file descriptor conectado a su entrada o salida.
 * 
 * CONCEPTOS CLAVE:
 * 1. PIPES: Comunicación entre procesos
 * 2. FORK: Crear proceso hijo
 * 3. EXEC: Reemplazar imagen del proceso
 * 4. DUP2: Redirigir file descriptors
 * 5. FILE DESCRIPTORS: Gestión de descriptores de archivo
 * 
 * ALGORITMO:
 * 1. Crear pipe para comunicación
 * 2. Fork para crear proceso hijo
 * 3. En hijo: redirigir stdin/stdout y ejecutar comando
 * 4. En padre: retornar extremo apropiado del pipe
 */

#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    /*
     * PARÁMETROS:
     * - file: Nombre del ejecutable a lanzar
     * - argv: Array de argumentos (estilo execvp)
     * - type: 'r' para leer del comando, 'w' para escribir al comando
     * 
     * RETORNO:
     * - File descriptor conectado al comando
     * - -1 en caso de error
     */
    
    int fds[2];  // Array para los extremos del pipe
    pid_t pid;
    
    // Validar parámetros
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;
    
    /*
     * CREAR PIPE:
     * - fds[0]: extremo de lectura (read end)
     * - fds[1]: extremo de escritura (write end)
     * 
     * El pipe permite comunicación unidireccional:
     * datos escritos en fds[1] pueden leerse desde fds[0]
     */
    if (pipe(fds) == -1)
        return -1;
    
    /*
     * FORK PROCESO:
     * - Crea copia exacta del proceso actual
     * - pid == 0: estamos en el proceso hijo
     * - pid > 0: estamos en el proceso padre
     * - pid == -1: error al hacer fork
     */
    pid = fork();
    if (pid == -1)
    {
        close(fds[0]);
        close(fds[1]);
        return -1;
    }
    
    if (pid == 0)  // PROCESO HIJO
    {
        /*
         * CONFIGURACIÓN DEL HIJO:
         * - Redirigir stdin o stdout según el tipo
         * - Cerrar extremos no necesarios del pipe
         * - Ejecutar el comando especificado
         */
        
        if (type == 'r')
        {
            /*
             * TIPO 'r': Queremos LEER del comando
             * - El comando debe escribir a su stdout
             * - Redirigir stdout del comando al write end del pipe
             * - El padre leerá desde el read end
             */
            if (dup2(fds[1], STDOUT_FILENO) == -1)
                exit(1);
        }
        else  // type == 'w'
        {
            /*
             * TIPO 'w': Queremos ESCRIBIR al comando
             * - El comando debe leer desde su stdin
             * - Redirigir stdin del comando al read end del pipe
             * - El padre escribirá al write end
             */
            if (dup2(fds[0], STDIN_FILENO) == -1)
                exit(1);
        }
        
        // Cerrar ambos extremos del pipe (ya redirigidos)
        close(fds[0]);
        close(fds[1]);
        
        /*
         * EJECUTAR COMANDO:
         * - execvp busca el ejecutable en PATH
         * - Reemplaza completamente la imagen del proceso
         * - Si execvp retorna, hubo un error
         */
        execvp(file, argv);
        exit(1);  // Solo se ejecuta si execvp falla
    }
    
    // PROCESO PADRE
    /*
     * CONFIGURACIÓN DEL PADRE:
     * - Cerrar el extremo del pipe que no necesita
     * - Retornar el extremo que usará para comunicarse
     */
    
    if (type == 'r')
    {
        /*
         * TIPO 'r': Padre quiere leer del comando
         * - Cerrar write end (lo usa el hijo)
         * - Retornar read end para que el padre pueda leer
         */
        close(fds[1]);
        return fds[0];
    }
    else  // type == 'w'
    {
        /*
         * TIPO 'w': Padre quiere escribir al comando
         * - Cerrar read end (lo usa el hijo)
         * - Retornar write end para que el padre pueda escribir
         */
        close(fds[0]);
        return fds[1];
    }
}

/*
 * EJEMPLO DE USO:
 * 
 * // Leer output de un comando
 * int main()
 * {
 *     int fd;
 *     char buffer[1024];
 *     ssize_t bytes;
 * 
 *     fd = ft_popen("ls", (char *const []){"ls", "-l", NULL}, 'r');
 *     if (fd == -1)
 *         return 1;
 * 
 *     while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0)
 *     {
 *         buffer[bytes] = '\0';
 *         printf("%s", buffer);
 *     }
 * 
 *     close(fd);
 *     return 0;
 * }
 * 
 * // Escribir input a un comando
 * int main()
 * {
 *     int fd;
 *     
 *     fd = ft_popen("wc", (char *const []){"wc", "-l", NULL}, 'w');
 *     if (fd == -1)
 *         return 1;
 * 
 *     write(fd, "line 1\n", 7);
 *     write(fd, "line 2\n", 7);
 *     write(fd, "line 3\n", 7);
 * 
 *     close(fd);  // wc mostrará "3"
 *     return 0;
 * }
 */

/*
 * DIAGRAMA DE COMUNICACIÓN:
 * 
 * TIPO 'r' (leer del comando):
 * 
 * PADRE                    PIPE                    HIJO
 * ------                  ------                  ------
 *                     fds[0] ←------ fds[1]
 * read(fd) ←----------- |              | ←------ stdout
 *                      |              |         (comando)
 * fd = fds[0]          |              |
 * close(fds[1])        |              |         close(fds[0])
 *                      |              |         close(fds[1])
 *                      |              |         dup2(fds[1], STDOUT)
 * 
 * TIPO 'w' (escribir al comando):
 * 
 * PADRE                    PIPE                    HIJO  
 * ------                  ------                  ------
 *                     fds[0] ------→ fds[1]
 * write(fd) ----------→ |              | ------→ stdin
 *                      |              |         (comando)
 * fd = fds[1]          |              |
 * close(fds[0])        |              |         close(fds[0])
 *                      |              |         close(fds[1])
 *                      |              |         dup2(fds[0], STDIN)
 */

/*
 * GESTIÓN DE ERRORES Y RECURSOS:
 * 
 * 1. VALIDACIÓN DE PARÁMETROS:
 *    - Verificar que file y argv no sean NULL
 *    - Verificar que type sea 'r' o 'w'
 * 
 * 2. MANEJO DE ERRORES DE SISTEMA:
 *    - pipe() puede fallar (no hay descriptores disponibles)
 *    - fork() puede fallar (no hay memoria/procesos)
 *    - dup2() puede fallar (descriptor inválido)
 *    - execvp() puede fallar (comando no encontrado)
 * 
 * 3. GESTIÓN DE FILE DESCRIPTORS:
 *    - CRUCIAL cerrar descriptores no necesarios
 *    - Evitar leaks de descriptores
 *    - Cerrar en orden correcto para evitar deadlocks
 * 
 * 4. PROCESOS ZOMBIE:
 *    - Esta implementación no maneja wait()
 *    - En implementación completa, necesitaríamos ft_pclose()
 *    - Almacenar PIDs para hacer wait() posteriormente
 */

/*
 * DIFERENCIAS CON POPEN() ESTÁNDAR:
 * 
 * 1. INTERFACE SIMPLIFICADA:
 *    - popen() usa string de comando, ft_popen() usa argv[]
 *    - popen() retorna FILE*, ft_popen() retorna int (fd)
 * 
 * 2. SIN SHELL:
 *    - popen() ejecuta comando a través de /bin/sh
 *    - ft_popen() ejecuta directamente con execvp()
 *    - Más seguro (no injection), pero menos flexible
 * 
 * 3. SIN CLEANUP AUTOMÁTICO:
 *    - popen() se empareja con pclose()
 *    - ft_popen() requiere close() manual del fd
 *    - No maneja automáticamente procesos zombie
 */

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. CONCEPTO DE PIPE:
 *    - Comunicación unidireccional entre procesos
 *    - write end → read end
 *    - pipe() crea ambos extremos
 * 
 * 2. REDIRECCIÓN CON DUP2:
 *    - dup2(oldfd, newfd) hace que newfd sea copia de oldfd
 *    - dup2(pipe_fd, STDOUT_FILENO) redirige stdout al pipe
 *    - Fundamental para conectar comando con pipe
 * 
 * 3. GESTIÓN DE DESCRIPTORES:
 *    - Cerrar descriptores no necesarios
 *    - Cada extremo del pipe debe cerrarse en el proceso correcto
 *    - Leaks de descriptores pueden causar deadlocks
 * 
 * 4. EXECVP vs EXEC FAMILY:
 *    - execvp() busca en PATH automáticamente
 *    - Usa array de argumentos (más seguro que system())
 *    - Reemplaza completamente la imagen del proceso
 * 
 * 5. TIPOS DE COMUNICACIÓN:
 *    - 'r': padre lee, hijo escribe (stdout del hijo → pipe → padre)
 *    - 'w': padre escribe, hijo lee (padre → pipe → stdin del hijo)
 *    - Entender la dirección del flujo de datos es crucial
 */