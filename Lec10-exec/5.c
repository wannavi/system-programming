#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  int i = 0;
  char buf[256], *tok[256] = {0,};

  printf("$ > ");
  gets(buf);

  char *p = strtok(buf, " \t");
  while (p != NULL) {
    tok[i++] = p;
    p = strtok(NULL, " \t");
  }

  execve(tok[0], tok, 0);

  return 0;
}