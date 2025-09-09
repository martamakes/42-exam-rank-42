Assignment name  : sandbox
Expected files   : sandbox.c
Allowed functions: fork, waitpid, exit, alarm, sigaction, kill, printf, strsignal,
errno, sigaddset, sigemptyset, sigfillset, sigdelset, sigismember
--------------------------------------------------------------------------------------

Write the following function:

#include <stdbool.h>
int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

This function must test if the function f is a nice function or a bad function, you
will return 1 if f is nice, 0 if f is bad or -1 in case of an error in your function.

A function is considered bad if it is terminated or stopped by a signal (segfault, abort...),
if it exit with any other exit code than 0 or if it times out.

If verbose is true, you must write the appropriate message among the following:
"Nice function!\n"
"Bad function: exited with code <exit_code>\n"
"Bad function: <signal description>\n"
"Bad function: timed out after <timeout> seconds\n"

You must not leak processes (even in zombie state, this will be checked using wait).

We will test your code with very bad functions.