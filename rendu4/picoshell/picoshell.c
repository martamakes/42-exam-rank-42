#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int picoshell(char **cmds[])
{
	// Caso de error: no hay comandos
	if (!cmds || !cmds[0])
		return 1;

	int		fd[2];       // pipe actual: fd[0]=lectura, fd[1]=escritura
	int		prev_fd = -1; // lectura del pipe ANTERIOR (-1 = no hay, es el primer comando)
	int		i = 0;
	pid_t	pid;

	while (cmds[i])
	{
		// Creamos pipe solo si hay un comando siguiente que reciba la salida
		// Ej: "ls | grep" necesita pipe. Solo "ls" no.
		if (cmds[i + 1] && pipe(fd) == -1)
			return 1;

		pid = fork(); // duplicamos el proceso para ejecutar cmds[i] en el hijo
		if (pid == -1) // fork falló
		{
			if (cmds[i + 1]) // cerramos el pipe recién creado
			{
				close(fd[0]);
				close(fd[1]);
			}
			while (wait(NULL) != -1) // esperamos hijos ya lanzados
				;
			return (1);
		}

		if (pid == 0) // ── HIJO ──────────────────────────────────────────────
		{
			// Si hay pipe anterior → redirigir stdin al extremo de lectura
			// El hijo leerá la salida del comando previo como si fuera el teclado
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}

			// Si hay comando siguiente → redirigir stdout al extremo de escritura
			// Lo que el hijo escriba lo leerá el siguiente hijo
			if (cmds[i + 1])
			{
				close(fd[0]);                   // el hijo no lee de este pipe
				dup2(fd[1], STDOUT_FILENO);     // stdout → pipe
				close(fd[1]);
			}

			execvp(cmds[i][0], cmds[i]); // ejecutar el comando (reemplaza el hijo)
			exit(1);                      // solo llega aquí si execvp falla
		}

		// ── PADRE ─────────────────────────────────────────────────────────────
		if (prev_fd != -1)
			close(prev_fd); // ya se lo pasamos al hijo, el padre no lo necesita

		if (cmds[i + 1])
		{
			close(fd[1]);       // el padre no escribe en el pipe
			prev_fd = fd[0];    // guardamos la lectura para el PRÓXIMO hijo
		}
		i++;
	}

	// Esperamos a todos los hijos antes de volver
	while (wait(NULL) != -1)
		;
	return 0;
}
