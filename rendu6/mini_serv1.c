#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

char *str_join(char *buf, char *add)
{
	char *newbuf;
	int len;

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
	strcat(newbuf, add);
	free(buf);
	return (newbuf);
}

int extract_message(char **buf, char **msg)
{
	char *newbuf;
	int i;

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

const int MAX_FDS = 1024;

int maxfd;
fd_set actfds;
fd_set rfds;
fd_set wfds;
char *msgs_to_send[1024];
char *recv_buf[1024];
int client_id[1024];
int next_id;

void fatal(void)
{
	write(2, "Fatal error\n", 12);
	exit(1);
}

void remove_client(int fd)
{
	FD_CLR(fd, &actfds);
	close(fd);
	free(recv_buf[fd]);
	recv_buf[fd] = 0;
	free(msgs_to_send[fd]);
	msgs_to_send[fd] = 0;
	client_id[fd] = -1;
}

void broadcast(int listenfd, char *msg, int except_fd)
{
	int fd;

	for (fd = 0; fd <= maxfd; fd++)
	{
		if (fd != listenfd && fd != except_fd && FD_ISSET(fd, &actfds))
		{
			msgs_to_send[fd] = str_join(msgs_to_send[fd], msg);
			if (msgs_to_send[fd] == 0)
				fatal();
		}
	}
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in addr;
	int fd;

	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		fatal();

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(atoi(argv[1]));

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		fatal();
	if (listen(sockfd, SOMAXCONN) == -1)
		fatal();

	FD_ZERO(&actfds);
	FD_SET(sockfd, &actfds);
	maxfd = sockfd;
	next_id = 0;

	for (fd = 0; fd < MAX_FDS; fd++)
	{
		msgs_to_send[fd] = 0;
		recv_buf[fd] = 0;
		client_id[fd] = -1;
	}

	while (1)
	{
		rfds = actfds;
		wfds = actfds;

		if (select(maxfd + 1, &rfds, &wfds, 0, 0) == -1)
			continue;

		for (fd = 0; fd <= maxfd; fd++)
		{
			if (fd == sockfd)
			{
				if (FD_ISSET(fd, &rfds))
				{
					struct sockaddr_in cli_addr;
					socklen_t len;
					int newfd;
					char buf[64];

					len = sizeof(cli_addr);
					newfd = accept(sockfd, (struct sockaddr *)&cli_addr, &len);
					if (newfd != -1 && newfd < MAX_FDS)
					{
						if (newfd > maxfd)
							maxfd = newfd;
						FD_SET(newfd, &actfds);
						client_id[newfd] = next_id;
						sprintf(buf, "server: client %d just arrived\n", next_id);
						broadcast(sockfd, buf, newfd);
						next_id++;
					}
					else if (newfd != -1)
						close(newfd);
				}
				continue;
			}

			if (!FD_ISSET(fd, &actfds))
				continue;

			if (FD_ISSET(fd, &wfds) && msgs_to_send[fd] != 0)
			{
				int len;
				int sent;

				len = strlen(msgs_to_send[fd]);
				sent = send(fd, msgs_to_send[fd], len, 0);
				if (sent > 0)
				{
					if (sent == len)
					{
						free(msgs_to_send[fd]);
						msgs_to_send[fd] = 0;
					}
					else
					{
						char *rest;

						rest = malloc(len - sent + 1);
						if (rest == 0)
							fatal();
						strcpy(rest, msgs_to_send[fd] + sent);
						free(msgs_to_send[fd]);
						msgs_to_send[fd] = rest;
					}
				}
			}

			if (FD_ISSET(fd, &rfds) && FD_ISSET(fd, &actfds))
			{
				char buf[4097];
				int r;

				r = recv(fd, buf, 4096, 0);
				if (r <= 0)
				{
					char out[64];
					int left_id;

					left_id = client_id[fd];
					remove_client(fd);
					sprintf(out, "server: client %d just left\n", left_id);
					broadcast(sockfd, out, -1);
				}
				else
				{
					char *line;
					int ret;

					buf[r] = 0;
					recv_buf[fd] = str_join(recv_buf[fd], buf);
					if (recv_buf[fd] == 0)
						fatal();

					ret = extract_message(&recv_buf[fd], &line);
					while (ret == 1)
					{
						char *out;
						char prefix[32];

						sprintf(prefix, "client %d: ", client_id[fd]);
						out = malloc(strlen(prefix) + strlen(line) + 1);
						if (out == 0)
							fatal();
						out[0] = 0;
						strcat(out, prefix);
						strcat(out, line);
						broadcast(sockfd, out, fd);
						free(out);
						free(line);
						ret = extract_message(&recv_buf[fd], &line);
					}
					if (ret == -1)
						fatal();
				}
			}
		}
	}
	return (0);
}