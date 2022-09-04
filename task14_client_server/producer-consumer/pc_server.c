#include <arpa/inet.h>
#include <errno.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEND_BUFF_SIZE 156
#define RECV_BUFF_SIZE 128
#define SERVER_QUEUE_NAME "/eltex-homework-pc"
#define QUEUE_PERIMISSIONS 0660  // r+w for owner & group
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 128
#define MSG_BUFFER_SIZE 138  // a little larger than MAX_MSG_SIZE
#define THRD_NUM 3

int sock;
int sock_client;

void reverse(char s[]) {
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[]) {
    int i, sign;

    if ((sign = n) < 0) /* записываем знак */
        n = -n;         /* делаем n положительным числом */
    i = 0;
    do {                       /* генерируем цифры в обратном порядке */
        s[i++] = n % 10 + '0'; /* берем следующую цифру */
    } while ((n /= 10) > 0);   /* удаляем */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void init_attr(struct mq_attr* attr) {
    attr->mq_flags = 0;
    attr->mq_maxmsg = MAX_MESSAGES;
    attr->mq_msgsize = MAX_MSG_SIZE;
    attr->mq_curmsgs = 0;
}

void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    close(sock);
    close(sock_client);
    mq_unlink(SERVER_QUEUE_NAME);
    exit(EXIT_SUCCESS);
}

void error(char* msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

void reaper(int sig) {
    int status;
    while (wait3(&status, WNOHANG, (struct rusage*)0) >= 0)
        ;
}

void* thread_work(void* input) {
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];
    int socket;
    char socket_ch[MSG_BUFFER_SIZE];
    size_t size;
    mqd_t mqueue;
    if ((mqueue = mq_open(SERVER_QUEUE_NAME, O_RDONLY)) == -1) {
        perror("Server: mq_open");
        exit(1);
    }
    while (1) {
        if (mq_receive(mqueue, socket_ch, sizeof(socket_ch), NULL) == -1) {
            perror("Server: mq_receive name");
            exit(1);
        }
        socket = atoi(socket_ch);
        size = recv(socket, recv_buff, RECV_BUFF_SIZE, 0);
        recv_buff[size - 1] = '\0';
        sprintf(send_buff, "OK![✔][tid:%ld], MSG:[%s]\n", pthread_self(), recv_buff);
        printf("[tid:%ld]: %s\n", pthread_self(), recv_buff);
        size = send(socket, send_buff, strlen(send_buff), 0);
        if (size < 0) {
            error("send");
        }
        close(socket);
    }
}

int main() {
    signal(SIGINT, handle_shutdown);
    signal(SIGCHLD, reaper);
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];
    int new_sock;
    struct sockaddr_in server;
    struct mq_attr attr;
    size_t size;
    mqd_t mqueue;
    pid_t pid1, pid2, pid3;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        error("socket");
    }
    init_attr(&attr);
    if ((mqueue = mq_open(SERVER_QUEUE_NAME, O_WRONLY | O_CREAT, QUEUE_PERIMISSIONS, &attr)) == -1) {
        perror("Server: mq_open");
        exit(1);
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = 7777;
    int reuseaddr_on = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                   &reuseaddr_on, sizeof(reuseaddr_on)) < 0) {
        error("setsockopt");
    }

    if (bind(sock, (const struct sockaddr*)&server, sizeof(server)) < 0) {
        error("bind");
    }
    if (listen(sock, 3) < 0) {
        error("listen");
    }
    pthread_t child[THRD_NUM];

    int status = 0;
    for (int i = 0; i < THRD_NUM; i++) {
        status = pthread_create(&(child[i]), NULL, thread_work, NULL);
        if (status < 0) {
            error("pthread_create");
        }
    }

    int idx_proc;
    char sock_ch[MAX_MSG_SIZE];
    while (1) {
        new_sock = accept(sock, 0, 0);
        if (new_sock < 0) {
            error("accept");
        }
        itoa(new_sock, sock_ch);
        if (mq_send(mqueue, sock_ch, sizeof(sock_ch), 0) < 0) {
            error("mq_send");
        }
    }
    return 0;
}