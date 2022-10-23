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
#define UDP_HEADER_SIZE 8
#define IP_HEADER_SIZE 20

typedef struct UDP_header {
    uint16_t source;
    uint16_t destination;
    uint16_t length;
    uint16_t checksum;
} UDP_header;

typedef struct IP_header {
    char ver_IHL;            // version=4,header length=5;
    char DS;                 // always 0;
    short length;            // auto fill
    short id;                // auto fill if 0
    short flags_and_offset;  // flags=0,frag,end; offset of fragment in full packet
    char TTL;                // time to live
    char Tproto;             // transport protocol
    short checksum;          // auto fill
    int source;              // auto fill if 0
    int destination;
} IP_header;

void init_udp_header(UDP_header* udp_header) {
    udp_header->source = htons(7777);
    udp_header->destination = htons(7778);
    udp_header->length = htons(136);
    udp_header->checksum = 0;
}

void init_ip_header(IP_header* ip_header) {
    ip_header->ver_IHL = 69;  // 0100_0101 -> 69
    ip_header->DS = 0;
    ip_header->length = htons(136);
    ip_header->id = 0;
    ip_header->flags_and_offset = 0;
    ip_header->TTL = 255;
    ip_header->Tproto = IPPROTO_UDP;
    ip_header->checksum = 0;
    ip_header->source = inet_addr("127.0.0.1");
    ip_header->destination = inet_addr("127.0.0.2");
}

int sock;
void udp_ip_insert(char* dest, char* udp_header, char* ip_header, char* src) {
    int i = 0, j = 0, k = 0;

    for (; i < IP_HEADER_SIZE; i++) {
        dest[i] = ip_header[i];
    }

    for (; k < UDP_HEADER_SIZE; k++, i++) {
        dest[i] = udp_header[k];
    }

    for (; src[j] != '\0'; j++, i++) {
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
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(7777);
    size_t size;
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];

    fgets(send_buff, SEND_BUFF_SIZE, stdin);
    char buff_with_header[SEND_BUFF_SIZE + UDP_HEADER_SIZE + IP_HEADER_SIZE];
    UDP_header udp_header;
    IP_header ip_header;
    init_udp_header(&udp_header);
    init_ip_header(&ip_header);

    udp_ip_insert(buff_with_header, (char*)&udp_header, (char*)&ip_header, send_buff);
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock < 0) {
        error("socket");
    }

    // Чтобы иметь возможность создать IP заголовок самому
    int flag = 1;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
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
