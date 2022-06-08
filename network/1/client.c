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
  int master_socket, nread;
  struct sockaddr_in address;

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

  // send input to the server
  printf("now I am connected to the server.\nEnter a string to send\n");
  scanf("%s", buf);
  write(master_socket, buf, strlen(buf));

  printf("now let's rad from the server\n");
  nread = read(master_socket, buf, 50);
  buf[nread] = 0;

  printf("what echoed from the server is %s\n", buf);
  close(master_socket);
}