#ifndef SHELL_H
#define SHELL_H

#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFF_SIZE 256
#define EXIT_SUCCESS 0
#define EXIT_FAIL 1

extern char *path[BUFF_SIZE];
extern int default_path_set;

void error(void);
void path_cleanup(char *path[]);
int cd(int token_count, char *line_tokens[]);
int path_cmd(int token_count, char *line_tokens[], char *path[]);

char *confirm_path(char *line_tokens[], char *path[]);
int exec_cmd(int token_count, int redirection, char *line_tokens[],
             char *path[], char *out_file);

#endif
