#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

// These functions are provided in the exam
int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

// Error handling function
void fatal_error() {
    write(2, "Fatal error\n", 12);
    exit(1);
}

// Client structure
typedef struct s_client {
    int id;
    int fd;
    char* buffer;
    struct s_client* next;
} t_client;

// Global variables
int g_id = 0;
t_client* clients = NULL;

// Your implementation starts here
// TODO: Implement the server functionality

int main(int ac, char **av) {
    // Check arguments
    if (ac != 2) {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }

    // Socket setup
    int sockfd;
    struct sockaddr_in servaddr;
    
    // TODO: Create socket, bind, and listen
    
    // TODO: Implement select() for non-blocking I/O
    
    // TODO: Accept connections and handle clients
    
    return 0;
}