#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void read_from_file(int fd) {
    FILE* stream;
    char c = '1';
    stream = fdopen(fd, "r");
    while ((c = fgetc(stream)) != EOF)
        putchar(c);
    fclose(stream);
}

void write_to_file(int fd) {
    FILE* stream;
    stream = fdopen(fd, "w");
    fprintf(stream, "Gira, il mondo gira\n");
    fprintf(stream, "Nello spazio senza fine\n");
    fprintf(stream, "Con gli amori appena nati\n");
    fprintf(stream, "Con gli amori già finiti\n");
    fprintf(stream, "Con la gioia e col dolore\n");
    fprintf(stream, "Della gente come me\n");
    fclose(stream);
}

int main() {
    pid_t pid;
    int mypipe[2];
    if (pipe(mypipe)) {
        fprintf(stderr, "Pipe error\n");
        return EXIT_FAILURE;
    }

    pid = fork();

    if (pid == (pid_t)0) {
        /* Child process */
        close(mypipe[1]);
        read_from_file(mypipe[0]);
        return EXIT_SUCCESS;
    } else if (pid < (pid_t)0) {
        fprintf(stderr, "Fork failed\n");
        return EXIT_FAILURE;
    } else {
        /* Parent process */
        close(mypipe[0]);
        write_to_file(mypipe[1]);
        return EXIT_SUCCESS;
    }
    return 0;
}