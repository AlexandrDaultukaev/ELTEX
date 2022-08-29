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
char server_path[FILENAME_SIZE] = "/tmp/mysock";
char client_path[FILENAME_SIZE] = "/tmp/myclientsock";

void handle_shutdown(int sig) {
    printf("\nShutdown\n");
    close(sock);
    unlink(client_path);
    exit(EXIT_SUCCESS);
}

void error(char* msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

void make_named_socket() {
    struct sockaddr_un name;
    size_t size;

    sock = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }
    unlink(client_path);
    name.sun_family = AF_LOCAL;
    strncpy(name.sun_path, client_path, sizeof(name.sun_path));
    name.sun_path[sizeof(name.sun_path) - 1] = '\0';

    if (bind(sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
        error("bind");
    }
}

void run_client() {
    make_named_socket();
    char recv_buff[BUFF_SIZE];
    char send_buff[] = "Hi!(from client)";
    struct sockaddr_un server;
    int n, len;

    server.sun_family = AF_LOCAL;
    strncpy(server.sun_path, server_path, sizeof(server.sun_path));
    len = sizeof(server);

    n = sendto(sock, send_buff, strlen(send_buff), MSG_CONFIRM, (struct sockaddr*)&server, sizeof(server));
    if (n < 0) {
        error("sendto");
    }

    n = recvfrom(sock, recv_buff, BUFF_SIZE, 0, (struct sockaddr*)&server, &len);
    if (n < 0) {
        error("recvfrom");
    }

    if (n > 0) {
        recv_buff[n] = '\0';
    }

    printf("RECEIVED: %s\n", recv_buff);
}

int main() {
    signal(SIGINT, handle_shutdown);
    run_client();
    return 0;
}