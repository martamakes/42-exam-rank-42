#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

int server_fd;
int max_fd = 0;
int next_id = 0;
int ids[65536];
char *msgs[65536];
fd_set active_set, read_set;

// Exits program reporting a fatal error.
void fatal() {
    write(2, "Fatal error\n", 12);
    exit(1);
}

// Provided function to extract exactly one message string ending with '\n'
int extract_message(char **buf, char **msg) {
    char *newbuf;
    int i = 0;

    *msg = 0;
    if (*buf == 0) return 0;
    while ((*buf)[i]) {
        if ((*buf)[i] == '\n') {
            newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
            if (newbuf == 0) return -1;
            strcpy(newbuf, *buf + i + 1);
            *msg = *buf;
            (*msg)[i + 1] = 0;
            *buf = newbuf;
            return 1;
        }
        i++;
    }
    return 0;
}

// Provided function to join read strings cleanly while avoiding memory leaks
char *str_join(char *buf, char *add) {
    char *newbuf;
    int len = 0;

    if (buf != 0) len = strlen(buf);
    newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
    if (newbuf == 0) return 0;
    newbuf[0] = 0;
    if (buf != 0) strcpy(newbuf, buf);
    strcat(newbuf, add);
    free(buf);
    return newbuf;
}

// Broadcasts the message string to all other connected clients
void send_all(int sender, char *msg) {
    for (int i = 0; i <= max_fd; i++) {
        if (FD_ISSET(i, &active_set) && i != sender && i != server_fd) {
            // "client can be lazy and if they don't read your message you must NOT disconnect them"
            // We ignore send() failure / blocks entirely.
            send(i, msg, strlen(msg), 0);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }

    int port = atoi(argv[1]);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) fatal();

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    
    // Instead of htonl(2130706433), we replicate 127.0.0.1 directly with bitwise operators.
    // 127 is in the lowest byte memory location, keeping it cross-platform for standard little-endian machines used in 42
    servaddr.sin_addr.s_addr = (1 << 24) | 127; 
    
    // Instead of htons(port), we manually swap the bytes for network (big-endian) layout.
    servaddr.sin_port = ((port >> 8) & 0xFF) | ((port & 0xFF) << 8);

    if (bind(server_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) fatal();
    if (listen(server_fd, 10) < 0) fatal();

    FD_ZERO(&active_set);
    FD_SET(server_fd, &active_set);
    max_fd = server_fd;

    while (1) {
        read_set = active_set;
        
        if (select(max_fd + 1, &read_set, NULL, NULL, NULL) < 0) continue;

        for (int fd = 0; fd <= max_fd; fd++) {
            if (FD_ISSET(fd, &read_set)) {
                if (fd == server_fd) {
                    // Start of a new connection
                    int client_fd = accept(server_fd, NULL, NULL);
                    if (client_fd >= 0) {
                        if (client_fd > max_fd) max_fd = client_fd;
                        ids[client_fd] = next_id++;
                        msgs[client_fd] = NULL;
                        FD_SET(client_fd, &active_set);
                        
                        char buf[64];
                        sprintf(buf, "server: client %d just arrived\n", ids[client_fd]);
                        send_all(client_fd, buf);
                    }
                } else {
                    // Reading operations from existing clients
                    char read_buf[4096];
                    int bytes = recv(fd, read_buf, sizeof(read_buf) - 1, 0);
                    if (bytes <= 0) {
                        // Connection dropped or lost
                        char buf[64];
                        sprintf(buf, "server: client %d just left\n", ids[fd]);
                        send_all(fd, buf);
                        FD_CLR(fd, &active_set);
                        close(fd);
                        // Clean up hanging buffer memory to guarantee 0 leaks
                        if (msgs[fd]) {
                            free(msgs[fd]);
                            msgs[fd] = NULL;
                        }
                    } else {
                        // Received valid transmission, load into client's specific buffer then process line-by-line via loop
                        read_buf[bytes] = '\0';
                        char *new_msgs = str_join(msgs[fd], read_buf);
                        if (!new_msgs) fatal();
                        msgs[fd] = new_msgs;
                        
                        char *msg = NULL;
                        while (1) {
                            int ret = extract_message(&(msgs[fd]), &msg);
                            if (ret == -1) fatal();
                            if (ret == 0) break;
                            
                            char buf[64];
                            sprintf(buf, "client %d: ", ids[fd]);
                            send_all(fd, buf);
                            send_all(fd, msg);
                            free(msg);
                        }
                    }
                }
            }
        }
    }
    return 0;
}