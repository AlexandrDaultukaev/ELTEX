#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int status;
    char city[20];
    scanf("%19s", city);
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Can't clone process: %s\n", strerror(errno));
    }
    // Available cities: Novosibirsk, Berdsk, Moscow, St Petersburg
    char* my_args[] = {"python", "script.py", city, NULL};
    if (!pid) {
        int res = execvp("python", my_args);
        if(res == -1)
            fprintf(stderr, "Execvp failed: %s\n", strerror(errno));
        exit(EXIT_SUCCESS);
    } else {
        wait(&status);
    }
    return 0;
}