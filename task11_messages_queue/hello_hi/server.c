#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_QUEUE_NAME "/eltex-homework-hello-hi"
#define QUEUE_PERIMISSIONS 0660  // r+w for owner & group
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 255
#define MSG_BUFFER_SIZE 265  // a little larger than MAX_MSG_SIZE

void handle_shutdown(int sig)
{

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
    char client_queue_name[MSG_BUFFER_SIZE];
    char in_buffer[MSG_BUFFER_SIZE];
    char out_buffer[] = "hi";
    mqd_t server, client;
    struct mq_attr attr;
    init_attr(&attr);

    if ((server = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERIMISSIONS, &attr)) == -1) {
        perror("Server: mq_open");
        exit(1);
    }

    while (getchar() != 'q') {
        if (mq_receive(server, client_queue_name, MSG_BUFFER_SIZE, NULL) == -1) {
            perror("Server: mq_receive name");
            exit(1);
        }

        if (mq_receive(server, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror("Server: mq_receive in_buffer");
            exit(1);
        }

        printf("SERVER: from client(%s): %s\n", client_queue_name, in_buffer);

        if ((client = mq_open(client_queue_name, O_WRONLY)) == -1) {
            perror("Server: mq_open(cli)");
            exit(1);
        }

        if (mq_send(client, out_buffer, strlen(out_buffer) + 1, 0) == -1) {
            perror("Server: send");
            continue;
        }
        getchar(); // getchar in while keeps \n
    }

    mq_unlink(SERVER_QUEUE_NAME);
    mq_close(client);
    mq_close(server);

    return 0;
}
