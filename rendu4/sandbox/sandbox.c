#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Variable global para el timeout
static bool timed_out = false;

// Handler para SIGALRM (timeout)
void alarm_handler(int sig)
{
    (void)sig; // Evitar warning de variable no usada
    timed_out = true;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid;
    int status;
    struct sigaction sa, old_sa;
    
    // Validar parámetros
    if (!f)
        return -1;
    
    // Configurar handler para SIGALRM
    timed_out = false;
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGALRM, &sa, &old_sa) == -1)
        return -1;
    
    // Crear proceso hijo
    pid = fork();
    if (pid == -1)
    {
        sigaction(SIGALRM, &old_sa, NULL); // Restaurar handler original
        return -1;
    }
    
    if (pid == 0)
    {
        // Proceso hijo: ejecutar la función f
        f();
        exit(0); // Si f() termina normalmente, salir con código 0
    }
    else
    {
        // Proceso padre: configurar timeout y esperar
        alarm(timeout);
        
        // Esperar al proceso hijo
        if (waitpid(pid, &status, 0) == -1)
        {
            if (errno == EINTR && timed_out)
            {
                // Timeout: matar al proceso hijo
                kill(pid, SIGKILL);
                waitpid(pid, &status, 0); // Limpiar proceso zombie
                
                if (verbose)
                    printf("Bad function: timed out after %u seconds\n", timeout);
                
                alarm(0); // Cancelar alarma
                sigaction(SIGALRM, &old_sa, NULL); // Restaurar handler
                return 0;
            }
            
            alarm(0);
            sigaction(SIGALRM, &old_sa, NULL);
            return -1;
        }
        
        // Cancelar alarma y restaurar handler
        alarm(0);
        sigaction(SIGALRM, &old_sa, NULL);
        
        // Analizar el status del proceso hijo
        if (WIFEXITED(status))
        {
            // El proceso terminó normalmente
            int exit_code = WEXITSTATUS(status);
            if (exit_code == 0)
            {
                if (verbose)
                    printf("Nice function!\n");
                return 1; // Función buena
            }
            else
            {
                if (verbose)
                    printf("Bad function: exited with code %d\n", exit_code);
                return 0; // Función mala (exit code != 0)
            }
        }
        else if (WIFSIGNALED(status))
        {
            // El proceso fue terminado por una señal
            int sig = WTERMSIG(status);
            if (verbose)
                printf("Bad function: %s\n", strsignal(sig));
            return 0; // Función mala (terminada por señal)
        }
        else
        {
            // Caso inesperado (proceso parado, etc.)
            if (verbose)
                printf("Bad function: unexpected termination\n");
            return 0;
        }
    }
}