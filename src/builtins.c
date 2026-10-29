#include "builtins.h"
#include "utils.h"
#include <string.h>
#include <unistd.h>

extern int default_path_set;

int cd(int token_count, char *line_tokens[]) {
    if (token_count != 2) {
        error();
        return 1;
    }

    if (chdir(line_tokens[1]) != 0) {
        error();
        return 1;
    }
    return 0;
}

int path_cmd(int token_count, char *line_tokens[], char *path[]) {
    path_cleanup(path);
    default_path_set = 0;

    for (int i = 0; i < token_count - 1; i++) {
        path[i] = strdup(line_tokens[i + 1]);
    }
    path[token_count - 1] = NULL;
    return 0;
}
