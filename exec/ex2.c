#include <stdio.h>
#include <unistd.h>>

int main() {
  execve("./ex1", 0, 0);
  printf("I am ex2\n");
}