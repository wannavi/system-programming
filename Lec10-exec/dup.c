#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
  int x, y, k;

  x = open("f1", O_RDONLY, 00777);
  y = dup(x);

  printf("x:%d y:%d\n", x, y);
  char buf[50];

  k = read(x, buf, 5);
  buf[k] = 0;

  printf("buf: %s\n", buf);
}