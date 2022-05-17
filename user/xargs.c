#include "kernel/types.h"
#include "kernel/param.h"

#include "user/user.h"

int read_args_from_input(char command_with_args[MAXARG][MAXARG], char *cmd_ptrs[MAXARG], int argc) {
  char input_args[1024];
  if (read(0, input_args, sizeof(input_args)) <= 0) {
    return 1;
  }
  int arg_id_to_fill = argc - 1;
  char *out_ptr = command_with_args[arg_id_to_fill];
  for (char *p = input_args; *p; ++p) {
    if (*p != ' ' && *p != '\n') {
      *out_ptr= *p;
      out_ptr += 1;
    } else {
      *out_ptr = '\0';
      arg_id_to_fill += 1;
      out_ptr = command_with_args[arg_id_to_fill];
    }
  }
  *out_ptr = '\0'; // terminate command argument
  cmd_ptrs[arg_id_to_fill + 1] = 0; // ensure that all other ptr a zerod after previous steps
  return 0;
}


int main(int argc, char *argv[]) {
  char command_with_args[MAXARG][MAXARG];
  char *cmd_ptrs[MAXARG];
  for (int i = 0; i < 32; ++i) {
    cmd_ptrs[i] = command_with_args[i];
  }

  for (int i = 1; i < argc; ++i) {
    strcpy(command_with_args[i - 1], argv[i]);
  }

  while (1) {
    if (read_args_from_input(command_with_args, cmd_ptrs, argc) != 0) {
      break;
    }
    if (fork()) {
      wait(0);
    } else {
      exec(argv[1], cmd_ptrs);
    }
  }
  exit(0);
}