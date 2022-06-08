#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
  pid_t pid;
  int status;
  char *argv[10] = { 0, };

  argv[0] = "/bin/ls";
  argv[1] = "-l";
  argv[2] = 0;

  if ((pid = fork()) < 0) {
    perror("fork() error");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    execve(argv[0], argv, 0);
  } else {
    if (wait(&status) == -1) {
      perror("wait() error");
    } else {
      puts("Job done");
    }
  }
}