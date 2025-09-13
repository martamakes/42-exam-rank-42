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


/*
implementación alternativa
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> 

void do_nothing(int sig) { (void)sig; }


int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    if(!f)
        return -1;
    pid_t pid = fork();
    if (pid == -1)
        return (-1);
    if (pid == 0)
    {
        alarm(timeout);
        f();
        exit(0);
    }
    struct sigaction sa;
    sa.sa_handler = do_nothing;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);
    alarm(timeout);

    int st;
    pid_t r = waitpid(pid, &st, 0);
    alarm(0);
    if (r == -1)
    {
        if (errno == EINTR)
        {
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            if (verbose)
                printf("Bad function: timed out after %d seconds\n", timeout);
            return (0);
        }
        return (-1);
    }
    if (WIFEXITED(st))
    {
        int ex_code = WEXITSTATUS(st);
        if (ex_code == 0)
        {
            if (verbose)
                printf("Nice function!\n");
            return (1);
        }
        if (verbose)
            printf("Bad function: exited with code %d\n", ex_code);
        return (0);
    }
    if (WIFSIGNALED(st))
    {
        if (WTERMSIG(st) == SIGALRM)
        {
            if (verbose)
                printf("Bad function: timed out after %d seconds\n", timeout);
            return (0);
        }
        if (verbose)
            printf("Bad function: %s\n", strsignal(WTERMSIG(st)));
        return (0);
    }
    return (-1);
}


//main para testar compilación
int main()
{
    return 0;
}

//otro main testeo funcionalidad

#include <stdio.h>
  #include <unistd.h>
  #include <stdbool.h>

  // Funciones de test
  void good_function(void) {
      printf("Soy una función buena\n");
  }

  void bad_function(void) {
      printf("Voy a fallar...\n");
      int *p = NULL;
      *p = 42;  // Segfault
  }

  void slow_function(void) {
      printf("Voy a tardar mucho...\n");
      sleep(5);  // Más que el timeout
  }

  void exit_function(void) {
      printf("Salgo con código 1\n");
      exit(1);
  }

  int main() {
      printf("=== TEST SANDBOX ===\n\n");

      // Test 1: Función buena
      printf("Test 1 - Función buena: ");
      int result = sandbox(good_function, 2, true);
      printf("Resultado: %d\n\n", result);

      // Test 2: Función con segfault
      printf("Test 2 - Función con segfault: ");
      result = sandbox(bad_function, 2, true);
      printf("Resultado: %d\n\n", result);

      // Test 3: Función lenta (timeout)
      printf("Test 3 - Función lenta: ");
      result = sandbox(slow_function, 2, true);
      printf("Resultado: %d\n\n", result);

      // Test 4: Función que sale con código != 0
      printf("Test 4 - Exit code != 0: ");
      result = sandbox(exit_function, 2, true);
      printf("Resultado: %d\n\n", result);

      return 0;
  }

*/