#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9924
#define ADDR INADDR_ANY

int main() {
    int master_socket, nread, i;
    struct sockaddr_in address;
    pid_t pid;

    char buf[50];

    printf("Hi, I am the client\n");

    bzero((char*)&address, sizeof(address));
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = ADDR;
    address.sin_port = htons(PORT);

    // open a tcp socket
    if ((master_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    printf("socket opened successfully. socket num is %d\n", master_socket);

    // connect to the server
    if (connect(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connect() error");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) < 0) {
        perror("fork() error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        for (i = 0; i < 20; i++) {
            scanf("%s", buf);
            write(master_socket, buf, strlen(buf));
        }
    } else {
        for (i = 0; i < 20; i++) {
            if ((nread = read(master_socket, buf, sizeof buf)) > 0) {
                buf[nread] = 0;
                printf("$ SERVER > %s\n", buf);
            }
        }
    }

    close(master_socket);
    return 0;
}