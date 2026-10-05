#ifndef EXEC_H
#define EXEC_H

#include <sys/types.h>

pid_t exec_cmd(int token_count, int redirection, char *line_tokens[],
               char *path[], char *out_file);

#endif
