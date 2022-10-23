#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
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
#define RECV_BUFF_SIZE 200
#define UDP_HEADER_SIZE 8
#define LINK_HEADER_SIZE 14
#define IP_HEADER_SIZE 20

typedef struct LINK_header {
    char destination_mac[6];
    char source_mac[6];
    short type;
} LINK_header;

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

short CRC16(char* buf) {
    int csum = 0;
    short* ptr = (short*)buf;
    for (int i = 0; i < 10; i++) {
        csum += *ptr;
        ptr++;
    }
    while ((csum >> 16) != 0) {
        short tmp = csum >> 16;
        csum = (csum & 0xFFFF) + tmp;
    }
    csum = ~csum;
    return csum;
}

void init_link_header(LINK_header* link_header) {
    link_header->destination_mac[0] = 0x38;  // 30:5a:3a:48:46:73
    link_header->destination_mac[1] = 0xba;
    link_header->destination_mac[2] = 0xf8;
    link_header->destination_mac[3] = 0xe7;
    link_header->destination_mac[4] = 0xc4;
    link_header->destination_mac[5] = 0x8a;
    link_header->source_mac[0] = 0x30;
    link_header->source_mac[1] = 0x5a;
    link_header->source_mac[2] = 0x3a;
    link_header->source_mac[3] = 0x48;
    link_header->source_mac[4] = 0x46;
    link_header->source_mac[5] = 0x73;
    link_header->type = htons(0x0800);
}

void init_udp_header(UDP_header* udp_header) {
    udp_header->source = htons(7777);
    udp_header->destination = htons(7778);
    udp_header->length = htons(136);
    udp_header->checksum = 0;
}

void init_ip_header(IP_header* ip_header) {
    ip_header->ver_IHL = 69;  // 0100_0101 -> 69
    ip_header->DS = 0;

    ip_header->id = 0;
    ip_header->flags_and_offset = 0;
    ip_header->TTL = 255;
    ip_header->Tproto = IPPROTO_UDP;
    ip_header->source = inet_addr("192.168.1.4");
    ip_header->checksum = 0;
    ip_header->destination = inet_addr("192.168.1.10");
    ip_header->checksum = CRC16((char*)ip_header);
}

int sock;
void udp_ip_link_insert(char* dest, char* udp_header, char* ip_header, char* link_header, char* src) {
    int i = 0, j = 0;

    for (; i < LINK_HEADER_SIZE; i++) {
        dest[i] = link_header[i];
    }

    for (j = 0; j < IP_HEADER_SIZE; i++, j++) {
        dest[i] = ip_header[j];
    }

    for (j = 0; j < UDP_HEADER_SIZE; j++, i++) {
        dest[i] = udp_header[j];
    }

    for (j = 0; src[j] != '\0'; j++, i++) {
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
    struct sockaddr_ll server;

    server.sll_family = AF_PACKET;
    server.sll_ifindex = if_nametoindex("enp2s0");
    if (server.sll_ifindex == 0) {
        error("nametoindex");
    }
    server.sll_addr[0] = 0x38;
    server.sll_addr[1] = 0xba;
    server.sll_addr[2] = 0xf8;
    server.sll_addr[3] = 0xe7;
    server.sll_addr[4] = 0xc4;
    server.sll_addr[5] = 0x8a;
    size_t size;
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];

    fgets(send_buff, SEND_BUFF_SIZE, stdin);
    char buff_with_header[SEND_BUFF_SIZE + UDP_HEADER_SIZE + IP_HEADER_SIZE + LINK_HEADER_SIZE];
    UDP_header udp_header;
    IP_header ip_header;
    LINK_header link_header;
    init_udp_header(&udp_header);
    ip_header.length = htons(sizeof(send_buff) + 28);
    init_ip_header(&ip_header);
    init_link_header(&link_header);
    udp_ip_link_insert(buff_with_header, (char*)&udp_header, (char*)&ip_header, (char*)&link_header, send_buff);
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
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
        // short* dest_port = (short*)(recv_buff + 36);
        // if (htons(*dest_port) == 7777) {
        printf("%s\n", recv_buff + 42);
        // }
    }

    close(sock);
    return 0;
}
