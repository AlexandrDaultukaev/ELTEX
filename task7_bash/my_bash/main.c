#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

/* Функция без использования pipe */
// void run_process(char** cmd1) {
//     int status;
//     pid_t pid = fork();
//     if (pid == -1) {
//         fprintf(stderr, "Can't clone process: %s\n", strerror(errno));
//     }

//     if (!pid) {
//         int res = execvp(cmd1[0], cmd1);
//         if (res == -1)
//             fprintf(stderr, "Execvp failed: %s\n", strerror(errno));
//         exit(EXIT_SUCCESS);
//     } else {
//         wait(&status);
//     }
// }

// /* cmds представляет собой массив команд, разделенные каналами:
//  * Пример: $ ls | grep ake | grep 135
//  *
//  * cmds = {    0        1
//  *      0|    "ls",
//  *      1|    "grep", "ake",
//  *      2|    "grep", "135",
//  *        }

// /* Функция с использованием pipe */
// void run_pipe(char* cmds[20][20], int num_pipes) {
//     int fd[2];
//     if (pipe(fd) == -1) {
//         fprintf(stderr, "pipe error\n");
//     }
//     int status = 0;
//     pid_t wpid;
//     int row = 0;
//     pid_t* pid = malloc(sizeof(pid_t) * num_pipes);
//     for (int i = 0; i <= num_pipes; i++) {
//         pid[i] = fork();
//         if (!pid[i]) {
//             dup2(fd[0], 0);
//             if (i != num_pipes) {
//                 dup2(fd[1], 1);
//             }
//             close(fd[0]);
//             close(fd[1]);

//             execvp(cmds[row][0], cmds[row]);
//             perror("execvp");
//             exit(EXIT_FAILURE);
//         }
//         row++;
//     }
//     close(fd[0]);
//     close(fd[1]);

//     do {
//         for (int i = 0; i < num_pipes; i++) {
//             waitpid(pid[i], &status, WUNTRACED);
//         }
//     } while (!WIFEXITED(status) && !WIFSIGNALED(status));
//     free(pid);
// }

int main(int argc, char* argv[]) {
    /* my_args - одномерный массив команд = {"ls", "grep", "..."} */
    char*** cmds;

    int row;
    int ch_num;
    int command_num;

    /* entire_command - вся команда посимвольно: {'l', 's', ' ', '|', ...} */
    char entire_command[128];

    while(1) {
        row = 0;
        command_num = 0;
        ch_num = 0;
        printf("\n>>> ");
        fgets(entire_command, 128, stdin);
        if(strcmp(entire_command, "exit\n") == 0) {
            break;
        }
        if(entire_command[0] == ' ' || entire_command[0] == '\n' || entire_command[0] == '\0') {
            printf("Unexpected space at beginning of command\n");
            continue;
        }
        cmds = (char***)malloc(sizeof(char**) * 20);
        cmds[row] = (char**)malloc(sizeof(char*) * 20);
        cmds[row][command_num] = (char*)malloc(sizeof(char) * 20);
        for(int i = 0; i < 128; i++) {
            if(entire_command[i] == ' ') {
                cmds[row][command_num][ch_num] = '\0';
                if(entire_command[i-1] != '|' && isalpha(entire_command[i+1])) {
                    cmds[row][++command_num] = (char*)malloc(sizeof(char) * 20);
                }
                ch_num = 0;
            } else if(entire_command[i] == '\0' || entire_command[i] == '\n') {
                cmds[row][command_num][ch_num] = '\0';
                cmds[row][++command_num] = NULL;
                break;
            } else if(entire_command[i] == '|') {
                cmds[row++][++command_num] = NULL;
                command_num = 0;
                ch_num = 0;
                cmds[row] = (char**)malloc(sizeof(char*) * 20);
                cmds[row][command_num] = malloc(sizeof(char) * 20);
            } else {
                cmds[row][command_num][ch_num++] = entire_command[i];
            }
        }
        printf("row: %d\n", row);
        for(int i = 0; i <= row; i++) {
            printf("[ ");
            for(int j = 0; cmds[i][j] != NULL; j++) {

                printf("%s, ", cmds[i][j]);
            }
            printf(" ]\n");
        }
        for(int i = 0; i <= row; i++) {
            for(int j = 0; cmds[i][j] != NULL; j++) {
                free(cmds[i][j]);
                cmds[i][j] = NULL;
            }
            free(cmds[i]);
            
        }
        free(cmds);
    }
    return 0;
}