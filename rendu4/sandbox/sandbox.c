#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void	handle_alrm(int sig)
{
	(void)sig;
	// handler vacío: solo interrumpe waitpid con EINTR
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t				pid;
	int					status;
	int					ret;
	struct sigaction	sa;
	struct sigaction	old_sa;

	if (!f)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0) // HIJO: ejecutar f y salir limpio
	{
		f();
		_exit(0); // _exit no vacía buffers de stdio (evita doble output al hacer fork)
	}
	// PADRE: instalar handler de SIGALRM sin SA_RESTART
	// así alarm() interrumpe waitpid() con EINTR cuando expira el tiempo
	sa.sa_handler = handle_alrm;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0; // sin SA_RESTART es clave
	sigaction(SIGALRM, &sa, &old_sa);
	alarm(timeout);
	ret = waitpid(pid, &status, 0);
	alarm(0); // cancelar alarma pendiente
	sigaction(SIGALRM, &old_sa, NULL); // restaurar handler original

	if (ret == -1) // waitpid interrumpido → timeout
	{
		kill(pid, SIGKILL);
		waitpid(pid, NULL, 0); // recoger zombie
		if (verbose)
			printf("Bad function: timed out after %u seconds\n", timeout);
		return (0);
	}
	if (WIFEXITED(status))
	{
		int code = WEXITSTATUS(status);
		if (code == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		if (verbose)
			printf("Bad function: exited with code %d\n", code);
		return (0);
	}
	if (WIFSIGNALED(status))
	{
		if (verbose)
			printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
		return (0);
	}
	return (-1);
}
