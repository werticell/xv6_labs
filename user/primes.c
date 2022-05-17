#include "kernel/types.h" // uint usage in user.h
#include "user.h"

/* Since we need the main process to wait for all subprocesses
 * and we want neighbouring processes to be connected with their own pipe
 * we can't fork all processes from main.
 * That's why we need the main process to be the last processor (36-th)
 *
*/

void setup_pipeline(int read_from_fd) {
  int prime = 0;
  if (read(read_from_fd, &prime, sizeof(prime))) {
    printf("prime %d\n", prime);
    int my_pipe[2];// 0 - Read STDIN, 1 - Write STDOUT.
    pipe(my_pipe);
    if (fork()) {
      close(my_pipe[1]);
      setup_pipeline(my_pipe[0]);
    } else {
      close(my_pipe[0]);
      int n = 0;
      while (read(read_from_fd, &n, sizeof(n))) {
        if (n % prime != 0) {
          write(my_pipe[1], &n, sizeof(n));
        }
      }
    }
  }
}


int main() {
  int my_pipe[2]; // 0 - Read STDIN, 1 - Write STDOUT.
  pipe(my_pipe);

  if (fork()) {
    // Parent Process going deeper.
    close(my_pipe[1]);
    setup_pipeline(my_pipe[0]);
  } else {
    close(my_pipe[0]);
    for (int i = 2; i <= 35; ++i) {
      write(my_pipe[1], &i, sizeof(i));
    }
  }
  exit(0);
}