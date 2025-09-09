/*
 * EJERCICIO: SANDBOX
 * 
 * DESCRIPCIÓN:
 * Crear un "sandbox" que ejecute una función y determine si es "buena" o "mala".
 * Una función es mala si: segfault, abort, exit code != 0, o timeout.
 * 
 * CONCEPTOS CLAVE:
 * 1. SIGNALS: Manejar SIGALRM para timeout
 * 2. FORK: Ejecutar función en proceso separado
 * 3. WAITPID: Obtener información sobre cómo terminó el proceso
 * 4. ALARM: Establecer timeout para la función
 * 5. SIGNAL HANDLING: Configurar manejadores de señales
 * 
 * ALGORITMO:
 * 1. Fork proceso hijo para ejecutar la función
 * 2. En el padre: establecer alarm y esperar con waitpid
 * 3. Analizar cómo terminó el proceso (normal, signal, timeout)
 * 4. Retornar 1 (buena), 0 (mala), o -1 (error)
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

// Variable global para el PID del proceso hijo
static pid_t child_pid;

// Manejador de señal para SIGALRM (timeout)
void alarm_handler(int sig)
{
    /*
     * MANEJADOR DE TIMEOUT:
     * - Se ejecuta cuando el alarm() expira
     * - No necesita hacer nada especial
     * - Su existencia hace que waitpid() retorne con EINTR
     */
    (void)sig;  // Suprimir warning de parámetro no usado
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    /*
     * PARÁMETROS:
     * - f: Función a probar
     * - timeout: Tiempo límite en segundos
     * - verbose: Si imprimir mensajes de diagnóstico
     * 
     * RETORNO:
     * - 1: función "buena" (exit code 0, sin signals, sin timeout)
     * - 0: función "mala" (exit code != 0, signal, o timeout)
     * - -1: error en sandbox (fork falló, etc.)
     */
    
    struct sigaction sa;
    pid_t pid;
    int status;
    
    /*
     * CONFIGURAR MANEJADOR DE SIGALRM:
     * - Configurar manejador personalizado para timeout
     * - Limpiar máscara de señales
     * - No reiniciar syscalls automáticamente
     */
    sa.sa_handler = alarm_handler;
    sa.sa_flags = 0;  // No SA_RESTART: queremos que waitpid sea interrumpido
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);
    
    /*
     * FORK PROCESO HIJO:
     */
    pid = fork();
    if (pid == -1)
        return -1;  // Error en fork
    
    if (pid == 0)  // PROCESO HIJO
    {
        /*
         * EJECUTAR FUNCIÓN EN HIJO:
         * - Llamar a la función proporcionada
         * - Si retorna normalmente, salir con código 0
         * - Si hace segfault/abort, el kernel enviará señal
         */
        f();
        exit(0);  // Función terminó normalmente
    }
    
    // PROCESO PADRE
    child_pid = pid;
    
    /*
     * ESTABLECER TIMEOUT:
     * - alarm() envía SIGALRM después de timeout segundos
     * - Esto interrumpirá waitpid() si la función toma demasiado tiempo
     */
    alarm(timeout);
    
    /*
     * ESPERAR AL PROCESO HIJO:
     * - waitpid() puede retornar por varias razones:
     *   1. El hijo terminó normalmente (exit)
     *   2. El hijo fue terminado por una señal
     *   3. waitpid fue interrumpido por SIGALRM (timeout)
     */
    if (waitpid(pid, &status, 0) == -1)
    {
        if (errno == EINTR)  // Interrumpido por SIGALRM
        {
            /*
             * TIMEOUT DETECTADO:
             * - waitpid fue interrumpido por alarm
             * - El hijo probablemente sigue ejecutándose
             * - Matarlo con SIGKILL y recoger su estado
             */
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);  // Recoger proceso zombie
            
            if (verbose)
                printf("Bad function: timed out after %d seconds\n", timeout);
            return 0;
        }
        return -1;  // Otro tipo de error
    }
    
    /*
     * ANALIZAR CÓMO TERMINÓ EL PROCESO:
     */
    
    if (WIFEXITED(status))
    {
        /*
         * TERMINACIÓN NORMAL (exit):
         * - El proceso llamó exit() o retornó de main
         * - Verificar el código de salida
         */
        if (WEXITSTATUS(status) == 0)
        {
            if (verbose)
                printf("Nice function!\n");
            return 1;  // Función buena
        }
        else
        {
            if (verbose)
                printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
            return 0;  // Función mala
        }
    }
    
    if (WIFSIGNALED(status))
    {
        /*
         * TERMINACIÓN POR SEÑAL:
         * - El proceso fue terminado por una señal (segfault, abort, etc.)
         * - Obtener el número de señal para diagnóstico
         */
        int sig = WTERMSIG(status);
        if (verbose)
            printf("Bad function: %s\n", strsignal(sig));
        return 0;  // Función mala
    }
    
    return -1;  // Estado no reconocido
}

/*
 * EJEMPLOS DE FUNCIONES PARA PROBAR:
 * 
 * void nice_function(void)
 * {
 *     // Esta función no hace nada malo
 *     return;
 * }
 * 
 * void bad_function_exit_code(void)
 * {
 *     // Esta función termina con código de error
 *     exit(1);
 * }
 * 
 * void bad_function_segfault(void)
 * {
 *     // Esta función causa segmentation fault
 *     int *ptr = NULL;
 *     *ptr = 42;
 * }
 * 
 * void bad_function_timeout(void)
 * {
 *     // Esta función nunca termina
 *     while (1) {}
 * }
 * 
 * void bad_function_abort(void)
 * {
 *     // Esta función llama abort()
 *     abort();
 * }
 */

/*
 * EJEMPLO DE USO:
 * 
 * int main()
 * {
 *     int result;
 * 
 *     printf("Testing nice function:\n");
 *     result = sandbox(nice_function, 5, true);
 *     printf("Result: %d\n\n", result);  // Esperado: 1
 * 
 *     printf("Testing bad function (segfault):\n");
 *     result = sandbox(bad_function_segfault, 5, true);
 *     printf("Result: %d\n\n", result);  // Esperado: 0
 * 
 *     printf("Testing bad function (timeout):\n");
 *     result = sandbox(bad_function_timeout, 2, true);
 *     printf("Result: %d\n\n", result);  // Esperado: 0
 * 
 *     return 0;
 * }
 */

/*
 * ANÁLISIS DE ESTADOS DEL PROCESO:
 * 
 * 1. WIFEXITED(status):
 *    - true si el proceso terminó con exit() o return
 *    - WEXITSTATUS(status) obtiene el código de salida
 * 
 * 2. WIFSIGNALED(status):
 *    - true si el proceso fue terminado por señal
 *    - WTERMSIG(status) obtiene el número de señal
 *    - Incluye: SIGSEGV, SIGABRT, SIGKILL, etc.
 * 
 * 3. WIFSTOPPED(status):
 *    - true si el proceso fue detenido (SIGSTOP)
 *    - No relevante para este ejercicio
 * 
 * 4. WIFCONTINUED(status):
 *    - true si el proceso fue continuado (SIGCONT)
 *    - No relevante para este ejercicio
 */

/*
 * SEÑALES COMUNES QUE PUEDEN RECIBIRSE:
 * 
 * - SIGSEGV (11): Segmentation fault
 * - SIGABRT (6): Abort signal (llamada a abort())
 * - SIGFPE (8): Floating point exception (división por cero)
 * - SIGILL (4): Illegal instruction
 * - SIGBUS (7): Bus error
 * - SIGKILL (9): Kill signal (enviado por el sandbox en timeout)
 */

/*
 * GESTIÓN DE PROCESOS ZOMBIE:
 * 
 * Es CRUCIAL hacer waitpid() después de kill() en caso de timeout:
 * - kill(pid, SIGKILL) termina el proceso
 * - Pero el proceso queda como "zombie" hasta que el padre haga wait
 * - waitpid(pid, NULL, 0) limpia el zombie
 * - Sin esto, se acumulan procesos zombie en el sistema
 */

/*
 * PUNTOS CLAVE PARA EL EXAMEN:
 * 
 * 1. CONFIGURACIÓN DE SEÑALES:
 *    - Usar sigaction() en lugar de signal() (más portable)
 *    - No usar SA_RESTART: queremos que waitpid sea interrumpido
 *    - El manejador puede estar vacío, solo necesita existir
 * 
 * 2. DETECCIÓN DE TIMEOUT:
 *    - alarm() + waitpid() interrumpido por SIGALRM
 *    - Verificar errno == EINTR
 *    - Matar proceso hijo y recoger zombie
 * 
 * 3. ANÁLISIS DE STATUS:
 *    - Usar macros WIFEXITED, WIFSIGNALED, etc.
 *    - No asumir formato específico de status
 *    - Manejar todos los casos posibles
 * 
 * 4. PREVENCIÓN DE LEAKS:
 *    - Siempre hacer waitpid() después de kill()
 *    - No dejar procesos zombie
 *    - Verificar que todos los caminos de código limpien procesos
 * 
 * 5. ROBUSTEZ:
 *    - Manejar errores de fork(), waitpid(), etc.
 *    - Funciones muy malas pueden hacer cosas inesperadas
 *    - El sandbox debe ser más robusto que las funciones que prueba
 */