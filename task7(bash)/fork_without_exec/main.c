#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int status;
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Can't clone process: %s\n", strerror(errno));
    }

    if (!pid) {
        printf("child pid: %d\n", getpid());
        printf("child ppid: %d\n", getppid());
        exit(EXIT_SUCCESS);
    } else {
        printf("parent pid: %d\n", getpid());
        printf("parent ppid: %d\n", getppid());
        wait(&status);
    }
    return 0;
}