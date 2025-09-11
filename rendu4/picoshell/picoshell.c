#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char **cmds[])
{
    // Paso 1: Validaci�n b�sica
    if (!cmds || !cmds[0])
        return 1;
    
    // Contar cu�ntos comandos tenemos
    int cmd_count = 0;
    while (cmds[cmd_count])
        cmd_count++;
    
    // Si solo hay un comando, caso especial (sin pipes)
    if (cmd_count == 1)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            // Proceso hijo: ejecutar el comando
            execvp(cmds[0][0], cmds[0]);
            exit(1); // Si execvp falla
        }
        else if (pid > 0)
        {
            // Proceso padre: esperar al hijo
            int status;
            wait(&status);
            return 0;
        }
        else
        {
            // Error en fork
            return 1;
        }
    }
    
    // Múltiples comandos: necesitamos pipeline
    int pipes[cmd_count - 1][2]; // Array de pipes
    pid_t pids[cmd_count]; // Array de PIDs de procesos hijos
    
    // Crear todos los pipes
    for (int i = 0; i < cmd_count - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            // Error: cerrar pipes ya creados
            for (int j = 0; j < i; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            return 1;
        }
    }
    
    // Crear procesos hijos para cada comando
    for (int i = 0; i < cmd_count; i++)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            // Proceso hijo
            // Conectar stdin si no es el primer comando
            if (i > 0)
            {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            
            // Conectar stdout si no es el último comando
            if (i < cmd_count - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            // Cerrar todos los pipes en el hijo
            for (int j = 0; j < cmd_count - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            // Ejecutar el comando
            execvp(cmds[i][0], cmds[i]);
            exit(1); // Si execvp falla
        }
        else if (pids[i] == -1)
        {
            // Error en fork: cerrar todos los pipes y matar procesos previos
            for (int j = 0; j < cmd_count - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            return 1;
        }
    }
    
    // Proceso padre: cerrar todos los pipes
    for (int i = 0; i < cmd_count - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    // Esperar a todos los procesos hijos
    for (int i = 0; i < cmd_count; i++)
    {
        int status;
        wait(&status);
    }
    
    return 0;
}