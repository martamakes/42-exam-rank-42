#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    int pipefd[2];
    pid_t pid;
    
    // Validar parámetros
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;
    
    // Crear pipe
    if (pipe(pipefd) == -1)
        return -1;
    
    // Crear proceso hijo
    pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }
    
    if (pid == 0)
    {
        // Proceso hijo: ejecutar el comando
        if (type == 'r')
        {
            // Modo READ: redirigir stdout del comando al pipe
            close(pipefd[0]);           // Cerrar extremo de lectura
            dup2(pipefd[1], STDOUT_FILENO); // stdout -> pipe write end
            close(pipefd[1]);           // Cerrar el duplicado
        }
        else if (type == 'w')
        {
            // Modo WRITE: redirigir stdin del comando desde el pipe
            close(pipefd[1]);           // Cerrar extremo de escritura
            dup2(pipefd[0], STDIN_FILENO);  // stdin <- pipe read end
            close(pipefd[0]);           // Cerrar el duplicado
        }
        
        // Ejecutar el comando
        execvp(file, argv);
        exit(1); // Si execvp falla
    }
    else
    {
        // Proceso padre: retornar el file descriptor apropiado
        if (type == 'r')
        {
            // Modo READ: retornar extremo de lectura
            close(pipefd[1]); // Cerrar extremo de escritura
            return pipefd[0]; // Retornar extremo de lectura
        }
        else if (type == 'w')
        {
            // Modo WRITE: retornar extremo de escritura
            close(pipefd[0]); // Cerrar extremo de lectura
            return pipefd[1]; // Retornar extremo de escritura
        }
    }
    
    return -1; // No debería llegar aquí
}