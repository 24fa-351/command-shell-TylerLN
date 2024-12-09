#include "environment.h"

#include <stdio.h>
#include <string.h>

void set_env_var(char *var, char *value) {
  if (setenv(var, value, 1) == -1) {
    perror("setenv failed");
  }
}

void unset_env_var(char *var) {
  if (unsetenv(var) == -1) {
    perror("unsetenv failed");
  }
}

char *get_env_var(char *var) { return getenv(var); }

void replace_env_vars(char *input) {
  char *start = input;
  while ((start = strchr(start, '$')) != NULL) {
    char *end = strchr(start + 1, ' ');
    if (end == NULL) {
      end = start + strlen(start);
    }

    char var_name[256];
    strncpy(var_name, start + 1, end - start - 1);
    var_name[end - start - 1] = '\0';

    char *value = get_env_var(var_name);
    if (value != NULL) {
      size_t len = strlen(value);
      memmove(start + len, end, strlen(end) + 1);
      memcpy(start, value, len);
      start += len;
    } else {
      memmove(start, end, strlen(end) + 1);
      start = end;
    }
  }
}
