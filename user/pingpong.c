#include "kernel/types.h" // uint usage in user.h
#include "user.h"


int main() {
  int my_pipe[2];
  pipe(my_pipe);
  if (!fork()) {
    // In Child process.
    int byte = 0;
    read(my_pipe[0], &byte, sizeof(byte));
    printf("%d: received ping\n", getpid());
    write(my_pipe[1], &byte, 1);
  } else {
    int byte = 42;
    write(my_pipe[1], &byte, sizeof(byte));
    read(my_pipe[0], &byte, sizeof(byte));
    printf("%d: received pong\n", getpid());
  }
  exit(0);
}