#include "command.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "environment.h"

#define PATH_MAX 4096

int handle_internal_command(char *cmd, char *args[]) {
  if (strcmp(cmd, "cd") == 0) {
    if (args[1] == NULL) {
      fprintf(stderr, "cd: missing argument\n");
    } else if (chdir(args[1]) != 0) {
      perror("cd");
    }
    return 1;
  } else if (strcmp(cmd, "pwd") == 0) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("%s\n", cwd);
    } else {
      perror("pwd");
    }
    return 1;
  } else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
    exit(0);
  } else if (strcmp(cmd, "set") == 0) {
    if (args[1] != NULL && args[2] != NULL) {
      set_env_var(args[1], args[2]);
    } else {
      fprintf(stderr, "Usage: set <VAR> <VALUE>\n");
    }
    return 1;
  } else if (strcmp(cmd, "unset") == 0) {
    if (args[1] != NULL) {
      unset_env_var(args[1]);
    } else {
      fprintf(stderr, "Usage: unset <VAR>\n");
    }
    return 1;
  }

  return 0;
}
