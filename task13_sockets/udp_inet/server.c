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

#define FILENAME_SIZE 108
#define BUFF_SIZE 128

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

int readable_timeo(int fd, int sec) {
    fd_set rset;
    struct timeval timeout;
    FD_ZERO(&rset);
    FD_SET(fd, &rset);

    timeout.tv_sec = 20;
    timeout.tv_usec = 0;
    return (select(fd + 1, &rset, NULL, NULL, &timeout));
}

void make_socket() {
    struct sockaddr_in name;
    size_t size;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }
    int flag = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1) {
        error("setsockopt fail");
    }

    name.sin_family = AF_INET;
    name.sin_addr.s_addr = inet_addr("127.0.0.1");
    name.sin_port = 7777;

    if (bind(sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
        error("bind");
    }
}

void server() {
    char recv_buff[BUFF_SIZE];
    char send_buff[] = "Hello!(from server)";
    struct sockaddr_in client;
    int n, len = sizeof(client);

    make_socket();
    while (1) {
        if (readable_timeo(sock, 40) == 0) {
            printf("Socket timeout\n");
            close(sock);
            exit(EXIT_SUCCESS);
        } else {
            n = recvfrom(sock, recv_buff, BUFF_SIZE, 0, (struct sockaddr*)&client, &len);
            if (n < 0) {
                error("recvfrom");
            }

            if (n > 0) {
                recv_buff[n] = '\0';
            }

            n = sendto(sock, send_buff, strlen(send_buff), MSG_CONFIRM, (struct sockaddr*)&client, len);
            if (n < 0) {
                error("sendto");
            }

            printf("RECEIVED: %s\n", recv_buff);
        }
    }
}

int main() {
    signal(SIGINT, handle_shutdown);
    server();
    return 0;
}