#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void run_process(char** cmd1) {
    int status;
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Can't clone process: %s\n", strerror(errno));
    }

    if (!pid) {
        int res = execvp(cmd1[0], cmd1);
        if (res == -1)
            fprintf(stderr, "Execvp failed: %s\n", strerror(errno));
        exit(EXIT_SUCCESS);
    } else {
        wait(&status);
    }
}

void run_pipe(char* cmds[20][20], int num_pipes) {
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "pipe error\n");
    }
    int row = 0;
    pid_t pid1;
    pid_t pid2;
    for (int i = 0; i <= num_pipes; i++) {
        pid1 = fork();
        if (!pid1) {
            dup2(fd[0], 0);
            if (i != num_pipes) {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            close(fd[1]);

            execvp(cmds[row][0], cmds[row]);
        }
        row++;
    }

    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

int main(int argc, char* argv[]) {
    char* my_args[20] = {[0 ... 19] = NULL};
    char* cmds[20][20] = {[0 ... 19] = NULL};
    char command[20];
    int flag = 1;
    int pipe_pos[10] = {[0 ... 9] = -1};
    int pipe_idx = 0;
    char entire_command[128];
    char separator[] = " ";
    while (strcmp(command, "exit") != 0) {
        int num_args = 0;
        int spaces_num = 0;
        printf(">>> ");
        fgets(entire_command, 128, stdin);

        if (strcmp(entire_command, "exit\n") == 0) {
            printf("exit");
            exit(EXIT_SUCCESS);
        }

        for (int i = 0; entire_command[i] != '\0'; i++) {
            if (entire_command[i] == ' ') {
                spaces_num++;
            }
        }
        char* token = strtok(entire_command, separator);
        if (token != NULL) {
            strcpy(command, token);
        }
        do {
            my_args[num_args] = malloc(sizeof(char) * 20);
            if (strcmp(token, "|") == 0) {
                pipe_pos[pipe_idx++] = num_args;
                flag = 0;
            }
            strcpy(my_args[num_args++], token);
            token = strtok(NULL, separator);
        } while (token != NULL && spaces_num > 0);
        my_args[num_args] = NULL;
        /* Удаляем \n из строк, т.к. fgets добавляет его */
        my_args[num_args - 1][strcspn(my_args[num_args - 1], "\n")] = 0;
        command[strcspn(command, "\n")] = 0;
        int p_idx = pipe_idx;
        int cmd_begin = 0;
        int row = 0;

        int command_num = 0;
        int arg_num = 0;
        for (int i = 0; i <= pipe_idx; i++) {
            if (pipe_pos[i] == -1 && !flag) {
                pipe_pos[i] = num_args;
                flag = 1;
            }
            for (int j = cmd_begin; j < pipe_pos[i]; j++) {
                cmds[row][command_num++] = my_args[arg_num++];
            }
            cmd_begin = pipe_pos[i] + 1;
            arg_num++;
            row++;
            command_num = 0;
        }
        if (pipe_pos[0] != -1) {
            run_pipe(cmds, pipe_idx);
        } else {
            run_process(my_args);
        }

        for (int i = 0; i < num_args; i++) {
            free(my_args[i]);
            my_args[i] = NULL;
        }
        for (int i = 0; i < 20; i++) {
            pipe_pos[i] = -1;
            for (int j = 0; j < 20; j++) {
                cmds[i][j] = NULL;
            }
        }
        pipe_idx = 0;
    }
    return 0;
}