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
#include <unistd.h>

#define SEND_BUFF_SIZE 128
#define RECV_BUFF_SIZE 156

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

int main() {
    signal(SIGINT, handle_shutdown);
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

    fgets(send_buff, SEND_BUFF_SIZE, stdin);

    if (connect(sock, (const struct sockaddr*)&server, sizeof(server)) < 0) {
        error("connect");
    }
    size = send(sock, send_buff, strlen(send_buff), 0);
    if (size < 0) {
        error("send");
    }

    if (recv(sock, recv_buff, RECV_BUFF_SIZE, 0) < 0) {
        error("recv");
    }
    recv_buff[strlen(recv_buff)] = '\0';

    printf("%s\n", recv_buff);

    return 0;
}
