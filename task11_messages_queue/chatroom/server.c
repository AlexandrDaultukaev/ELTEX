#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SERVER_QUEUE_NAME "/eltex-homework-hello-hi"
#define QUEUE_PERIMISSIONS 0660  // r+w for owner & group
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 128
#define MSG_BUFFER_SIZE 138  // a little larger than MAX_MSG_SIZE

void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    mq_unlink(SERVER_QUEUE_NAME);
    exit(0);
}

void init_attr(struct mq_attr* attr) {
    attr->mq_flags = 0;
    attr->mq_maxmsg = MAX_MESSAGES;
    attr->mq_msgsize = MAX_MSG_SIZE;
    attr->mq_curmsgs = 0;
}

int main() {
    signal(SIGINT, handle_shutdown);

    char* client_queue_name[MSG_BUFFER_SIZE];
    int client_queue_idx = 0;

    char in_buffer[MSG_BUFFER_SIZE];

    mqd_t server, client;
    struct mq_attr attr;
    init_attr(&attr);
    int i;
    if ((server = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERIMISSIONS, &attr)) == -1) {
        perror("Server: mq_open");
        exit(1);
    }

    while (1) {
        if (mq_receive(server, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror("Server: mq_receive name");
            exit(1);
        }
        if (in_buffer[0] == '/') {
            client_queue_name[client_queue_idx] = malloc(sizeof(char) * MSG_BUFFER_SIZE);
            strcpy(client_queue_name[client_queue_idx], in_buffer);
            client_queue_idx++;
        } else {
            for (i = 0; i < client_queue_idx; i++) {
                if ((client = mq_open(client_queue_name[i], O_WRONLY)) == -1) {
                    if (strcmp(strerror(errno), "No such file or directory") == 0) {
                        printf("ye");
                        continue;
                    }
                    perror("Server: mq_open(cli)");
                    exit(1);
                }
                if (mq_send(client, in_buffer, strlen(in_buffer) + 1, 0) == -1) {
                    perror("Server: send");
                    continue;
                }
                mq_close(client);
            }
        }

        // printf("SERVER: from client(%s): %s\n", client_queue_name, in_buffer);
    }

    mq_unlink(SERVER_QUEUE_NAME);
    mq_close(server);

    return 0;
}
