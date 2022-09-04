#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BUFF 1024

int listenfd;
int udp_fd;
int client[FD_SETSIZE];
int maxi;
void handle_shutdown(int sig) {
    for (int i = 0; i <= maxi; i++) {
        if (client[i] != -1) {
            printf("\nClient socket closed(socket: %d)\n", client[i]);
            close(client[i]);
        }
    }
    close(listenfd);
    close(udp_fd);
    printf("\nServer socket closed\n");
    exit(0);
}

void error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    signal(SIGINT, handle_shutdown);
    int maxfd, connfd, sockfd;
    int select_ret;
    ssize_t n;
    fd_set rset, allset;
    char line[MAX_BUFF];
    char send_buff[] = "OK![✔]\n";
    struct sockaddr_in servaddr_TCP, servaddr_UDP;
    struct sockaddr_in client_UDP;
    socklen_t len_client_UPD = sizeof(client_UDP);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr_TCP.sin_family = AF_INET;
    servaddr_TCP.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr_TCP.sin_port = 7777;

    int reuseaddr_on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                   &reuseaddr_on, sizeof(reuseaddr_on)) < 0) {
        error("setsockopt_tcp");
    }

    if (bind(listenfd, (struct sockaddr *)&servaddr_TCP, sizeof(servaddr_TCP)) < 0) {
        error("bind_tcp");
    }

    udp_fd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr_UDP.sin_family = AF_INET;
    servaddr_UDP.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr_UDP.sin_port = 7778;

    if (setsockopt(udp_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on, sizeof(reuseaddr_on)) < 0) {
        error("setsockopt_udp");
    }

    if (bind(udp_fd, (struct sockaddr *)&servaddr_UDP, sizeof(servaddr_UDP)) < 0) {
        error("bind_udp");
    }

    unsigned int length = sizeof(servaddr_TCP);

    listen(listenfd, 5);

    maxfd = (listenfd > udp_fd) ? listenfd : udp_fd;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    FD_SET(udp_fd, &allset);
    for (;;) {
        rset = allset;
        select_ret = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (select_ret < 0) {
            error("select");
        }
        if (FD_ISSET(udp_fd, &rset)) {
            printf("UDP readable\n");
            n = recvfrom(udp_fd, line, MAX_BUFF, 0, (struct sockaddr *)&client_UDP, &len_client_UPD);
            if (n < 0) {
                error("recvfrom");
            }

            line[n] = '\0';

            n = sendto(udp_fd, send_buff, strlen(send_buff), MSG_CONFIRM, (struct sockaddr *)&client_UDP, len_client_UPD);
            if (n < 0) {
                error("sendto");
            }

            printf("RECEIVED(UDP): %s\n", line);
        } else if (FD_ISSET(listenfd, &rset)) {
            printf("TCP readable\n");

            connfd = accept(listenfd, 0, 0);

            if ((n = recv(connfd, &line, MAX_BUFF, 0)) == 0) {
                printf("\nClient socket closed(socket: %d)\n", sockfd);
                close(connfd);
            }

            if (n < 0) {
                error("recv_TCP");
            }

            line[n] = '\0';

            printf("RECEIVED(TCP): %s\n", line);

            if (send(connfd, send_buff, strlen(send_buff), 0) < 0) {
                error("send_TCP");
            }

            close(connfd);
        }
    }
}
