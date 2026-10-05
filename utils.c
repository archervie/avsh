#include "utils.h"
#include "avsh.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ANSI_COLOR_CYAN   "\x1b[36m"
#define ANSI_COLOR_GREEN  "\x1b[32m"
#define ANSI_COLOR_BOLD   "\x1b[1m"
#define ANSI_COLOR_RESET  "\x1b[0m"

extern int default_path_set;

void error(void) {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void print_prompt(void) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN "%s" ANSI_COLOR_GREEN " [avsh] > " ANSI_COLOR_RESET, cwd);
    } else {
        printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN "[avsh] > " ANSI_COLOR_RESET);
    }
    fflush(stdout);
}

void path_cleanup(char *path[]) {
    for (int i = 0; i < BUFF_SIZE; i++) {
        if (path[i] != NULL) {
            if (!default_path_set) {
                free(path[i]);
            }
            path[i] = NULL;
        } else {
            break;
        }
    }
}

char *confirm_path(char *line_tokens[], char *path[]) {
    char *cmd = line_tokens[0];

    for (int i = 0; i < BUFF_SIZE; i++) {
        if (path[i] != NULL) {
            char *full_path = malloc(strlen(path[i]) + strlen(cmd) + 2);
            if (!full_path) {
                return NULL;
            }
            sprintf(full_path, "%s/%s", path[i], cmd);

            if (access(full_path, X_OK) == 0) {
                return full_path;
            }
            free(full_path);
        }
    }
    return NULL;
}
