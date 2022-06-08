#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TRUE 1
#define FALSE 0
#define PORT 8888

void run(int sd, fd_set *readfds, int *client_socket);

int main() {
    int master_socket, addrlen, new_socket, client_socket[30], sd,
        max_clients=30, max_sd, activity, i;
    struct sockaddr_in address;

    // set of socket descriptors
    fd_set readfds;

    for (i = 0; i < max_clients; i++) client_socket[i] = 0;

    // create a master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    bzero((char*)&address, sizeof(address));
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    printf("socket opened successfully. socket num is %d\n", master_socket);

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind() error");
        exit(EXIT_FAILURE);
    }

    if (listen(master_socket,5) < 0) {
        perror("listen() error");
        exit(EXIT_FAILURE);
    }

    // accept the incoming connection
    addrlen = sizeof(address);
    puts("... waiting for connections");

    while (TRUE) {
        // clear the socket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        // add child sockets to set
        for (i = 0; i < max_clients; i++) {
            // socket descriptor
            sd = client_socket[i];
            if (sd > 0) FD_SET(sd, &readfds);
            if (sd > max_sd) max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            perror("select() error");
        }

        // If something happened on the master socket.
        // then it is an incoming connection
        if (FD_ISSET(master_socket, &readfds)) {
            if ((new_socket = accept(
                    master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept() error");
                exit(EXIT_FAILURE);
            }

            printf("new client is accepted: %d\n", new_socket);

            // add new socket to array of sockets
            for (i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        } else {
           // if some IO operation on some other sockets
           for (i = 0; i < max_clients; i++) {
               sd = client_socket[i];
               if (FD_ISSET(sd, &readfds)) {
                   run(sd, &readfds, &client_socket[i]);
               }
           }
        }
    }
}

void run(int sd, fd_set *readfds, int *client_socket) {
    char buf[1024];
    int nread;

    nread = read(sd, buf, sizeof buf);
    buf[nread] = 0;

    if (strcmp(buf, "ping") == 0) {
        write(sd, "pong", 4);
    } else if (strcmp(buf, "pong") == 0) {
        write(sd, "pung", 4);
    } else {
        write(sd, "protocol error", 14);

        *client_socket = 0;
        close(sd);
        FD_CLR(sd, readfds);
    }
}