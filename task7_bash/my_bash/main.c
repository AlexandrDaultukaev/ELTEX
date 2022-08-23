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
        // close(fd[1]);
        // dup2(fd[0], 0);
        wait(&status);
    }
}

void run_pipe(char** cmd1, char** cmd2) {
    printf("PIPE: %s, %s\n", cmd1[0], cmd2[0]);
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "pipe error\n");
    }
    int status;
    pid_t pid1 = fork();
    if (!pid1) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execvp(cmd1[0], cmd1);
    }

    pid_t pid2 = fork();
    if (!pid2) {
                dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execvp(cmd2[0], cmd2);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

int main(int argc, char* argv[]) {
    char* my_args[20] = {[0 ... 19] = NULL};
    char* cmd1[20] = {[0 ... 19] = NULL};
    char* cmd2[20] = {[0 ... 19] = NULL};
    char command[20];
    int pipe_pos = -1;
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
                pipe_pos = num_args;
            }
            strcpy(my_args[num_args++], token);
            token = strtok(NULL, separator);
        } while (token != NULL && spaces_num > 0);

        /* Удаляем \n из строк, т.к. fgets добавляет его */
        my_args[num_args - 1][strcspn(my_args[num_args - 1], "\n")] = 0;
        command[strcspn(command, "\n")] = 0;

        for (int i = 0; i < pipe_pos; i++) {
            cmd1[i] = my_args[i];
        }

        if (pipe_pos != 19) {
            int j = 0;
            for (int i = pipe_pos + 1; i < 20; i++) {
                cmd2[j++] = my_args[i];
            }
        }
        if (pipe_pos != -1) {
            run_pipe(cmd1, cmd2);
        } else {
            run_process(my_args);
        }
        pipe_pos = -1;
        for (int i = 0; i < num_args; i++) {
            free(my_args[i]);
            my_args[i] = NULL;
            cmd1[i] = NULL;
            cmd2[i] = NULL;
        }
    }
    return 0;
}