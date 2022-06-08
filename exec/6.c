#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void exec_on_child_process(char *argv[]);

int main() {
  int i;
  char buf[256], *argv[256] = {0,}, *p;

  for (;;) {
    i = 0;
    printf("$ > ");
    scanf("%s", buf);
      
    p = strtok(buf, " \t");
    while (p != NULL) {
      argv[i++] = p;
      p = strtok(NULL, " \t");
    }

    exec_on_child_process(argv);
    memset(buf, 0, sizeof buf);
    memset(argv, 0, sizeof argv);
  }

  return 0;
}

void exec_on_child_process(char *argv[]) {
  pid_t pid;
  int status;

  if ((pid = fork()) < 0) {
    perror("fork() error");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    execve(argv[0], argv, 0);
  } else {
    if (wait(&status) == -1) {
      perror("wait() error");
    }
  }
}
