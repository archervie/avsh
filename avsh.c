#include "builtins.h"
#include "exec.h"
#include "utils.h"
#include "avsh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int default_path_set = 1;
char *path[BUFF_SIZE] = {"/bin", NULL};

int main(int argc, char *argv[]) {
    int batch_mode = 0;
    FILE *input = NULL;

    switch (argc) {
    case 1:
        input = stdin;
        batch_mode = 0;
        break;

    case 2:
        input = fopen(argv[1], "r");
        if (input == NULL) {
            error();
            exit(EXIT_FAIL_CODE);
        }
        batch_mode = 1;
        break;

    default:
        error();
        exit(EXIT_FAIL_CODE);
    }

    char *line = NULL;
    size_t len = 0;

    while (1) {
        if (!batch_mode) {
            print_prompt();
        }

        ssize_t user_cmd = getline(&line, &len, input);
        if (user_cmd == -1) {
            break;
        }

        char *full_cmd_line = line;
        char *commands;
        int pids[BUFF_SIZE];
        int pid_count = 0;

        while ((commands = strsep(&full_cmd_line, "&")) != NULL) {
            char *line_tokens[BUFF_SIZE];
            char *command_copy = strdup(commands);
            char *orig_command_copy = command_copy;
            char *out_file = NULL;
            int redirection = 0;
            int right_token_count = 0;

            char *redirection_ptr_1 = strchr(command_copy, '>');
            char *redirection_ptr_2 = strrchr(command_copy, '>');
            char *redirection_ptr = NULL;

            if (redirection_ptr_1 != NULL && redirection_ptr_2 != NULL &&
                redirection_ptr_1 != redirection_ptr_2) {
                error();
                free(orig_command_copy);
                continue;
            } else {
                redirection_ptr = redirection_ptr_1;
            }

            if (redirection_ptr != NULL) {
                char *start = command_copy;
                while (*start == ' ' || *start == '\t') {
                    start++;
                }

                if (start == redirection_ptr) {
                    error();
                    free(orig_command_copy);
                    continue;
                }

                redirection = 1;
                *redirection_ptr = '\0';
                char *right_side = redirection_ptr + 1;
                char *right_side_token;
                while ((right_side_token = strsep(&right_side, " \t\n\r")) != NULL) {
                    if (*right_side_token != '\0') {
                        if (right_token_count == 0) {
                            out_file = right_side_token;
                        }
                        right_token_count++;
                    }
                }

                if (right_token_count != 1) {
                    error();
                    free(orig_command_copy);
                    continue;
                }
            }

            char *token;
            int token_count = 0;

            while ((token = strsep(&command_copy, " \t\n\r")) != NULL) {
                if (*token != '\0') {
                    line_tokens[token_count++] = token;
                }
            }
            line_tokens[token_count] = NULL;

            if (token_count == 0) {
                free(orig_command_copy);
                continue;
            } else if (strcmp(line_tokens[0], "exit") == 0) {
                if (token_count != 1) {
                    error();
                    free(orig_command_copy);
                    continue;
                }

                if (batch_mode) {
                    fclose(input);
                }

                free(line);
                free(orig_command_copy);
                path_cleanup(path);
                exit(EXIT_SUCCESS_CODE);

            } else if (strcmp(line_tokens[0], "path") == 0) {
                path_cmd(token_count, line_tokens, path);

            } else if (strcmp(line_tokens[0], "cd") == 0) {
                cd(token_count, line_tokens);

            } else {
                pid_t pid = exec_cmd(token_count, redirection, line_tokens, path, out_file);
                if (pid == -1) {
                    error();
                    free(orig_command_copy);
                    continue;
                } else if (pid > 0) {
                    pids[pid_count++] = pid;
                }
            }
            free(orig_command_copy);
        }

        for (int i = 0; i < pid_count; i++) {
            waitpid(pids[i], NULL, 0);
        }
    }

    if (batch_mode && input != NULL) {
        fclose(input);
    }

    free(line);
    path_cleanup(path);
    exit(EXIT_SUCCESS_CODE);
}
