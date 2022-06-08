#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 80
#define ADDR "165.246.13.106"

int main() {
    int master_socket, nread;
    struct sockaddr_in address;
    char buf[50];

    char header[] = "GET / HTTP/1.1\r\nHOST: www.inha.ac.kr\r\n\r\n";

    printf("Hi, I am the client\n");

    bzero((char*)&address, sizeof(address));
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = inet_addr(ADDR);
    address.sin_port = htons(PORT);

    // open a tcp socket
    if ((master_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    // connect to the server
    if (connect(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("connect() error");
        exit(EXIT_FAILURE);
    }

    if (write(master_socket, header, strlen(header)) < 0) {
        perror("write() error");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        nread = read(master_socket, buf, sizeof buf);
        if (nread <= 0) break;

        buf[nread] = 0;
        printf("%s", buf);
    }

    close(master_socket);
    return 0;
}