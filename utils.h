#ifndef UTILS_H
#define UTILS_H

void error(void);
void path_cleanup(char *path[]);
void print_prompt(void);
char *confirm_path(char *line_tokens[], char *path[]);

#endif
