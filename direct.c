#include "direct.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void redirect(const char *input_file, const char *output_file) {
  if (input_file && strcmp(input_file, "-") != 0) {
    int fdin = open(input_file, O_RDONLY);
    if (fdin < 0) {
      perror("Failed to open input file");
      exit(EXIT_FAILURE);
    }
    dup2(fdin, STDIN_FILENO);
    close(fdin);
  }

  if (output_file && strcmp(output_file, "-") != 0) {
    int fdout = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdout < 0) {
      perror("Failed to open output file");
      exit(EXIT_FAILURE);
    }
    dup2(fdout, STDOUT_FILENO);
    close(fdout);
  }
}

void split(const char *cmd, char *words[], char delimiter) {
  int word_count = 0;
  char current_word[10000] = "";
  const char *next_char = cmd;

  while (*next_char != '\0') {
    if (*next_char == delimiter) {
      if (strlen(current_word) > 0) {
        words[word_count++] = strdup(current_word);
        current_word[0] = '\0';
      }
    } else {
      strncat(current_word, next_char, 1);
    }
    ++next_char;
  }

  if (strlen(current_word) > 0) {
    words[word_count++] = strdup(current_word);
  }
  words[word_count] = NULL;
}

void handle_redirection_pipe_background(char *cmd, char *args[], int pipefd[2],
                                        int in_fd, int out_fd,
                                        const char *input_file,
                                        const char *output_file) {
  pid_t pid = fork();
  if (pid == 0) {
    redirect(input_file, output_file);

    if (in_fd != -1) {
      dup2(in_fd, STDIN_FILENO);
    }
    if (out_fd != -1) {
      dup2(out_fd, STDOUT_FILENO);
    }

    close(pipefd[0]);
    execvp(cmd, args);
    perror("execvp failed");
    exit(EXIT_FAILURE);
  }
}
