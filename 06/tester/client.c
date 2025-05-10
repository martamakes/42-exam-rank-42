#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set up server address structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to server on port %d\n", port);
    printf("Type messages to send (Ctrl+D to exit):\n");

    fd_set read_fds;
    int max_fd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(sockfd, &read_fds);

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            break;
        }

        // Check for input from stdin
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            memset(buffer, 0, BUFFER_SIZE);
            if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                printf("\nDisconnecting from server...\n");
                break;
            }
            
            // Send message to server
            if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
                perror("Send failed");
                break;
            }
        }

        // Check for messages from server
        if (FD_ISSET(sockfd, &read_fds)) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
            
            if (bytes_received <= 0) {
                printf("Server disconnected\n");
                break;
            }
            
            buffer[bytes_received] = '\0';
            printf("%s", buffer);
        }
    }

    close(sockfd);
    return 0;
}