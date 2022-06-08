#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9924
#define ADDR INADDR_ANY

int main() {
  int master_socket, client_socket, nread;
  struct sockaddr_in address;
  socklen_t addrlen;

  char buf[50];

  printf("Hi, I am the server\n");

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

  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind() error");
    exit(EXIT_FAILURE);
  }

  if (listen(master_socket,5) < 0) {
    perror("listen() error");
    exit(EXIT_FAILURE);
  }
  
  addrlen = sizeof(address);
  client_socket = accept(master_socket, (struct sockaddr *)&address, &addrlen);
  printf("we passed accept. new socket num is %d\n", client_socket); 

  nread = read(client_socket, buf, sizeof buf);
  buf[nread] = 0;
  printf("we got %s from cli\n", buf);

  printf("what do you want to send to cli? enter your string\n");
  scanf("%s", buf);
  write(client_socket, buf, strlen(buf));

  close(client_socket);
  close(master_socket);
}