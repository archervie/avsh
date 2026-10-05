#include "exec.h"
#include "utils.h"
#include "avsh.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

pid_t exec_cmd(int token_count, int redirection, char *line_tokens[],
               char *path[], char *out_file) {
    char *args[token_count + 1];
    args[token_count] = NULL;

    for (int i = 0; i < token_count; i++) {
        args[i] = line_tokens[i];
    }

    char *full_path = confirm_path(line_tokens, path);
    if (full_path == NULL) {
        return -1;
    }

    pid_t rc = fork();

    if (rc == 0) {
        if (redirection) {
            int fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                error();
                exit(EXIT_FAIL_CODE);
            }
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        execv(full_path, args);

        free(full_path);
        path_cleanup(path);
        error();
        exit(EXIT_FAIL_CODE);
    } else {
        free(full_path);
        return rc;
    }
}
