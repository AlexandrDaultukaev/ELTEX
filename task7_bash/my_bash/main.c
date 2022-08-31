#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

struct fd_pipe {
    int fd[2];
} typedef fd_pipe;

// #define PRINT_CMDS

void run_pipe(char*** cmds, int num_pipes) {
    fd_pipe fdesc[num_pipes+1];
    for(int i = 0; i <= num_pipes; i++) {
        if (pipe(fdesc[i].fd) == -1) {
            fprintf(stderr, "pipe error\n");
        }
    }
    int status = 0;
    pid_t pid;
    int i;
    for (i = 0; i <= num_pipes; i++) {
        pid = fork();
        if (!pid) {
            if(i != num_pipes) {
                dup2(fdesc[i].fd[0], 0);   //pipe_in <-- stdin
                dup2(fdesc[i+1].fd[1], 1); //stdout  --> pipe_out
            } else if(i != 0) {
                dup2(fdesc[i].fd[0], 0);
            }
            close(fdesc[i].fd[0]);
            close(fdesc[i].fd[1]);

            execvp(cmds[i][0], cmds[i]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        close(fdesc[i].fd[0]);
        close(fdesc[i].fd[1]);
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
}

int main(int argc, char* argv[]) {
    /* cmds представляет собой массив команд, разделенные каналами:
 * Пример: $ ls | grep ake | grep 135
 *
 * cmds = {    0        1
 *      0|    "ls",
 *      1|    "grep", "ake",
 *      2|    "grep", "135",
 *        }
 */
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
                // Если пробел сделан после | то ничего не делаем, иначе засчитываем как новую команду
                if(entire_command[i-1] != '|' && (isalpha(entire_command[i+1]) || entire_command[i+1] == '-')) {
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

        #ifdef PRINT_CMDS
        printf("row: %d\n", row);
        for(int i = 0; i <= row; i++) {
            printf("[ ");
            for(int j = 0; cmds[i][j] != NULL; j++) {

                printf("%s, ", cmds[i][j]);
                if(cmds[i][j+1] == NULL) {
                    printf("NULL ");
                }
            }
            printf(" ]\n");
        }
        #endif
        run_pipe(cmds, row);
        
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