#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "direct.h"
#include "environment.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    int background;

    while (1) {
        printf("xsh# ");
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        replace_env_vars(input);

        char *pipe_cmds[MAX_ARGS];
        split(input, pipe_cmds, '|');

        int pipefd[2];
        int prev_fd = -1;

        for (int ix = 0; pipe_cmds[ix] != NULL; ++ix) {
            char *subcmd_args[MAX_ARGS];
            char *input_file = NULL, *output_file = NULL;

            char *redir_cmd = strtok(pipe_cmds[ix], ">");
            if (redir_cmd != NULL) {
                subcmd_args[0]   = strdup(redir_cmd);
                char *redir_rest = strtok(NULL, ">");
                if (redir_rest != NULL)
                    output_file = strtok(redir_rest, " ");
            }

            if (input_file)
                input_file = strtok(pipe_cmds[ix], "<");

            split(pipe_cmds[ix], subcmd_args, ' ');

            background = 0;
            if (subcmd_args[0] != NULL && strcmp(subcmd_args[0], "&") == 0) {
                background     = 1;
                subcmd_args[0] = NULL;
            }

            if (directory_command(subcmd_args[0], subcmd_args))
                continue;

            if (pipe_cmds[ix + 1] != NULL)
                pipe(pipefd);

            background_redirect(subcmd_args[0], subcmd_args, pipefd, prev_fd,
                                pipefd[1], input_file, output_file);

            close(pipefd[1]);
            prev_fd = pipefd[0];

            if (!background)
                wait(NULL);
        }
    }
    return 0;
}