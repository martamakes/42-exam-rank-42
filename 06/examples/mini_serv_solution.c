#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

// Client structure to manage connected clients
typedef struct s_client {
    int id;
    int fd;
    char *buffer;
    struct s_client *next;
} t_client;

// Global variables
int g_id = 0;
t_client *clients = NULL;
fd_set read_fds, write_fds, active_fds;
char buffer[65536];
char send_buffer[65536];

// Function to handle fatal errors
void fatal_error() {
    write(2, "Fatal error\n", 12);
    exit(1);
}

// Function to extract messages from buffer
int extract_message(char **buf, char **msg) {
    char *newbuf;
    int i;

    *msg = 0;
    if (*buf == 0)
        return (0);
    i = 0;
    while ((*buf)[i]) {
        if ((*buf)[i] == '\n') {
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

// Function to join strings
char *str_join(char *buf, char *add) {
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
    free(buf);
    strcat(newbuf, add);
    return (newbuf);
}

// Function to add a new client
void add_client(int fd) {
    t_client *new_client = malloc(sizeof(t_client));
    if (!new_client)
        fatal_error();

    new_client->id = g_id++;
    new_client->fd = fd;
    new_client->buffer = NULL;
    new_client->next = NULL;

    // Add to client list
    if (!clients)
        clients = new_client;
    else {
        t_client *temp = clients;
        while (temp->next)
            temp = temp->next;
        temp->next = new_client;
    }

    // Notify everyone about the new client
    sprintf(buffer, "server: client %d just arrived\n", new_client->id);
    t_client *current = clients;
    while (current) {
        if (current->fd != fd && FD_ISSET(current->fd, &write_fds))
            if (send(current->fd, buffer, strlen(buffer), 0) < 0)
                fatal_error();
        current = current->next;
    }
}

// Function to remove a client
void remove_client(int fd) {
    t_client *prev = NULL;
    t_client *current = clients;
    t_client *to_remove = NULL;
    int client_id = -1;

    // Find the client to remove
    while (current) {
        if (current->fd == fd) {
            to_remove = current;
            client_id = current->id;
            if (prev)
                prev->next = current->next;
            else
                clients = current->next;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (to_remove) {
        // Notify everyone about the client leaving
        sprintf(buffer, "server: client %d just left\n", client_id);
        current = clients;
        while (current) {
            if (FD_ISSET(current->fd, &write_fds))
                if (send(current->fd, buffer, strlen(buffer), 0) < 0)
                    fatal_error();
            current = current->next;
        }

        // Clean up resources
        if (to_remove->buffer)
            free(to_remove->buffer);
        free(to_remove);

        // Remove from fd sets
        FD_CLR(fd, &active_fds);
        FD_CLR(fd, &read_fds);
        FD_CLR(fd, &write_fds);
        close(fd);
    }
}

// Function to broadcast a message to all clients except the sender
void broadcast_message(int sender_fd, char *message) {
    t_client *sender = NULL;
    t_client *current = clients;

    // Find the sender
    while (current) {
        if (current->fd == sender_fd) {
            sender = current;
            break;
        }
        current = current->next;
    }

    if (!sender)
        return;

    // Prepare message format with client ID
    sprintf(send_buffer, "client %d: %s", sender->id, message);

    // Send to all clients except the sender
    current = clients;
    while (current) {
        if (current->fd != sender_fd && FD_ISSET(current->fd, &write_fds))
            if (send(current->fd, send_buffer, strlen(send_buffer), 0) < 0)
                fatal_error();
        current = current->next;
    }
}

// Function to handle incoming messages
void handle_message(int fd) {
    t_client *client = clients;
    char msg_buffer[65536];
    char *msg = NULL;
    ssize_t bytes_read;

    // Find the client
    while (client && client->fd != fd)
        client = client->next;

    if (!client)
        return;

    // Read from socket
    bytes_read = recv(fd, msg_buffer, sizeof(msg_buffer) - 1, 0);
    if (bytes_read <= 0) {
        remove_client(fd);
        return;
    }
    msg_buffer[bytes_read] = '\0';

    // Add to client's buffer
    client->buffer = str_join(client->buffer, msg_buffer);
    if (!client->buffer)
        fatal_error();

    // Extract and broadcast complete messages
    while (extract_message(&client->buffer, &msg) == 1) {
        broadcast_message(fd, msg);
        free(msg);
        msg = NULL;
    }
}

// Function to get the maximum file descriptor
int get_max_fd() {
    int max_fd = -1;
    t_client *current = clients;

    while (current) {
        if (current->fd > max_fd)
            max_fd = current->fd;
        current = current->next;
    }
    return max_fd;
}

int main(int ac, char **av) {
    if (ac != 2) {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }

    // Socket setup
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t len;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        fatal_error();

    // Set socket options
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        fatal_error();

    // Setup address structure
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    servaddr.sin_port = htons(atoi(av[1]));

    // Bind socket
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
        fatal_error();

    // Listen for connections
    if (listen(sockfd, 10) != 0)
        fatal_error();

    // Initialize fd sets
    FD_ZERO(&active_fds);
    FD_SET(sockfd, &active_fds);
    int max_fd = sockfd;

    // Main server loop
    while (1) {
        read_fds = write_fds = active_fds;

        // Wait for activity on one of the sockets
        if (select(max_fd + 1, &read_fds, &write_fds, NULL, NULL) < 0)
            fatal_error();

        // Check all possible file descriptors
        for (int fd = 0; fd <= max_fd; fd++) {
            // If current fd is ready for reading
            if (FD_ISSET(fd, &read_fds)) {
                // If it's the server socket, accept new connection
                if (fd == sockfd) {
                    len = sizeof(cli);
                    connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
                    if (connfd < 0)
                        fatal_error();

                    // Add to active set
                    FD_SET(connfd, &active_fds);
                    if (connfd > max_fd)
                        max_fd = connfd;

                    // Add the new client
                    add_client(connfd);
                }
                // If it's a client socket, handle the message
                else {
                    handle_message(fd);
                }
            }
        }
    }

    return 0;
}