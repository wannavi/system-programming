#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888

int main() {
    pid_t pid;
    int master_socket, nread;
    struct sockaddr_in address;

    char buf[50];

    printf("Hi, I am the client\n");

    bzero((char *) &address, sizeof(address));
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // open a tcp socket
    if ((master_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    printf("socket opened successfully. socket num is %d\n", master_socket);

    // connect to the server
    if (connect(master_socket, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("connect() error");
        exit(EXIT_FAILURE);
    }

    // ping
    gets(buf);
    write(master_socket, buf, strlen(buf));
    nread = read(master_socket, buf, 50);
    write(1, buf, nread);
    printf("\n");

    // pang
    gets(buf);
    write(master_socket, buf, strlen(buf));
    nread = read(master_socket, buf, 50);
    write(1, buf, nread);
    printf("\n");

    // chatting
    if ((pid = fork()) < 0) {
        perror("fork() error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        for (;;) {
            gets(buf);
            write(master_socket, buf, strlen(buf));
        }
    } else {
        for (;;) {
            if ((nread = read(master_socket, buf, sizeof buf)) == 0) {
                perror("read() error");
                close(master_socket);
            }
            write(1, buf, nread);
            printf("\n");
        }
    }
}