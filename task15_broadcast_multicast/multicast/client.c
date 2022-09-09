#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
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

void run_client() {
    char recv_buff[BUFF_SIZE];
    char send_buff[] = "Hi!(from client)";
    struct sockaddr_in server;
    int n, len;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = 7777;

    len = sizeof(server);
    struct ip_mreqn group;
    group.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    group.imr_address.s_addr = server.sin_addr.s_addr;
    group.imr_ifindex = 0;
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group)) == -1) {
        error("setsockopt fail");
    }
    if (bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        error("bind");
    }
    while (1) {
        n = recvfrom(sock, recv_buff, BUFF_SIZE, 0, (struct sockaddr*)&server, &len);
        if (n < 0) {
            error("recvfrom");
        }

        if (n > 0) {
            recv_buff[n] = '\0';
        }

        printf("RECEIVED: %s\n", recv_buff);
    }
}

int main() {
    signal(SIGINT, handle_shutdown);
    run_client();
    return 0;
}