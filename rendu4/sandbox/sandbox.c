#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include  <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/errno.h>

volatile sig_atomic_t g_timeout = 0;

void handler(int sig)
{ 
    (void)sig;
    g_timeout = 1;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    if(!f)
        return -1;
    
    //conf SIGACTION
    struct sigaction sa = {0};
    sa.sa_handler = handler;
    sigaction(SIGALRM, &sa, NULL);


    pid_t pid = fork();
    if(pid == -1) return -1;
    
    if(pid == 0)
    {
        f();
        exit (0);
    }
    int status;
    alarm(timeout); //arranca timeout
    int ret = waitpid(pid, &status, 0);
    alarm(0); //cancela si termina antes

    if(ret == -1 && errno == EINTR && g_timeout) //timeout
    {
        kill(pid, SIGKILL); //matar hijo
        waitpid(pid, NULL, 0); //recojo zombie
        if(verbose) printf("Bad function: timed out after %u seconds\n", timeout);
        return 0;
    }
    if(ret == -1)
        return -1;

    if(WIFEXITED(status))
    {
        int code = WEXITSTATUS(status);
        if(code == 0)
        {
            //nice
            if(verbose) printf("Nice function!\n");
            return 1;
        }
        else
        {
            //bad
            if(verbose) printf("Bad function: exited with code %d\n", code);
            return 0;
        }
    }
    else if(WIFSIGNALED(status))
    {
        //recurperar señal error
        char *code = strsignal(WTERMSIG(status));
        if(verbose) printf("Bad function: %s\n",code);
        return 0;
    }
    return -1;
}