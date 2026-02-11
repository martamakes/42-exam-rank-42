#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char **cmds[])
{
	int		pipefd[2];
	int		prev_fd;
	int		i;
	pid_t	pid;

	if (!cmds || !cmds[0])
		return (1);
	prev_fd = -1; // no hay pipe anterior al empezar
	i = 0;
	while (cmds[i])
	{
		// si hay comando siguiente, crear pipe para conectar este con el próximo
		if (cmds[i + 1] && pipe(pipefd) == -1)
			return (1);
		pid = fork();
		if (pid == -1)
		{
			// error en fork: cerrar el pipe recién creado
			if (cmds[i + 1])
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			return (1);
		}
		if (pid == 0) // HIJO
		{
			// si hay un pipe anterior, leer de él (stdin = salida del comando previo)
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			// si hay comando siguiente, escribir en el pipe (stdout = entrada del próximo)
			if (cmds[i + 1])
			{
				close(pipefd[0]);           // el hijo no lee de este pipe
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1); // solo llega aquí si execvp falla
		}
		// PADRE
		if (prev_fd != -1)
			close(prev_fd);         // ya no necesitamos el pipe anterior
		if (cmds[i + 1])
		{
			close(pipefd[1]);       // el padre no escribe en el pipe
			prev_fd = pipefd[0];    // guardar extremo de lectura para el siguiente hijo
		}
		i++;
	}
	// esperar a todos los hijos
	while (wait(NULL) != -1)
		;
	return (0);
}
