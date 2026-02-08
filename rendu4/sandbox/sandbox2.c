#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    if (!f)
        return -1;

    pid_t cpid;
    int status = 0;

    cpid = fork();
    if (cpid == -1)
        return -1;

    if (cpid == 0) {    /* Child process */
        alarm(timeout);
        f();
        exit(0); // Solo llega aquí si f() termina sin problema
    }

    /* Parent process */
    waitpid(cpid, &status, 0);

    int exit_code = 0;
    int sig = 0;
    int result = 0;  // Por defecto: bad function

    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
        if (exit_code == 0)
            result = 1;  // Nice function
    }
    else if (WIFSIGNALED(status)) {
        sig = WTERMSIG(status);
    }

    /* Verbose output */
    if (verbose) {
        if (result == 1) {
            printf("Nice function!\n");
        }
        else if (WIFEXITED(status) && exit_code != 0) {
            printf("Bad function: exited with code %d\n", exit_code);
        }
        else if (WIFSIGNALED(status) && sig == SIGALRM) {
            printf("Bad function: timed out after %u seconds\n", timeout);
        }
        else if (WIFSIGNALED(status)) {
            char *str = strsignal(sig);
            if (str)
                printf("Bad function: %s\n", str);
        }
    }

    return result;
}

