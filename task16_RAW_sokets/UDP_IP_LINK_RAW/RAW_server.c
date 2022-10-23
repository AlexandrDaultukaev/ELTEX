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

void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    close(sock);

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
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }

    server.sin_addr.s_addr = htonl(INADDR_ANY);  // inet_addr("192.168.1.4");
    server.sin_family = AF_INET;
    server.sin_port = htons(7778);
    int reuseaddr_on = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                   &reuseaddr_on, sizeof(reuseaddr_on)) < 0) {
        error("setsockopt");
    }

    if (bind(sock, (const struct sockaddr*)&server, sizeof(server)) < 0) {
        error("bind");
    }
    struct sockaddr_in client;
    socklen_t client_size = sizeof(client);
    while (1) {
        size = recvfrom(sock, recv_buff, RECV_BUFF_SIZE, 0, (struct sockaddr*)&client, &client_size);
        if (size == -1) {
            error("size");
        }

        // recv_buff[size - 1] = '\0';
        printf("RECV: %s\n", recv_buff);
        sprintf(send_buff, "OK![✔], MSG:%s\n", recv_buff);
        printf("TO SEND:%s", send_buff);
        size = sendto(sock, send_buff, sizeof(send_buff), 0, (struct sockaddr*)&client, client_size);
        if (size < 0) {
            error("send");
        }
        }

    return 0;
}