#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int status;
    char* my_args[20] = {[0 ... 19] = NULL};
    char command[20];
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
            strcpy(my_args[num_args++], token);
            token = strtok(NULL, separator);
        } while (token != NULL && spaces_num > 0);
        my_args[num_args - 1][strcspn(my_args[num_args - 1], "\n")] = 0;
        command[strcspn(command, "\n")] = 0;

        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Can't clone process: %s\n", strerror(errno));
        }

        if (!pid) {
            int res = execvp(command, my_args);
            if (res == -1)
                fprintf(stderr, "Execvp failed: %s\n", strerror(errno));
            exit(EXIT_SUCCESS);
        } else {
            wait(&status);
        }
        for (int i = 0; i < num_args; i++) {
            free(my_args[i]);
            my_args[i] = NULL;
        }
    }
    return 0;
}