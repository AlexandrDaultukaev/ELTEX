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

void server() {
    struct sockaddr_in name;

    size_t size;
    char send_buff[] = "Hello!";
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }
    int flag = 1;

    name.sin_family = AF_INET;
    name.sin_addr.s_addr = inet_addr("224.0.0.1");
    name.sin_port = 7777; 
    int n;
    while (1) {
        n = sendto(sock, send_buff, strlen(send_buff), 0, (struct sockaddr*)&name, sizeof(name));
        if (n < 0) {
            error("sendto");
        }
        printf("sendto\n");
        sleep(1);
    }
}

int main() {
    signal(SIGINT, handle_shutdown);
    server();
    return 0;
}