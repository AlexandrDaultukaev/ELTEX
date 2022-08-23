#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Функция без использования pipe */
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


/* cmds представляет собой массив команд, разделенные каналами:
 * Пример: $ ls | grep ake | grep 135
 * 
 * cmds = {    0        1
 *      0|    "ls",
 *      1|    "grep", "ake",
 *      2|    "grep", "135",   
 *        }

/* Функция с использованием pipe */
void run_pipe(char* cmds[20][20], int num_pipes) {
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "pipe error\n");
    }
    int status = 0;
    pid_t wpid;
    int row = 0;
    pid_t* pid = malloc(sizeof(pid_t) * num_pipes);
    for (int i = 0; i <= num_pipes; i++) {
        pid[i] = fork();
        if (!pid[i]) {
            dup2(fd[0], 0);
            if (i != num_pipes) {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            close(fd[1]);

            execvp(cmds[row][0], cmds[row]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        row++;
    }
    close(fd[0]);
    close(fd[1]);
    free(pid);
    do {
        for (int i = 0; i < num_pipes; i++) {
            waitpid(pid[i], &status, WUNTRACED);
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

int main(int argc, char* argv[]) {

    /* my_args - одномерный массив команд = {"ls", "grep", "..."} */
    char* my_args[20] = {[0 ... 19] = NULL};
    char* cmds[20][20] = {[0 ... 19] = NULL};

    /* command - хранится первая команда, для сравнения с exit */
    char command[20];

    /* flag позволяет понять, когда каналов(pipes) больше не будет дальше
     * и можно будет читать оставшиеся команды до конца my_args */
    int flag = 1;

    /* pipe_pos хранит все позиции pipe-ов в команде */
    /* pipe_idx является индексом, куда будет записан следующая позиция pipe */
    int pipe_pos[10] = {[0 ... 9] = -1};
    int pipe_idx = 0;

    /* entire_command - вся команда посимвольно: {'l', 's', ' ', '|', ...} */
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
                
                /* Не делаем лишнюю копию, а просто храним указатель на команду my_args */
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

        /* На всякий случай делаем указатели NULL
         * Так как cmds нигде не выделяет память, то
         * освобождать ничего не нужно */
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