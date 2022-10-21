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

#define SEND_BUFF_SIZE 128
#define RECV_BUFF_SIZE 156
#define HEADER_SIZE 8

int sock;
void udp_insert(char* dest, char* udp_header, char* src) {
    int i = 0, j = 0;
    for (; i < HEADER_SIZE; i++) {
        dest[i] = udp_header[i];
    }

    for (int j = 0; src[j] != '\0'; j++, i++) {
        dest[i] = src[j];
    }
}
void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    close(sock);

    exit(EXIT_SUCCESS);
}

void error(char* msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

int main() {
    signal(SIGINT, handle_shutdown);
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(7777);
    size_t size;
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];
    fgets(send_buff, SEND_BUFF_SIZE, stdin);
    char buff_with_header[SEND_BUFF_SIZE + HEADER_SIZE];
    uint16_t UDP_header[4];
    UDP_header[0] = htons(7777);
    UDP_header[1] = htons(7778);
    UDP_header[2] = htons(136);
    UDP_header[3] = 0;
    udp_insert(buff_with_header, (char*)UDP_header, send_buff);
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock < 0) {
        error("socket");
    }

    size = sendto(sock, buff_with_header, sizeof(buff_with_header), 0, (struct sockaddr*)&server, sizeof(server));
    if (size < 0) {
        error("send");
    }
    while (1) {
        size = recvfrom(sock, recv_buff, RECV_BUFF_SIZE, 0, (struct sockaddr*)&server, 0);
        if (size < 0) {
            error("recv");
        }
        recv_buff[size - 1] = '\0';

        printf("%s\n", recv_buff + 28);
    }

    close(sock);
    return 0;
}
