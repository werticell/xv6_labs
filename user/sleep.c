#include "kernel/types.h" // uint usage in user.h
#include "user.h"


int main(int argc, char* argv []) {
  if (argc != 2) {
    // User forgot to pass time to sleep.
    printf("Sleep usage: sleep <time>");
    exit(1);
  }
  int ticks = atoi(argv[1]);
  int ret_status = sleep(ticks);
  exit(ret_status);
}