#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdlib.h>

void set_env_var(char *var, char *value);
void unset_env_var(char *var);
char *get_env_var(char *var);
void replace_env_vars(char *input);

#endif
