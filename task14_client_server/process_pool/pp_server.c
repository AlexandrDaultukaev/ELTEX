#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEND_BUFF_SIZE 256
#define RECV_BUFF_SIZE 128
#define PROC_NUM 3

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

void reaper(int sig) {
    int status;
    while (wait3(&status, WNOHANG, (struct rusage*)0) >= 0)
        ;
}

/*
 * Если free_idx передаётся с положительным значением, то
 * просто устанавливаем флаг, что процесс освобожден.
 * Если free_idx отрицательно, тогда функция используется
 * для возвращения индекса следующего свободного процесса.
 */
int get_next_proc(int free_idx) {
    static int is_free[PROC_NUM] = {1, 1, 1};
    static int next_proc = 0;
    if (free_idx > -1) {
        is_free[free_idx];
        return 0;
    } else {
        for (int i = 0; i < PROC_NUM; i++) {
            next_proc = (next_proc + 1) % PROC_NUM;
            if (is_free[next_proc]) {
                is_free[next_proc] = 0;
                return next_proc;
            }
        }
    }
    return -1;
}

int main() {
    signal(SIGINT, handle_shutdown);
    signal(SIGCHLD, reaper);
    char send_buff[SEND_BUFF_SIZE];
    char recv_buff[RECV_BUFF_SIZE];
    struct sockaddr_in server;
    size_t size;
    int pipes[PROC_NUM][2];
    for (int i = 0; i < PROC_NUM; i++) {
        if (pipe(pipes[i])) {
            fprintf(stderr, "Pipe error\n");
            return EXIT_FAILURE;
        }
    }

    pid_t pid1, pid2, pid3;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        error("socket");
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = 7777;
    int reuseaddr_on = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                   &reuseaddr_on, sizeof(reuseaddr_on)) < 0) {
        error("setsockopt");
    }

    if (bind(sock, (const struct sockaddr*)&server, sizeof(server)) < 0) {
        error("bind");
    }
    if (listen(sock, 5) < 0) {
        error("listen");
    }

    for (int i = 0; i < PROC_NUM; i++)  // loop will run n times (n=5)
    {
        if (fork() == 0) {
            FILE* r_stream = fdopen(pipes[i][0], "r");
            FILE* w_stream = fdopen(pipes[i][1], "w");
            fcntl(pipes[i][0], F_SETFL, fcntl(pipes[i][0], F_GETFL) | O_NONBLOCK);
            char socket_client_ch[20];
            while (1) {
                if (read(pipes[i][0], &sock_client, 4) > 0) {
                    printf("hef2\n");
                    // sock_client = atoi(socket_client_ch);
                    // printf("scc: %s\n", socket_client_ch);
                    printf("sc: %d\n", sock_client);
                    size = recv(sock_client, recv_buff, RECV_BUFF_SIZE, 0);
                    printf("hef3\n");
                    if (size == -1) {
                        error("recv");
                    }
                    recv_buff[size - 1] = '\0';
                    sprintf(send_buff, "OK![✔][pid:%d], MSG:[%s]\n", getpid(), recv_buff);
                    printf("[pid:%d]: %s\n", getpid(), recv_buff);
                    size = send(sock_client, send_buff, strlen(send_buff), 0);
                    if (size < 0) {
                        error("send");
                    }
                    close(sock_client);
                    get_next_proc(i);
                    // fprintf(w_stream, "%d", i);
                }
            }
            exit(0);
        }
    }
    char sock_client_ch[20];

    int idx_proc;
    while (1) {
        sock_client = accept(sock, 0, 0);
        if (sock_client < 0) {
            error("accept");
        }
        idx_proc = get_next_proc(-1);
        if (idx_proc < 0) {
            printf("No free process");
        }
        // sprintf(sock_client_ch, "%d", sock_client);
        write(pipes[idx_proc][1], &sock_client, 4);
        printf("serv: %d\n", sock_client);
        // close(sock_client);
    }
    return 0;
}