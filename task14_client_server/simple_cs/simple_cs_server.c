#include <arpa/inet.h>
#include <errno.h>
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

int sock;
int sock_client;

void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    close(sock);
    close(sock_client);
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

int main() {
    signal(SIGINT, handle_shutdown);
    signal(SIGCHLD, reaper);
    struct sockaddr_in server;
    size_t size;
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        error("socket");
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
    if (listen(sock, 5) < 0) {
        error("listen");
    }
    while (1) {
        sock_client = accept(sock, 0, 0);
        if (sock_client < 0) {
            error("accept");
        }
        switch (fork()) {
            case -1:
                error("fork");
                break;
            case 0:
                size = recv(sock_client, recv_buff, RECV_BUFF_SIZE, 0);
                if (size == -1) {
                    error("size");
                }
                recv_buff[size - 1] = '\0';
                sprintf(send_buff, "OK![✔], MSG:[%s]\n", recv_buff);
                printf("%s\n", recv_buff);
                size = send(sock_client, send_buff, strlen(send_buff), 0);
                if (size < 0) {
                    error("send");
                }
                break;
            default:
                close(sock_client);
        }
    }

    return 0;
}