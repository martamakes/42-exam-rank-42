#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    //validar parámetros
    if(!file || !argv || (type != 'r' && type != 'w'))
        return -1;
    //crear pipe
    int pipefd[2];
    if(pipe(pipefd) == -1)
        return -1;
    //fork
    pid_t pid = fork();
    if(pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }
    
    //hijo

    if(pid == 0)
    {
        if(type == 'r')
        {
            //si r, redirigir stdout al pipe
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
        } else {
            // si w, redirigir stdin desde el pipe
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
        }
        execvp(file, argv);
        exit(1);
    }

    //proceso padre
    if(type == 'r')
    {
        close(pipefd[1]);
        return pipefd[0];
    } else {
        close(pipefd[0]);
        return pipefd[1];
    }
   
} 

