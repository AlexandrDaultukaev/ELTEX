#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
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
#define UDP_HEADER_SIZE sizeof(struct udphdr)
#define LINK_HEADER_SIZE sizeof(struct ether_header)
#define IP_HEADER_SIZE sizeof(struct iphdr)
#define HEADERS_SIZE IP_HEADER_SIZE + LINK_HEADER_SIZE + UDP_HEADER_SIZE

unsigned char macSource[6] = {0x30, 0x5a, 0x3a, 0x48, 0x46, 0x73};
unsigned char macDest[6] = {0x38, 0xba, 0xf8, 0xe7, 0xc4, 0x8a};

unsigned short check_sum(unsigned short* ptr, int nbytes) {
    register long sum = 0;
    unsigned short oddbyte;
    register short answer;

    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char*)&oddbyte) = *(u_char*)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (short)~sum;

    return (answer);
}

void init_link_header(struct ether_header* link_header) {
    for (int i = 0; i < 6; i++) {
        link_header->ether_dhost[i] = macDest[i];
        link_header->ether_shost[i] = macSource[i];
    }
    link_header->ether_type = htons(0x0800);
}

void init_udp_header(struct udphdr* udp_header, size_t len) {
    udp_header->source = htons(7777);
    udp_header->dest = htons(7778);
    udp_header->check = 0;
    udp_header->len = htons(len + UDP_HEADER_SIZE - 1);
}

void init_ip_header(struct iphdr* ip_header, size_t len) {
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->id = htonl(11111);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->saddr = inet_addr("192.168.1.4");
    ip_header->daddr = inet_addr("192.168.1.10");
    ip_header->tot_len = htons(IP_HEADER_SIZE + UDP_HEADER_SIZE + len - 1);
    ip_header->check = 0;
    ip_header->check = check_sum((unsigned short*)ip_header, IP_HEADER_SIZE);
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
    server.sll_halen = 6;

    for (int i = 0; i < 6; i++) {
        server.sll_addr[i] = macDest[i];
    }
    size_t size;
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];
    printf("Enter msg: ");
    fgets(send_buff, SEND_BUFF_SIZE, stdin);
    char buff_with_header[SEND_BUFF_SIZE + HEADERS_SIZE];

    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        error("socket");
    }

    socklen_t len = sizeof(struct sockaddr_ll);

    // Инициализируем заголовок data link уровня
    struct ether_header* link_header;
    link_header = (struct ether_header*)buff_with_header;
    init_link_header(link_header);

    // Инициализируем ip заголовок
    struct iphdr* ip_header;
    ip_header = (struct iphdr*)(buff_with_header + LINK_HEADER_SIZE);
    init_ip_header(ip_header, strlen(send_buff));

    // Инициализируем udp заголовок
    struct udphdr* udp_header;
    udp_header = (struct udphdr*)(buff_with_header + LINK_HEADER_SIZE + IP_HEADER_SIZE);
    init_udp_header(udp_header, strlen(send_buff));

    // Записываем само сообщение в пакет, после всех заголовков
    int i = 0;
    for (; send_buff[i] != '\0'; i++) {
        buff_with_header[HEADERS_SIZE + i] = send_buff[i];
    }

    ssize_t s = sendto(sock, buff_with_header, HEADERS_SIZE + strlen(send_buff), 0, (struct sockaddr*)&server, len);
    if (s == -1) {
        error("send");
    }

    while (1) {
        memset(recv_buff, 0, RECV_BUFF_SIZE);
        size = recvfrom(sock, recv_buff, RECV_BUFF_SIZE, 0, (struct sockaddr*)&server, 0);
        if (size < 0) {
            error("recv");
        }
        recv_buff[size - 1] = '\0';
        udp_header = (struct udphdr*)(buff_with_header + LINK_HEADER_SIZE + IP_HEADER_SIZE);
        if (ntohs(udp_header->dest) == 7777)
            printf("%s\n", recv_buff + HEADERS_SIZE);
    }

    close(sock);
    return 0;
}
