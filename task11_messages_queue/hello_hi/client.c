#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_QUEUE_NAME "/eltex-homework-hello-hi"
#define QUEUE_PERIMISSIONS 0660  // r+w for owner & group
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 128
#define MSG_BUFFER_SIZE 265  // a little larger than MAX_MSG_SIZE

char client_queue_name[128];

void handle_shutdown(int sig) {
    printf("exit\n");
    mq_unlink(client_queue_name);
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
    char in_buffer[MSG_BUFFER_SIZE];
    char out_buffer[] = "hello";

    sprintf(client_queue_name, "/client-queue-%d", getpid());

    mqd_t server, client;

    struct mq_attr attr;
    init_attr(&attr);

    if ((server = mq_open(SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
        perror("Client: mq_open");
        exit(1);
    }

    if ((client = mq_open(client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERIMISSIONS, &attr)) == -1) {
        perror("Client: mq_open(cli)");
        exit(1);
    }

    while (getchar() != 'q') {
        if (mq_send(server, client_queue_name, strlen(client_queue_name) + 1, 0) == -1) {
            perror("Server: send");
            continue;
        }

        if (mq_send(server, out_buffer, strlen(out_buffer) + 1, 0) == -1) {
            perror("Server: send");
            continue;
        }

        if (mq_receive(client, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror("Server: mq_receive in_buffer");
            exit(1);
        }

        printf("CLIENT: from server: %s\n", in_buffer);
        getchar();
    }

    mq_unlink(client_queue_name);
    mq_close(client);
    mq_close(server);

    return 0;
}
