#ifndef DIRECT_H
#define DIRECT_H

void redirect(const char *input_file, const char *output_file);
void split(const char *cmd, char *words[], char delimiter);
void background_redirect(char *cmd, char *args[], int pipefd[2], int in_fd,
                         int out_fd, const char *input_file,
                         const char *output_file);

#endif
